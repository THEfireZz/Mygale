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

    [[nodiscard]] QString getJobName() const;

    [[nodiscard]] QString getJobType() const;

    [[nodiscard]] QHash<QString, QString> getJobParameters() const;

    void addJobParameter(const QString &key, const QString &value);

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

        Job build() const;
    };

    void setJobName(QString newJobName);
};


#endif //MYGALE_JOB_H
