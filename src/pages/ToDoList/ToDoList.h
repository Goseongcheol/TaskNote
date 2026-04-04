#ifndef TODOLIST_H
#define TODOLIST_H

#include <QWidget>
#include <QListWidgetItem>
#include "ScheduleItem.h"

class DatabaseManager;

namespace Ui {
class ToDoList;
}

class ToDoList : public QWidget
{
    Q_OBJECT

public:
    explicit ToDoList(DatabaseManager* dbManager, QWidget *parent = nullptr);
    ~ToDoList();

private slots:
    void onToDoItemClicked(QListWidgetItem* item);
    void addButtonClicked();
    void saveButtonClicked();
    void deleteButtonClicked();

signals:
    void scheduleChanged();

private:
    Ui::ToDoList *ui;
    int m_selectedId = -1;

    DatabaseManager* m_dbManager;
    void loadToDoList();
    void showToDoDetail(const ScheduleItem& item);

};

#endif // TODOLIST_H
