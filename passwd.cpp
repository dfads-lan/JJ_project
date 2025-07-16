#include "passwd.h"
#include "ui_passwd.h"
#include "sqlit.h"

passwd::passwd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::passwd)
{
    ui->setupUi(this);
    password = ""; // 初始化密码为空
    mpassword = "";
    isOk = false;
    ui->lineEdit->setEchoMode(QLineEdit::Password); // 设置密码显示模式
    ui->lineEdit->setFocus();                            // 默认焦点

    ui->p_0->setFocusPolicy(Qt::NoFocus);
    ui->p_1->setFocusPolicy(Qt::NoFocus);
    ui->p_2->setFocusPolicy(Qt::NoFocus);
    ui->p_3->setFocusPolicy(Qt::NoFocus);
    ui->p_4->setFocusPolicy(Qt::NoFocus);
    ui->p_5->setFocusPolicy(Qt::NoFocus);
    ui->p_6->setFocusPolicy(Qt::NoFocus);
    ui->p_7->setFocusPolicy(Qt::NoFocus);
    ui->p_8->setFocusPolicy(Qt::NoFocus);
    ui->p_9->setFocusPolicy(Qt::NoFocus);
    ui->pushButton_del->setFocusPolicy(Qt::NoFocus);
}

passwd::~passwd()
{
    delete ui;
}

void passwd::on_p_0_clicked() { appendToFocus("0"); }
void passwd::on_p_1_clicked() { appendToFocus("1"); }
void passwd::on_p_2_clicked() { appendToFocus("2"); }
void passwd::on_p_3_clicked() { appendToFocus("3"); }
void passwd::on_p_4_clicked() { appendToFocus("4"); }
void passwd::on_p_5_clicked() { appendToFocus("5"); }
void passwd::on_p_6_clicked() { appendToFocus("6"); }
void passwd::on_p_7_clicked() { appendToFocus("7"); }
void passwd::on_p_8_clicked() { appendToFocus("8"); }
void passwd::on_p_9_clicked() { appendToFocus("9"); }

void passwd::on_pushButton_del_clicked()
{
    QLineEdit *edit = qobject_cast<QLineEdit*>(focusWidget());
    if (!edit) return;

    edit->backspace();   // 删除光标前一个字符
}
void passwd::appendToFocus(const QString &ch)
{
    QLineEdit *edit = qobject_cast<QLineEdit*>(focusWidget());
    if (!edit) return;

    edit->insert(ch);   // 在当前光标处插入字符
}

bool passwd::getIsOk()
{
    return isOk;
}
void passwd::setPassword(QString pwd)
{
    mpassword = pwd;
}
QString passwd::queryID()
{
    return ui->lineEdit_2->text();
}

void passwd::on_pushButton_submit_clicked()
{
    QString id = ui->lineEdit_2->text();
    QString inputPwd = ui->lineEdit->text();
    QString realPwd = sqlit::getInstance()->getAdminPassword(id);

    if (inputPwd == realPwd && !realPwd.isEmpty()) {
        QMessageBox::information(this, "登录成功", "密码正确，欢迎！");
        isOk = true;
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        this->close();
    } else {
        QMessageBox::warning(this, "登录失败", "密码错误，请重试。");
        isOk = false;
        ui->lineEdit->clear();
    }
}

void passwd::on_pushButton_clicked()
{
    this->close();
}
