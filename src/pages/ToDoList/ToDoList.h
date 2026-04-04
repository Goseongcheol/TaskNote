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
    DatabaseManager* m_dbManager;

    // 초기로딩
    void loadToDoList();
    void addItemToList(QListWidget* listWidget, const ScheduleItem& item);

    // 항목 선택
    int m_selectedId = -1;
    void showToDoDetail(const ScheduleItem& item);

    //다중 리스트위젯 선택 해제
    QList<QListWidget*> allListWidgets() const;
    void clearOtherListSelections(QListWidget* activeList);

    //스타일시트
    void setupStyle();
};

#endif // TODOLIST_H
