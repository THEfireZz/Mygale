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

void
BaseScript::copyRemoteScript(const QString &scriptName, const QString &launcherName) const {
    // Create the local job location folder if it does not exist
    if (QDir local_job_location(local_job_location_); !local_job_location.exists()) {
        local_job_location.mkpath(".");
        local_job_location.mkpath("lsf");
        local_job_location.mkpath("appli");
    }

    // Copy the remote script to the local job location
    if (QFile remote_script(remote_script_path_); !remote_script.copy(local_job_location_ + "lsf/" + scriptName)) {
        // throw exception
        throw std::runtime_error("Could not copy the remote script " + remote_script_path_.toStdString() +
                                 " to the local job location " + local_job_location_.toStdString() + " Error: " +
                                 remote_script.errorString().toStdString());
    }
    qDebug() << "Copied the remote script " << remote_script_path_ << " to the local job location "
             << local_job_location_ + "lsf/" + scriptName;

    // Copy the remote launchers to the local job location
    if (QFile remote_launchers(remote_launchers_path_); !remote_launchers.copy(
            local_job_location_ + "lsf/" + launcherName)) {
        // throw exception
        throw std::runtime_error("Could not copy the remote launchers " + remote_launchers_path_.toStdString() +
                                 " to the local job location " + local_job_location_.toStdString() + " Error: " +
                                 remote_launchers.errorString().toStdString());
    }
    qDebug() << "Copied the remote launchers " << remote_launchers_path_ << " to the local job location "
             << local_job_location_ + "lsf/" + launcherName;

}

void BaseScript::replaceScriptParameters(const QString &scriptName, const QString &launcherName) const {
    // Open the script file
    QFile script_file(local_job_location_ + "lsf/" + scriptName);
    if (!script_file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        // throw exception
        throw std::runtime_error("Could not open the script file, error: " + script_file.errorString().toStdString());
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

void BaseScript::appendResubmissionJobExecutionLine(const QString &executionLine, const QString &scriptName) const {
    // Open the script file
    QFile script_file(local_job_location_ + "lsf/" + scriptName);
    if (!script_file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        // throw exception
        throw std::runtime_error("Appening: Could not open the script file " + scriptName.toStdString() + " Error: " +
                                 script_file.errorString().toStdString() + " at location " +
                                 local_job_location_.toStdString());
    }

    // Read the script file
    QTextStream script_stream(&script_file);
    QString script = script_stream.readAll();

    // Append the resubmission job execution line
    script.append(executionLine);

    // Write the script file
    script_file.resize(0);
    script_stream << script;
    script_file.close();

}

QString BaseScript::executeScript(const QString &scriptName) const {
    QProcess process;
    process.setWorkingDirectory(local_job_location_ + "lsf");

    QString fullScriptPath = local_job_location_ + "lsf/" + scriptName;
    qDebug() << "Executing script: " << fullScriptPath;

    process.start("cmd.exe", QStringList() << "/c" << fullScriptPath);
    process.waitForFinished(-1);
    return process.readAllStandardOutput();
}





