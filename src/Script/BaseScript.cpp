//
// Created by SD43247 on 05/07/2024.
//

#include <utility>
#include <qfile.h>
#include <qdir.h>

#include "BaseScript.h++"

BaseScript::BaseScript(Job job, QString remoteScriptPath, QString remoteLaunchersPath, QString localJobLocation) : job_(
        std::move(job)), remote_script_path_(std::move(remoteScriptPath)), remote_launchers_path_(
        std::move(remoteLaunchersPath)), local_job_location_(std::move(localJobLocation)) {

}

void BaseScript::initialize() {
    copyRemoteScript();
}

void BaseScript::copyRemoteScript() const {
    // Create the local job location folder if it does not exist
    if (QDir local_job_location(local_job_location_); !local_job_location.exists()) {
        local_job_location.mkpath(".");
    }

    // Copy the remote script to the local job location
    QFile remote_script(remote_script_path_);
    if (!remote_script.copy(local_job_location_ + job_.getJobName() + ".bat")) {
        // throw exception
        throw std::runtime_error("Could not copy the remote script to the local job location");
    }

    // Copy the remote launchers to the local job location
    QFile remote_launchers(remote_launchers_path_);
    if (!remote_launchers.copy(local_job_location_ + "lance.bat")) {
        // throw exception
        throw std::runtime_error("Could not copy the remote launchers to the local job location");
    }

}




