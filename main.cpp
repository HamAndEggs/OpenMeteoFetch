
#include <iostream>
#include <curl/curl.h> // libcurl4-openssl-dev
#include <string>

#include "open-meteo.h"

static int CURLWriter(char *data, size_t size, size_t nmemb,std::string *writerData)
{
	if(writerData == NULL)
		return 0;

	writerData->append(data, size*nmemb);

	return size * nmemb;
}

std::string DownloadJson(const std::string& pURL)
{
    CURL *curl = curl_easy_init();
    std::string result;
    if(curl)
    {
        char errorBuffer[CURL_ERROR_SIZE];
        errorBuffer[0] = 0;
        if( curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer) == CURLE_OK )
        {
            if( curl_easy_setopt(curl, CURLOPT_URL, pURL.c_str()) == CURLE_OK )
            {
                if( curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CURLWriter) == CURLE_OK )
                {
                    std::string json;
                    if( curl_easy_setopt(curl, CURLOPT_WRITEDATA, &json) == CURLE_OK )
                    {
                        if( curl_easy_perform(curl) == CURLE_OK )
                        {
                            result = json;
                        }
                    }
                }
            }
        }

        if( result.size() == 0 )
		{
			std::cerr << "Lib curl, failed, [" << errorBuffer << "]\n";
		}


        /* always cleanup */ 
        curl_easy_cleanup(curl);
    }

    return result;
}


int main()
{
    const std::string openMeta = 
        "https://api.open-meteo.com/v1/forecast?"
        "latitude=51.50985954887405&"
        "longitude=-0.12022833383470222&"
        "hourly=temperature_2m,precipitation_probability,weather_code,cloud_cover,visibility,wind_speed_10m,is_day";

    const std::string weatherJson = DownloadJson(openMeta);

    openmeteo::OpenMeteo weather(weatherJson);

    for( const std::string f : weather.GetForcast() )
    {
        std::cout << f << "\n";
    }

    try
    {
        const auto forcase = weather.GetForcast(30,11,2024,15);

        std::cout << "\n\n" << forcase.toString() << "\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << "Failed getting forcase:" << e.what() << '\n';
    }
    
}

