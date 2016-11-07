/*
 * bing_search.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: chris
 */

#include"bing_search.hpp"

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

	if(json_txt.memory==NULL)
	{
		cerr<<"\nImage_search:"
				"\n json memory not allocated. "
				"\n no memory available";
		return -1;
	}

	char *c_keywords = (char *) keywords.c_str();

	if(api_key.empty() == 1)
	{
		std::cout<<"\nImage_search: "
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
		  cerr<<"\nImage_search (libcurl):"
			  "\n unable to perform search. Check internet connection";
		  return -1;

	}

	if(json_txt.size==0)
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
				"\n no URL";
		return URL_result;
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
				"\n format Unknown\n";
		return "NO FMT";
	}
	format_result.assign(cJSON_Print(json_url));
	format_result.erase(format_result.begin());//Remove quotes
	format_result.erase(format_result.end() - 1);

	return format_result;
}

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

	if(json_txt.memory==NULL)
	{
		cerr<<"\nWeb_search:"
				"\n json memory not allocated. "
				"\n no memory available";
		return -1;
	}

	char *c_keywords = (char *) keywords.c_str();

	if(api_key.empty() == 1)
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

	if(json_txt.size==0)
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
				"\n no URL";
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
				"\n no URL";
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
