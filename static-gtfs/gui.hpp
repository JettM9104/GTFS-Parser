#ifndef GUI_HPP
#define GUI_HPP
#include <iostream>
#include "gtfs.hpp"

#include <QApplication>
#include <QLabel>

int helloWorld(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QLabel label("Hello, Qt!");
    label.resize(300, 100);
    label.show();

    return app.exec();
}

int startGUI(int argc, char* argv[]) {
    return helloWorld(argc, argv);
}

#endif