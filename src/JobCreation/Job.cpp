//
// Created by SD43247 on 04/07/2024.
//

#include <utility>
#include <QXmlStreamReader>
#include <qfile.h>

#include "Job.h++"
#include "../exception/CustomErrors.h++"

QString Job::getJobName() const {
    return job_name_;
}

QString Job::getJobType() const {
    return job_type_;
}

QHash<QString, QString> Job::getJobParameters() const {
    return job_parameters_;
}

void Job::addJobParameter(const QString &key, const QString &value) {
    job_parameters_[key] = value;
}

void Job::setJobName(QString newJobName) {
    job_name_ = std::move(newJobName);
}

void Job::checkRequiredParameters(const QString &configPath) const {
    QFile file(configPath + "mainConfig.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw FileOpenException("Could not open the file " + file.fileName());
    }

    QXmlStreamReader xmlReader(&file);
    QString currentJobType;

    while (!xmlReader.atEnd() && !xmlReader.hasError()) {
        xmlReader.readNext();

        if (xmlReader.isStartElement() && xmlReader.name().toString() == "Option") {
            currentJobType.clear();  // Reset the current job type for each Option block
        }

        if (xmlReader.isStartElement() && xmlReader.name().toString() == "name") {
            currentJobType = xmlReader.readElementText();
        }

        if (!currentJobType.isEmpty() && currentJobType == job_type_) {
            if (xmlReader.isStartElement() && xmlReader.name().toString() == "Required") {
                while (xmlReader.readNextStartElement()) {
                    if (xmlReader.name().toString() == "option") {
                        QString requiredOption = xmlReader.readElementText();
                        if (!job_parameters_.contains('<' + requiredOption + '>') ||
                            job_parameters_.value('<' + requiredOption + '>').isEmpty()) {
                            throw MissingRequiredParameterException(
                                    "The parameter " + requiredOption + " is required for the job type " + job_type_);
                        }
                    } else {
                        xmlReader.skipCurrentElement();
                    }
                }
            }

            if (xmlReader.isEndElement() && xmlReader.name().toString() == "Option") {
                currentJobType.clear();  // Reset the current job type after exiting an Option block
            }
        }
    }

    if (xmlReader.hasError()) {
        throw XmlParseException("Error while reading the XML file " + file.fileName());
    }
    file.close();
}

/**
 * @class Job
 * @brief This method sets the job name to the builder
 *
 * @param jobName The name of the job
 **/
Job::Builder &Job::Builder::setJobName(const QString &jobName) {
    job_name_ = jobName;
    return *this;
}

/**
 * @class Job
 * @brief This method sets the job type to the builder
 **/
Job::Builder &Job::Builder::setJobType(const QString &jobType) {
    job_type_ = jobType;
    return *this;
}

/**
 * @class Job
 * @brief This method adds a job parameter to the builder. Format is <key> : value
 *
 * @param key The key of the parameter
 * @param value The value of the parameter
 **/
Job::Builder &Job::Builder::addJobParameter(const QString &key, const QString &value) {
    job_parameters_[key] = value;
    return *this;
}

/**
 * @class Job
 * @brief This method builds the job object
 *
 * @return The job object
 **/
Job Job::Builder::build() const {
    Job job;
    job.job_name_ = job_name_;
    job.job_type_ = job_type_;
    job.job_parameters_ = job_parameters_;
    return job;
}

Job::Builder &Job::Builder::setJobParameters(const QHash<QString, QString> &jobParameters) {
    job_parameters_ = jobParameters;
    return *this;
}


