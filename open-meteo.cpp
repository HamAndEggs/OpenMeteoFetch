/*
   Copyright (C) 2024, Richard e Collins.

   https://open-meteo.com/en/docs

   Original code base is at https://github.com/HamAndEggs/OpenMeteo
*/

#include "open-meteo.h"
#include "TinyJson.h"

#include <sstream>
#include <iomanip>

namespace openmeteo { // Using a namespace to try to prevent name clashes as my class names are kind of obvious :)
///////////////////////////////////////////////////////////////////////////////////////////////////////////

OpenMeteo::OpenMeteo(const std::string& jsonString)
{
	tinyjson::JsonProcessor json(jsonString);
	const tinyjson::JsonValue& root = json.GetRoot();

	const tinyjson::JsonValue& hourly = root["hourly"];

	for( int n = 0 ; n < hourly["time"].GetArraySize() ; n++ )
	{
		const std::string time = hourly["time"][n];
		const float temperature = hourly["temperature_2m"][n];
		const int precipitation_probability = hourly["precipitation_probability"][n];
		const int weather_code = hourly["weather_code"][n];
		const int cloud_cover = hourly["cloud_cover"][n];
		const int visibility = hourly["visibility"][n];
		const float wind_speed_10m = hourly["wind_speed_10m"][n];
		const bool is_day = hourly["wind_speed_10m"][n].GetInt() == 1;



		Forcast.push_back({
				GetUnixTime(time),
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

const uint64_t OpenMeteo::GetUnixTime(const std::string timeString)const
{
	std::tm t{};
	std::istringstream ss(timeString);

	ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S");
	if (ss.fail()) {
		throw std::runtime_error{ "failed to parse time string" };
	}
	std::time_t time_stamp = mktime(&t);
	return time_stamp;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////
}; //
