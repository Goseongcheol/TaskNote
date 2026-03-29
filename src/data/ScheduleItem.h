#ifndef SCHEDULEITEM_H
#define SCHEDULEITEM_H

#include <QDate>
#include <QString>

struct ScheduleItem
{
    int id = -1;
    QDate date;
    QString title;
    QString status;
    QString content;
    QString contentDetail;
    QDateTime createdAt;
    QDateTime updatedAt;
};

#endif // SCHEDULEITEM_H
