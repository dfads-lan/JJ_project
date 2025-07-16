#ifndef PASSWD_H
#define PASSWD_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class passwd;
}

class passwd : public QDialog
{
    Q_OBJECT

public:
    explicit passwd(QWidget *parent = nullptr);
    ~passwd();
    bool getIsOk();

    void setPassword(QString pwd);
    QString queryID();

private slots:
    void on_p_0_clicked();

    void on_p_1_clicked();

    void on_p_2_clicked();

    void on_p_3_clicked();

    void on_p_4_clicked();

    void on_p_5_clicked();

    void on_p_6_clicked();

    void on_p_7_clicked();

    void on_p_8_clicked();

    void on_p_9_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_submit_clicked();


    void on_pushButton_clicked();

private:
    Ui::passwd *ui;
    QString password; // 用于存储当前输入的密码
    QString mpassword;
    bool isOk;
    void appendToFocus(const QString &ch);
};

#endif // PASSWD_H
