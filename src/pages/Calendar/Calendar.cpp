#include "Calendar.h"
#include "ui_Calendar.h"

#include <QGridLayout>
#include <QLabel>

Calendar::Calendar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Calendar)
    , m_currentMonth(QDate::currentDate())
    , m_selectedDate(QDate::currentDate())
{
    ui->setupUi(this);
    createCalendarCells();
    updateCalendar();

    connect(ui->prevMonthButton, &QPushButton::clicked,
            this, &Calendar::onPrevMonthClicked);

    connect(ui->nextMonthButton, &QPushButton::clicked,
            this, &Calendar::onNextMonthClicked);
}

void Calendar::createCalendarCells()
{
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 7; ++col) {
            auto* cell = new CalendarCellWidget(this);
            ui->calendarGridLayout->addWidget(cell, row, col);
            m_cells.append(cell);

            //캘린더 셀 위젯의 클릭 시그널 connect
            connect(cell, &CalendarCellWidget::clicked,
                    this, &Calendar::onCellClicked);
        }
    }
}

void Calendar::updateCalendar()
{
    QDate firstDay(m_currentMonth.year(), m_currentMonth.month(), 1);

    int dayOfWeek = firstDay.dayOfWeek();
    // 월=1 ... 일=7

    QDate startDate = firstDay.addDays(-(dayOfWeek - 1));

    for (int i = 0; i < m_cells.size(); ++i) {
        QDate cellDate = startDate.addDays(i);

        m_cells[i]->setDate(cellDate);
        m_cells[i]->setCurrentMonth(cellDate.month() == m_currentMonth.month() &&
                                    cellDate.year() == m_currentMonth.year());
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

    qDebug() << m_selectedDate ;
    // 이전/다음 달 날짜를 눌렀을 때 그 달로 이동하고 싶으면 아래 유지
    m_currentMonth = QDate(date.year(), date.month(), 1);

    updateCalendar();
}



Calendar::~Calendar()
{
    delete ui;
}
