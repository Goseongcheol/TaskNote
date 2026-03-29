#include "CalendarCellWidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>

CalendarCellWidget::CalendarCellWidget(QWidget *parent)
    : QFrame(parent)
{
    setObjectName("calendarCell");
    setFrameShape(QFrame::StyledPanel);
    setMinimumSize(100, 80);
    setAttribute(Qt::WA_Hover, true);

    m_dayLabel = new QLabel(this);
    m_dayLabel->setObjectName("dayLabel");
    m_dayLabel->setAlignment(Qt::AlignCenter | Qt::AlignCenter);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 6, 8, 6);
    layout->addWidget(m_dayLabel);
    layout->addStretch();

    updateStyle();
}


void CalendarCellWidget::setDate(const QDate &date)
{
    m_date = date;
    m_dayLabel->setText(QString::number(date.day()));
}

QDate CalendarCellWidget::date() const
{
    return m_date;
}

void CalendarCellWidget::setCurrentMonth(bool currentMonth)
{
    m_isCurrentMonth = currentMonth;
    updateStyle();
}

//cliecked 탔을떄 이벤트
void CalendarCellWidget::setSelected(bool selected)
{
    m_isSelected = selected;
    updateStyle();
}

//마우스 눌림 이벤트
void CalendarCellWidget::mousePressEvent(QMouseEvent *event)
{
    QFrame::mousePressEvent(event);

    if (event->button() == Qt::LeftButton) {
        emit clicked(m_date);
    }
}

void CalendarCellWidget::updateStyle()
{
    QString textColor = m_isCurrentMonth ? "#222222" : "#AAAAAA";
    QString bgColor = m_isSelected ? "#BFE3FF" : "#FFFFFF";
    QString borderColor = m_isSelected ? "#3399FF" : "#DDDDDD";
    QString hoverColor = m_isSelected ? "#A7D8FF" : "#F5F9FF";

    setStyleSheet(QString(
                      "QFrame#calendarCell {"
                      " background-color: %1;"
                      " border: 1px solid %2;"
                      " border-radius: 6px;"
                      "}"

                      "QFrame#calendarCell:hover {"
                      " background-color: %4;"
                      " border: 1px solid #66B2FF;"
                      "}"

                      "QLabel#dayLabel {"
                      " color: %3;"
                      " font-size: 14px;"
                      " font-weight: 600;"
                      " border: none;"
                      " background-color: white;"
                      " padding: 0px;"
                      "}"
                      ).arg(bgColor, borderColor, textColor, hoverColor));
}