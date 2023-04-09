import serial
import time
import requests
import json
import ast

server = "http://192.168.188.126:8000/"
arduino = serial.Serial(port='COM4', baudrate=115200, timeout=1000)

def read_dedicated_node():
    print("Waiting for Arduino")
    data = arduino.readline().strip()

    if(len(data) == 0):
        return read_dedicated_node()
    else:
        return data.decode()


def write_dedicated_node(reply):
    arduino.write(bytes(reply, 'utf-8'))
    time.sleep(0.05)    

global response
while True:
    msg = read_dedicated_node()
    print(msg)
    
    try:
        index = msg.index('|')
        url = server + msg[1:index]
        data = json.loads(msg[index + 1:].replace(']', ''))

        print("URL of server: ", end="")
        print(url)
        print("Data to Send: ", end="")
        print(data)
        
        if len(url) > len(server) + 6:
            response = requests.put(url, json=data)
        else:
            response = requests.post(url, json=data)
            print("Response Sent Successfully")
            status = response.status_code
            payload = ast.literal_eval(response.text)

            print("Replying Arduino: ", end="")
            if "id" in payload.keys() and status != 500:
                reply = "(" + str(payload["id"]) + "|" + str(payload["macid"]) + ")"
                print(reply)
                write_dedicated_node(reply)
            else:
                write_dedicated_node("$")


        print("\n\n")
    except:
        print("Exception Occurred!!!")
        pass

    

