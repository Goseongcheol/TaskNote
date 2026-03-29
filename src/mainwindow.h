#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class PageManager;
class Calendar;
class ToDoList;
class Memo;
class Setting;
class DatabaseManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    PageManager* m_pageManager;
    Calendar* m_calendar;
    ToDoList* m_todoList;
    Memo* m_memo;
    Setting* m_setting;
    DatabaseManager* m_dbManager;

    void setupPages();
    void setupConnections();
};
#endif // MAINWINDOW_H
