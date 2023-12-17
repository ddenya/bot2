import requests

binance_api_url = 'https://api.binance.com/api/v3/ticker/price?symbol=BTCUSDT'
coinbase_api_url = 'https://api.coinbase.com/v2/prices/BTC-USD/spot'

def get_binance_price():
    response = requests.get(binance_api_url)
    return float(response.json()['price'])

def get_coinbase_price():
    response = requests.get(coinbase_api_url)
    return float(response.json()['data']['amount'])

binance_price = get_binance_price()
coinbase_price = get_coinbase_price()

# Compare prices and look for arbitrage opportunities
if binance_price > coinbase_price:
    profit = binance_price - coinbase_price
    print(f"Arbitrage opportunity! Potential profit: ${profit}")
else:
    print("No arbitrage opportunity found.")