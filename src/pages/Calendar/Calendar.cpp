#include "Calendar.h"
#include "ui_Calendar.h"
#include "DatabaseManager.h"
#include "ScheduleItem.h"
#include "ToDoList.h"

#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include <QMap>

Calendar::Calendar(DatabaseManager* dbManager, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Calendar)
    , m_dbManager(dbManager)
    , m_currentMonth(QDate::currentDate())
    , m_selectedDate(QDate::currentDate())
{
    ui->setupUi(this);

    createCalendarCells();
    updateCalendar();

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "일정" << "상태");
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget->setStyleSheet(
        "QTableWidget::item:selected {"
        "    background-color: rgb(210, 230, 255);"
        "    color: black;"
        "}"
        );

    connect(ui->tableWidget, &QTableWidget::cellClicked,
            this, &Calendar::showScheduleDetail);

    connect(ui->prevMonthButton, &QPushButton::clicked,
            this, &Calendar::onPrevMonthClicked);

    connect(ui->nextMonthButton, &QPushButton::clicked,
            this, &Calendar::onNextMonthClicked);

    //타이머 설정없이 업데이트스케쥴뷰함수 실행시 일정을 바로 못불러오는 현상으로 변경
    QTimer::singleShot(0, this, [this]() {
        updateScheduleView(m_selectedDate);
    });
}

void Calendar::updateToDo()
{
    updateCalendar();
    QTimer::singleShot(0, this, [this]() {
        updateScheduleView(m_selectedDate);
    });
}

//캘린더 셀 추가
void Calendar::createCalendarCells()
{
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 7; ++col) {
            auto* cell = new CalendarCellWidget(this);
            ui->calendarGridLayout->addWidget(cell, row, col);
            m_cells.append(cell);

            connect(cell, &CalendarCellWidget::clicked,
                    this, &Calendar::onCellClicked);
        }
    }
}

void Calendar::updateCalendar()
{
    QDate firstDay(m_currentMonth.year(), m_currentMonth.month(), 1);

    int dayOfWeek = firstDay.dayOfWeek();
    QDate startDate = firstDay.addDays(-(dayOfWeek - 1));
    QDate endDate = startDate.addDays(41);

    QMap<QDate, QStringList> scheduleMap;

    if (m_dbManager) {
        QList<ScheduleItem> schedules = m_dbManager->getSchedulesInRange(startDate, endDate);

        for (const ScheduleItem& item : schedules) {
            scheduleMap[item.date].append(item.title);
        }
    }

    for (int i = 0; i < m_cells.size(); ++i) {
        QDate cellDate = startDate.addDays(i);

        m_cells[i]->setDate(cellDate);
        m_cells[i]->setCurrentMonth(cellDate.month() == m_currentMonth.month() &&
                                    cellDate.year() == m_currentMonth.year());
        m_cells[i]->setScheduleTitles(scheduleMap.value(cellDate));
    }

    updateHeader();
    updateSelection();
}

void Calendar::updateHeader()
{
    ui->monthLabel->setText(
        QString("%1년 %2월")
            .arg(m_currentMonth.year())
            .arg(m_currentMonth.month())
        );
}

void Calendar::updateSelection()
{
    for (auto* cell : m_cells) {
        cell->setSelected(cell->date() == m_selectedDate);
    }
}

void Calendar::onPrevMonthClicked()
{
    m_currentMonth = m_currentMonth.addMonths(-1);
    updateCalendar();
}

void Calendar::onNextMonthClicked()
{
    m_currentMonth = m_currentMonth.addMonths(1);
    updateCalendar();
}

void Calendar::onCellClicked(const QDate &date)
{
    m_selectedDate = date;
    m_currentMonth = QDate(date.year(), date.month(), 1);

    updateCalendar();
    updateScheduleView(m_selectedDate);
}

void Calendar::updateScheduleView(const QDate& date)
{
    if (!m_dbManager) {
        ui->titleLabel->setText("DB 연결 없음");
        ui->tableWidget->setRowCount(0);
        ui->textEdit->clear();
        return;
    }

    ui->titleLabel->setText(date.toString("yyyy-MM-dd"));


    m_scheduleItems = m_dbManager->getSchedulesByDate(date);


    displayScheduleTable(m_scheduleItems);

    ui->textEdit->clear();
    ui->tableWidget->clearSelection();
}

void Calendar::displayScheduleTable(const QList<ScheduleItem>& schedules)
{
    ui->tableWidget->setRowCount(schedules.size());

    for (int row = 0; row < schedules.size(); ++row) {
        const ScheduleItem& item = schedules[row];

        auto* titleItem = new QTableWidgetItem(item.title);
        auto* statusItem = new QTableWidgetItem(item.status);

        ui->tableWidget->setItem(row, 0, titleItem);
        ui->tableWidget->setItem(row, 1, statusItem);
    }

    if (schedules.isEmpty()) {
        ui->tableWidget->setRowCount(0);
    }
}

void Calendar::showScheduleDetail(int row, int column)
{
    Q_UNUSED(column);

    if (row < 0 || row >= m_scheduleItems.size()) {
        ui->textEdit->clear();
        return;
    }

    ui->textEdit->setPlainText(m_scheduleItems[row].content);
}

Calendar::~Calendar()
{
    delete ui;
}