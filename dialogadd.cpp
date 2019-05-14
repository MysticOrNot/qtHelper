#include "dialogadd.h"
#include "mainwindow.h"
#include "ui_dialogadd.h"

void DialogAdd::setTopic(QString str)
{
    this->ui->topic->setText(str);
}

DialogAdd::DialogAdd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialogAdd)
{
    ui->setupUi(this);
}

DialogAdd::~DialogAdd()
{
    delete ui;
}

void DialogAdd::on_addBtn_released()
{
    if (this->ui->topic->text().size() < 3) return;

    MainWindow* w = (MainWindow *)this->pr;

    w->dataset.AddNewCommand(
                this->ui->topic->text(),
                this->ui->descr->text(),
                this->ui->command->toPlainText()
    );

    if (startTopic != this->ui->topic->text()){
        w->topicListChange(w->getCurrTopicIndex());
    }

    w->nameChange();
}

void DialogAdd::showEvent(QShowEvent *event)
{
    QWidget::showEvent( event );
    startTopic = this->ui->topic->text();
}
