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


int main() {

    const char* cryptoPair = "BTC-USD";
    double priceBinance, priceCoinbase;
    double amount = 1.0;

    const char* symbols[] = {
        "BTCUSDT", "ETHUSD", "BNBUSDT", "SOLUSDT"
    };
    
    while (1) {

    printf("| %-8s | %-15s | %-15s | %-15s | %-15s | %-8s | \n", "Symbol", "Binance Price", "Coinbase Price","Diff B-C", "Diff C-B", "");
    printf("|----------|-----------------|-----------------|-----------------|-----------------|-----------|\n");

    for (int i = 0; i < sizeof(symbols) / sizeof(symbols[0]); ++i) {
        char** urls = constructURLs(symbols[i]);
        if (urls != NULL) {
        //printf("%s : %s : %s \n ", symbols[i], urls[0], urls[1]);
        double priceBinance = getBinancePrice(urls[0]);
        double priceCoinbase = getCoinbasePrice(urls[1]);
        double diffB_C = priceBinance - priceCoinbase;
        double diffC_B = priceCoinbase - priceBinance;
        char flag[7];
        flag[0] = '\0';
        double limit = 0.5;
        if ((diffB_C > limit )|| (diffC_B > limit) ) {
            strcat(flag, "+++");
        }

        printf("| %-8s | %-15.2lf | %-15.2lf | %-15.2lf | %-15.2lf | %-8s | \n", symbols[i], priceBinance, priceCoinbase, diffB_C, diffC_B, flag);
        freeURLs(urls);
        }
        else{
            printf("%s : %s : %s \n ", symbols[i], "null", "null");
        }

    }

    printf("|----------|-----------------|-----------------|-----------------|-----------------|-----------|\n");


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