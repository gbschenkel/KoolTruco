#include <QtGui/QApplication>
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog dialog;
    dialog.show();

    return a.exec();
}
