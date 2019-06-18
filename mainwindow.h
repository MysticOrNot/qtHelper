#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "iconlist.h"
#include "xorcrypt.h"

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
    void hotKeyPressed(int i);
    void on_pushButton_released();
    void on_pasteBtn_pressed();

    void on_pasteBtn_released();

private:
    Ui::MainWindow *ui;
    DialogAdd *dialogForm;    
    QAction *globalAction[10];
    void closeEvent (QCloseEvent *event) override;
    unsigned int controlKey;
    unsigned int shiftKey;
    unsigned int insertKey;
    unsigned int vKey;
    unsigned int firstKey;
    unsigned int lastKey;
    void initPasteKeys();
};

#endif // MAINWINDOW_H
