#include "mainwindow.h"

#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mainWindow;
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  geometry = screen->availableGeometry();
    const int H = 28; //border height;
    mainWindow.setGeometry(int(geometry.width() * 0.1), int(geometry.height() * 0.05)+H,
                           int(geometry.width() * 0.8), int(geometry.height() * 0.9)-H);
    mainWindow.show();
    return app.exec();
}
