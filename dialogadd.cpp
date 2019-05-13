#include "dialogadd.h"
#include "mainwindow.h"
#include "ui_dialogadd.h"

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
    MainWindow* w = (MainWindow *)this->pr;

    w->dataset.AddNewCommand(
                w->getCurrentTag(),
                this->ui->descr->text(),
                this->ui->command->toPlainText()
    );

    w->nameChange();
}
