#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QDesktopWidget>


#include "masterwindow.h"
#include "iostream"

using namespace std;



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

   // QPixmap pixmap("logo.png");
   // QSplashScreen splash(pixmap.scaled(400, 300, Qt::KeepAspectRatio));
   // splash.move(QApplication::desktop()->availableGeometry().center() - splash.rect().center());
   // splash.show();

    MasterWindow window;

    window.move(QApplication::desktop()->availableGeometry().center() - window.rect().center());
    window.show();

    //splash.finish(&window);

    return app.exec();
}
