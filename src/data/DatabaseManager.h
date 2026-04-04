#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QList>
#include <QDate>
#include "ScheduleItem.h"


class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseManager(QObject* parent = nullptr);
    ~DatabaseManager();

    bool initialize();

    QList<ScheduleItem> getSchedulesByDate(const QDate& date);
    QList<ScheduleItem> getSchedulesInRange(const QDate& startDate, const QDate& endDate);

    QList<ScheduleItem> getAllSchedules();

    ScheduleItem getScheduleById(int id);

    bool addSchedule(const ScheduleItem& item);
    bool updateSchedule(const ScheduleItem& item);
    bool deleteSchedule(int id);

private:
    QString databasePath() const;
    bool openDatabase();
    bool createTables();

private:
    QSqlDatabase m_db;
    const QString m_connectionName = "tasknote_connection";
};
#endif // DATABASEMANAGER_H
