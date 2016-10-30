/*
 * bing_search.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: chris
 */

#include"bing_search.hpp"

bing_search::bing_search()
{
	json_txt.memory=(char *)malloc(1);
	json_txt.size = 0;
	//curl_global_init(CURL_GLOBAL_ALL);
	bing_crl_hndl=curl_easy_init();
	headers=NULL;

	headers = curl_slist_append(headers, USER_AGENT);
	headers = curl_slist_append(headers, "Host: api.cognitive.microsoft.com");

	curl_easy_setopt(bing_crl_hndl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(bing_crl_hndl, CURLOPT_HTTPGET, 1);
	curl_easy_setopt(bing_crl_hndl, CURLOPT_USE_SSL, CURLUSESSL_ALL); //API requires SSL connection
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
	{/*
		Currently this case will create a bug because it will not erase the old header information and send two keys
	 */
		api_key.clear();
		api_key.assign(api_key_INPUT);

		return -1; //until this problem is corrected
	}

	key_header.append(api_key);

	headers = curl_slist_append(headers,  key_header.c_str());
	curl_easy_setopt(bing_crl_hndl, CURLOPT_HTTPHEADER, headers);
	return 0;
}

int bing_search::image_search(std::string keywords)
{
	int res;
	//reset json memory
	free(json_txt.memory);
	json_txt.memory=(char *)malloc(1);
	json_txt.size = 0;

	std::string URL;
	const std::string domain = "https://api.cognitive.microsoft.com/bing/v5.0/images/search?q=";
	const std::string market = "&mkt=en-us";
	std::string query;

	char *c_keywords = (char *) keywords.c_str();

	if(api_key.empty() == 1)
	{
		std::cout<<"Please enter your bing API key before attempting to search! \n";
		return -1;
	}

	URL.assign(domain);

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

	res=curl_easy_perform(bing_crl_hndl);
	if(res != CURLE_OK)
	      fprintf(stderr, "curl_easy_perform() failed\n");


	return res;

}

std::string *bing_search::result_url(int result_index_number)
{
	std::string *URL_result = new std::string;

	cJSON * root = cJSON_Parse(json_txt.memory);
	cJSON * value = cJSON_GetObjectItem(root,"value");
	cJSON * result = cJSON_GetArrayItem(value, result_index_number);
	cJSON * json_url = cJSON_GetObjectItem(result, "contentUrl");

	if(!json_url)
	{
		std::cout<<"No data recieved from search result\nNo URL";
		URL_result->assign("NO URL");
		return URL_result;
	}
	URL_result->assign(cJSON_Print(json_url));
	URL_result->erase(URL_result->begin());//Remove quotes
	URL_result->erase(URL_result->end() - 1);

	return URL_result;

}

std::string bing_search::result_format(int result_index_number)
{
	std::string format_result;

	cJSON * root = cJSON_Parse(json_txt.memory);
	cJSON * value = cJSON_GetObjectItem(root,"value");
	cJSON * result = cJSON_GetArrayItem(value, result_index_number);
	cJSON * json_url = cJSON_GetObjectItem(result, "encodingFormat");

	if(!json_url)
	{
		std::cout<<"\nNo data recieved from search result"<<"\nFormat Unknown\n";
		return "NO FMT";
	}
	format_result.assign(cJSON_Print(json_url));
	format_result.erase(format_result.begin());//Remove quotes
	format_result.erase(format_result.end() - 1);

	return format_result;
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
