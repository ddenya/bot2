import websocket
import json
import pandas as pandas

endpoint = 'wss://stream.binance.com:9443/ws'

our_msg = json.dumps(
  {'method':'SUBSCRIBE',
  'params':['btcusd@ticker'], 'id':1})

def on_open(ws):
  ws.send(our_msg)

def on_message(ws, message):
  out = json.loads(message)
  print(out)

ws = websocket.WebSocketApp(endpoint, 
                            on_message=on_message,
                            on_open=on_open
                            )

ws.run_forever()