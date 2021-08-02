from matplotlib.pyplot import plot, show, xlabel, ylabel
from measure import measure



def graph(data):
    voltage = list()
    hour = list()
    ampere = list()

    for i in range(120):
        voltage.append(data[i]["voltage"])
        ampere.append(data[i]["amperage"])
        if data[i]["minute"] < 10:
            hour.append(str(data[i]["hour"]) + ":0" +str(data[i]["minute"]))    
        else:
            hour.append(str(data[i]["hour"]) + ":" +str(data[i]["minute"]))
            
    print(hour)
    xlabel('Horário')
    
    # ylabel('Tensão da rede')
    # plot(hour, voltage)
    # plot(hour, voltage, "go")

    ylabel('Corrente da AC')
    plot(hour, ampere)
    plot(hour, ampere, "go")

    show()
 
    

if __name__ == "__main__":
    graph(measure)