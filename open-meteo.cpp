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

const std::string Hourly::toString()const
{
	std::stringstream s;
	s << " Time Source:" << time_string 
		<< " Unix Time:" << unix_time
		<< " Human Time:" << ctime.tm_mday << "/" << ctime.tm_mon + 1 << "/" << 1900 + ctime.tm_year << " " << ctime.tm_hour << ":" << ctime.tm_min
		<< " Precipitation Probability:" << precipitation_probability << "%"
		<< " Weather Code:" << weather_code
		<< " Icon Code:" << icon_code
		<< " visibility:" << visibility << "M"
		<< " cloud_cover:" << cloud_cover << "%"
		<< " Wind Speed:" << wind_speed_10m << "M/s"
		<< " " << (is_day ? "DAY" : "NIGHT");
	return s.str();
}

OpenMeteo::OpenMeteo(const std::string& jsonString)
{
	tinyjson::JsonProcessor json(jsonString);
	const tinyjson::JsonValue& root = json.GetRoot();

	const tinyjson::JsonValue& hourly = root["hourly"];
	// Git anoying but they return a load of arrays for each reading you asked for and you have to assume all are the same size.
	for( size_t n = 0 ; n < hourly["time"].GetArraySize() ; n++ )
	{
		const std::string timeString = hourly["time"][n];
		const float temperature = hourly["temperature_2m"][n];
		const int precipitation_probability = hourly["precipitation_probability"][n];
		const int weather_code = hourly["weather_code"][n];
		const int cloud_cover = hourly["cloud_cover"][n];
		const int visibility = hourly["visibility"][n];
		const float wind_speed_10m = hourly["wind_speed_10m"][n];
		const bool is_day = hourly["is_day"][n].GetInt() == 1;

		const std::tm ctime = GetCTime(timeString);


		Forcast.push_back({
				timeString,
				GetUnixTime(ctime),
				ctime,
				temperature,
				precipitation_probability,
				weather_code,
				MakeIconCode(weather_code,is_day),
				cloud_cover,
				visibility,
				wind_speed_10m,
				is_day
			});
	}
}

const std::string OpenMeteo::MakeIconCode(int weatherCode,bool is_day)const
{
	std::string icon = "01";
	// Converts the day mode and eather code to the icon code used by open weather map
	switch(weatherCode)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		icon = "0" + std::to_string(weatherCode+1);
		break;

	case 45:
	case 48:
		icon = "50";
		break;

	case 51:
	case 56:
	case 61:
	case 66:
	case 80:
		icon = "09";
		break;

	case 53:
	case 55:
	case 57:
	case 63:
	case 65:
	case 67:
	case 81:
	case 82:
		icon = "10";
		break;

	case 71:
	case 73:
	case 75:
	case 77:
	case 85:
	case 86:
		icon = "13";
		break;

	case 95:
	case 96:
	case 99:
		icon = "11";
		break;
	}

	return icon + (is_day?"d":"n");
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
