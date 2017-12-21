#include <curl/curl.h>

static const char apiKey[] = ""; //insert your api key here

const char * mkString(char *s[])
{
	char result[300];
	int n = sizeof(s) / sizeof(s[0]);
	strcpy(result, s[0]);
	for (int i = 1; i < n; i++) {
		
		char token[20];
		strcpy(token, ",");
		strcat(token,s[i]);
		strcat(result, token);
	}
	return result;
}


const char * getParams(char *ingridients[])
{
	char result[300];
	
	strcpy(result, "q=");
	strcat(result, ingridients);
	strcat(result, "&app_id=54edf467&app_key=");
	strcat(result, apiKey);
	strcat(result, "&from=0&to=3&calories=gte%20591,%20lte%20722&health=alcohol-free");
	return result;
}
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
	return written;
}
const char * readRecipe() {
	FILE *file;
	file = fopen("recipe.out", "r");
	printf("%s",file);

}

/*
This method download json and save it to file
*/
void getRecipe(char *ingridients)
{

	CURL *curl = curl_easy_init();;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_ALL);
	

	static const char *bodyfilename = "recipe.out";
	FILE *bodyfile;

	if (curl) 
	{
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.edamam.com/search?");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, getParams(ingridients));
		res = curl_easy_perform(curl);
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

		bodyfile = fopen(bodyfilename, "wb");
		if (!bodyfile) {
			curl_easy_cleanup(curl);
			//fclose(headerfile);
			return -1;
		}

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, bodyfile);
		curl_easy_perform(curl);
		fclose(bodyfile);

//		return res;
		curl_easy_cleanup(curl);
	}
	return "error";
}