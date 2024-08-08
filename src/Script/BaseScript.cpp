//
// Created by SD43247 on 05/07/2024.
//

#include <utility>
#include <qfile.h>
#include <qdir.h>

#include "BaseScript.h++"
#include "../exception/CustomErrors.h++"

BaseScript::BaseScript(Job job, QString remoteScriptPath, QString remoteLaunchersPath, QString localJobLocation) : job_(
        std::move(job)), remote_script_path_(std::move(remoteScriptPath)), remote_launchers_path_(
        std::move(remoteLaunchersPath)), local_job_location_(std::move(localJobLocation)) {

}

void
BaseScript::copyRemoteScript(const QString &scriptName, const QString &launcherName) const {
    // Create the local job location folder if it does not exist

    if (QDir local_job_location(local_job_location_); local_job_location.exists()) {
        throw JobAlreadyExistsException("The job already exists in the local job location " + local_job_location_);
    } else {
        local_job_location.mkpath(".");
        local_job_location.mkpath("lsf");
        local_job_location.mkpath("appli");
    }

    // Copy the remote script to the local job location
    if (QFile remote_script(remote_script_path_); !remote_script.copy(local_job_location_ + "lsf\\" + scriptName)) {
        // throw exception
        throw FileCopyException(
                "Could not copy the remote script " + remote_script_path_ + " to the local job location " +
                local_job_location_);
    }

    // Copy the remote launchers to the local job location
    if (QFile remote_launchers(remote_launchers_path_); !remote_launchers.copy(
            local_job_location_ + "lsf\\" + launcherName)) {
        // throw exception
        throw FileCopyException(
                "Could not copy the remote launchers " + remote_launchers_path_ + " to the local job location " +
                local_job_location_);
    }

}

void BaseScript::replaceScriptParameters(const QString &scriptName, const QString &launcherName) const {
    // Open the script file
    QFile script_file(local_job_location_ + "lsf/" + scriptName);
    if (!script_file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        // throw exception
        throw FileOpenException("Could not open the script file " + script_file.fileName());
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
    QFile launchers_file(local_job_location_ + "lsf/" + launcherName);
    if (!launchers_file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        // throw exception
        throw FileOpenException("Could not open the launchers file " + launchers_file.fileName());
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

/**
 * @class BaseScript
 * @brief This method appends the resubmission job execution line to the script file
 *
 * @param executionLinePath The path of the file containing the execution line
 * @param scriptName The name of the script file to append the execution line
 **/

void BaseScript::appendResubmissionJobExecutionLine(const QString &executionLinePath, const QString &scriptName) const {
    // Open the script file
    QFile script_file(local_job_location_ + "lsf/" + scriptName);
    if (!script_file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        // throw exception
        throw FileOpenException("Could not open the script file " + script_file.fileName());
    }

    // Read the script file
    QTextStream script_stream(&script_file);
    QString script = script_stream.readAll();

    // Open the execution line file
    QFile execution_line_file(executionLinePath);
    if (!execution_line_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // throw exception
        throw FileOpenException("Could not open the execution line file " + execution_line_file.fileName());
    }

    // Read the execution line file
    QTextStream execution_line_stream(&execution_line_file);
    QString execution_line = execution_line_stream.readAll();

    // Append the execution line to the script
    script.append(execution_line);

    // Write the script file
    script_file.resize(0);
    script_stream << script;
    script_file.close();

}

QString BaseScript::executeScript(const QString &scriptName) const {
    QProcess process;
    process.setWorkingDirectory(local_job_location_ + "lsf");

    QString fullScriptPath = local_job_location_ + "lsf\\" + scriptName;

    process.start("cmd.exe", QStringList() << "/c" << fullScriptPath);
    process.waitForFinished(-1);
    return process.readAllStandardOutput();
}





