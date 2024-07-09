//
// Created by SD43247 on 05/07/2024.
//

#ifndef MYGALE_BASESCRIPT_H
#define MYGALE_BASESCRIPT_H


#include "../JobCreation/Job.h++"

class BaseScript {
public:
    explicit BaseScript(Job job,QString remoteScriptPath, QString remoteLaunchersPath, QString localJobLocation);

    void replaceScriptParameters(QString scriptName, QString launcherName) const ;

    virtual void copyRemoteScript(QString scriptName, QString launcherName) const;

protected:
    Job job_;
    QString remote_script_path_;
    QString remote_launchers_path_;
    QString local_job_location_;

};


#endif //MYGALE_BASESCRIPT_H
