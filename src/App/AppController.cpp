//
// Created by SD43247 on 03/07/2024.
//

#include "AppController.h++"

AppController::AppController(QObject *parent) : QObject(parent) {

}
/**
 * @class AppController
 * @brief This method initializes application and connects signals and slots
 **/

void AppController::initialize() {

    main_window_ = std::make_unique<MainWindow>();
    main_window_->show();

    job_creation_ = std::make_unique<JobCreation>(main_window_->getJobCreationWidget());
    job_creation_->initialize();
}
