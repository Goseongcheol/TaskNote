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

    // 날짜 라벨
    m_dayLabel = new QLabel(this);
    m_dayLabel->setObjectName("dayLabel");
    m_dayLabel->setAlignment(Qt::AlignCenter | Qt::AlignCenter);

    // 일정 미리보기 라벨 (추가)
    m_scheduleLabel = new QLabel(this);
    m_scheduleLabel->setObjectName("scheduleLabel");
    m_scheduleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_scheduleLabel->setVisible(false);

    // 레이아웃
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 6, 8, 6);
    layout->addWidget(m_dayLabel);
    layout->addWidget(m_scheduleLabel);   // 추가
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

void CalendarCellWidget::setSelected(bool selected)
{
    m_isSelected = selected;
    updateStyle();
}

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

                      "QLabel#scheduleLabel {"
                      " background-color: #148a43;"
                      " color: white;"
                      " border-radius: 6px;"
                      " padding-left: 6px;"
                      " min-height: 20px;"
                      " font-size: 11px;"
                      "}"
                      ).arg(bgColor, borderColor, textColor, hoverColor));
}

void CalendarCellWidget::setScheduleTitles(const QStringList& titles)
{
    m_scheduleTitles = titles;

    if (titles.isEmpty()) {
        m_scheduleLabel->hide();
        return;
    }

    QString text;

    if (titles.size() == 1) {
        text = titles.first();
    } else {
        text = QString("%1 외 %2건")
                   .arg(titles.first())
                   .arg(titles.size() - 1);
    }

    m_scheduleLabel->setText(text);
    m_scheduleLabel->show();
}