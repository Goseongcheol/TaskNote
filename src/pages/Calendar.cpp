#include "Calendar.h"
#include "ui_Calendar.h"

#include <QDate>
#include <QCalendar>

Calendar::Calendar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Calendar)
{
    ui->setupUi(this);

}

Calendar::~Calendar()
{
    delete ui;
}
