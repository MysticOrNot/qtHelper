#include "xorcrypt.h"

#include <QInputDialog>
#include <QTextCodec>

XorCrypt::XorCrypt(QString key)
{
    requestPass(key);
}

char XorCrypt::getKey(int i)
{
    return this->key[i % this->key.length()];
}

void XorCrypt::Xor(QByteArray& arr)
{
    for (int i = 0; i < arr.length(); i++) {
        arr[i] = arr[i] ^ this->getKey(i);
    }
}

void XorCrypt::requestPass(QString defPass)
{
    QInputDialog inputDialog;

    inputDialog.setMinimumWidth(200);
    inputDialog.resize(inputDialog.size());
    inputDialog.setWindowTitle("Пароль для расшифровки");
    inputDialog.setLabelText("Пароль:");
    inputDialog.setTextValue("");
    inputDialog.setTextEchoMode(QLineEdit::Normal);

    bool ok = !!inputDialog.exec();
    QString passw = inputDialog.textValue();

    if (ok && passw.size() > 7)
         this->key = passw.toUtf8();
    else
         this->key = defPass.toUtf8();
}

QString XorCrypt::Ecrypt(QString Text)
{
    QByteArray textArr = Text.toUtf8();
    Xor(textArr);
    return textArr.toBase64();
}

QString XorCrypt::Decrypt(QString cryptedText)
{
    QByteArray textArr = QByteArray::fromBase64(cryptedText.toUtf8());
    Xor(textArr);
    return QTextCodec::codecForMib(106)->toUnicode(textArr);
}


