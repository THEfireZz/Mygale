//
// Created by SD43247 on 30/07/2024.
//

#ifndef MYGALE_CUSTOMERRORS_H
#define MYGALE_CUSTOMERRORS_H


#include <exception>
#include <QString>
#include <utility>

//FileCopyException
//FileOpenException
//ParcStyleException
//BatchCalculationException
//PathNotFoundException
//NotRemoteDriveException
//JobAlreadyExistsException
//MissingRequiredParameterException
//XmlParseException

class CustomErrors : public std::exception {
private:
    const QString message_;
public:
    explicit CustomErrors(QString message) : message_(std::move(message)) {}

    [[nodiscard]] const char *what() const noexcept override {
        return message_.toStdString().c_str();
    }
};

class FileCopyException : public CustomErrors {
public:
    explicit FileCopyException(QString message) : CustomErrors(std::move(message)) {}
};

class FileOpenException : public CustomErrors {
public:
    explicit FileOpenException(QString message) : CustomErrors(std::move(message)) {}
};

class ParcStyleException : public CustomErrors {
public:
    explicit ParcStyleException(QString message) : CustomErrors(std::move(message)) {}
};

class BatchCalculationException : public CustomErrors {
public:
    explicit BatchCalculationException(QString message) : CustomErrors(std::move(message)) {}
};

class PathNotFoundException : public CustomErrors {
public:
    explicit PathNotFoundException(QString message) : CustomErrors(std::move(message)) {}
};

class NotRemoteDriveException : public CustomErrors {
public:
    explicit NotRemoteDriveException(QString message) : CustomErrors(std::move(message)) {}
};

class JobAlreadyExistsException : public CustomErrors {
public:
    explicit JobAlreadyExistsException(QString message) : CustomErrors(std::move(message)) {}
};

class MissingRequiredParameterException : public CustomErrors {
public:
    explicit MissingRequiredParameterException(QString message) : CustomErrors(std::move(message)) {}
};

class XmlParseException : public CustomErrors {
public:
    explicit XmlParseException(QString message) : CustomErrors(std::move(message)) {}
};

#endif //MYGALE_CUSTOMERRORS_H
