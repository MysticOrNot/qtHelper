#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "iconlist.h"

#include <QMainWindow>
#include <QListWidgetItem>
#include <QShortcut>
#include <QPen>
#include <dataset.h>
#include <dialogadd.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    DataSet dataset;
    void showMsg(QString msg);
    void nameChange();
    void toItem(QString s);
    void slotShortcut();
    void topicListChange(int currItem);
    int getCurrTopicIndex();
    QString toClipboard(QString s);
    QString getCurrentTag();
    IconList icon;
private slots:
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_toolButton_released();
private:
    Ui::MainWindow *ui;
    DialogAdd *dialogForm;
    void closeEvent (QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
