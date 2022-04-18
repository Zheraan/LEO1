'''
MQTT subscriber example
2022-03-31 Kjeld Jensen kjen@mmmi.sdu.dk

History:
2022-03-31 KJ First version

You will have to install the package paho-mqtt:

$ sudo apt install python3-pip
$ pip3 install paho-mqtt
'''

import paho.mqtt.client as mqtt
from datetime import datetime

MQTT_CLIENT_ID = 'PythonSubscriber'
MQTT_SERVER = '10.0.0.10'
MQTT_SERVER_PORT = 1883
MQTT_USERNAME = 'Dragomin'
MQTT_KEY = 'Nerd'
MQTT_TOPIC = 'Dragomin/voltage'


def on_connect(client, userdata, flags, rc):
    print('Connected with result code ' + str(rc))
    client.subscribe(MQTT_TOPIC)


def on_message(client, userdata, msg):
    payload_string = msg.payload.decode('utf-8')
    payload_value = int(payload_string)
    R1 = 3600.
    R2 = 240.
    VBatt = (payload_value * (R1 + R2)) / R2 / 1024
    print('%s Battery was %f Volt' % (datetime.now(), VBatt))


if __name__ == '__main__':
    mqtt_client = mqtt.Client(MQTT_CLIENT_ID)
    mqtt_client.username_pw_set(MQTT_USERNAME, MQTT_KEY)
    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message
    mqtt_client.connect(MQTT_SERVER, MQTT_SERVER_PORT)
    mqtt_client.loop_forever()
