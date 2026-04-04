#include "ToDoList.h"
#include "ui_ToDoList.h"
#include "DatabaseManager.h"

ToDoList::ToDoList(DatabaseManager* dbManager, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ToDoList)
    , m_dbManager(dbManager)
{
    ui->setupUi(this);

    //각 버튼 connect
    connect(ui->listWidget, &QListWidget::itemClicked, this, &ToDoList::onToDoItemClicked);
    connect(ui->add_Button, &QPushButton::clicked, this, &ToDoList::addButtonClicked);
    connect(ui->save_Button, &QPushButton::clicked, this, &ToDoList::saveButtonClicked);
    connect(ui->delete_Button, &QPushButton::clicked, this, &ToDoList::deleteButtonClicked);

    //일정 dbManager에서 초기화(가져오기)
    loadToDoList();

    //dateEdit 초기화
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setDisplayFormat("yyyy-MM-dd");
    ui->dateEdit->setCalendarPopup(true);
}

//일정 불러오기
void ToDoList::loadToDoList()
{
    ui->listWidget->clear();

    QList<ScheduleItem> items = m_dbManager->getAllSchedules();

    for(const ScheduleItem& item : items)
    {
        QListWidgetItem* listItem = new QListWidgetItem(item.title);



        listItem->setData(Qt::UserRole, item.id);

        ui->listWidget->addItem(listItem);
    }
}

void ToDoList::onToDoItemClicked(QListWidgetItem* item)
{
    if (!item)
        return;

    int id = item->data(Qt::UserRole).toInt();

    ScheduleItem schedule = m_dbManager->getScheduleById(id);

    m_selectedId = id;
    showToDoDetail(schedule);
}

void ToDoList::showToDoDetail(const ScheduleItem& item)
{
    ui->title->setText(item.title);
    ui->content->setPlainText(item.content);
    ui->status->setCurrentText(item.status);
    ui->dateEdit->setDate(item.date);
}

void ToDoList::addButtonClicked()
{
    //기존 선택된 id와 QListWidget 확실한 초기화
    m_selectedId = -1;
    ui->listWidget->setCurrentRow(-1);

    ui->title->clear();
    ui->content->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->status->setCurrentIndex(0);
}

void ToDoList::saveButtonClicked()
{
    ScheduleItem item;
    item.title = ui->title->text().trimmed();
    item.content = ui->content->toPlainText().trimmed();
    item.date = ui->dateEdit->date();
    item.status = ui->status->currentText();
    item.contentDetail = "";

    if (item.title.isEmpty()) {
        return;
    }

    if (item.status == "선택") {
        return;
    }

    bool ok = false;

    if (m_selectedId == -1) {
        ok = m_dbManager->addSchedule(item);
    } else {
        item.id = m_selectedId;
        ok = m_dbManager->updateSchedule(item);
    }

    if (!ok) {
        return;
    }

    //일정 저장/수정 후 다시 로드
    loadToDoList();
    addButtonClicked();
    //캘린더의 초기화슬롯함수 실행시키기 위하여
    emit scheduleChanged();
}

void ToDoList::deleteButtonClicked()
{
    if (m_selectedId == -1) {
        return;
    }

    bool ok = m_dbManager->deleteSchedule(m_selectedId);
    if (!ok) {
        return;
    }

    loadToDoList();
    addButtonClicked();
    emit scheduleChanged();
}

ToDoList::~ToDoList()
{
    delete ui;
}
