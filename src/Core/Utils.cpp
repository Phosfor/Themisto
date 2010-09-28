#include "Core/Utils.hpp"

void Utils::setMediaFolder(const string &path)
{
    mMediaFolder = path;
}

string Utils::getMediaFolder()
{
    return mMediaFolder;
}

ptime Utils::getCurrentTime() const
{
    return microsec_clock::local_time();
}

string Utils::checkLocation(const string &path)
{
    if (!boost::filesystem::exists(path))
    {
        LOG(FORMAT("The path `%1%` could be invalid!", path));
    }

    return path;
}

string Utils::getTimeDifference()
{
    // If we don't call writeTimestamp before this function
    if (mTimeStamp.is_not_a_date_time()) LOG("You are trying to get time difference but you have to call writeTimestamp first!");

    time_duration result = getCurrentTime() - mTimeStamp;
    mTimeStamp = ptime(not_a_date_time);

    return to_simple_string(result);
}
