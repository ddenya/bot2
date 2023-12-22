#include "binance.h"
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h> 
#include "cJSON.h" // Include the cJSON library header

// Function to perform HTTP GET request
static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    char* data = (char*)userp;

    strcat(data, (char*)contents);
    return realsize;
}

double getBinancePrice(const char* binanceAPI) {
    CURL* curl;
    CURLcode res;
    char responseBuffer[4096] = "";

    curl = curl_easy_init();
    double price = 0.0;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, binanceAPI);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, responseBuffer);

        res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            cJSON* root = cJSON_Parse(responseBuffer);

            if (root != NULL) {
                // char* jsonString = cJSON_Print(root);
                // printf("Parsed JSON: %s\n", jsonString);
                // free(jsonString); // Free the string when done

                cJSON* priceJson = cJSON_GetObjectItemCaseSensitive(root, "price");
                
                if (priceJson != NULL) {
                    if (cJSON_IsString(priceJson)) {
                        price = atof(priceJson->valuestring);
                    } else if (cJSON_IsNumber(priceJson)) {
                        price = priceJson->valuedouble;
                    }
                } else {
                    //fprintf(stderr, "N/A.\n");
                    char price[] = "N/A";
                }

                cJSON_Delete(root);
            } else {
                fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
            }
        } else {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }

    return price;
}