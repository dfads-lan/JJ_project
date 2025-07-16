#ifndef RECORD_H
#define RECORD_H

#include <QWidget>
#include <QSqlTableModel>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlRecord>

#include "sqlit.h"

namespace Ui {
class record;
}

class record : public QWidget
{
    Q_OBJECT

public:
    explicit record(QWidget *parent = nullptr);
    ~record();
    void setSqlDatabase(QSqlDatabase db);

private slots:
    void on_pushButton_down_clicked();

    void on_pushButton_down_2_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_clear_2_clicked();

    void on_pushButton_timeSearch_clicked();

private:
    Ui::record *ui;
    QSqlDatabase db;
    QSqlTableModel *mSQLModel;
};

#endif // RECORD_H
