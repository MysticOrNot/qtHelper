#ifndef ICONLIST_H
#define ICONLIST_H

#include <QIcon>
#include <QMap>

struct Icons
{
    QString path;
    QIcon   icon;
};

class IconList
{
    QString appPath;
    QMap<QString, Icons> iconData;
public:
    IconList(QString path);
    QIcon getIcon(QString name);
};

#endif // ICONLIST_H
