#ifndef ADMI_H
#define ADMI_H

#include <QWidget>
#include "record.h"
#include "changepwd.h"

namespace Ui {
class admi;
}

class admi : public QWidget
{
    Q_OBJECT

public:
    explicit admi(QWidget *parent = nullptr);
    ~admi();
    void setSqlDatabase(QSqlDatabase db);

private slots:
    void on_pushButton_record_clicked();

    void on_pushButton_quit_clicked();

    void on_pushButton_changePwd_clicked();

private:
    Ui::admi *ui;
    record *mRecord;
};

#endif // ADMI_H
