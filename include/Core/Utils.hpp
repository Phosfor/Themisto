#ifndef _UTILS_H_
#define _UTILS_H_

#include <boost/serialization/singleton.hpp>
#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/algorithm/string.hpp>

#include "Managers/LogManager.hpp"

using namespace std;
using namespace boost;
using namespace boost::serialization;
using namespace boost::posix_time;

// Make formatted string. Usage: FORMAT("some %1% string %2%", firstArg % secondArg);
#define FORMAT(parentString, params) (boost::format(parentString) % params).str()

// Check the location exists
#define LOCATION(path) Utils::get_mutable_instance().checkLocation(path)

#define utils (Utils::get_mutable_instance())
#define utilsConst (Utils::get_const_instance())

class Utils : public singleton<Utils>
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

