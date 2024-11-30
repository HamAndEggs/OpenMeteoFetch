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
#include <sstream>
#include <utility>
#include <iomanip>
#include <assert.h>

namespace openmeteo { // Using a namespace to try to prevent name clashes as my class names are kind of obvious :)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Hourly
{// to be used with the following API call.
 // https://api.open-meteo.com/v1/forecast?latitude= YOUR LAT &longitude= YOUR LONG &hourly=temperature_2m,precipitation_probability,weather_code,cloud_cover,visibility,wind_speed_10m,is_day
    const std::string time_string; // A direct copy of the time as sent to us.
    const uint64_t unix_time;
    const std::tm ctime;
    const float temperature_2m;
    const int precipitation_probability; //0 -> 100
    const int weather_code;
    const std::string icon_code;
    const int cloud_cover; // 0 -> 100
    const int visibility; // View distance in meters
    const float wind_speed_10m;
    const bool is_day;

    operator const std::string()const
    {
        return toString();
    }

    const std::string toString()const
    {
        std::stringstream s;
        s << " Time Source:" << time_string 
          << " Unix Time:" << unix_time
          << " Human Time:" << ctime.tm_mday << "/" << ctime.tm_mon + 1 << "/" << 1900 + ctime.tm_year << " " << ctime.tm_hour << ":" << ctime.tm_min
          << " Precipitation Probability:" << precipitation_probability << "%"
          << " Weather Code:" << weather_code
          << " Icon Code:" << icon_code
          << " visibility:" << visibility << "M"
          << " Wind Speed:" << wind_speed_10m << "M/s"
          << " " << (is_day ? "DAY" : "NIGHT");
        return s.str();
    }

};

struct OpenMeteo
{
    OpenMeteo(const std::string& pJSONString);

    const std::vector<Hourly> GetForcast()const {return Forcast;}
    const Hourly GetForcast(int day,int month,int year,int hour)const;

private:
    std::vector<Hourly> Forcast;
    const std::string MakeIconCode(int weatherCode)const;
    const std::tm GetCTime(const std::string timeString)const;
    const uint64_t GetUnixTime(std::tm t)const;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////

}; //



#endif //OPEN_METEO_H__
