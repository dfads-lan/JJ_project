#ifndef CAMERATHREAD_H
#define CAMERATHREAD_H

#include <QThread>
#include <QImage>
#include <QMutex>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <QDateTime>
#include <QDebug>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QElapsedTimer>
#include <QFileInfo>


using namespace std;
using namespace cv;

class CameraThread : public QThread
{
    Q_OBJECT
public:
    explicit CameraThread(QObject *parent = nullptr);
    ~CameraThread();

    void stop();                         // 线程安全地结束 run()
    void setCamera(int id);              // 动态切换摄像头
    void setCascade(const QString &xml); // 动态切换分类器

signals:
    void newFrame(const QImage &img);    // 原始帧
    void newResult(const QImage &img,
                   const QString &text); // 画框+文本后的帧

protected:
    void run() override;

private:
    /* 工具函数 */
    static QImage mat2QImage(const cv::Mat &src);

    /* 算法函数 */
    void detectAndDraw(cv::Mat &frame, QString &text);

    /* 成员变量 */
    std::atomic_bool m_stop {false};
    QMutex           m_cameraMutex;
    int              m_cameraId {0};
    QString          m_cascadePath {":/XML/my_haarcascade.xml"};
    cv::VideoCapture m_cap;
    cv::CascadeClassifier m_cascade;
};

#endif // CAMERATHREAD_H
