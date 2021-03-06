/*
 * bing_search.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: chris
 */

#include "bingSearch.hpp"

using namespace std;

bing_search::bing_search()
{
	json_txt.memory=(char *)malloc(1);
	json_txt.size = 0;
	bing_crl_hndl=curl_easy_init();
	headers=NULL;
	headers = curl_slist_append(headers, USER_AGENT);
	headers = curl_slist_append(headers, "Host: api.cognitive.microsoft.com");

	curl_easy_setopt(bing_crl_hndl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(bing_crl_hndl, CURLOPT_HTTPGET, 1);
	curl_easy_setopt(bing_crl_hndl, CURLOPT_USE_SSL, CURLUSESSL_ALL); //Bing API requires SSL connection
	curl_easy_setopt(bing_crl_hndl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(bing_crl_hndl, CURLOPT_WRITEDATA, (void *)&json_txt);

}

int bing_search::set_key(std::string api_key_INPUT)
{
	std::string key_header = "Ocp-Apim-Subscription-Key: ";
	if(api_key.empty())
	{
		api_key.assign(api_key_INPUT);
	}
	else
	{
		api_key.clear();
		api_key.assign(api_key_INPUT);

		//Reset entire header info
		curl_slist_free_all(headers);
		headers=NULL;
		headers = curl_slist_append(headers, USER_AGENT);
		if (headers == NULL)//check if it did allocate header
		   return -1;
		headers = curl_slist_append(headers, "Host: api.cognitive.microsoft.com");
	}

	key_header.append(api_key);

	headers = curl_slist_append(headers,  key_header.c_str());
	curl_easy_setopt(bing_crl_hndl, CURLOPT_HTTPHEADER, headers);
	return 0;
}

size_t bing_search::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = (char *)realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
	/* out of memory! */
	printf("not enough memory (realloc returned NULL)\n");
	return 0;
  }

  std::memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}
