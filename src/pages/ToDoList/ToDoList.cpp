#include "ToDoList.h"
#include "ui_ToDoList.h"

ToDoList::ToDoList(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ToDoList)
{
    ui->setupUi(this);
}

ToDoList::~ToDoList()
{
    delete ui;
}
