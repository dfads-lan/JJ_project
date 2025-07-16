#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    {
#if __arm__
    /* 重设大小 */
    this->resize(list_screen.at(0)->geometry().width(),
                 list_screen.at(0)->geometry().height());
#else
    /* 否则则设置主窗体大小为800x480 */
    this->resize(800, 480);
#endif
    }
    scanCameraDevice();

    mAdmi = new admi();
    sqlit::getInstance()->initDB("mysql.db");
    sqlit::getInstance()->createTable();
    mAdmi->setSqlDatabase(sqlit::getInstance()->m_db);

    mUpdateTimeTimer = new QTimer(this);
    connect(mUpdateTimeTimer, SIGNAL(timeout()), this, SLOT(showTime()));
    mUpdateTimeTimer->start(1000);

    mCameraThread = new CameraThread();
    connect(mCameraThread, &CameraThread::newFrame, this, &MainWindow::slotShowImage);
    connect(mCameraThread, &CameraThread::newResult, this, &MainWindow::slotShowResult);

}

MainWindow::~MainWindow()
{
    mCameraThread->stop();
    mCameraThread->wait();
    delete ui;
}

void MainWindow::showTime()
{
    QDateTime local(QDateTime::currentDateTime());
    ui->label_time->setText(local.toString("yyyy年MM月dd日  hh:mm:ss"));
}

void MainWindow::slotShowImage(QImage imag)
{
//    try {
//        QPixmap p = QPixmap::fromImage(imag);
//        p = p.scaled(ui->label_cam->size(),
//                     Qt::KeepAspectRatio,
//                     Qt::SmoothTransformation);
//        ui->label_cam->setPixmap(p);

//    } catch (const std::exception &ex) {
//        qDebug() << "图片显示异常" << ex.what();
//    }
}

void MainWindow::slotShowResult(QImage imag,QString text)
{
    appendLog(QString("识别结果: %1").arg(text));
    try {
        QPixmap p = QPixmap::fromImage(imag);
        p = p.scaled(ui->label_cam->size(),
                     Qt::KeepAspectRatio,
                     Qt::SmoothTransformation);
        ui->label_cam->setPixmap(p);

    } catch (const std::exception &ex) {
        qDebug() << "图片显示异常" << ex.what();
    }
    QString id = "17";
    QString date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString percent = "1%";

    sqlit::getInstance()->insertRecord(id, date, time, percent);
}

void MainWindow::appendLog(const QString &text)
{
    QString timeStr = QDateTime::currentDateTime()
                          .toString("yyyy-MM-dd hh:mm:ss");
    // 固定 19 位宽，右对齐后加两个空格再跟日志内容
    ui->textBrowser->append(QString("%1  %2")
                            .arg(timeStr, 19)    // 19 表示右对齐占 19 位
                            .arg(text));
}


void MainWindow::on_pushButton_clicked()
{
    static bool isOpen = false;
    if (isOpen) {
        mCameraThread->stop();
        mCameraThread->wait();
        ui->label_cam->clear();
        appendLog("摄像头关闭！");
    } else {
        // 获取当前选中的摄像头索引
        int camId = ui->comboBox->currentIndex();
        mCameraThread->setCamera(camId);
        mCameraThread->start();
        appendLog("摄像头已打开！");
    }
    isOpen = !isOpen;

}

void MainWindow::on_pushButton_admi_clicked()
{
    passwd tPasswordDialog;
    tPasswordDialog.exec();
    if(tPasswordDialog.getIsOk()){
        mAdmi->setWindowModality(Qt::ApplicationModal);
        mAdmi->show();
        appendLog("管理员登陆成功！");
    }
}

void MainWindow::on_pushButton_admi_2_clicked()
{
    ui->textBrowser->clear();
}
void MainWindow::scanCameraDevice()
{
    QFile file("/dev/video0");
    if (file.exists())
        ui->comboBox->addItem("video0");
    file.setFileName("/dev/video1");
    if (file.exists())
        ui->comboBox->addItem("video1");
    file.setFileName("/dev/video2");
    if (file.exists())
        ui->comboBox->addItem("video2");

    // 默认选第一个
    ui->comboBox->setCurrentIndex(0);

    // 摄像头切换时，停止线程再重启
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int idx){
        if (mCameraThread->isRunning()) {
            mCameraThread->stop();
            mCameraThread->wait();
        }
        mCameraThread->setCamera(idx);
        mCameraThread->start();
        appendLog(QString("切换到摄像头 %1").arg(idx));
    });
}
