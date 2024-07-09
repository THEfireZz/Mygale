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

void BaseScript::copyRemoteScript(QString scriptName, QString launcherName) const {
    // Create the local job location folder if it does not exist
    if (QDir local_job_location(local_job_location_); !local_job_location.exists()) {
        local_job_location.mkpath(".");
    }

    // Copy the remote script to the local job location
    QFile remote_script(remote_script_path_);
    if (!remote_script.copy(local_job_location_ + scriptName)) {
        // throw exception
        throw std::runtime_error("Could not copy the remote script " + remote_script_path_.toStdString() +
                                 " to the local job location " + local_job_location_.toStdString() + " Error: " + remote_script.errorString().toStdString());
    }

    // Copy the remote launchers to the local job location
    QFile remote_launchers(remote_launchers_path_);
    if (!remote_launchers.copy(local_job_location_ + launcherName)) {
        // throw exception
        throw std::runtime_error("Could not copy the remote launchers to the local job location");
    }

}

void BaseScript::replaceScriptParameters(QString scriptName, QString launcherName) const {
    // Open the script file
    QFile script_file(local_job_location_ + scriptName);
    if (!script_file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        // throw exception
        throw std::runtime_error("Could not open the script file");
    }

    // Read the script file
    QTextStream script_stream(&script_file);
    QString script = script_stream.readAll();

    // Replace the parameters in the script
    const auto &jobParameters = job_.getJobParameters();
    for (auto it = jobParameters.cbegin(); it != jobParameters.cend(); ++it) {
        script.replace(it.key(), it.value());
    }

    // Write the script file
    script_file.resize(0);
    script_stream << script;
    script_file.close();

    // Open the launchers file
    QFile launchers_file(local_job_location_ + launcherName);
    if (!launchers_file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        // throw exception
        throw std::runtime_error("Could not open the launchers file");
    }

    // Read the launchers file
    QTextStream launchers_stream(&launchers_file);
    QString launchers = launchers_stream.readAll();

    for (auto it = jobParameters.cbegin(); it != jobParameters.cend(); ++it) {
        launchers.replace(it.key(), it.value());
    }

    // Write the launchers file
    launchers_file.resize(0);
    launchers_stream << launchers;
    launchers_file.close();
}





