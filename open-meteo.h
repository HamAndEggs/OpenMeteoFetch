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
    std::string time_string; // A direct copy of the time as sent to us.
    uint64_t unix_time;
    std::tm ctime;
    float temperature_2m;
    int precipitation_probability; //0 -> 100
    int weather_code;
    std::string icon_code;
    int cloud_cover; // 0 -> 100
    int visibility; // View distance in meters
    float wind_speed_10m;
    bool is_day;

    operator const std::string()const{return toString();}
    const std::string toString()const;

};

struct OpenMeteo
{
    OpenMeteo(const std::string& pJSONString);

    const std::vector<Hourly> GetForcast()const {return Forcast;}
    const Hourly GetForcast(int day,int month,int year,int hour)const;

private:
    std::vector<Hourly> Forcast;
    const std::string MakeIconCode(int weatherCode,bool is_day)const;
    const std::tm GetCTime(const std::string timeString)const;
    const uint64_t GetUnixTime(std::tm t)const;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////

}; //



#endif //OPEN_METEO_H__
