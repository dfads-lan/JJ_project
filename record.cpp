#include "record.h"
#include "ui_record.h"
#include "keyboard.h"

record::record(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::record)
{
    ui->setupUi(this);

    // 创建软键盘实例
    Keyboard *kb = new Keyboard(this);

    // 让键盘监听所有 lineEdit 的焦点事件
    for (auto le : QList<QLineEdit*>{ui->lineEdit}) {
        le->installEventFilter(kb);
    }
}

record::~record()
{
    delete ui;
}

void record::on_pushButton_down_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("导出记录"),
        QDir::homePath() + "/record.csv",
        tr("CSV Files (*.csv)"));

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "错误", "无法创建文件：" + file.errorString());
        return;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");            // 保证中文不乱码
    out.setGenerateByteOrderMark(true);

    int rows = mSQLModel->rowCount();
    int cols = mSQLModel->columnCount();

    /* ---------- 可选：写入表头 ---------- */
    for (int c = 0; c < cols; ++c) {
        if (c) out << ",";
        out << mSQLModel->headerData(c, Qt::Horizontal).toString();
    }
    out << "\n";

    /* ---------- 写入数据 ---------- */
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (c) out << ",";
            out << mSQLModel->data(mSQLModel->index(r, c)).toString();
        }
        out << "\n";
    }

    file.close();
    QMessageBox::information(this, "提示", "导出完成！\n文件：" + fileName);
}

void record::on_pushButton_down_2_clicked()
{
    QString idFilter="";
    idFilter = tr("id = '%1'").arg(ui->lineEdit->text().toInt());
    mSQLModel->setFilter(idFilter);
    mSQLModel->select(); //显示结果
}

void record::setSqlDatabase(QSqlDatabase db)
{
    this->db = db;
    mSQLModel = new QSqlTableModel(this, this->db);
    mSQLModel->setTable("record");
    mSQLModel->setHeaderData(0,Qt::Horizontal,tr("序号"));
    mSQLModel->setHeaderData(1,Qt::Horizontal,tr("ID"));
    mSQLModel->setHeaderData(2,Qt::Horizontal,tr("日期"));
    mSQLModel->setHeaderData(3,Qt::Horizontal,tr("时间"));
    mSQLModel->setHeaderData(4,Qt::Horizontal,tr("是否"));
    mSQLModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setModel(mSQLModel);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置单元格不可编辑
    ui->tableView->horizontalHeader()->setStretchLastSection(true);//设置最后一列填充后面表格
    ui->tableView->setColumnHidden(0,true);//设置第0列隐藏
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mSQLModel->select(); //选取整个表的所有行
}

//清除记录
void record::on_pushButton_clear_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  tr("确认"),
                                  tr("确定要清空所有记录吗？此操作不可恢复！"),
                                  QMessageBox::No | QMessageBox::Yes);

    if (reply == QMessageBox::Yes) {
        // 1. 删除表中全部记录
        mSQLModel->setFilter("");            // 清空过滤，保证全表
        mSQLModel->select();                 // 重新拉取
        int rowCount = mSQLModel->rowCount();
        for (int i = 0; i < rowCount; ++i) {
            mSQLModel->removeRow(i);         // 标记删除
        }
        mSQLModel->submitAll();              // 提交到数据库
        mSQLModel->select();                 // 刷新界面
    }
}

void record::on_pushButton_clear_2_clicked()
{
    /* 1. 拿到点击的单元格所在行号 */
    QModelIndex idx = ui->tableView->currentIndex();
    if (!idx.isValid()) {
        QMessageBox::warning(this, "提示", "请先选中要删除的记录！");
        return;
    }
    int row = idx.row();

    /* 2. 二次确认 */
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  tr("确认删除"),
                                  tr("确定要删除选中行吗？此操作不可恢复！"),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes)
        return;

    /* 3. 删除并提交 */
    mSQLModel->removeRow(row);
    if (!mSQLModel->submitAll()) {
        QMessageBox::critical(this, "错误",
                              "删除失败：" + mSQLModel->lastError().text());
    } else {
        QMessageBox::information(this, "提示", "删除成功！");
    }

    /* 4. 刷新视图 */
    mSQLModel->select();
}

void record::on_pushButton_timeSearch_clicked()
{
    QDate startDate = ui->dateEdit_start->date();
    QDate endDate = ui->dateEdit_end->date();

    // 数据库中日期字段名为 "date"，格式为 yyyy-MM-dd
    QString filter = QString("date >= '%1' AND date <= '%2'")
                        .arg(startDate.toString("yyyy-MM-dd"))
                        .arg(endDate.toString("yyyy-MM-dd"));

    mSQLModel->setFilter(filter);
    mSQLModel->select();
}
