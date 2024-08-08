//
// Created by SD43247 on 03/07/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "MainWindow.h++"
#include "../../resources/ui/ui_MainWindow.h"

/**
 * @class MainWindow
 *
 * @brief The MainWindow class is responsible for initializing the main window and its widgets.
 **/

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

/**
 * @brief Returns the job creation widget.
 *
 * @return JobCreationWidget
 **/
JobCreationWidget *MainWindow::getJobCreationWidget() const {
    return ui->jobCreationTab;
}

/**
 * @brief Returns the app settings widget.
 *
 * @return AppSettingsWidget
 **/
AppSettingsWidget *MainWindow::getAppSettingsWidget() const {
    return ui->appSettingsTab;
}

/**
 * @brief Returns the tab widget.
 *
 * @return QTabWidget
 **/
QTabWidget *MainWindow::getTabWidget() const {
    return ui->tabWidget;
}

/**
 * @brief Saves user input when the main window is closed.
 *
 * @param QCloseEvent event
 **/
void MainWindow::closeEvent(QCloseEvent *event) {
    ui->jobCreationTab->saveUserInput();
    ui->appSettingsTab->saveUserInput();
    event->accept();
}

