#include "admi.h"
#include "ui_admi.h"

admi::admi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::admi)
{
    ui->setupUi(this);
    mRecord = new record(this);
    ui->stackedWidget->addWidget(mRecord);
    ui->stackedWidget->setCurrentWidget(mRecord);
}

admi::~admi()
{
    delete ui;
}
void admi::setSqlDatabase(QSqlDatabase db)
{
    mRecord->setSqlDatabase(db);
}

void admi::on_pushButton_record_clicked()
{
    ui->stackedWidget->setCurrentWidget(mRecord);
}

void admi::on_pushButton_quit_clicked()
{
    this->close();
}

void admi::on_pushButton_changePwd_clicked()
{
    changepwd dlg(this);
    dlg.exec();
}
