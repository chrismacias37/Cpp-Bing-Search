# Cpp_bing
C++ library to search the web using Bing Cognitive Services.
This library uses libcurl to retrieve web results, and cjson to parse the search result.
cjson is included as part of this repository but curl is not and must be linked in as part of this library. 

#Requierments
1. A azure Cognitive Service key.
 * You can sign up https://www.microsoft.com/cognitive-services/en-us/apis
 * You can get a trial if its your first time.
2. Libcurl
 * All network interaction is handled by this library
 * You must download and link this to your project from the libCurl repository.
3. LibcJson
 * This is the library handles the json response from the service
 * It is included as part of this library. "cJson.o"
 * https://github.com/kbranigan/cJSON
