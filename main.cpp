/****************************************************************************
**
** Created by Andrzej Borucki on 2022-08-17
**
** This file is part of the EdiQ project
** see file LICENSE
**
****************************************************************************/

#include "mainwindow.h"

#include <QApplication>
#include <SingleApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    SingleApplication app(argc, argv, true);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();

    // If this is a secondary instance
    if( app.isSecondary() ) {
        app.sendMessage( app.arguments().join(' ').toUtf8() );
        return 0;
    } else {
        MainWindow mainWindow;
        QObject::connect(
                &app,
                &SingleApplication::receivedMessage,
                &mainWindow,
                &MainWindow::receivedMessage
        );
        QPoint smallScreen(1366,768);
        for (int i=1; i<app.arguments().size(); i++)
            mainWindow.openOrActivate(app.arguments()[i]);
        mainWindow.setGeometry(int(std::min(screenGeometry.width(),smallScreen.x())*0.1),
                               int(std::min(screenGeometry.height(),smallScreen.y())*0.1),
                int(screenGeometry.width()*0.8), int(screenGeometry.height()*0.8));
        mainWindow.show();
        return app.exec();
    }
}
