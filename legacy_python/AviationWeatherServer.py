


import urequests as requests

import json
import time
import sys
from light_control import*
try:
  import config
except ImportError:
  class config:  # type: ignore
    CHECKWX_API_KEY = "your_api_key_here"




def AvWeather():


  airports = "CYGK,CYHM,CYKF,CYND,CYOO,CYOW,CYPQ,CYQA,CYSN,CYTR,CYTZ,CYXU,CYYZ,KBUF,KIAG,KMSS,KOGS"



  ltOrder = [14,41,44,8,21,7,19,26,35,17,31,48,32,38,36,4,1]

  i = 0

  hdr = {"X-API-Key": getattr(config, "CHECKWX_API_KEY", "your_api_key_here")}
  r = requests.request("GET","https://api.checkwx.com/metar/"+ airports +"/decoded", headers=hdr)

  cyData = json.loads(r.text)['data']


  for cy in cyData:

    if cy['flight_category'] == "VFR":
      colours("VFR",ltOrder[i])
    elif cy['flight_category'] == "MVFR":
      # print("MVFR")
      colours("MVFR",ltOrder[i])
    elif cy['flight_category'] == "IFR":
      # print("IFR")
      colours("IFR",ltOrder[i])
    elif cy['flight_category'] == "LIFR":
      # print("LIFR")
      colours("LIFR",ltOrder[i])

    # print(cy)
    i=i+1


def startup(conStat):
  light_setup()
  if conStat == False:
    print("No connection")
    turnOn("red")
    time.sleep(3)
    turnOff()
    sys.exit()
  else:
    print("Connection successful")
    turnOn("green")
    time.sleep(3)
    turnOff()









