#include "iconlist.h"

#include <QIcon>
#include <QMap>

IconList::IconList(QString path)
{
    appPath = path + "/icons/";

    Icons i;

    i.path = "ssh.png";
    i.icon = QIcon(appPath + i.path);
    iconData["ssh"] = i;

    i.path = "docker.png";
    i.icon = QIcon(appPath + i.path);
    iconData["docker"] = i;

    i.path = "ubuntu.png";
    i.icon = QIcon(appPath + i.path);
    iconData["ubuntu"] = i;

    i.path = "default.png";
    i.icon = QIcon(appPath + i.path);
    iconData["default"] = i;

}

QIcon IconList::getIcon(QString name)
{
    QList<QString> keys = iconData.keys();

    if (keys.indexOf(name) < 0){
        name = "default";
    }

    return iconData[name].icon;
}
