#include "mainwindow.h"

#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mainWindow;
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    mainWindow.resize(int(screenGeometry.width()*0.8), int(screenGeometry.height()*0.8));
    mainWindow.show();
    return app.exec();
}
