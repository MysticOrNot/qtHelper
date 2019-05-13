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
    explicit DialogAdd(QWidget *parent = nullptr);
    ~DialogAdd();
private slots:
    void on_addBtn_released();

private:
    Ui::DialogAdd *ui;
};

#endif // DIALOGADD_H
