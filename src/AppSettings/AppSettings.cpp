//
// Created by SD43247 on 01/08/2024.
//


#include <QMessageBox>
#include "AppSettings.h++"

/**
 * @class AppSettings
 *
 * @brief The AppSettings class is responsible for handling the app settings widget.
 **/

AppSettings::AppSettings(AppSettingsWidget *app_settings_widget) : app_settings_widget_(app_settings_widget) {

}

/**
 * @brief Connects signals and slots for the app settings widget.
 **/
void AppSettings::connectSignalsAndSlots() {
    QAbstractButton::connect(app_settings_widget_->getRemoteLocationToolButton(), &QAbstractButton::clicked, [this] {
        openRemoteConfigFileDialog();
    });

    QAbstractButton::connect(app_settings_widget_->getLocalLocationToolButton(), &QAbstractButton::clicked, [this] {
        openLocalConfigFileDialog();
    });
}

/**
 * @brief Loads user input from the app settings widget into the line edits, combo boxes, check boxes and spin boxes.
 **/
void AppSettings::loadUserInput() const {
    QSettings settings("Stellantis", "Mygale");
    settings.beginGroup("MainWindow");
    settings.beginGroup("AppSettingsWidget");

    QStringList keys = settings.childKeys();
    for (QWidget *widget: app_settings_widget_->findChildren<QWidget *>()) {
        const QString objectName = widget->objectName();
        if (keys.contains(objectName)) {
            if (auto *lineEdit = qobject_cast<QLineEdit *>(widget)) {
                lineEdit->setText(settings.value(objectName).toString());
            } else if (auto *comboBox = qobject_cast<QComboBox *>(widget)) {
                comboBox->setCurrentIndex(settings.value(objectName).toInt());
            } else if (auto *checkBox = qobject_cast<QCheckBox *>(widget)) {
                checkBox->setChecked(settings.value(objectName).toBool());
            } else if (auto *spinBox = qobject_cast<QSpinBox *>(widget)) {
                spinBox->setValue(settings.value(objectName).toInt());
            }
        }
    }

}

/**
 * @brief Opens a file dialog to select the remote config location verifing if the folder contains the mainConfig.xml file and sets the remote location line edit text.
 **/
void AppSettings::openRemoteConfigFileDialog() {
    QString remote_config_location = QFileDialog::getExistingDirectory(app_settings_widget_,
                                                                       "Select Remote Config Location",
                                                                       QDir::homePath());
    //check if the folder contains the mainConfig.xml file
    if (!QFile::exists(remote_config_location + "/mainConfig.xml")) {
        QMessageBox::warning(app_settings_widget_, "Config folder error",
                             "The selected folder does not contain the mainConfig.xml file");
        return;
    }
    app_settings_widget_->getRemoteLocationLineEdit()->setText(remote_config_location);
}

/**
 * @brief Opens a file dialog to select the local config location and sets the local location line edit text.
 **/
void AppSettings::openLocalConfigFileDialog() {
    QString local_config_location = QFileDialog::getExistingDirectory(app_settings_widget_,
                                                                      "Select Local Config Location",
                                                                      QDir::homePath());
    app_settings_widget_->getLocalLocationLineEdit()->setText(local_config_location);

}

/**
 * @brief Returns the remote config location.
 *
 * @return The remote config location.
 **/
QString AppSettings::getRemoteConfigLocation() const {
    return app_settings_widget_->getRemoteLocationLineEdit()->text();
}

/**
 * @brief Returns the local config location.
 *
 * @return The local config location.
 **/
QString AppSettings::getLocalConfigLocation() const {
    return app_settings_widget_->getLocalLocationLineEdit()->text();
}
