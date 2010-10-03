#ifndef _UTILS_H_
#define _UTILS_H_

#include <boost/serialization/singleton.hpp>
#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/algorithm/string.hpp>

#include "Core/LogManager.hpp"

using namespace std;
using namespace boost;
using namespace boost::serialization;
using namespace boost::posix_time;

// Make formatted string. Usage: FORMAT("some %1% string %2%", firstArg % secondArg);
#define FORMAT(parentString, params) (boost::format(parentString) % params).str()

// Check the location exists
#define LOCATION(path) Utils::get_mutable_instance().checkLocation(path)

/*TODO: Calculate for different resolutions */
#define PixelsPerMeter 40.0

// 40 pixels ~ 1 meter
#define Meters2Pixels(meters) ((float)meters*PixelsPerMeter)
#define Pixels2Meters(pixels) ((float)pixels/PixelsPerMeter)

// Pi number (took from google)
#define Pi 3.14159265

// Radian → Degree
#define Rad2Deg(rad) (rad * 180 / Pi)

// Degree → Radian
#define Deg2Rad(deg) (deg * Pi/180)

// The speed of free falling
#define G Meters2Pixels(9.81)

#define utils (Utils::get_mutable_instance())
#define utilsConst (Utils::get_const_instance())

class Utils : public boost::serialization::singleton<Utils>
{
    protected:
        ptime mTimeStamp;
        string mMediaFolder;

    public:
        string checkLocation(const string &path);

        void setMediaFolder(const string &path);
        string getMediaFolder();

        // For time measuring
        ptime getCurrentTime() const;
        void writeTimestamp();
        string getTimeDifference();
};

#endif

