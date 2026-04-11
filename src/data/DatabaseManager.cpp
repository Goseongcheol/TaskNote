#include "DatabaseManager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
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

//DB PATH
QString DatabaseManager::databasePath() const
{
    QString basePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(basePath);

    if (!dir.exists()) {
        dir.mkpath(".");
    }

    return dir.filePath("TaskNote.sqlite");
}

//DB연결
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

//초기 CREATE TABLE
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

//해당 날짜 일정 조회 SELECT
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

//todoList의 모든 스케쥴 가져오기
QList<ScheduleItem> DatabaseManager::getAllSchedules()
{
    QList<ScheduleItem> list;

    if (!m_db.isOpen()) {
        return list;
    }

    QSqlQuery query(m_db);
    query.prepare(R"(
        SELECT id, date, title, status, content, contentdetail, created_at, updated_at
        FROM schedules
        ORDER BY id ASC
    )");

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

// ToDoList 상세 표시 조회용
ScheduleItem DatabaseManager::getScheduleById(int id)
{
    ScheduleItem item;

    QSqlQuery query(m_db);
    query.prepare("SELECT id, date, title, status, content, contentdetail, created_at, updated_at "
                  "FROM schedules WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec())
        return item;

    if (query.next())
    {
        item.id = query.value("id").toInt();
        item.date = QDate::fromString(query.value("date").toString(), "yyyy-MM-dd");
        item.title = query.value("title").toString();
        item.status = query.value("status").toString();
        item.content = query.value("content").toString();
        item.contentDetail = query.value("contentdetail").toString();
        item.createdAt = QDateTime::fromString(query.value("created_at").toString(), Qt::ISODate);
        item.updatedAt = QDateTime::fromString(query.value("updated_at").toString(), Qt::ISODate);
    }

    return item;
}


//해당 월 데이터 전부 조회
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

//TODOLIST 추가
bool DatabaseManager::addSchedule(const ScheduleItem& item)
{
    if (!m_db.isOpen()) {
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(
        "INSERT INTO schedules "
        "(date, title, status, content, contentdetail, created_at, updated_at) "
        "VALUES (:date, :title, :status, :content, :contentdetail, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)"
        );

    query.bindValue(":date", item.date.toString("yyyy-MM-dd"));
    query.bindValue(":title", item.title);
    query.bindValue(":status", item.status);
    query.bindValue(":content", item.content);
    query.bindValue(":contentdetail", item.contentDetail);

    return query.exec();
}

//TODOLIST에서 수정/저장 해당 ID가 이미 존재하면 UPDATE 아니라면 INSERT로
bool DatabaseManager::updateSchedule(const ScheduleItem& item)
{
    if (!m_db.isOpen()) {
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare(
        "UPDATE schedules SET "
        "date = :date, "
        "title = :title, "
        "status = :status, "
        "content = :content, "
        "contentdetail = :contentdetail, "
        "updated_at = CURRENT_TIMESTAMP "
        "WHERE id = :id"
        );

    query.bindValue(":id", item.id);
    query.bindValue(":date", item.date.toString("yyyy-MM-dd"));
    query.bindValue(":title", item.title);
    query.bindValue(":status", item.status);
    query.bindValue(":content", item.content);
    query.bindValue(":contentdetail", item.contentDetail);

    return query.exec();
}

//TODOLIST에서 삭제
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