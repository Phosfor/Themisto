#ifndef _UTILS_H_
#define _UTILS_H_

#include <boost/serialization/singleton.hpp>
#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/algorithm/string.hpp>

#include "Core/LogManager.hpp"

using namespace std;

/////////////////////////////////////////////////////////////////////////
//////////////////////// UTILS FUNCTIONS ////////////////////////////////

// Make formatted string. Usage: FORMAT("some %1% string %2%", firstArg % secondArg);
#define FORMAT(parentString, params) (boost::format(parentString) % params).str()

// Check the location exists
#define LOCATION(path) Utils::get_mutable_instance().checkLocation(path)

// Find random number between [lower; 1.0]
#define Randf(lower) ((float)(rand()%10 + (float)lower)/10.0f)

// Game window resolution (height)
#define ScreenResolutionY appManager.getGraphic().get_height()

/////////////////////////////////////////////////////////////////////////
/////////////////////// GAME MAGNITUDES /////////////////////////////////

// Don't ask me where did I take this
#define MagicKoef 19.2

// How many pixels in one game meter in current screen height
#define PixelsPerMeter (ScreenResolutionY / MagicKoef)

// 40 pixels ~ 1 meter
#define Meters2Pixels(meters) ((float)(meters)*PixelsPerMeter)
#define Pixels2Meters(pixels) ((float)(pixels)/PixelsPerMeter)

/////////////////////////////////////////////////////////////////////////
/////////////////////////// GAME MATH ///////////////////////////////////

// Pi number (took from google)
#define Pi 3.14159265

// Radian → Degree
#define Rad2Deg(rad) ((float)(rad * 180 / Pi))

// Degree → Radian
#define Deg2Rad(deg) ((float)(deg * Pi/180))

// The speed of free falling
#define G Meters2Pixels(9.81)

/////////////////////////////////////////////////////////////////////////
//////////////////////// TIME MANAGEMENT ////////////////////////////////

// Game time is 220 times faster than in real life
#define TimeKoef 3000.0f

//---------- Real time → game time

// How many game-ms is in passed real microseconds
#define GameMicroseconds(microseconds) ((float)(microseconds * TimeKoef))

// How many game-seconds is in passed real microseconds
#define GameSeconds(microseconds) ((float)(GameMicroseconds(microseconds) / 1000.0f))

// How many game-minutes is in passed real microseconds
#define GameMinutes(microseconds) ((float)(GameSeconds(microseconds) / 60.0f))

// How many game-hours is in passed real microseconds
#define GameHours(microseconds) ((float)(GameMinutes(microseconds) / 60.0f))

//---------- Real time → game time

// Get number of game ms to achive passed real minutes
#define Microseconds2Seconds(realMs) ((float)(realMs * TimeKoef / 1000.0f))

// Get number of game seconds to achive passed real minutes
#define Seconds2Seconds(realSeconds) ((float)(realSeconds * TimeKoef))

// Get number of game seconds to achive passed real minutes
#define Minutes2Seconds(realMinutes) ((float)(Seconds2Seconds(realMinutes * 60.0f)))

// Get number of game seconds to achive passed real hours
#define Hours2Seconds(realHours) ((float)(Minutes2Seconds(realHours * 60.0f)))

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

