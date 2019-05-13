#ifndef DATASET_H
#define DATASET_H

#include <QMainWindow>

class Data{
    QString name;
    QString value;
public:
    Data(QString name, QString value);
    QString getName(){return this->name;}
    QString getValue(){return this->value;}
};

class DataSet
{
    QString path;
    QList<Data *> data;
public:
    DataSet(QString path);
    void AddNewCommand(QString tag, QString descr, QString command);
    QList<QString> getUniqueNames();
    QList<QString> getValues(QString name);
private:
    void read();
    void write(QString line);
};

#endif // DATASET_H
