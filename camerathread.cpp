#include "camerathread.h"
CameraThread::CameraThread(QObject *parent) : QThread(parent) {}

CameraThread::~CameraThread()
{
    stop();
    wait();
}

void CameraThread::stop()
{
    m_stop = true;
}

void CameraThread::setCamera(int id)
{
    QMutexLocker lock(&m_cameraMutex);
    m_cameraId = id;
}

void CameraThread::setCascade(const QString &xml)
{
    QMutexLocker lock(&m_cameraMutex);
    if (xml != m_cascadePath) {
        m_cascadePath = xml;
        m_cascade.load(m_cascadePath.toStdString());
    }
}

void CameraThread::run()
{
    /* ---------- 初始化 ---------- */
    {
        QMutexLocker lock(&m_cameraMutex);
        m_cap.open(m_cameraId);
        if (!m_cap.isOpened()) {
            qCritical() << "Cannot open camera" << m_cameraId;
            return;
        }
        m_cap.set(cv::CAP_PROP_FRAME_WIDTH,  640);
        m_cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
        m_cap.set(cv::CAP_PROP_FPS, 15);

        if (!m_cascade.load(m_cascadePath.toStdString())) {
            qCritical() << "Cannot load cascade" << m_cascadePath;
            return;
        }
    }

    cv::Mat frame;
    while (!m_stop) {
        /* ---------- 采集 ---------- */
        {
            QMutexLocker lock(&m_cameraMutex);
            if (!m_cap.read(frame) || frame.empty()) {
                msleep(50);
                continue;
            }
        }

        QImage img = mat2QImage(frame);
        emit newFrame(img);

        /* ---------- 算法 ---------- */
        QElapsedTimer t; t.start();
        QString text;
        detectAndDraw(frame, text);
        qDebug().nospace() << "Detect cost " << t.elapsed() << " ms";

        img = mat2QImage(frame);
        emit newResult(img, text);
    }

    m_cap.release();
}

/* -------------------------------------------------- */
QImage CameraThread::mat2QImage(const cv::Mat &src)
{
    switch (src.type()) {
    case CV_8UC1: {
        QImage img(src.data, src.cols, src.rows,
                   static_cast<int>(src.step), QImage::Format_Grayscale8);
        return img.copy();          // detach 深拷贝
    }
    case CV_8UC3: {
        QImage img(src.data, src.cols, src.rows,
                   static_cast<int>(src.step), QImage::Format_RGB888);
        return img.rgbSwapped();    // BGR → RGB
    }
    case CV_8UC4: {
        QImage img(src.data, src.cols, src.rows,
                   static_cast<int>(src.step), QImage::Format_ARGB32);
        return img.copy();
    }
    default:
        qWarning() << "Unsupported cv::Mat format";
        return {};
    }
}

/* -------------------------------------------------- */
void CameraThread::detectAndDraw(cv::Mat &frame, QString &text)
{
    if (m_cascade.empty()) {
        text = "NO_CASCADE";
        return;
    }
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, gray);

    std::vector<cv::Rect> eyes;
    m_cascade.detectMultiScale(gray, eyes, 1.1, 3, 0, cv::Size(30, 30));

    if (eyes.empty()) {
        text = "NONE";
    } else {
        text = "yes";
    }

    for (const cv::Rect &r : eyes) {
        cv::rectangle(frame, r, cv::Scalar(0, 0, 255), 2);
        cv::putText(frame, text.toStdString(), cv::Point(r.x, r.y - 5),
                    cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 255), 2);
    }
}
