/*
   Copyright 2010-2011 Tyslenko Max, Bogatirev Pavel.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <vector>
#include <fstream>
#include <string>

#include <boost/serialization/singleton.hpp>
#include <boost/filesystem.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

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
inline void LOG(const std::string &message)
{
    logManager().write(message, "", "");
}

// data : message (into passed file)
inline void LOG_FILE(const std::string &message, const std::string &filename)
{
    LogManager().write(message, filename);
}

// message
inline void LOG_NOFORMAT(const std::string &message)
{
    logManager().write(message, "", "", false);
}

// date : function \n message
#define LOG_META(message) logManager().write(message, "", __PRETTY_FUNCTION__)
