#include "Memo.h"
#include "ui_Memo.h"

Memo::Memo(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Memo)
{
    ui->setupUi(this);
}

Memo::~Memo()
{
    delete ui;
}
