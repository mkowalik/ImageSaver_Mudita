// Michal Kowalik <kowalik.michal1@gmail.com>
#include <fstream>

#include "log_saver.h"

FileLogSaver::FileLogSaver(const std::string& _logFilePath) 
: fStream(_logFilePath)
{
}

void FileLogSaver::log(const std::exception& e)
{
    log(e.what(), LogType::ERROR);
}

void FileLogSaver::log(const std::string& s, LogSaver::LogType t)
{
    switch (t)
    {
    case LogSaver::LogType::ERROR:
        fStream << "Error: " << s << std::endl;
        break;
    
    case LogType::INFO:
        fStream << s << std::endl;
        break;
    }
}