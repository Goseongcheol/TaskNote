#include "ToDoList.h"
#include "ui_ToDoList.h"
#include "DatabaseManager.h"
#include "ToDoListItemWidget.h"
#include <QTimer>

ToDoList::ToDoList(DatabaseManager* dbManager, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ToDoList)
    , m_dbManager(dbManager)
{
    ui->setupUi(this);

    //각 버튼 connect
    connect(ui->wait_ListWidget, &QListWidget::itemClicked, this, &ToDoList::onToDoItemClicked);
    connect(ui->ing_ListWidget, &QListWidget::itemClicked, this, &ToDoList::onToDoItemClicked);
    connect(ui->done_ListWidget, &QListWidget::itemClicked, this, &ToDoList::onToDoItemClicked);
    connect(ui->stop_ListWidget, &QListWidget::itemClicked, this, &ToDoList::onToDoItemClicked);


    connect(ui->add_Button, &QPushButton::clicked, this, &ToDoList::addButtonClicked);
    connect(ui->save_Button, &QPushButton::clicked, this, &ToDoList::saveButtonClicked);
    connect(ui->delete_Button, &QPushButton::clicked, this, &ToDoList::deleteButtonClicked);

    //일정 dbManager에서 초기화(가져오기)
    QTimer::singleShot(0, this, [this]() {
        loadToDoList();
    });

    //dateEdit 초기화
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setDisplayFormat("yyyy-MM-dd");
    ui->dateEdit->setCalendarPopup(true);

    setupStyle();

}

QList<QListWidget*> ToDoList::allListWidgets() const
{
    return {
        ui->wait_ListWidget,
        ui->ing_ListWidget,
        ui->done_ListWidget,
        ui->stop_ListWidget
    };
}

void ToDoList::clearOtherListSelections(QListWidget* activeList)
{
    const QList<QListWidget*> lists = allListWidgets();

    for (QListWidget* list : lists)
    {
        if (!list)
            continue;

        if (list == activeList)
            continue;

        list->clearSelection();
        list->setCurrentItem(nullptr);
        list->clearFocus();
    }
}

//일정 불러오기
void ToDoList::loadToDoList()
{
    ui->wait_ListWidget->clear();
    ui->ing_ListWidget->clear();
    ui->done_ListWidget->clear();
    ui->stop_ListWidget->clear();



    QList<ScheduleItem> items = m_dbManager->getAllSchedules();

    for (const ScheduleItem& item : items)
    {
        if (item.status == "대기") {
            addItemToList(ui->wait_ListWidget, item);
        }
        else if (item.status == "진행 중") {
            addItemToList(ui->ing_ListWidget, item);
        }
        else if (item.status == "완료") {
            addItemToList(ui->done_ListWidget, item);
        }
        else if (item.status == "보류") {
            addItemToList(ui->stop_ListWidget, item);
        }
    }
}

//상태 별로 listWidget에 전달
void ToDoList::addItemToList(QListWidget* listWidget, const ScheduleItem& item)
{
    QListWidgetItem* listItem = new QListWidgetItem();
    listItem->setData(Qt::UserRole, item.id);

    ToDoListItemWidget* widget = new ToDoListItemWidget;
    widget->setData(
        item.title,
        item.date.toString("yyyy-MM-dd"),
        item.status
        );

    listItem->setSizeHint(QSize(listWidget->viewport()->width() - 16, 82));

    listWidget->addItem(listItem);
    listWidget->setItemWidget(listItem, widget);
}

void ToDoList::onToDoItemClicked(QListWidgetItem* item)
{
    if (!item)
        return;

    QListWidget* clickedList = qobject_cast<QListWidget*>(sender());
    if (!clickedList)
        return;

    clearOtherListSelections(clickedList);

    //userRole 에 id 저장
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
    ui->wait_ListWidget->setCurrentRow(-1);
    ui->ing_ListWidget->setCurrentRow(-1);
    ui->done_ListWidget->setCurrentRow(-1);
    ui->stop_ListWidget->setCurrentRow(-1);

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

//스타일시트 선언 부
void ToDoList::setupStyle()
{
    this->setStyleSheet(
        "QListWidget#wait_ListWidget {"
        "    background-color: rgb(245, 240, 235);"
        "    border: 1px solid rgb(210, 200, 190);"
        "    border-radius: 12px;"
        "    outline: none;"
        "    padding: 8px;"
        "}"

        "QListWidget#ing_ListWidget {"
        "    background-color: rgb(235, 245, 255);"
        "    border: 1px solid rgb(190, 215, 240);"
        "    border-radius: 12px;"
        "    outline: none;"
        "    padding: 8px;"
        "}"

        "QListWidget#done_ListWidget {"
        "    background-color: rgb(238, 248, 240);"
        "    border: 1px solid rgb(195, 220, 200);"
        "    border-radius: 12px;"
        "    outline: none;"
        "    padding: 8px;"
        "}"

        "QListWidget#stop_ListWidget {"
        "    background-color: rgb(250, 242, 236);"
        "    border: 1px solid rgb(220, 205, 195);"
        "    border-radius: 12px;"
        "    outline: none;"
        "    padding: 8px;"
        "}"

        "QListWidget::item {"
        "    border: 1px solid transparent;"
        "    border-radius: 10px;"
        "    background: transparent;"
        "    margin: 4px 0px 4px 0px;"
        "}"

        "QListWidget::item:selected {"
        "    border: 4px solid rgb(80, 150, 255);"
        "    border-radius: 10px;"
        "    background-color: rgba(80, 150, 255, 20);"
        "}"

        "QLineEdit, QDateEdit, QComboBox, QTextEdit {"
        "    background-color: rgb(252, 252, 252);"
        "    border: 1px solid rgb(205, 210, 215);"
        "    border-radius: 8px;"
        "    padding: 6px 10px;"
        "    color: rgb(40, 40, 40);"
        "    selection-background-color: rgb(80, 150, 255);"
        "}"

        "QPushButton {"
        "    min-height: 32px;"
        "    padding: 6px 14px;"
        "    border-radius: 8px;"
        "    border: 1px solid rgb(200, 200, 200);"
        "    background-color: rgb(250, 250, 250);"
        "    color: rgb(45, 45, 45);"
        "    font-weight: 600;"
        "}"

        "QPushButton:hover {"
        "    background-color: rgb(240, 245, 250);"
        "}"

        "QPushButton:pressed {"
        "    background-color: rgb(225, 232, 240);"
        "}"

        "QScrollBar:vertical {"
        "    background: transparent;"
        "    width: 10px;"
        "    margin: 2px;"
        "}"

        "QScrollBar::handle:vertical {"
        "    background: rgb(190, 190, 190);"
        "    border-radius: 5px;"
        "    min-height: 20px;"
        "}"

        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "}"

        "QComboBox::drop-down, QDateEdit::drop-down {"
        "    subcontrol-origin: padding;"
        "    subcontrol-position: top right;"
        "    width: 24px;"
        "    border: none;"
        "    background: transparent;"
        "}"

        "QComboBox::down-arrow, QDateEdit::down-arrow {"
        "    image: none;"
        "    width: 8px;"
        "    height: 8px;"
        "    border-left: 2px solid rgb(100, 100, 100);"
        "    border-bottom: 2px solid rgb(100, 100, 100);"
        "    transform: rotate(-45deg);"
        "}"
        );
}