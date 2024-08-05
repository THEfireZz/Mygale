//
// Created by SD43247 on 01/08/2024.
//

#ifndef MYGALE_APPSETTINGS_H
#define MYGALE_APPSETTINGS_H

#include <QFileDialog>
#include <QToolButton>
#include <QAbstractButton>
#include "AppSettingsWidget.h++"

class AppSettings {
public:
    explicit AppSettings(AppSettingsWidget *parent = nullptr);

    ~AppSettings() = default;

    void connectSignalsAndSlots();

    [[nodiscard]] QString getRemoteConfigLocation() const;

    [[nodiscard]] QString getLocalConfigLocation() const;

private:
    AppSettingsWidget *app_settings_widget_;

    void openRemoteConfigFileDialog();

    void openLocalConfigFileDialog();
};


#endif //MYGALE_APPSETTINGS_H
