#include "Setting.h"
#include "ui_Setting.h"

Setting::Setting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Setting)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &Setting::applyButtonClicked);
}

void Setting::applyButtonClicked()
{
    if (ui->whiteRadioButton->isChecked()) {
        emit themeChanged(ThemeMode::Light);
    } else if (ui->darkRadioButton->isChecked()) {
        emit themeChanged(ThemeMode::Dark);
    }
}

Setting::~Setting()
{
    delete ui;
}
