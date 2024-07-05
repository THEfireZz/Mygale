//
// Created by SD43247 on 04/07/2024.
//

#include "Job.h++"

/**
 * @class Job
 * @brief This method sets the job name
 *
 * @param jobName The name of the job
 **/
void Job::setJobName(const QString &jobName) {
    job_name_ = jobName;
}

/**
 * @class Job
 * @brief This method sets the job type
 *
 * @param jobType The type of the job
 **/
void Job::setJobType(const QString &jobType) {
    job_type_ = jobType;
}

/**
 * @class Job
 * @brief This method sets the job parameters
 *
 * @param jobParameters The list of parameters of the job with their keys and values
 **/
void Job::setJobParameters(const QHash<QString, QString> &jobParameters) {
    job_parameters_ = jobParameters;
}

QString Job::getJobName() const {
    return job_name_;
}

QString Job::getJobType() const {
    return job_type_;
}

QHash<QString, QString> Job::getJobParameters() const {
    return job_parameters_;
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
Job Job::Builder::build() {
    Job job;
    job.setJobName(job_name_);
    job.setJobType(job_type_);
    job.job_parameters_ = job_parameters_;
    return job;
}

Job::Builder &Job::Builder::setJobParameters(const QHash<QString, QString> &jobParameters) {
    job_parameters_ = jobParameters;
    return *this;
}


