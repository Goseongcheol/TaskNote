#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H

#include <QObject>
#include <QMap>
#include "PageID.h"


class QStackedWidget;
class QWidget;

class PageManager : public QObject
{
    Q_OBJECT

public:
    explicit PageManager(QStackedWidget* stack,QObject *parent = nullptr);

    void registerPage(PageID id, QWidget* page);
    void goTo(PageID id);
    PageID currentPage() const;

private:
    QStackedWidget* m_stack;
    QMap<PageID, QWidget*> m_pages;
    PageID m_currentPage;

};

#endif // PAGEMANAGER_H
