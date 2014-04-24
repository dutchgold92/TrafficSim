#include "mainwindow.h"
#include <QApplication>
#include <network.h>
#include <iostream>

using namespace std;

/**
 * @brief main Launches the application.
 * @param argc Unused parameter.
 * @param argv Unused parameter.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
