// Michal Kowalik <kowalik.michal1@gmail.com>
#pragma once

#include <stdexcept>
#include <string>
#include <fstream>

class LogSaver {
public:

    enum class LogType {
        ERROR,
        INFO
    };

    virtual void log(const std::exception& e) = 0;
    virtual void log(const std::string& s, LogType t) = 0;
};

class FileLogSaver : public LogSaver {
protected:
    std::ofstream fStream;
public:
    FileLogSaver(const std::string& logFilePath);
    virtual void log(const std::exception& e) override;
    virtual void log(const std::string& s, LogSaver::LogType t) override;
};