#ifndef TODOLISTITEMWIDGET_H
#define TODOLISTITEMWIDGET_H

#include <QWidget>
#include <QLabel>

class ToDoListItemWidget: public QWidget
{
    Q_OBJECT


public:
    explicit ToDoListItemWidget(QWidget *parent = nullptr);
    void setData(const QString& title, const QString& date, const QString& status);

private:
    QLabel* m_titleLabel;
    QLabel* m_dateLabel;
    QLabel* m_statusLabel;
};

#endif // TODOLISTITEMWIDGET_H
