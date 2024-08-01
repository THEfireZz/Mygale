//
// Created by SD43247 on 01/08/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AppSettingsWidget.h" resolved

#include "AppSettingsWidget.h++"
#include "../../resources/ui/ui_AppSettingsWidget.h"


AppSettingsWidget::AppSettingsWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::AppSettingsWidget) {
    ui->setupUi(this);
}

AppSettingsWidget::~AppSettingsWidget() {
    delete ui;
}

QLineEdit *AppSettingsWidget::getRemoteLocationLineEdit() {
    return ui->remoteLocationLineEdit;
}

QToolButton *AppSettingsWidget::getRemoteLocationToolButton() {
    return ui->remoteLocationToolButton;
}

QLineEdit *AppSettingsWidget::getLocalLocationLineEdit() {
    return ui->localLocationLineEdit;
}

QToolButton *AppSettingsWidget::getLocalLocationToolButton() {
    return ui->localLocationToolButton;
}
