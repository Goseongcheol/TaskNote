#ifndef CALENDAR_H
#define CALENDAR_H

#include "CalendarCellWidget.h"

#include <QWidget>
#include <QVector>
#include <QDate>

namespace Ui {
class Calendar;
}

class Calendar : public QWidget
{
    Q_OBJECT

public:
    explicit Calendar(QWidget *parent = nullptr);
    ~Calendar();


private slots:
    void onPrevMonthClicked();
    void onNextMonthClicked();
    void onCellClicked(const QDate& date);

private:
    Ui::Calendar *ui;

    void createCalendarCells();
    void updateCalendar();
    void updateHeader();
    void updateSelection();

    QVector<CalendarCellWidget*> m_cells;
    QDate m_currentMonth;
    QDate m_selectedDate;
};

#endif // CALENDAR_H
