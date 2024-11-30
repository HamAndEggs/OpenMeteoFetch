/*
   Copyright (C) 2024, Richard e Collins.

   https://open-meteo.com/en/docs

   Original code base is at https://github.com/HamAndEggs/OpenMeteo
*/

#include "open-meteo.h"
#include "TinyJson/TinyJson.h"

#include <sstream>
#include <iomanip>
#include <iostream>
#include <exception>

namespace openmeteo { // Using a namespace to try to prevent name clashes as my class names are kind of obvious :)
///////////////////////////////////////////////////////////////////////////////////////////////////////////

OpenMeteo::OpenMeteo(const std::string& jsonString)
{
	tinyjson::JsonProcessor json(jsonString);
	const tinyjson::JsonValue& root = json.GetRoot();

	const tinyjson::JsonValue& hourly = root["hourly"];

	for( size_t n = 0 ; n < hourly["time"].GetArraySize() ; n++ )
	{
		const std::string timeString = hourly["time"][n];
		const float temperature = hourly["temperature_2m"][n];
		const int precipitation_probability = hourly["precipitation_probability"][n];
		const int weather_code = hourly["weather_code"][n];
		const int cloud_cover = hourly["cloud_cover"][n];
		const int visibility = hourly["visibility"][n];
		const float wind_speed_10m = hourly["wind_speed_10m"][n];
		const bool is_day = hourly["wind_speed_10m"][n].GetInt() == 1;

		const std::tm ctime = GetCTime(timeString);


		Forcast.push_back({
				timeString,
				GetUnixTime(ctime),
				ctime,
				temperature,
				precipitation_probability,
				weather_code,
				MakeIconCode(weather_code),
				cloud_cover,
				visibility,
				wind_speed_10m,
				is_day
			});
	}
}

const std::string OpenMeteo::MakeIconCode(int weatherCode)const
{
	return "10";
}

const Hourly OpenMeteo::GetForcast(int day,int month,int year,int hour)const
{
	for( Hourly h : Forcast )
	{
		if( h.ctime.tm_mday == day &&
			h.ctime.tm_mon + 1 == month &&
			h.ctime.tm_year + 1900 == year &&
			h.ctime.tm_hour == hour)
		{
			return h;
		}
	}

	throw std::runtime_error("Hourly forcast was not found for the date and time passed" + std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year) + " " + std::to_string(hour) + ":00");
}

const std::tm OpenMeteo::GetCTime(const std::string timeString)const
{
	std::tm t{};
	std::istringstream ss(timeString);

	ss >> std::get_time(&t, "%Y-%m-%dT%H:%M");
	if (ss.fail()) {
		throw std::runtime_error{ "failed to parse time string" };
	}
	return t;
}

const uint64_t OpenMeteo::GetUnixTime(std::tm t)const
{
	std::time_t time_stamp = mktime(&t);
	return time_stamp;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////
}; //
