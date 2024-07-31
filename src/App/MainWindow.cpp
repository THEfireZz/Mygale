//
// Created by SD43247 on 03/07/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "MainWindow.h++"
#include "../../resources/ui/ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

JobCreationWidget *MainWindow::getJobCreationWidget() const {
    return ui->jobCreationTab;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    ui->jobCreationTab->saveUserInput();
    event->accept();
}
