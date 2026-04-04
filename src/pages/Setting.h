#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include "ThemeMode.h"

namespace Ui {
class Setting;
}

class Setting : public QWidget
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = nullptr);
    ~Setting();

signals:
    void themeChanged(ThemeMode mode);


private:
    Ui::Setting *ui;
    void applyButtonClicked();
};

#endif // SETTING_H
