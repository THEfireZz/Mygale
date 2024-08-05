//
// Created by SD43247 on 03/07/2024.
//

#ifndef MYGALE_JOBCREATION_H
#define MYGALE_JOBCREATION_H

#include <QLineEdit>
#include <QFileDialog>
#include <QXmlStreamReader>
#include <QStorageInfo>
#include <QMessageBox>
#include <windows.h>
#include "JobCreationWidget.h++"
#include "Job.h++"
#include "../Script/BaseScript.h++"

class JobCreation {
public:
    explicit JobCreation(JobCreationWidget *job_creation_widget, QString configFilePath);

    ~JobCreation() = default;

    void initialize();

    void connectSignalsAndSlots();

    void loadUserInput() const;

    Job createJob(QString priority);

    void createAndExecuteJob(QString priority);

private:
    JobCreationWidget *job_creation_widget_;
    QString config_file_path_;
    Job job_ = Job::Builder().build();

    void incrementJobNumber();

    void openSceneFileDialog();

    void openOutputFolderDialog();

    static QStringList getJobTypesFromConfigFile(const QString &configFilePath);

    static QStringList getFormatsFromConfigFile(const QString &configFilePath, const QString &jobType);

    static QString getJobParameterValueFromConfigFile(const QString &configFilePath, const QString &jobType, const QString &parameter);

    static QString convertToUncPath(const QString &path);

    [[nodiscard]] QString getJobType() const;

    [[nodiscard]] QString getJobName() const;

    [[nodiscard]] QString getScenePath() const;

    [[nodiscard]] QString getOutputPath() const;

    [[nodiscard]] QString getFormat() const;

    [[nodiscard]] static QString getRawFormat(const QString& selection) ;

    [[nodiscard]] static QString getFormatName(const QString& selection) ;

    [[nodiscard]] QString getFirstImage() const;

    [[nodiscard]] QString getLastImage() const;

    [[nodiscard]] QString getFirstIndex() const;

    [[nodiscard]] QString getLastIndex() const;

    [[nodiscard]] QString getCameraName() const;

    [[nodiscard]] QString getMinCpu() const;

    [[nodiscard]] QString getMaxCpu() const;

    [[nodiscard]] QString getSubmissionOption() const;

    [[nodiscard]] QString getName();

    [[nodiscard]] QString getCpuInterval() const;

    [[nodiscard]] QString getMemoryInterval() const;

    [[nodiscard]] QString getParcStyleList() const;

    [[nodiscard]] QString getSteps();
};


#endif //MYGALE_JOBCREATION_H
