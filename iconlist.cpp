#include "iconlist.h"

#include <QFileInfo>
#include <QIcon>
#include <QMap>

IconList::IconList(QString path)
{
    appPath = path + "/icons/";
}

QIcon IconList::getIcon(QString name)
{
    QFileInfo check_file(appPath + name + ".png");
    if (!check_file.exists() || !check_file.isFile()) {
        name = "default";
    }

    return QIcon(appPath + name + ".png");;
}
