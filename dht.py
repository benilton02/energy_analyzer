from measure import measure

def dht():
    volt = 0
    ampere = 0
    for i in range(1,480):
        volt = volt + (measure[i]["voltage"])**2
        ampere = ampere + (measure[i]["amperage"])**2

    ampere = ampere**.5 / measure[0]["amperage"]  
    volt = volt**.5 / measure[0]["voltage"]
    
    print(f"Volts DHT: {ampere}")
    print(f"Ampere DHT: {volt}")

dht()

