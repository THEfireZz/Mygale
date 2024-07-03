//
// Created by SD43247 on 03/07/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MygaleWidget.h" resolved

#include "JobCreationWidget.h++"
#include "../../resources/ui/ui_JobCreationWidget.h"


JobCreationWidget::JobCreationWidget(QWidget *parent) :
        QWidget(parent), ui(new Ui::JobCreationWidget) {
    ui->setupUi(this);
}

JobCreationWidget::~JobCreationWidget() {
    delete ui;
}
