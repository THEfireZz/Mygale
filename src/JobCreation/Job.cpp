//
// Created by SD43247 on 04/07/2024.
//

#include <utility>
#include <QXmlStreamReader>
#include <qfile.h>

#include "Job.h++"
#include "../exception/CustomErrors.h++"

/**
 * @brief This method returns the job name
 *
 * @return The job name
 **/
QString Job::getJobName() const {
    return job_name_;
}

/**
 * @brief This method returns the job type
 *
 * @return The job type
 **/
QString Job::getJobType() const {
    return job_type_;
}

/**
 * @brief This method returns the job parameters
 *
 * @return The job parameters
 **/
QHash<QString, QString> Job::getJobParameters() const {
    return job_parameters_;
}

/**
 * @brief This method adds a job parameter to the job. Format is <key> : value
 *
 * @param key The key of the parameter
 * @param value The value of the parameter
 **/
void Job::addJobParameter(const QString &key, const QString &value) {
    job_parameters_[key] = value;
}

/**
 * @brief This method sets the job name
 *
 * @param newJobName The new job name
 **/
void Job::setJobName(QString newJobName) {
    job_name_ = std::move(newJobName);
}

/**
 * @brief This method checks if the required parameters are present in the job parameters
 *
 * @param configPath The path to the configuration file
 *
 * @throw MissingRequiredParameterException If a required parameter is missing
 * @throw FileOpenException If the configuration file could not be opened
 * @throw XmlParseException If there was an error while reading the XML file
 *
 **/
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
 * @brief This method sets the job name to the builder
 *
 * @param jobName The job name
 *
 * @return The builder object
 **/
Job::Builder &Job::Builder::setJobName(const QString &jobName) {
    job_name_ = jobName;
    return *this;
}

/**
 * @brief This method sets the job type to the builder
 *
 * @param jobType The job type
 *
 * @return The builder object
 **/
Job::Builder &Job::Builder::setJobType(const QString &jobType) {
    job_type_ = jobType;
    return *this;
}

/**
 * @brief This method adds a job parameter to the builder
 *
 * @param key The key of the parameter
 * @param value The value of the parameter
 *
 * @return The builder object
 **/
Job::Builder &Job::Builder::addJobParameter(const QString &key, const QString &value) {
    job_parameters_[key] = value;
    return *this;
}

/**
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

/**
 * @brief This method sets the job parameters to the builder
 *
 * @param jobParameters The job parameters
 *
 * @return The builder object
 **/
Job::Builder &Job::Builder::setJobParameters(const QHash<QString, QString> &jobParameters) {
    job_parameters_ = jobParameters;
    return *this;
}


