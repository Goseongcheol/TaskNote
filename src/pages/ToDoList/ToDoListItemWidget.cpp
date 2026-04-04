#include "ToDoListItemWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

ToDoListItemWidget::ToDoListItemWidget(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    m_titleLabel = new QLabel(this);
    m_dateLabel = new QLabel(this);
    m_statusLabel = new QLabel(this);

    QFont titleFont;
    titleFont.setPointSize(11);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);

    QFont subFont;
    subFont.setPointSize(9);
    m_dateLabel->setFont(subFont);
    m_statusLabel->setFont(subFont);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 10, 12, 10);
    mainLayout->setSpacing(6);

    QHBoxLayout* bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(m_dateLabel);
    bottomLayout->addStretch();
    bottomLayout->addWidget(m_statusLabel);

    mainLayout->addWidget(m_titleLabel);
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);
}

//데이터 전달 setData
void ToDoListItemWidget::setData(const QString& title, const QString& date, const QString& status)
{
    m_titleLabel->setText(title);
    m_dateLabel->setText(date);
    m_statusLabel->setText(status);

    if (status == "대기") {
        setStyleSheet(
            "background-color: rgb(96, 92, 86);"
            "border: 1px solid rgb(120, 114, 106);"
            "border-radius: 10px;"
            );
        m_statusLabel->setStyleSheet("color: rgb(225, 225, 225); border: none; background: transparent;");
    }
    else if (status == "진행 중") {
        setStyleSheet(
            "background-color: rgb(57, 96, 145);"
            "border: 1px solid rgb(76, 120, 175);"
            "border-radius: 10px;"
            );
        m_statusLabel->setStyleSheet("color: rgb(225, 235, 255); border: none; background: transparent;");
    }
    else if (status == "완료") {
        setStyleSheet(
            "background-color: rgb(52, 110, 72);"
            "border: 1px solid rgb(68, 132, 89);"
            "border-radius: 10px;"
            );
        m_statusLabel->setStyleSheet("color: rgb(210, 255, 215); border: none; background: transparent;");
    }
    else if (status == "보류") {
        setStyleSheet(
            "background-color: rgb(125, 87, 60);"
            "border: 1px solid rgb(150, 105, 74);"
            "border-radius: 10px;"
            );
        m_statusLabel->setStyleSheet("color: rgb(255, 220, 200); border: none; background: transparent;");
    }
    else {
        setStyleSheet(
            "background-color: rgb(96, 92, 86);"
            "border: 1px solid rgb(120, 114, 106);"
            "border-radius: 10px;"
            );
        m_statusLabel->setStyleSheet("color: rgb(225, 225, 225); border: none; background: transparent;");
    }

    m_titleLabel->setStyleSheet("color: white; border: none; background: transparent;");
    m_dateLabel->setStyleSheet("color: rgb(240, 240, 240); border: none; background: transparent;");
}