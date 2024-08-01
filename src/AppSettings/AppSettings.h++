//
// Created by SD43247 on 01/08/2024.
//

#ifndef MYGALE_APPSETTINGS_H
#define MYGALE_APPSETTINGS_H


#include "AppSettingsWidget.h++"

class AppSettings {
public:
    explicit AppSettings(AppSettingsWidget *parent = nullptr);

    ~AppSettings() = default;

    void initialize();

    [[nodiscard]] QString getRemoteConfigLocation() const;

    [[nodiscard]] QString getLocalConfigLocation() const;

private:
    AppSettingsWidget *app_settings_widget_;



    void connectSignalsAndSlots();
};


#endif //MYGALE_APPSETTINGS_H
