#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QShowEvent>
#include <QListView>
#include <QtGui>
#include <QClipboard>
#include <QListWidget>
#include <QShortcut>
#include <QKeyEvent>
#include <QStyledItemDelegate>
#include <QInputDialog>
#include <QAction>
#include "dataset.h"
#include "dialogadd.h"
#include "xorcrypt.h"
#include "GlobalActionHelper.h"
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>

class MyDelegate : public QStyledItemDelegate  {
    public:
    QPen hotKey = QColor("#ADFF2F");
    QPen penDescr = QColor("#999999");
    QPen penCommand = QColor("#336699");
    MyDelegate(QObject *parent=0) : QStyledItemDelegate (parent){}

    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const{

        if(option.state & QStyle::State_Selected){
            painter->fillRect(option.rect, option.palette.color(QPalette::Highlight));
        }

        QString title = index.data(Qt::DisplayRole).toString();
        QString description = index.data(Qt::UserRole + 1).toString();
        int row = index.row();
        int deltaX = 0;
        QRect r = option.rect.adjusted(2, 2, 2, 2);

        if (row >=0 && row <=9){

            painter->setPen(hotKey);
            if (row == 9) row = 0;
            else row += 1;
            painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignLeft, "Ctrl+" + QString::number(row) + ": ", &r);
            r = option.rect.adjusted(60, 2, 2, 2);
        }

        painter->setPen(penDescr);
        painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignLeft, title, &r);

        r = option.rect.adjusted(2, 20, 2, 2);
        painter->setPen(penCommand);
        painter->drawText(r.left(), r.top(), r.width(), r.height(), Qt::AlignLeft, description, &r);
    }

    QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const{
        return QSize(option.widget->width(), 50);
    }
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    dataset(*new DataSet(QDir::currentPath())),
    icon(*new IconList(QDir::currentPath())),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dialogForm = new DialogAdd();
    dialogForm->pr = this;

    this->ui->listWidget->setItemDelegate(new MyDelegate(this->ui->listWidget));

    initPasteKeys();
    topicListChange(0);
    nameChange();

    connect(this->ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=](){
         nameChange();
    });

    // горячие клавиши Ctrl + [0 - 9]
    GlobalActionHelper::init ();
    for (int i = 0; i < 10; i++) {
        globalAction[i] = new QAction(this);
        globalAction[i]->setShortcut(QKeySequence ("Ctrl+" + QString::number(i)));
        connect(globalAction[i], &QAction::triggered, [=](){
          hotKeyPressed(i);
        });
        GlobalActionHelper::makeGlobal (globalAction[i]);
    }
}

void MainWindow::showMsg(QString msg){
    QMessageBox msgBox;
    msgBox.setText(msg);
    //msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    //msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    //msgBox.setDefaultButton(QMessageBox::Save);
    msgBox.exec();
}

void MainWindow::nameChange(){
    ui->listWidget->blockSignals(true);
    ui->listWidget->clear();
    ui->listWidget->blockSignals(false);

    QList<QString> names = this->dataset.getValues(getCurrentTag());

    for (int i = 0; i < names.length(); i++)
       {
        toItem(names[i]);
       }

    this->ui->listWidget->show();
}

MainWindow::~MainWindow()
{
    delete dialogForm;
    delete ui;
}

void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    QClipboard *clipboard = QGuiApplication::clipboard();

    QString comm = QVariant(current->data(Qt::UserRole + 1)).toString();

    clipboard->setText(toClipboard(comm));
}


void MainWindow::toItem(QString s){
    QString desc;
    QString command = "";

    int i = s.indexOf(':');
    if (i < 0) desc = s;
    else{
        desc = s.left(i);
        command = s.right(s.length() - i - 1);
    }

    QListWidgetItem *item = new QListWidgetItem();
    item->setData(Qt::DisplayRole, desc);
    item->setData(Qt::UserRole + 1, command);
    this->ui->listWidget->addItem(item);
}

void MainWindow::topicListChange(int currItem)
{
    ui->comboBox->blockSignals(true);
    ui->comboBox->clear();

    QList<QString> names = this->dataset.getUniqueNames();
    for (int i = 0; i < names.length(); i++)
    {
       this->ui->comboBox->addItem(
                    this->icon.getIcon(names[i]),
                    names[i]
       );
    }
    this->ui->comboBox->setCurrentIndex(currItem);

    ui->comboBox->blockSignals(false);
}

int MainWindow::getCurrTopicIndex()
{
    return this->ui->comboBox->currentIndex();
}

QString MainWindow::toClipboard(QString s){
    int i = s.indexOf('\n');
    if (i < 0) return s;
    else{
        s.remove(0, i+1);
        return s;
    }
}

QString MainWindow::getCurrentTag()
{
    return this->ui->comboBox->itemText(
                getCurrTopicIndex()
    );
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    dialogForm->close();

    /*QMessageBox::StandardButton resBtn = QMessageBox::question( this, APP_NAME,
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
    }*/
}

void MainWindow::initPasteKeys()
{
    Display *display;
    display=XOpenDisplay(NULL);

    controlKey = XKeysymToKeycode(display, XK_Control_L);
    shiftKey   = XKeysymToKeycode(display, XK_Shift_L);
    insertKey  = XKeysymToKeycode(display, XK_Insert);
    vKey       = XKeysymToKeycode(display, XK_V);

    firstKey = controlKey;
    lastKey  = vKey;

    XCloseDisplay(display);
}

void MainWindow::on_toolButton_released()
{
    dialogForm->setWindowTitle("Добавление команды для " + getCurrentTag());
    dialogForm->setTopic(getCurrentTag());
    dialogForm->show();
}

void MainWindow::hotKeyPressed(int i)
{
    if ((i >=0) && (i < this->ui->listWidget->count())){
        int item = i;

        if (item == 0){
            if (this->ui->listWidget->count() >= 10) item = 10;
            else return;
        }

        this->ui->listWidget->setCurrentRow(item - 1);

        Display *display;
        display=XOpenDisplay(NULL);
        unsigned int currKey    = XKeysymToKeycode(display, 48 + i);



        XTestFakeKeyEvent(display, controlKey,  False, CurrentTime);
        XTestFakeKeyEvent(display, currKey,     False, CurrentTime);
        XFlush(display);

        XTestFakeKeyEvent(display, firstKey,  True, CurrentTime);
        XTestFakeKeyEvent(display, lastKey,   True, CurrentTime);
        XTestFakeKeyEvent(display, lastKey,   False, CurrentTime);
        XTestFakeKeyEvent(display, firstKey,  False, CurrentTime);
        XFlush(display);

        XCloseDisplay(display);

    }
}

void MainWindow::on_pasteBtn_released()
{
    if (this->ui->pasteBtn->isFlat()){
        this->ui->pasteBtn->setFlat(false);
        this->ui->pasteBtn->setText("Ctrl+V");

        firstKey = controlKey;
        lastKey  = vKey;
    }
    else{
        this->ui->pasteBtn->setFlat(true);
        this->ui->pasteBtn->setText("Shift+Ins");

        firstKey = shiftKey;
        lastKey  = insertKey;
    }
}
