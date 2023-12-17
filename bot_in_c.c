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
const char* BINANCE_API = "https://api.binance.com/api/v3/ticker/price?symbol=BTCUSDT";

// Coinbase API endpoint for BTC-USD pair
const char* COINBASE_API = "https://api.coinbase.com/v2/prices/BTC-USD/spot";


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
    
    while (1) {
        // Get market data from Binance and Coinbase
        priceBinance = getBinancePrice(BINANCE_API);
        priceCoinbase = getCoinbasePrice(COINBASE_API);

        printf("BinancePrice: %lf \n", priceBinance);
        printf("CoinbasePrice: %lf \n", priceCoinbase);

        double profit = priceBinance - priceCoinbase;

        if (profit > 0) {
            printf("Arbitrage opportunity found! Potential profit: %f\n", profit);

            // Execute the trade
            // executeTrade(cryptoPair, amount, "Binance", "Coinbase");

            printf("Trade executed!\n");
        } else {
            printf("No arbitrage opportunity at the moment.\n");
        }

        sleep(1); // Arbitrage check every 5 seconds
    }

    return 0;
}