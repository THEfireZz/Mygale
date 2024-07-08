//
// Created by SD43247 on 05/07/2024.
//

#ifndef MYGALE_BASESCRIPT_H
#define MYGALE_BASESCRIPT_H


#include "../JobCreation/Job.h++"

class BaseScript {
public:
    explicit BaseScript(Job job,QString remoteScriptPath, QString remoteLaunchersPath, QString localJobLocation);

    void initialize();

    void replaceScriptParameters() const;

private:
    Job job_;
    QString remote_script_path_;
    QString remote_launchers_path_;
    QString local_job_location_;

    void copyRemoteScript() const;
};


#endif //MYGALE_BASESCRIPT_H
