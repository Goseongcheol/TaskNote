#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "PageManager.h"
#include "PageID.h"

#include "Calendar.h"
#include "ToDoList.h"
#include "Memo.h"
#include "Setting.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pageManager(nullptr)
    , m_calendar(nullptr)
    , m_todoList(nullptr)
    , m_memo(nullptr)
    , m_setting(nullptr)
{
    ui->setupUi(this);

    m_pageManager = new PageManager(ui->stackedWidget, this);

    setupPages();
    setupConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupPages()
{
    m_calendar = new Calendar(this);
    m_todoList = new ToDoList(this);
    m_memo = new Memo(this);
    m_setting = new Setting(this);

    m_pageManager->registerPage(PageID::Calendar, m_calendar);
    m_pageManager->registerPage(PageID::TodoList, m_todoList);
    m_pageManager->registerPage(PageID::Memo, m_memo);
    m_pageManager->registerPage(PageID::Setting, m_setting);

    //초기 스택위젯 페이지
    m_pageManager->goTo(PageID::Calendar);

}

// 페이지 연결 생성자 분리
void MainWindow::setupConnections()
{
    connect(ui->calendarButton, &QPushButton::clicked,this,[this]()
            {m_pageManager->goTo(PageID::Calendar);
    });
    connect(ui->todoButton, &QPushButton::clicked,this,[this]()
            {m_pageManager->goTo(PageID::TodoList);
            });
    connect(ui->memoButton, &QPushButton::clicked,this,[this]()
            {m_pageManager->goTo(PageID::Memo);
            });
    connect(ui->settingButton, &QPushButton::clicked,this,[this]()
            {m_pageManager->goTo(PageID::Setting);
            });

}


