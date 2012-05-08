#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    // This set a valid port range in portEdit.
    ui->portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(ServerInfo()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::ServerInfo()
{
    emit sendServerInfo(ui->ipLineEdit->text(), ui->portLineEdit->text().toInt());

}
