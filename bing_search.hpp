/*
 * bing_search.hpp
 *
 *  Created on: Oct 26, 2016
 *      Author: chris
 */

#ifndef BING_SEARCH_HPP_
#define BING_SEARCH_HPP_

#define USER_AGENT "User-Agent: Mozilla/5.0 (compatible; MSIE 10.0; Windows Phone 8.0; Trident/6.0; IEMobile/10.0; ARM; Touch; NOKIA; Lumia 822)"

extern "C"
{
#include<curl/curl.h>
#include"cJSON.h"
#include<stdlib.h>
#include<stdio.h>
}

#include<string>
#include<iostream>
#include <cstdint>
#include <cstring>

class bing_search
{
public:
	bing_search();
	int set_key(std::string api_key);
	int image_search(std::string keywords);
	std::string image_format(int result_index_number);
	std::string image_url(int result_index_number);
	int web_search(std::string keywords);
	std::string web_url(int result_index_number);
	std::string web_title(int result_index_number);
	std::string web_snippet(int result_index_number);


private:

	std::string api_key;
	curl_slist *headers;
	CURL *bing_crl_hndl;
	static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
	struct MemoryStruct {
	  char *memory;
	  size_t size;
	}json_txt;


};

#endif /* BING_SEARCH_HPP_ */
