//
// Created by SD43247 on 05/07/2024.
//

#ifndef MYGALE_BASESCRIPT_H
#define MYGALE_BASESCRIPT_H

#include <QProcess>
#include "../JobCreation/Job.h++"

class BaseScript {
public:
    QString getLocalScriptPath() const;

    explicit BaseScript(Job job,QString remoteScriptPath, QString remoteLaunchersPath, QString localJobLocation);

    void replaceScriptParameters(const QString& scriptName, const QString& launcherName) const ;

    void copyRemoteScript(const QString &scriptName, const QString &launcherName) const;

    void appendResubmissionJobExecutionLine(const QString& executionLinePath, const QString& scriptName) const;

    QString executeScript(const QString& scriptName) const;

protected:
    Job job_;
    QString remote_script_path_;
    QString remote_launchers_path_;
    QString local_job_location_;

};


#endif //MYGALE_BASESCRIPT_H
