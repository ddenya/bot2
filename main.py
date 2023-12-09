import websocket
import json
import pandas as pandas
import logging

logging.basicConfig(
    format="%(message)s",
    level=logging.DEBUG,
)

binance_endpoint = 'wss://fstream.binance.com/ws'

our_msg = json.dumps(
  {"method":"SUBSCRIBE",
  "params":["btcusdt@trade"], "id":1})

def on_open(ws):
  print(our_msg)
  ws.send(our_msg)

def on_message(ws, message):
  out = json.loads(message)
  out = pandas.DataFrame({'price': float(out['p'])}, index=[pandas.to_datetime(out['E'],unit='ms')])
  print(out)

ws = websocket.WebSocketApp(binance_endpoint, 
                            on_message=on_message,
                            on_open=on_open
                            )

ws.run_forever()