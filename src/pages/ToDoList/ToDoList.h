#ifndef TODOLIST_H
#define TODOLIST_H

#include <QWidget>

namespace Ui {
class ToDoList;
}

class ToDoList : public QWidget
{
    Q_OBJECT

public:
    explicit ToDoList(QWidget *parent = nullptr);
    ~ToDoList();

private:
    Ui::ToDoList *ui;
};

#endif // TODOLIST_H
