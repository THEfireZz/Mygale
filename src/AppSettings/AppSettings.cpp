//
// Created by SD43247 on 01/08/2024.
//

#include "AppSettings.h++"

AppSettings::AppSettings(AppSettingsWidget *parent) : app_settings_widget_(parent) {

}

void AppSettings::initialize() {
    connectSignalsAndSlots();
}

void AppSettings::connectSignalsAndSlots() {
    return;
}

QString AppSettings::getRemoteConfigLocation() const {
    return app_settings_widget_->getRemoteLocationLineEdit()->text();
}

QString AppSettings::getLocalConfigLocation() const {
    return app_settings_widget_->getLocalLocationLineEdit()->text();
}
