/*
 * bingImageSearch.cpp
 *
 *  Created on: Nov 7, 2016
 *      Author: Christian Macias
 */

#include "bingSearch.hpp"

using namespace std;

int bing_search::image_search(std::string keywords)
{
	std::string URL;
	const std::string domain = "https://api.cognitive.microsoft.com/bing/v5.0/images/search?q=";
	const std::string market = "&mkt=en-us";
	std::string query;

	//reset json memory
	if(json_txt.memory!=NULL)
	{
		free(json_txt.memory);
		json_txt.memory=NULL;
	}

	json_txt.memory=(char *)malloc(1);
	json_txt.size = 0;

	char *c_keywords = (char *) keywords.c_str();

	if(json_txt.memory==NULL)
	{
		cerr<<"\nImage_search:"
				"\n json memory not allocated. "
				"\n no memory available";
		return -1;
	}

	else if(api_key.empty())
	{
		std::cout<<"\nImage_search: "
				"\n please enter your bing API key before attempting to search";
		return -1;
	}
	else if(c_keywords==NULL)
	{

		std::cout<<"\nImage_search: "
				"\n Error while checking keywords.";
		return -1;
	}


	URL.assign(domain);

	//Replace spaces with +
	c_keywords=strtok(c_keywords," ");
	while(c_keywords!=NULL)
	{
		URL.append(c_keywords);
		c_keywords=strtok(NULL," ");
		if(c_keywords!=NULL)
		{
			URL.append("+");
		}
	}

	URL.append(market);

	curl_easy_setopt(bing_crl_hndl, CURLOPT_URL, URL.c_str());

	if(curl_easy_perform(bing_crl_hndl) != CURLE_OK)
	{
		  cerr<<"\nImage_search (libcurl):"
			  "\n unable to perform search. Check internet connection";
		  return -1;

	}

	else if(json_txt.size==0)
	{
		cerr<<"\nImage_search:"
				"\n no response from image search "
				"\n make sure you have a valid key";
		free(json_txt.memory);//Free any previously allocated data
		json_txt.size = 0;
		return -1;
	}

	return 0;

}

string bing_search::image_url(int result_index_number)
{
	std::string URL_result;
	if(json_txt.size==0)
	{
		cerr<<"\nImage _url:"
				"\n no data received from search result"
				"\n make sure you have performed an image search";
		return URL_result;
	}

	cJSON * root = cJSON_Parse(json_txt.memory);
	cJSON * value = cJSON_GetObjectItem(root,"value");
	cJSON * result = cJSON_GetArrayItem(value, result_index_number);
	cJSON * json_url = cJSON_GetObjectItem(result, "contentUrl");

	if(!json_url)
	{
		cerr<<"\nImage_url:"
				"\n no data received from search result"
				"\n ensure you performed an image search";
		return URL_result;//return blank string
	}
	URL_result.assign(cJSON_Print(json_url));
	URL_result.erase(URL_result.begin());//Remove quotes
	URL_result.erase(URL_result.end() - 1);

	return URL_result;

}

string bing_search::image_format(int result_index_number)
{
	std::string format_result;

	cJSON * root = cJSON_Parse(json_txt.memory);
	cJSON * value = cJSON_GetObjectItem(root,"value");
	cJSON * result = cJSON_GetArrayItem(value, result_index_number);
	cJSON * json_url = cJSON_GetObjectItem(result, "encodingFormat");

	if(!json_url)
	{
		std::cout<<"\nImage_format"
				"\n no data recieved from search result"
				"\n make sure you have performed an image search";
		return format_result;
	}
	format_result.assign(cJSON_Print(json_url));
	format_result.erase(format_result.begin());//Remove quotes
	format_result.erase(format_result.end() - 1);

	return format_result;
}
