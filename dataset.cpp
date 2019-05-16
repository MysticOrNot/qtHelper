#include "dataset.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

DataSet::DataSet(QString path) :
    crypter("B5464Rf46 4WW 4C2!@4425%44(&#2C4)*)))R42!W")
{
    this->path = path+"/settings.txt";
    this->read();
}

void DataSet::AddNewCommand(QString tag, QString descr, QString command)
{

    QString newComm = encodePassword(descr + ":" + command);

    this->data.push_back(
                new Data (
                    tag,
                    decodePassword(
                        newComm
                    )
                )
    );

    write(tag + "=" + newComm);
}


Data::Data(QString name, QString value){
    this->name  = name;
    this->value = value;
}

QStringList split(QString line){
    QStringList rec;
    int demilPos = line.indexOf('=');
    rec.push_back(line.left(demilPos));
    rec.push_back(line.right(line.size() - demilPos - 1));
    return rec;
}

void DataSet::read(){
    QFile file( this->path);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(nullptr, "error", file.errorString());
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = split(line);
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

QString DataSet::decodePassword(QString strWithPass)
{
    int startPos = strWithPass.indexOf('<', 0);
    int endPos   = strWithPass.indexOf('>', 0);

    if (startPos >= 0 && endPos > 0  && endPos > startPos){
        QString encPass = strWithPass.mid(startPos + 1, endPos - startPos - 1);
        QString decPass = this->crypter.Decrypt(encPass);
        strWithPass.replace(startPos, endPos - startPos + 1, decPass);
    }

    return strWithPass;
}

QString DataSet::encodePassword(QString strWithPass)
{
    int startPos = strWithPass.indexOf('<', 0);
    int endPos   = strWithPass.indexOf('>', 0);

    if (startPos >= 0 && endPos > 0  && endPos > startPos){
        QString decPass = strWithPass.mid(startPos + 1, endPos - startPos - 1);
        QString encPass = this->crypter.Ecrypt(decPass);
        strWithPass.replace(startPos+1, endPos - startPos - 1, encPass);
    }

    return strWithPass;
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
            result.push_back(
                 decodePassword(
                     this->data[i]->getValue()
                 )
            );
        }
    }

    result.sort();
    return result;
}
