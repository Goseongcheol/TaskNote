#ifndef CALENDAR_H
#define CALENDAR_H

#include "CalendarCellWidget.h"

#include <QWidget>
#include <QVector>
#include <QDate>
#include "ScheduleItem.h"

class DatabaseManager;

namespace Ui {
class Calendar;
}

class Calendar : public QWidget
{
    Q_OBJECT

public:
    explicit Calendar(DatabaseManager* dbManager, QWidget *parent = nullptr);
    ~Calendar();


private slots:
    void onPrevMonthClicked();
    void onNextMonthClicked();
    void onCellClicked(const QDate& date);
    void updateScheduleView(const QDate& date);

    void showScheduleDetail(int row, int column);

private:
    Ui::Calendar *ui;

    void createCalendarCells();
    void updateCalendar();
    void updateHeader();
    void updateSelection();
    void displayScheduleTable(const QList<ScheduleItem>& schedules);

    DatabaseManager* m_dbManager;
    QVector<CalendarCellWidget*> m_cells;
    QDate m_currentMonth;
    QDate m_selectedDate;
    QList<ScheduleItem> m_scheduleItems;
};

#endif // CALENDAR_H
