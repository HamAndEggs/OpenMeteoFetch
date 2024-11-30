/*
   Copyright (C) 2024, Richard e Collins.

   https://open-meteo.com/en/docs

   Original code base is at https://github.com/HamAndEggs/OpenMeteo
*/
#ifndef OPEN_METEO_H__
#define OPEN_METEO_H__

#include <stdexcept>
#include <memory>
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <assert.h>

namespace openmeteo { // Using a namespace to try to prevent name clashes as my class names are kind of obvious :)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Hourly
{// to be used with the following API call.
 // https://api.open-meteo.com/v1/forecast?latitude= YOUR LAT &longitude= YOUR LONG &hourly=temperature_2m,precipitation_probability,weather_code,cloud_cover,visibility,wind_speed_10m,is_day
    const uint64_t time;
    const float temperature_2m;
    const int precipitation_probability; //0 -> 100
    const int weather_code;
    const std::string icon_code;
    const int cloud_cover; // 0 -> 100
    const int visibility; // View distance in meters
    const float wind_speed_10m;
    const bool is_day;
};

struct OpenMeteo
{
    OpenMeteo(const std::string& pJSONString);

    const std::vector<Hourly> GetForcast()const {return Forcast;}

private:
    std::vector<Hourly> Forcast;
    const std::string MakeIconCode(int weatherCode)const;
    const uint64_t GetUnixTime(const std::string timeString)const;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////

}; //



#endif //OPEN_METEO_H__
