/*
 * bingImageSearch.cpp
 *
 *  Created on: Nov 7, 2016
 *      Author: Christian Macias
 */

#include "bingSearch.hpp"

using namespace std;

int bing_search::web_search(std::string keywords)
{
	std::string URL;
	const std::string domain = "https://api.cognitive.microsoft.com/bing/v5.0/search?q=";
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
		cerr<<"\nWeb_search:"
				"\n json memory not allocated. "
				"\n no memory available";
		return -1;
	}
	else if(api_key.empty() == 1)
	{
		std::cout<<"\nWeb_search: "
				"\n please enter your bing API key before attempting to search";
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
		  cerr<<"\nWeb_search (libcurl):"
			  "\n unable to perform search. Check internet connection";
		  return -1;

	}
	else if(json_txt.size==0)
	{
		cerr<<"\nWeb_search:"
				"\n no response from web search "
				"\n make sure you have a valid key";
		free(json_txt.memory);//Free any previously allocated data
		json_txt.size = 0;
		return -1;
	}

	return 0;

}

string bing_search::web_url(int result_index_number)
{
	std::string URL_result;
	if(json_txt.size==0)
	{
		cerr<<"\nWeb_url:"
				"\n no data received from search result"
				"\n make sure you have performed a web search";
		return URL_result;
	}

	cJSON * root = cJSON_Parse(json_txt.memory);
	cJSON * webpages = cJSON_GetObjectItem(root,"webPages");
	cJSON * value = cJSON_GetObjectItem(webpages,"value");
	cJSON * result = cJSON_GetArrayItem(value, result_index_number);
	cJSON * json_url = cJSON_GetObjectItem(result, "url");

	if(!json_url)
	{
		cerr<<"\nWeb_url:"
				"\n no data received from search result"
				"\n make sure you have performed a web search";
		return URL_result;
	}
	URL_result.assign(cJSON_Print(json_url));
	URL_result.erase(URL_result.begin());//Remove quotes
	URL_result.erase(URL_result.end() - 1);

	return URL_result;

}

string bing_search::web_title(int result_index_number)
{
	std::string URL_result;
	if(json_txt.size==0)
	{
		cerr<<"\nWeb_title:"
				"\n no data received from search result"
				"\n make sure you have performed a web search";
		return URL_result;
	}

	cJSON * root = cJSON_Parse(json_txt.memory);
	cJSON * webpages = cJSON_GetObjectItem(root,"webPages");
	cJSON * value = cJSON_GetObjectItem(webpages,"value");
	cJSON * result = cJSON_GetArrayItem(value, result_index_number);
	cJSON * json_url = cJSON_GetObjectItem(result, "name");

	if(!json_url)
	{
		cerr<<"\nWeb_title:"
				"\n no data received from search result"
				"\n make sure you have performed a web search";
		return URL_result;
	}
	URL_result.assign(cJSON_Print(json_url));
	URL_result.erase(URL_result.begin());//Remove quotes
	URL_result.erase(URL_result.end() - 1);

	return URL_result;

}

string bing_search::web_snippet(int result_index_number)
{
	std::string URL_result;
	if(json_txt.size==0)
	{
		cerr<<"\nWeb_snippet:"
				"\n no data received from search result"
				"\n make sure you have performed a web search";
		return URL_result;
	}

	cJSON * root = cJSON_Parse(json_txt.memory);
	cJSON * webpages = cJSON_GetObjectItem(root,"webPages");
	cJSON * value = cJSON_GetObjectItem(webpages,"value");
	cJSON * result = cJSON_GetArrayItem(value, result_index_number);
	cJSON * json_url = cJSON_GetObjectItem(result, "snippet");

	if(!webpages)
	{
		cerr<<"\nWeb_snippet:"
				"\n no data or incorrect data received from search result";
		return URL_result;
	}
	else if(!json_url)
	{
		//no snippet available
		return URL_result;
	}
	URL_result.assign(cJSON_Print(json_url));
	URL_result.erase(URL_result.begin());//Remove quotes
	URL_result.erase(URL_result.end() - 1);

	return URL_result;

}
