//
// Created by SD43247 on 04/07/2024.
//

#ifndef MYGALE_JOB_H
#define MYGALE_JOB_H

#include <QHash>
#include <QString>

class Job {
private:
    QHash<QString, QString> job_parameters_;
    QString job_name_;
    QString job_type_;

    Job() = default;

public:
    void setJobName(const QString &jobName);

    void setJobType(const QString &jobType);

    void setJobParameters(const QHash<QString, QString> &jobParameters);

    [[nodiscard]] QString getJobName() const;

    [[nodiscard]] QString getJobType() const;

    [[nodiscard]] QHash<QString, QString> getJobParameters() const;

    class Builder {
    private:
        QHash<QString, QString> job_parameters_;
        QString job_name_;
        QString job_type_;

    public:
        Builder() = default;

        Builder &setJobName(const QString &jobName);

        Builder &setJobType(const QString &jobType);

        Builder &addJobParameter(const QString &key, const QString &value);

        Builder &setJobParameters(const QHash<QString, QString> &jobParameters);

        Job build();
    };

};


#endif //MYGALE_JOB_H
