#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow Gestion;
    Gestion.show();

    return app.exec();
}
