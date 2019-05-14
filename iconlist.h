#ifndef ICONLIST_H
#define ICONLIST_H

#include <QIcon>
#include <QMap>

class IconList
{
    QString appPath;
public:
    IconList(QString path);
    QIcon getIcon(QString name);
};

#endif // ICONLIST_H
