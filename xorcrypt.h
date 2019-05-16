#ifndef XORCRYPT_H
#define XORCRYPT_H

#include <qstring.h>

class XorCrypt
{
    QByteArray key;
public:
    XorCrypt(QString key);
    char getKey(int i);
    QString Ecrypt (QString Text);
    QString Decrypt(QString cryptedText);
private:
    void Xor(QByteArray& arr);
    void requestPass(QString defPass);
};

#endif // XORCRYPT_H
