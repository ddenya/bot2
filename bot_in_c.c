#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
#include <cjson/cJSON.h> // cJSON library for JSON parsing
#include "getprices/binance.h"  // Include binance.h from the getprices folder
#include "getprices/coinbase.h" // Include coinbase.h from the getprices folder

// gcc bot_in_c.c -o cbot -lcjson -lcurl
// gcc -o cbot bot_in_c.c ./getprices/binance.c ./getprices/coinbase.c -I./getprices -I/usr/local/Cellar/cjson/1.7.15/include/cjson  -lcurl -lcjson

// Replace with your Binance and Coinbase API keys
const char* BINANCE_API_KEY = "YOUR_BINANCE_API_KEY";
const char* COINBASE_API_KEY = "YOUR_COINBASE_API_KEY";

// Binance API endpoint for BTC-USDT pair
const char* BINANCE_BASE_URL = "https://api.binance.com/api/v3/ticker/price?symbol=";
// Coinbase API endpoint for BTC-USD pair
const char* COINBASE_BASE_URL = "https://api.coinbase.com/v2/prices/";

char** constructURLs(const char* symbol) {
    char** result = malloc(2 * sizeof(char*));
    if (result == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    char* binanceURL = NULL;
    char* coinbaseURL = NULL;
    if (strcmp(symbol, "ETHUSD") == 0) {
        binanceURL = malloc(strlen(BINANCE_BASE_URL) + strlen("ETHUSDT") + 1);
        if (binanceURL == NULL) {
            printf("Memory allocation failed\n");
            free(result);
            exit(1);
        }
        sprintf(binanceURL, "%sETHUSDT", BINANCE_BASE_URL);
        coinbaseURL = malloc(strlen(COINBASE_BASE_URL) + strlen("ETH-USD") + 7);
        if (coinbaseURL == NULL) {
            printf("Memory allocation failed\n");
            free(binanceURL);
            free(result);
            exit(1);
        }
        sprintf(coinbaseURL, "%sETH-USD/spot", COINBASE_BASE_URL);
    } else {
        int len = strlen(symbol);
        if (len == 7) {
            binanceURL = malloc(strlen(BINANCE_BASE_URL) + strlen(symbol) + 1);
            if (binanceURL == NULL) {
                printf("Memory allocation failed\n");
                free(result);
                exit(1);
            }
            sprintf(binanceURL, "%s%s", BINANCE_BASE_URL, symbol);

            char coinbaseSymbol[20];
            strncpy(coinbaseSymbol, symbol, 3);
            coinbaseSymbol[3] = '-';
            strncpy(coinbaseSymbol + 4, symbol + 3, 3);
            coinbaseSymbol[7] = '\0';

            coinbaseURL = malloc(strlen(COINBASE_BASE_URL) + strlen(coinbaseSymbol) + 7); // Assuming max length of coinbase URL
            if (coinbaseURL == NULL) {
                printf("Memory allocation failed\n");
                free(binanceURL);
                free(result);
                exit(1);
            }
            sprintf(coinbaseURL, "%s%s/spot", COINBASE_BASE_URL, coinbaseSymbol);
        } else {
            printf("Invalid symbol format: %s\n", symbol);
            result[0] = NULL;
            result[1] = NULL;
            free(result);
            return NULL;
        }
    }

    result[0] = binanceURL;
    result[1] = coinbaseURL;

    return result;
}

void freeURLs(char** urls) {
    free(urls[0]);
    free(urls[1]);
    free(urls);
}


// Function to execute a buy trade on Binance
void buyOnBinance(const char* pair, double amount) {
    // Simulate buy order execution on Binance
    printf("Buying %f %s on Binance\n", amount, pair);
}

// Function to execute a sell trade on Coinbase
void sellOnCoinbase(const char* pair, double amount) {
    // Simulate sell order execution on Coinbase
    printf("Selling %f %s on Coinbase\n", amount, pair);
}

// Function to execute trades
void executeTrade(const char* pair, double amount, const char* buyExchange, const char* sellExchange) {
    if (strcmp(buyExchange, "Binance") == 0) {
        buyOnBinance(pair, amount);
    }

    if (strcmp(sellExchange, "Coinbase") == 0) {
        sellOnCoinbase(pair, amount);
    }
}

#define DICT_SIZE 2

struct KeyValuePair {
    char key[20];
    double value;
};

void addToDictionary(struct KeyValuePair dict[], int *size, const char *key, double value) {
    if (*size >= DICT_SIZE) {
        printf("Dictionary is full.\n");
        return;
    }

    strcpy(dict[*size].key, key);
    dict[*size].value = value;
    (*size)++;
}

double getValue(struct KeyValuePair dict[], int size, const char *key) {
    for (int i = 0; i < size; ++i) {
        if (strcmp(dict[i].key, key) == 0) {
            return dict[i].value;
        }
    }
    return -1.0; // Return -1 if key is not found
}


void printTableLine(int sectionLengths[], int numSections) {
    int totalLength = 0;
    for (int i = 0; i < numSections; ++i) {
        totalLength += sectionLengths[i];
    }

    for (int i = 0; i < totalLength; ++i) {
        printf("-");
    }
    printf("\n");
}

// lenght of double in decimals
int double_len(double number) {
    return snprintf(NULL, 0, "%f", number);
}

int dict_size(struct KeyValuePair* array) {
    int count = 0;
    while (array[count].key != NULL) {
        count++;
    }
    return count;
}
// also for array
int array_size(const char* array[]) {
    int count = 0;
    while (array[count] != NULL) {
        count++;
    }
    return count;
}

int main() {
    double priceBinance, priceCoinbase;
    int dict_size_counter = 0;
    struct KeyValuePair dictionary[DICT_SIZE];
    double total = 0.0;
    double gain = 0.0;


    /*
    BTCUSD - Bitcoin to US Dollar
    ETHUSD - Ethereum to US Dollar
    LTCUSD - Litecoin to US Dollar
    BCHUSD - Bitcoin Cash to US Dollar
    ADAUSD - Cardano to US Dollar
    XRPUSD - Ripple to US Dollar
    DOTUSD - Polkadot to US Dollar
    UNIUSD - Uniswap to US Dollar
    SOLUSD - Solana to US Dollar */

    const char* symbols[] = {
    "BTCUSDT",
    "ETHUSDT",
    //"LTCUSDT",
    //"BCHUSDT",
    //"ADAUSDT",
    //"XRPUSDT",
    //"DOTUSDT",
    //"UNIUSDT",
    //"SOLUSDT"
    };

    const double amounts[] = {
        0.05, // BTCUSD
        2, // ETHUSD
        //1, // LTCUSD
        //1, // BCHUSD
        //1, // ADAUSD
        //1, // XRPUSD
        //1, // DOTUSD
        //1, // UNIUSD
        //1 //SOLUSD
    };

    for (int i = 0; i < array_size(symbols); i++){
        addToDictionary(dictionary, &dict_size_counter, symbols[i], amounts[i]);
    }
    for (int i = 0; i < array_size(symbols); i++){
        printf("%s => %.4f\n", symbols[i], getValue(dictionary, dict_size_counter, symbols[i]));
    }

    /*
    addToDictionary(dictionary, &dictSize, "BTCUSDT", 0.0005);
    addToDictionary(dictionary, &dictSize, "ETHUSD", 0.5);
    addToDictionary(dictionary, &dictSize, "LTCUSD", 0.0005);
    addToDictionary(dictionary, &dictSize, "BCHUSD", 0.5);
    addToDictionary(dictionary, &dictSize, "ADAUSD", 0.0);
    addToDictionary(dictionary, &dictSize, "XRPUSD", 0.0);
    addToDictionary(dictionary, &dictSize, "DOTUSD", 0.0);
    addToDictionary(dictionary, &dictSize, "UNIUSD", 0.0);
    addToDictionary(dictionary, &dictSize, "SOLUSD", 0.0);


    printf("BTCUSDT => %.4f\n", getValue(dictionary, dictSize, "BTCUSDT"));
    printf("BTCUSDT => %.4f\n", getValue(dictionary, dictSize, "ETHUSD"));
    printf("ETHUSD => %.1f\n", getValue(dictionary, dictSize, "ETHUSD"));
    printf("ETHUSD => %.1f\n", getValue(dictionary, dictSize, "ETHEUR"));
    printf("BNBUSDT => %.1f\n", getValue(dictionary, dictSize, "BNBUSDT"));
    printf("SOLUSDT => %.1f\n", getValue(dictionary, dictSize, "SOLUSDT"));
    */

    
    while (1) {
    printf("| %-8s | %-15s | %-15s | %-15s | %-15s | %-8s |  %-8s |%-8s|\n", "Symbol", "Binance Price", "Coinbase Price","Diff B-C", "Diff C-B", "", "Amt", "Amt * Diff");
    printf("|----------|-----------------|-----------------|-----------------|-----------------|----------|-----------|----------|\n");

    for (int i = 0; i < sizeof(symbols) / sizeof(symbols[0]); ++i) {
        char** urls = constructURLs(symbols[i]);
        //printf("%s : %s : %s \n ", symbols[i], urls[0], urls[1]);
        if (urls != NULL) {
            double priceBinance = getBinancePrice(urls[0]);
            double priceCoinbase = getCoinbasePrice(urls[1]);
            double diffB_C = priceBinance - priceCoinbase; // +0 - binance > coinbase -0 coinbase > binance
            double diffC_B = priceCoinbase - priceBinance; // +0 - coinbase > binance -0 binance > coinbase
            double amount = getValue(dictionary, dict_size_counter, symbols[i]);
            //printf("amount: %lf", amount);
            char flag[12];    
            flag[0] = '\0';

            double overall_limit = 1;
            double inner_limit = 0.5;
            double amt_diff = 0.0;

            if ((diffB_C > overall_limit ) ) {
                amt_diff = amount * diffB_C;
                if (amt_diff > inner_limit){
                    strcat(flag, "c->b");
                    total+=amt_diff;
                    gain = amt_diff;
                }

            }
            else if (diffC_B > overall_limit){
                amt_diff = amount * diffC_B;
                if (amt_diff > inner_limit){
                    strcat(flag, "b->c");
                    total+=amt_diff;
                    gain = amt_diff;
                }
            }
            printf("| %-8s | %-15.2lf | %-15.2lf | %-15.2lf | %-15.2lf | %-8s | %-8.8lf|%-8.8lf| \n", symbols[i], priceBinance, priceCoinbase, diffB_C, diffC_B, flag, amount, amt_diff);
            freeURLs(urls);
        }
        else{
            printf("%s : %s : %s \n ", symbols[i], "null", "null");
        }
    }
    printf("|----------|-----------------|-----------------|-----------------|-----------------|----------|-----------|----------|\n");
    printf("gain: %lf \n", gain);
    printf("total: %lf \n", total);

       // double profit = priceBinance - priceCoinbase;
       // if (profit > 0) {
           // printf("Arbitrage opportunity found! Potential profit: %f\n", profit);
            // Execute the trade
            // executeTrade(cryptoPair, amount, "Binance", "Coinbase");
           // printf("Trade executed!\n");
       // } else {
         //  // printf("No arbitrage opportunity at the moment.\n");
      //  }
        sleep(1); // Arbitrage check every 5 seconds
    }
    return 0;
}