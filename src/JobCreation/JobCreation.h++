//
// Created by SD43247 on 03/07/2024.
//

#ifndef MYGALE_JOBCREATION_H
#define MYGALE_JOBCREATION_H

#include <QLineEdit>
#include <QFileDialog>
#include <QXmlStreamReader>
#include "JobCreationWidget.h++"

class JobCreation {
public:
    explicit JobCreation(JobCreationWidget *job_creation_widget, QString configFilePath);

    ~JobCreation() = default;

    void initialize();
private:
    JobCreationWidget *job_creation_widget_;
    QString config_file_path_;

    void connectSignalsAndSlots();

    void incrementJobNumber();

    void openSceneFileDialog();

    void openOutputFolderDialog();

    QStringList getJobTypesFromConfigFile(const QString &configFilePath);
};


#endif //MYGALE_JOBCREATION_H
