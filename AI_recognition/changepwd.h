#ifndef CHANGEPWD_H
#define CHANGEPWD_H

#include <QDialog>
#include "sqlit.h"
#include <QMessageBox>
class Keyboard;
namespace Ui {
class changepwd;
}

class changepwd : public QDialog
{
    Q_OBJECT

public:
    explicit changepwd(QWidget *parent = nullptr);
    ~changepwd();

private slots:
    void on_pushButton_submit_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::changepwd *ui;
    Keyboard *m_kb = nullptr;
};

#endif // CHANGEPWD_H
