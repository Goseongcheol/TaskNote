#include "pagemanager.h"
#include <QStackedWidget>
#include <QWidget>

PageManager::PageManager(QStackedWidget* stack, QObject* parent)
    : QObject(parent)
    , m_stack(stack)
    , m_currentPage(PageID::Calendar)
{
}

//enum 페이지 stackwidget에 등록
void PageManager::registerPage(PageID id, QWidget* page)
{
    if(!m_stack || !page)
        return;
    if(m_pages.contains(id))
        return;

    m_pages.insert(id,page);
    m_stack->addWidget(page);

}

//페이지 호출 Main Window에서 signal-slot연결 예정
void PageManager::goTo(PageID id)
{

    if(!m_pages.contains(id))
        return;

    QWidget* page = m_pages.value(id,nullptr);

    if(!page)
        return;

    m_stack->setCurrentWidget(page);
    m_currentPage = id;
}


PageID PageManager::currentPage() const
{
    return m_currentPage;
}