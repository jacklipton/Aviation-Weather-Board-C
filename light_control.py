import machine, neopixel
try:
  import config
except ImportError:
  class config:  # type: ignore
    LED_PIN = 14
    LED_COUNT = 50



def light_setup():
  global np
  pin = getattr(config, 'LED_PIN', 14)
  count = getattr(config, 'LED_COUNT', 50)
  np = neopixel.NeoPixel(machine.Pin(pin), count)



def colours(fr,ltNum):
  if fr == 'VFR':
    np[ltNum] = (20,0,0)
  elif fr == 'MVFR':
    np[ltNum] = (0,0,35)
  elif fr == 'IFR':
    np[ltNum] = (0,25,0)
  elif fr == 'LIFR':
    np[ltNum] = (0,25,25)
  np.write()
  
def turnOff():
    for i in range(50):
        np[i] = (0,0,0)
        np.write()

def turnOn(clr):

  ltOrder = [1,4,7,8,14,17,19,21,26,31,32,35,36,38,41,44,48]
  for i in range(len(ltOrder)):
    if clr == "green":
      np[ltOrder[i]] = (25,0,0)
      np.write()
    elif clr == "red":
      np[ltOrder[i]] = (0,25,0)
      np.write()
