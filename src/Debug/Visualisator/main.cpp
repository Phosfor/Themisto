#include <QtGui/QApplication>
#include <ClanLib/network.h>

#include "include/Debug/Visualisator/mainwindow.h"
#include "include/Debug/Visualisator/Client.h"

int main(int argc, char *argv[])
{
    CL_SetupCore setup_core;
    CL_SetupNetwork setup_network;
    Client client;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
