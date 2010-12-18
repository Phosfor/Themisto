/*
 * Copyright (c) 2010 Tyslenko Max (Ockonal), Bogatirev Pavel (PFight)
 * This file is part of Themisto (Themisto project at https://github.com/Ockonal/Themisto).
 * Project is contributed with GPL license. For more information, visit project page.
 */

#include "Core/Utils.hpp"

void Utils::setMediaFolder(const std::string &path)
{
    mMediaFolder = path;
}

std::string Utils::intToStr(int p)
{
    std::ostringstream out;
    out << p;
    return out.str(); 
}

std::string Utils::vectorToStr(b2Vec2 p)
{
    std::ostringstream out;
    out << "(" << p.x << "," << p.y << ")";
    return out.str();
}

std::string Utils::boolToStr(bool p)
{
    if(p) return "true";
    else return "false";
}

std::string Utils::hexToStr(int p)
{
    std::ostringstream out;
    std::cout.flags(std::ios::hex);
    std::cout.fill('0');
    std::cout.width(6);
    out << p;
    return out.str();
}

std::string Utils::floatToStr(float p)
{
    std::ostringstream out;
    out << p;
    return out.str(); 
}

std::string Utils::getMediaFolder()
{
    return mMediaFolder;
}

boost::posix_time::ptime Utils::getCurrentTime() const
{
    using namespace boost;
    return posix_time::microsec_clock::local_time();
}

std::string Utils::checkLocation(const std::string &path)
{
    using namespace boost;
    if (!filesystem::exists(path))
    {
        LOG(cl_format("The path `%1` could be invalid!", path));
    }

    return path;
}

std::string Utils::getTimeDifference()
{
    using namespace boost;
    using namespace boost::posix_time;

    // If we don't call writeTimestamp before this function
    if (mTimeStamp.is_not_a_date_time()) LOG("You are trying to get time difference but you have to call writeTimestamp first!");

    time_duration result = getCurrentTime() - mTimeStamp;
    mTimeStamp = ptime(not_a_date_time);

    return to_simple_string(result);
}
