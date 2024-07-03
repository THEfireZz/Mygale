#include <QApplication>
#include "App/AppController.h++"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setStyle("Fusion");

    AppController appController;
    appController.initialize();
    return QApplication::exec();
}
