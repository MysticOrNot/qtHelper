#include "dataset.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

DataSet::DataSet(QString path)
{
    this->path = path+"/settings.txt";
    this->read();
}

void DataSet::AddNewCommand(QString tag, QString descr, QString command)
{
    this->data.push_back(new Data (tag, descr + ":" + command));
    write(tag + "=" + descr + ":" + command);
}


Data::Data(QString name, QString value){
    this->name  = name;
    this->value = value;
}

void DataSet::read(){
    QFile file( this->path);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(nullptr, "error", file.errorString());
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split("=");
        this->data.push_back(new Data (fields[0], fields[1]) );
    }

    file.close();
}

void DataSet::write(QString line)
{
    QFile file( this->path);
    if(!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::information(nullptr, "error", file.errorString());
    }

    QTextStream out(&file);
    out << line.toUtf8() << "\n";

    file.close();
}

QList<QString> DataSet::getUniqueNames(){
    QList<QString> result;

    for (int i = 0; i < this->data.length(); i++) {
        if(result.indexOf(this->data[i]->getName()) < 0){
            result.push_back(this->data[i]->getName());
        }
    }

    result.sort();
    return result;
}


QList<QString> DataSet::getValues(QString name){
    QList<QString> result;

    for (int i = 0; i < this->data.length(); i++) {
        if(QString::compare(this->data[i]->getName(), name, Qt::CaseSensitive) == 0){
            result.push_back(this->data[i]->getValue());
        }
    }

    result.sort();
    return result;
}
