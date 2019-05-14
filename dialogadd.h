#ifndef DIALOGADD_H
#define DIALOGADD_H

#include <QWidget>

namespace Ui {
class DialogAdd;
}

class DialogAdd : public QWidget
{
    Q_OBJECT

public:
    QObject *pr;
    void setTopic(QString str);
    explicit DialogAdd(QWidget *parent = nullptr);
    ~DialogAdd();
private slots:
    void on_addBtn_released();
    void showEvent( QShowEvent* event ) override;
private:
    Ui::DialogAdd *ui;
    QString startTopic;
};

#endif // DIALOGADD_H
