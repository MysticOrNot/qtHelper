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
#include "dataset.h"
#include "dialogadd.h"

class MyDelegate : public QStyledItemDelegate  {
    public:
    QPen penDescr = QColor("#999999");
    QPen penCommand = QColor("#336699");
    MyDelegate(QObject *parent=0) : QStyledItemDelegate (parent){}

    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const{

        if(option.state & QStyle::State_Selected){
            painter->fillRect(option.rect, option.palette.color(QPalette::Highlight));
        }

        QString title = index.data(Qt::DisplayRole).toString();
        QString description = index.data(Qt::UserRole + 1).toString();

        QRect r = option.rect.adjusted(0, 0, 0, 0);
        painter->setPen(penDescr);
        painter->drawText(r.left(), r.top(), r.width(), r.height(),Qt::AlignLeft, title, &r);

        r = option.rect.adjusted(0, 20, 0, 0);
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
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dialogForm = new DialogAdd();
    dialogForm->pr = this;

    this->ui->listWidget->setItemDelegate(new MyDelegate(this->ui->listWidget));

    QList<QString> names = this->dataset.getUniqueNames();
    for (int i = 0; i < names.length(); i++)
    {
       this->ui->comboBox->addItem(names[i]);
    }


    nameChange();


    connect(this->ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
        [=](){
         nameChange();
    });
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
                this->ui->comboBox->currentIndex()
    );
}


void MainWindow::on_pushButton_released()
{
    dialogForm->setWindowTitle("Добавление команды для " + getCurrentTag());
    dialogForm->show();
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
