/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "Core/LogManager.hpp"

LogManager::LogManager()
{
    mDefaultPath = "Engine.log";
}

void LogManager::_forceLog(const std::string& logPath)
{
    if (std::find(mUsedFiles.begin(), mUsedFiles.end(), logPath) == mUsedFiles.end())
    {
        if (boost::filesystem::exists(logPath))
            boost::filesystem::remove(logPath);
        mUsedFiles.push_back(logPath);
    }
}

void LogManager::setDefaultLog(const std::string& logPath)
{
    mDefaultPath = logPath;
}

std::string LogManager::_cleanSignature(const std::string &signature)
{
    // Remove template stuff from signature. The std::string like: "type Class::function(arguments) [with ...]"
    // The goal is to make that std::string: Class::Function()
    // We really don't need arguments list and template insertion
    std::string result = signature;

    // Remove function return type
    int typePosition = result.find(" ");
    result = result.erase(0, typePosition+1); // Now we have "Class::function(arguments) [with ...]"

    // Remove function arguments and template stuff
    int argPosition = result.find("(");
    result = result.erase(argPosition);     // Now we have "Class::function"
    result += "()";

    return result;
}

void LogManager::write(const std::string &message, const std::string &logFile,
        const std::string &metaSignature, bool formatString)
{
    std::string workPath = mDefaultPath;
    if (logFile != "") workPath = logFile;

    this->_forceLog(workPath);

    std::string comand = "";
    std::string time = boost::posix_time::to_simple_string(boost::posix_time::microsec_clock::local_time());

    if (formatString)
    {
        if (metaSignature != "")
        {
            std::string signature = _cleanSignature(metaSignature);
            comand += time + " : ";
            comand += "[" + signature;
            comand += "] Message: ";
            comand += message + "\n";
        }
        else
        {
            comand += time + " : " + message + "\n";
        }
    }
    else
    {
        comand = message;
    }

    mLogHandle.open(workPath.c_str(), std::fstream::app | std::fstream::ate);
    mLogHandle << comand;
    std::cout << comand;
    mLogHandle.close();
}
