#include "DatabaseManager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDebug>
#include <QDateTime>

DatabaseManager::DatabaseManager(QObject* parent)
    : QObject(parent)
{
}

DatabaseManager::~DatabaseManager()
{
    if (m_db.isValid()) {
        if (m_db.isOpen()) {
            m_db.close();
        }
    }

    QSqlDatabase::removeDatabase(m_connectionName);
}

bool DatabaseManager::initialize()
{
    if (!openDatabase()) {
        return false;
    }

    if (!createTables()) {
        return false;
    }

    return true;
}

QString DatabaseManager::databasePath() const
{
    QString basePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(basePath);

    if (!dir.exists()) {
        dir.mkpath(".");
    }

    return dir.filePath("TaskNote.sqlite");
}

bool DatabaseManager::openDatabase()
{
    if (m_db.isValid() && m_db.isOpen()) {
        return true;
    }


    QString dbPath = databasePath();


    if (QSqlDatabase::contains(m_connectionName)) {
        m_db = QSqlDatabase::database(m_connectionName);
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
    }

    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        return false;
    }

    return true;
}

bool DatabaseManager::createTables()
{
    if (!m_db.isOpen()) {
        return false;
    }

    QSqlQuery query(m_db);

    QString sql =
        "CREATE TABLE IF NOT EXISTS schedules ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "date TEXT NOT NULL, "
        "title TEXT, "
        "status TEXT, "
        "content TEXT, "
        "contentdetail TEXT, "
        "created_at TEXT DEFAULT CURRENT_TIMESTAMP, "
        "updated_at TEXT DEFAULT CURRENT_TIMESTAMP"
        ");";

    if (!query.exec(sql)) {

        return false;
    }

    return true;
}

QList<ScheduleItem> DatabaseManager::getSchedulesByDate(const QDate& date)
{
    QList<ScheduleItem> list;

    if (!m_db.isOpen()) {
        return list;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT id, date, title, status, content, contentdetail, created_at, updated_at
        FROM schedules
        WHERE date = :date
        ORDER BY id ASC
    )");
    query.bindValue(":date", date.toString("yyyy-MM-dd"));

    if (!query.exec()) {
        return list;
    }

    while (query.next()) {

        ScheduleItem item;
        item.id = query.value(0).toInt();
        item.date = QDate::fromString(query.value(1).toString(), "yyyy-MM-dd");
        item.title = query.value(2).toString();
        item.status = query.value(3).toString();
        item.content = query.value(4).toString();
        item.contentDetail = query.value(5).toString();
        item.createdAt = QDateTime::fromString(query.value(6).toString(), Qt::ISODate);
        item.updatedAt = QDateTime::fromString(query.value(7).toString(), Qt::ISODate);
        list.append(item);
    }

    return list;
}

QList<ScheduleItem> DatabaseManager::getSchedulesInRange(const QDate& startDate, const QDate& endDate)
{
    QList<ScheduleItem> list;

    if (!m_db.isOpen()) {
        return list;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT id, date, title, status, content, contentdetail, created_at, updated_at
        FROM schedules
        WHERE date >= :startDate AND date <= :endDate
        ORDER BY date ASC, id ASC
    )");
    query.bindValue(":startDate", startDate.toString("yyyy-MM-dd"));
    query.bindValue(":endDate", endDate.toString("yyyy-MM-dd"));

    if (!query.exec()) {
        return list;
    }

    while (query.next()) {
        ScheduleItem item;
        item.id = query.value(0).toInt();
        item.date = QDate::fromString(query.value(1).toString(), "yyyy-MM-dd");
        item.title = query.value(2).toString();
        item.status = query.value(3).toString();
        item.content = query.value(4).toString();
        item.contentDetail = query.value(5).toString();
        item.createdAt = QDateTime::fromString(query.value(6).toString(), Qt::ISODate);
        item.updatedAt = QDateTime::fromString(query.value(7).toString(), Qt::ISODate);

        list.append(item);
    }

    return list;
}

bool DatabaseManager::addSchedule(const QDate& date, const QString& title, const QString& content)
{
    if (!m_db.isOpen()) {
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(
        "INSERT INTO schedules (date, title, status, content, contentdetail) "
        "VALUES (:date, :title, :status, :content, :contentdetail)"
        );
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    query.bindValue(":title", title);
    query.bindValue(":status", "NONE");
    query.bindValue(":content", content);
    query.bindValue(":contentdetail", "");

    if (!query.exec()) {
        return false;
    }

    return true;
}

bool DatabaseManager::updateSchedule(int id, const QString& title, const QString& content)
{
    if (!m_db.isOpen()) {
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(
        "UPDATE schedules "
        "SET title = :title, content = :content, updated_at = CURRENT_TIMESTAMP "
        "WHERE id = :id"
        );
    query.bindValue(":title", title);
    query.bindValue(":content", content);
    query.bindValue(":id", id);

    if (!query.exec()) {

        return false;
    }

    return true;
}

bool DatabaseManager::deleteSchedule(int id)
{
    if (!m_db.isOpen()) {
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("DELETE FROM schedules WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        return false;
    }

    return true;
}