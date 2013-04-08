#include "iconloader.h"
#include "iconmanager.h"

IconLoader::IconLoader(QObject *parent) :
    QThread(parent)
{

}

void IconLoader::run()
{
    IconManager::getInstance().init();
}
