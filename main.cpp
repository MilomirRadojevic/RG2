#include <QApplication>
#include "glavnastrana.h"

GlavnaStrana *glavnaStrana;

int main(int argc, char **argv) {
    QApplication a(argc, argv);

    glavnaStrana = new GlavnaStrana();
    glavnaStrana->show();

    return a.exec();
}

