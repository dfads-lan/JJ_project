#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QDialog>
#include <QLineEdit>

class QPushButton;
class QGridLayout;

class Keyboard : public QDialog
{
    Q_OBJECT
public:
    explicit Keyboard(QWidget *parent = nullptr);
    ~Keyboard();

    /* 绑定要输入的编辑框 */
    void setFocusEdit(QLineEdit *edit);

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    bool eventFilter(QObject *obj, QEvent *ev) override;
private slots:
    void onBtnClicked();

private:
    void buildUI();

    QLineEdit *m_targetEdit {nullptr};
    QWidget   *m_keyWidget;
    QPoint     m_dragPos;
    bool m_visibleByFocus {false};
};

#endif // KEYBOARD_H
