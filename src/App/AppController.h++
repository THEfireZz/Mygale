//
// Created by SD43247 on 03/07/2024.
//

#ifndef MYGALE_APPCONTROLLER_H
#define MYGALE_APPCONTROLLER_H

#include <memory>
#include "MainWindow.h++"
#include "../JobCreation/JobCreation.h++"
#include "../AppSettings/AppSettings.h++"


class AppController : public QObject {

public:
    explicit AppController(QObject *parent = nullptr);

    ~AppController() override = default;

    void initialize();

private:
    std::unique_ptr<MainWindow> main_window_;
    std::unique_ptr<JobCreation> job_creation_;
    std::unique_ptr<AppSettings> app_settings_;
};


#endif //MYGALE_APPCONTROLLER_H
