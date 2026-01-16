#ifndef GUI_HPP
#define GUI_HPP
#include <iostream>
#include "gtfs.hpp"

#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

int startButtons(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("GUI GTFS");

    QVBoxLayout* layout = new QVBoxLayout();

    for (int i = 1; i <= 10; i++) {
        QPushButton* btn = new QPushButton(QString("Button %1").arg(i));
        layout->addWidget(btn);
    }

    window.setLayout(layout);
    window.resize(200, 400);
    window.show();

    return app.exec();
}

int startGUI(int argc, char* argv[]) {
    showMap(argc, argv);
    return startButtons(argc, argv);
}

void showMap(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("map gui testing");

    QVBoxLayout* layout = new QVBoxLayout();

    for (int i = 1; i <= 10; i++) {
        
    }


    return;
}

#endif
