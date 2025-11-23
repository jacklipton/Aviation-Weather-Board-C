
import requests
import json


airports = "KOGS,KMSS,CYOW,CYND,CYGK,CYTR,CYPQ,CYOO,CYQA,CYTZ,CYYZ,CYSN,KIAG,KBUF,CYHM,CYKF,CYXU"


hdr = {"X-API-Key": "e99ef131b8f04bc9853ddaed7f"}
r = requests.request("GET","https://api.checkwx.com/metar/"+ airports +"/decoded", headers=hdr)

cyData = json.loads(r.text)['data']


for cy in cyData:

    print(cy['station']['name'])
    print(cy['flight_category'])
