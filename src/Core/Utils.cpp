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

#include "Core/Utils.hpp"

std::string Utils::mMediaFolder = "media";
boost::posix_time::ptime Utils::mTimeStamp = boost::posix_time::ptime();

void Utils::setMediaFolder(const std::string &path)
{
    mMediaFolder = path;
}

std::string Utils::getTemplateFolder()
{
    return getMediaFolder() + "/templates";
}

std::string Utils::getCommonTemplateFile()
{
    return  getTemplateFolder() + "/common.tpl";
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

boost::posix_time::ptime Utils::getCurrentTime()
{
    return boost::posix_time::microsec_clock::local_time();
}

std::string Utils::checkLocation(const std::string &path)
{
    if (!boost::filesystem::exists(path))
    {
        LOG(cl_format("The path `%1` could be invalid!", path));
    }

    return path;
}

std::string Utils::getTimeDifference()
{
    // If we don't call writeTimestamp before this function
    if (mTimeStamp.is_not_a_date_time()) LOG("You are trying to get time difference but you have to call writeTimestamp first!");

    boost::posix_time::time_duration result = getCurrentTime() - mTimeStamp;
    mTimeStamp = boost::posix_time::ptime(boost::posix_time::not_a_date_time);

    return boost::posix_time::to_simple_string(result);
}
