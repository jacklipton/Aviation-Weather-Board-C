# This file is executed on every boot (including wake-boot from deepsleep)
import gc



import connect_to_wifi
import AviationWeatherServer
import server

wifiStat = connect_to_wifi.conNet()



gc.collect()
AviationWeatherServer.startup(wifiStat)
server.run_server()


