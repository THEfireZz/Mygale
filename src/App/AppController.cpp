//
// Created by SD43247 on 03/07/2024.
//

#include "AppController.h++"

AppController::AppController(QObject * parent)
:
QObject(parent) {

}

/**
 * @class AppController
 * @brief This method initializes application and connects signals and slots
 **/

void AppController::initialize() {

    main_window_ = std::make_unique<MainWindow>();
    main_window_->show();
    app_settings_ = std::make_unique<AppSettings>(main_window_->getAppSettingsWidget());
    app_settings_->initialize();
    job_creation_ = std::make_unique<JobCreation>(main_window_->getJobCreationWidget(),
                                                  app_settings_->getLocalConfigLocation());
    job_creation_->initialize();
    job_creation_->loadUserInput();
}
