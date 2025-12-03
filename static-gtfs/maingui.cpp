#include "gtfs.hpp"
#include "gui.hpp"
#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QLabel label("Hello, Qt!");
    label.resize(300, 100);
    label.show();

    return app.exec();
}