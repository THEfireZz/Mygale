//
// Created by SD43247 on 01/08/2024.
//

#ifndef MYGALE_APPSETTINGS_H
#define MYGALE_APPSETTINGS_H

#include <QFileDialog>
#include <QToolButton>
#include <QAbstractButton>
#include <QMessageBox>
#include "AppSettingsWidget.h++"

class AppSettings {
public:
    explicit AppSettings(AppSettingsWidget *app_settings_widget = nullptr);

    ~AppSettings() = default;

    void connectSignalsAndSlots();

    void loadUserInput() const;

    [[nodiscard]] QString getRemoteConfigLocation() const;

    [[nodiscard]] QString getLocalConfigLocation() const;

private:
    AppSettingsWidget *app_settings_widget_;

    void openRemoteConfigFileDialog();

    void openLocalConfigFileDialog();
};


#endif //MYGALE_APPSETTINGS_H
