//
// Created by SD43247 on 30/07/2024.
//

#ifndef MYGALE_CUSTOMERRORS_H
#define MYGALE_CUSTOMERRORS_H


#include <exception>
#include <QString>
#include <utility>

class FileCopyException : public std::exception {
private:
    const QString message_;
public:
    explicit FileCopyException(QString message) : message_(std::move(message)) {}

    [[nodiscard]] const char *what() const noexcept override {
        return message_.toStdString().c_str();
    }
};

class FileOpenException : public std::exception {
private:
    const QString message_;
public:
    explicit FileOpenException(QString message) : message_(std::move(message)) {}

    [[nodiscard]] const char *what() const noexcept override {
        return message_.toStdString().c_str();
    }
};

class ParcStyleException : public std::exception {
private:
    const QString message_;
public:
    explicit ParcStyleException(QString message) : message_(std::move(message)) {}

    [[nodiscard]] const char *what() const noexcept override {
        return message_.toStdString().c_str();
    }
};

class BatchCalculationException : public std::exception {
private:
    const QString message_;
public:
    explicit BatchCalculationException(QString message) : message_(std::move(message)) {}

    [[nodiscard]] const char *what() const noexcept override {
        return message_.toStdString().c_str();
    }
};

class PathNotFoundException : public std::exception {
private:
    const QString message_;
public:
    explicit PathNotFoundException(QString message) : message_(std::move(message)) {}

    [[nodiscard]] const char *what() const noexcept override {
        return message_.toStdString().c_str();
    }
};

class NotRemoteDriveException : public std::exception {
private:
    const QString message_;
public:
    explicit NotRemoteDriveException(QString message) : message_(std::move(message)) {}

    [[nodiscard]] const char *what() const noexcept override {
        return message_.toStdString().c_str();
    }
};

class JobAlreadyExistsException : public std::exception {
private:
    const QString message_;
public:
    explicit JobAlreadyExistsException(QString message) : message_(std::move(message)) {}

    [[nodiscard]] const char *what() const noexcept override {
        return message_.toStdString().c_str();
    }
};

class MissingRequiredParameterException : public std::exception {
private:
    const QString message_;
public:
    explicit MissingRequiredParameterException(QString message) : message_(std::move(message)) {}

    [[nodiscard]] const char *what() const noexcept override {
        return message_.toStdString().c_str();
    }
};

class XmlParseException : public std::exception {
private:
    const QString message_;
public:
    explicit XmlParseException(QString message) : message_(std::move(message)) {}

    [[nodiscard]] const char *what() const noexcept override {
        return message_.toStdString().c_str();
    }
};

#endif //MYGALE_CUSTOMERRORS_H
