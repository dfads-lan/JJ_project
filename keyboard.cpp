#include "keyboard.h"
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDebug>

Keyboard::Keyboard(QWidget *parent)
    : QDialog(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    setWindowTitle("软键盘");
    resize(450, 220);
    buildUI();
}

Keyboard::~Keyboard(){}

/* ---------- 构造界面 ---------- */
void Keyboard::buildUI()
{
    m_keyWidget = new QWidget(this);
    QGridLayout *grid = new QGridLayout(m_keyWidget);
    grid->setSpacing(4);

    /* 第0行：数字 0-9 */
    for (int i = 0; i < 10; ++i) {
        QPushButton *btn = new QPushButton(QString::number(i), this);
        btn->setMinimumHeight(42);
        btn->setFocusPolicy(Qt::NoFocus);
        connect(btn, &QPushButton::clicked, this, &Keyboard::onBtnClicked);
        grid->addWidget(btn, 0, i);
    }

    /* 第1-3行：字母区 */
    const char *rows[3] = {
        "qwertyuiop",
        "asdfghjkl",
        "zxcvbnm"
    };
    for (int r = 0; r < 3; ++r) {
        QString line(rows[r]);
        for (int c = 0; c < line.size(); ++c) {
            QPushButton *btn = new QPushButton(line.mid(c,1), this);
            btn->setMinimumHeight(42);
            btn->setFocusPolicy(Qt::NoFocus);
            connect(btn, &QPushButton::clicked, this, &Keyboard::onBtnClicked);
            grid->addWidget(btn, r+1, c);
        }
    }

    /* 第4行：功能键 */
    QPushButton *space  = new QPushButton("Space", this);
    QPushButton *back   = new QPushButton("Backspace", this);
    QPushButton *enter  = new QPushButton("Enter", this);
    QPushButton *close  = new QPushButton("关闭", this); // 新增关闭按钮

    space ->setMinimumHeight(42);
    back  ->setMinimumHeight(42);
    enter ->setMinimumHeight(42);
    close ->setMinimumHeight(42); // 设置高度

    space ->setFocusPolicy(Qt::NoFocus);
    back  ->setFocusPolicy(Qt::NoFocus);
    enter ->setFocusPolicy(Qt::NoFocus);
    close ->setFocusPolicy(Qt::NoFocus);

    connect(space , &QPushButton::clicked, this, &Keyboard::onBtnClicked);
    connect(back  , &QPushButton::clicked, this, &Keyboard::onBtnClicked);
    connect(enter , &QPushButton::clicked, this, &Keyboard::onBtnClicked);
    connect(close , &QPushButton::clicked, this, &Keyboard::onBtnClicked); // 连接关闭按钮

    grid->addWidget(space , 4, 0, 1, 4);
    grid->addWidget(back  , 4, 4, 1, 2);
    grid->addWidget(enter , 4, 6, 1, 2);
    grid->addWidget(close , 4, 8, 1, 2); // 放在最后两格

    QVBoxLayout *mainLay = new QVBoxLayout(this);
    mainLay->setContentsMargins(6,6,6,6);
    mainLay->addWidget(m_keyWidget);
}

/* ---------- 绑定目标输入框 ---------- */
void Keyboard::setFocusEdit(QLineEdit *edit)
{
    m_targetEdit = edit;
}

/* ---------- 按键处理 ---------- */
void Keyboard::onBtnClicked()
{
    if (!m_targetEdit) return;

    QString key = qobject_cast<QPushButton*>(sender())->text();

    if (key == "Enter" || key == "关闭") {
        m_visibleByFocus = false;  // 清掉标志
        hide();
        return;
    }

    if (key == "Space")        m_targetEdit->insert(" ");
    else if (key == "Backspace") m_targetEdit->backspace();
    else                         m_targetEdit->insert(key);
}


/* ---------- Esc / 拖动 ---------- */
void Keyboard::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape) {
        m_visibleByFocus = false;
        hide();
    }
}

void Keyboard::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
        m_dragPos = e->globalPos() - frameGeometry().topLeft();
}

void Keyboard::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton)
        move(e->globalPos() - m_dragPos);
}

bool Keyboard::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::FocusIn) {
        if (auto le = qobject_cast<QLineEdit*>(obj)) {
            setFocusEdit(le);

            // 如果键盘当前不可见，则弹出
            if (!isVisible()) {
                move(le->mapToGlobal(QPoint(0, le->height())));
                show();
                raise();
                m_visibleByFocus = true;   // 标记是“焦点触发”的
            }
        }
    }
    return QDialog::eventFilter(obj, ev);
}
