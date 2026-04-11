#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "PageManager.h"
#include "PageID.h"

#include "Calendar.h"
#include "ToDoList.h"
#include "Memo.h"
#include "Setting.h"
#include "DatabaseManager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pageManager(nullptr)
    , m_calendar(nullptr)
    , m_todoList(nullptr)
    , m_memo(nullptr)
    , m_setting(nullptr)
    , m_dbManager(nullptr)
{
    ui->setupUi(this);

    setWindowTitle("TaskNote");
    setWindowIcon(QIcon(":/src/resources/tasknote.ico"));

    m_pageManager = new PageManager(ui->stackedWidget, this);

    setupPages();
    setupConnections();
    applyTheme(ThemeMode::Light);

    connect(m_todoList, &ToDoList::scheduleChanged,
            m_calendar, &Calendar::updateToDo);
    //기본 테마모드 화이트로 ini파일로 테마 저장 미구현
    connect(m_setting, &Setting::themeChanged,
            this, &MainWindow::applyTheme);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupPages()
{
    m_memo = new Memo(this);
    m_setting = new Setting(this);
    m_dbManager = new DatabaseManager(this);

    m_dbManager->initialize();

    // dbManager생성후 calendar에 dbmanager 전달
    m_calendar = new Calendar(m_dbManager, this);
    m_todoList = new ToDoList(m_dbManager, this);



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

void MainWindow::applyTheme(ThemeMode mode)
{
    if (mode == ThemeMode::Light) {
        qApp->setStyleSheet(R"(
        QWidget {
            background-color: rgb(248,248,248);
            color: black;
        }

        QPushButton {
            min-width: 60px;
            min-height: 32px;
            background-color: white;
            color: black;
            border: 1px solid rgb(210,210,210);
            border-radius: 10px;
            padding: 8px 16px;
            font-size: 14px;
            font-weight: 600;
        }

        QPushButton:hover {
            background-color: rgb(242,246,252);
            border: 1px solid rgb(120,170,255);
        }

        QPushButton:pressed {
            background-color: rgb(225,235,250);
            padding-left: 17px;
            padding-top: 9px;
        }

        QLabel {
            background: transparent;
            color: rgb(35,35,35);
            font-size: 14px;
            padding: 2px;
        }

        QRadioButton {
                spacing: 8px;
            font-size: 14px;
            background: transparent;
        }

        QRadioButton::indicator {
            width: 16px;
            height: 16px;
        }

        QRadioButton::indicator:unchecked {
            border: 1px solid rgb(130,130,130);
            border-radius: 8px;
            background-color: white;
        }

        QRadioButton::indicator:checked {
            border: 1px solid rgb(70,130,255);
            border-radius: 8px;
            background-color: rgb(70,130,255);
        }
    )");
    }
    else {
        qApp->setStyleSheet(R"(
        QWidget {
            background-color: rgb(35,35,35);
            color: white;
        }

        QPushButton {
            min-width: 60px;
            min-height: 32px;
            background-color: rgb(55,55,55);
            color: white;
            border: 1px solid rgb(90,90,90);
            border-radius: 10px;
            padding: 8px 16px;
            font-size: 14px;
            font-weight: 600;
        }

        QPushButton:hover {
            background-color: rgb(70,70,70);
            border: 1px solid rgb(100,160,255);
        }

        QPushButton:pressed {
            background-color: rgb(85,85,85);
            padding-left: 17px;
            padding-top: 9px;
        }

        QLabel {
            background: transparent;
            color: rgb(235,235,235);
            font-size: 14px;
            padding: 2px;
        }

        QRadioButton {
                spacing: 8px;
            font-size: 14px;
            background: transparent;
        }

        QRadioButton::indicator {
            width: 16px;
            height: 16px;
        }

        QRadioButton::indicator:unchecked {
            border: 1px solid rgb(130,130,130);
            border-radius: 8px;
            background-color: white;
        }

        QRadioButton::indicator:checked {
            border: 1px solid rgb(70,130,255);
            border-radius: 8px;
            background-color: rgb(70,130,255);
        }
    )");
    }
}

