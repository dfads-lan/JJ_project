#include "changepwd.h"
#include "ui_changepwd.h"
#include "keyboard.h"
changepwd::changepwd(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::changepwd)
{
    ui->setupUi(this);

    m_kb = new Keyboard(this);          // 用成员保存

    for (auto le : QList<QLineEdit*>{
            ui->lineEdit_id,
            ui->lineEdit_oldPwd,
            ui->lineEdit_newPwd,
            ui->lineEdit_confirmPwd})
    {
        le->installEventFilter(m_kb);
    }
}

changepwd::~changepwd()
{
    // 注销事件过滤器
    for (auto le : QList<QLineEdit*>{
            ui->lineEdit_id,
            ui->lineEdit_oldPwd,
            ui->lineEdit_newPwd,
            ui->lineEdit_confirmPwd})
    {
        le->removeEventFilter(m_kb);
    }

    delete m_kb;   // Qt 父子关系会自动 delete，但写上更直观
    delete ui;
}

void changepwd::on_pushButton_submit_clicked()
{
    QString id = ui->lineEdit_id->text();
    QString oldPwd = ui->lineEdit_oldPwd->text();
    QString newPwd = ui->lineEdit_newPwd->text();
    QString confirmPwd = ui->lineEdit_confirmPwd->text();

    QString realPwd = sqlit::getInstance()->getAdminPassword(id);
    if (oldPwd != realPwd) {
        QMessageBox::warning(this, "错误", "原密码错误！");
        return;
    }
    if (newPwd.isEmpty() || newPwd != confirmPwd) {
        QMessageBox::warning(this, "错误", "新密码不一致或为空！");
        return;
    }
    sqlit::getInstance()->updateAdminPassword(id, newPwd);
    QMessageBox::information(this, "成功", "密码修改成功！");
    this->close();
}

void changepwd::on_pushButton_cancel_clicked()
{
    this->close();
}
