/*
   Copyright 2010 Tyslenko Max, Bogatirev Pavel.

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

#ifndef _UTILS_H_
#define _UTILS_H_

#include <boost/serialization/singleton.hpp>
#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/algorithm/string.hpp>
#include <Box2D/Box2D.h>
#include <ClanLib/core.h>

#include "Core/LogManager.hpp"
#include "Core/ConfigManager.hpp"

class Utils : public boost::serialization::singleton<Utils>
{
    protected:
        boost::posix_time::ptime mTimeStamp;
        std::string mMediaFolder;

    public:
        std::string checkLocation(const std::string &path);

        void setMediaFolder(const std::string &path);
        std::string getMediaFolder();

        // For time measuring
        boost::posix_time::ptime getCurrentTime() const;
        void writeTimestamp();
        std::string getTimeDifference();
        std::string intToStr(int p);
        std::string hexToStr(int p);
        std::string floatToStr(float p);
        std::string vectorToStr(b2Vec2 p);
        std::string boolToStr(bool p);
};

/////////////////////////////////////////////////////////////////////////
//////////////////////// UTILS FUNCTIONS ////////////////////////////////
const float GAME_VERSION = configManager().getValue<float>("application.version", 1.0);

// Check the location exists
inline std::string LOCATION(const std::string &path) { return Utils::get_mutable_instance().checkLocation(path); }

// Find random number between [lower; 1.0]
inline float Randf(float lower) { return ( (rand()%10 + lower) / 10.0f ); }

// Window resolution
const int ScreenResolutionY = configManager().getValue<int>("window.height", 1024);
const int ScreenResolutionX = configManager().getValue<int>("window.width", 768);

/////////////////////////////////////////////////////////////////////////
/////////////////////// GAME MAGNITUDES /////////////////////////////////

// Don't ask me where did I take this
const float MagicKoef = configManager().getValue<float>("application.resolution_koef", 19.2f);

// How many pixels in one game meter in current screen height
const float PixelsPerMeter = (float)ScreenResolutionY / MagicKoef;

// Some conversions
inline float Meters2Pixels(float meters) { return meters * PixelsPerMeter; }
inline float Pixels2Meters(float pixels) { return pixels / PixelsPerMeter; }

/////////////////////////////////////////////////////////////////////////
/////////////////////////// GAME MATH ///////////////////////////////////

// Pi number
const float Pi = 3.1415926535898f;

// Radian → Degree
inline float Rad2Deg(float rad) { return rad * 57.2957795130823; }

// Degree → Radian
inline float Deg2Rad(float deg) { return deg * 0.0174532925199433; }

// The speed of free falling
const float Gravity = 9.81 * PixelsPerMeter;

/////////////////////////////////////////////////////////////////////////
//////////////////////// TIME MANAGEMENT ////////////////////////////////

// Game time is 220(default) times faster than in real life (we can change this during runtime)
const float TimeKoef = configManager().getValue<float>("application.game_speed", 220.0f);

//---------- Real time → game time

// How many game-ms is in passed real microseconds
inline float GameMicroseconds(float ms) { return ms * TimeKoef; }

// How many game-seconds is in passed real microseconds
inline float GameSeconds(float ms) { return GameMicroseconds(ms) / 1000.0f; }

// How many game-minutes is in passed real microseconds
inline float GameMinutes(float ms) { return GameSeconds(ms) / 60.0f; }

// How many game-hours is in passed real microseconds
inline float GameHours(float ms) { return GameMinutes(ms) / 60.0f; }

//---------- Real time → game time

// Get number of game ms to achive passed real minutes
inline float Microseconds2Seconds(float realMs) { return realMs * TimeKoef / 1000.0f; }

// Get number of game seconds to achive passed real minutes
inline float Seconds2Seconds(float realSeconds) { return realSeconds * TimeKoef; }

// Get number of game seconds to achive passed real minutes
inline float Minutes2Seconds(float realMinutes) { return Seconds2Seconds(realMinutes * 60.0f); }

// Get number of game seconds to achive passed real hours
inline float Hours2Seconds(float realHours) { return Minutes2Seconds(realHours * 60.0f); }

/*TODO: Replace this defines with inline functions */

#define IntToStr(p) (utils().intToStr(p))
#define FloatToStr(p) (utils().floatToStr(p))
#define VectorToStr(p) (utils().vectorToStr(p))
#define HexToStr(p) (utils().hexToStr(p))
#define BoolToStr(p) (utils().boolToStr(p))

inline Utils &utils() { return Utils::get_mutable_instance(); }

#endif /* _UTILS_H_ */
