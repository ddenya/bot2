import websocket
import json
import pandas as pandas
import logging

logging.basicConfig(
    format="%(message)s",
    level=logging.DEBUG,
)

binance_endpoint = 'wss://fstream.binance.com/ws'
data_frame = pandas.DataFrame()
in_position = False
buyorders, sellorders = [], []

our_msg = json.dumps(
  {"method":"SUBSCRIBE",
  "params":["btcusdt@trade"], "id":1})

def on_open(ws):
  print(our_msg)
  ws.send(our_msg)

def on_message(ws, message):
  global data_frame, in_position
  out = json.loads(message)
  out = pandas.DataFrame({'price': float(out['p'])}, index=[pandas.to_datetime(out['E'],unit='ms')])
  data_frame = pandas.concat([data_frame, out], axis=0)
  print(data_frame)

  data_frame = data_frame.tail(10)
  last_price = data_frame.tail(1).price.values[0]
  print("last_price: " + str(last_price))
  sma_10 = data_frame.price.rolling(10).mean().tail(1).values[0]
  print("sma_10: " + str(sma_10))

  # Trading logic...
  if not in_position and last_price > sma_10:
    # ASSUMPTION BOUGHT FOR LAST PRICE BC OF DELAY !!!
    print('bought for: ' + str(last_price))
    buyorders.append(last_price)
    in_position = True
  
  if in_position and sma_10 > last_price:
    print('sold for ' + str(last_price))
    sellorders.append(last_price)
    in_position = False


ws = websocket.WebSocketApp(binance_endpoint, 
                            on_message=on_message,
                            on_open=on_open
                            )

ws.run_forever()