#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLabel>
#include <QTimer>
#include <QList>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>

#include <QComboBox>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "camerathread.h"
#include "passwd.h"
#include "sqlit.h"
#include "admi.h"

using namespace std;
using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void appendLog(const QString &text);
    /* 扫描是否存在摄像头 */
    void scanCameraDevice();

private slots:
    void showTime();
    void slotShowImage(QImage imag);
    void slotShowResult(QImage imag,QString text);



    void on_pushButton_clicked();

    void on_pushButton_admi_clicked();

    void on_pushButton_admi_2_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *mUpdateTimeTimer;
    QSqlDatabase db;
    CameraThread *mCameraThread;
    admi *mAdmi;
    QComboBox *comboBox;
};
#endif // MAINWINDOW_H
