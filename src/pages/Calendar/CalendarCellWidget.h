#ifndef CALENDARCELLWIDGET_H
#define CALENDARCELLWIDGET_H

#include <QFrame>
#include <QDate>
#include <QStringList>

class QLabel;

class CalendarCellWidget : public QFrame
{
    Q_OBJECT

public:
    explicit CalendarCellWidget(QWidget *parent = nullptr);

    void setDate(const QDate& date);
    QDate date() const;

    void setCurrentMonth(bool currentMonth);
    void setSelected(bool selected);
    void setScheduleTitles(const QStringList& titles);

signals:
    void clicked(const QDate& date);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    void updateStyle();

    QStringList m_scheduleTitles;
    QLabel* m_dayLabel;
    QLabel* m_scheduleLabel;
    QDate m_date;
    bool m_isCurrentMonth = true;
    bool m_isSelected = false;

};

#endif // CALENDARCELLWIDGET_H
