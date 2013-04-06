#include "iconmanager.h"
//
IconManager::DBIcon::DBIcon()
{
    m_ptrIcon = NULL;
}

IconManager::DBIcon::~DBIcon()
{
    if (m_ptrIcon)
        delete m_ptrIcon;
}
//
//----------------------------------------------------------------------------------------
//
IconManager::IconManager(QObject *parent) :
    QObject(parent)
{

}

IconManager::~IconManager()
{

}

IconManager& IconManager::getInstance()
{
    static IconManager instance;
    //check is open, if not - open it.
    //instance.init();
    //
    return instance;
}
