/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#ifndef _LOG_MANAGER_H_
#define _LOG_MANAGER_H_

#include <boost/serialization/singleton.hpp>
#include <boost/filesystem.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include <vector>
#include <fstream>
#include <iostream>

class LogManager : public boost::serialization::singleton<LogManager>
{
    protected:
        std::ofstream mLogHandle;
        std::string mDefaultPath;
        std::vector<std::string> mUsedFiles;

        void _forceLog(const std::string &logPath);
        std::string _cleanSignature(const std::string &signature);

    public:
        LogManager();

        void setDefaultLog(const std::string &logPath);
        void write(const std::string &message, const std::string &logFile = "",
                const std::string &metaSignature = "", bool formatString = true);
};

inline LogManager &logManager() { return LogManager::get_mutable_instance(); }

// date : message
inline void LOG(const std::string &message) { logManager().write(message, "", ""); }

// data : message (into passed file)
inline void LOG_FILE(const std::string &message, const std::string &filename) { logManager().write(message, filename); }

// message
inline void LOG_NOFORMAT(const std::string &message) { logManager().write(message, "", "", false); }

// date : function \n message
#define LOG_META(message) logManager().write(message, "", __PRETTY_FUNCTION__)

#endif
