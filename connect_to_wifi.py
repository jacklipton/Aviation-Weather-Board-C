import time
import webrepl
import network
import sys

# Load configuration from config.py if present, else use safe defaults
try:
    import config
except ImportError:
    class config:  # type: ignore
        WIFI_SSID = 'SSID'
        WIFI_PASSWORD = 'PASSWORD'
        WEBREPL_ENABLED = False

def conNet():
    ssid = getattr(config, 'WIFI_SSID', 'SSID')
    password = getattr(config, 'WIFI_PASSWORD', 'PASSWORD')

    station = network.WLAN(network.STA_IF)

    station.active(True)
    station.connect(ssid, password)


    start_time = time.time()

    while station.isconnected() == False:
        if (time.time() - start_time) >30:

            time.sleep(1)
            return False

        pass

    print('Connection successful')
    print(station.ifconfig())
    if getattr(config, 'WEBREPL_ENABLED', False):
        try:
            webrepl.start()
        except Exception as e:
            print('WebREPL start failed:', e)
    time.sleep(1)
    return True