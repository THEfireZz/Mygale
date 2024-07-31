#include <QApplication>
#include <QSettings>
#include "App/AppController.h++"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setStyle("Fusion");
    QSettings settings("Stellantis", "Mygale");
    //settings.clear();
    AppController appController;
    appController.initialize();
    return QApplication::exec();
}
