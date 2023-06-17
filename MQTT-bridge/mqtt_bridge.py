# Import AWS SDK packages
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient

import paho.mqtt.client as mqtt
import json
import time
from datetime import datetime
from configparser import ConfigParser

DEBUG = 1

# Constants
VALUE = "Value"

# Load configurations
config = ConfigParser()
config.read('config.ini')
MQTT_SECTION = 'mqtt'

# Paho constants
PAHO_MQTT_BROKER_IP = config.get(MQTT_SECTION, "PAHO_MQTT_BROKER_IP")
PAHO_MQTT_BROKER_PORT = 1883
PAHO_MQTT_BROKER_CLIENT_ID = "paho_mqtt_bridge"

# AWS constants
AWS_IOT_ENDPOINT = config.get(MQTT_SECTION, "AWS_IOT_ENDPOINT")
AWS_IOT_PORT = 8883
AWS_IOT_CLIENT_ID = "basicPubSub" # change with aws_mqtt_bridge
AWS_IOT_ROOT_CA_PATH = "root-CA.crt"
AWS_IOT_PRIVATE_KEY_PATH = "pc_ubuntu_iot.private.key"
AWS_IOT_CERTIFICATE_PATH = "pc_ubuntu_iot.cert.pem"

# Topics
MQTT_TOPIC_WATER = "iot/ADF/water"
MQTT_TOPIC_FOOD = "iot/ADF/food"

def on_message(_client, _userdata, message):

    value = message.payload.decode('utf-8')
    print("Message received, topic: ", message.topic, " Value: ", value)

    # Check the topic of the message
    if (message.topic == MQTT_TOPIC_FOOD or message.topic == MQTT_TOPIC_WATER):
        # Send the message to AWS
        aws_payload = '{"'+VALUE+'":"'+value+'"}'
        if(DEBUG):
            print("AWS payload: ",aws_payload)

        is_published = myMQTTClient.publish(message.topic, aws_payload, 2)

        if(DEBUG):
            print("Message published: ", is_published)

    
# On connect subscribe to topic
def on_connect(_client, _userdata, _flags, result):

    paho_mqtt_client.subscribe(MQTT_TOPIC_FOOD)
    print('Subscribed to ', MQTT_TOPIC_FOOD)
    paho_mqtt_client.subscribe(MQTT_TOPIC_WATER)
    print('Subscribed to ', MQTT_TOPIC_WATER)


if __name__ == '__main__':

    # AWS MQTT client
    # For certificate based connection
    myMQTTClient = AWSIoTMQTTClient(AWS_IOT_CLIENT_ID)
    # For TLS mutual authentication
    myMQTTClient.configureEndpoint(AWS_IOT_ENDPOINT, 8883)
    myMQTTClient.configureCredentials(AWS_IOT_ROOT_CA_PATH, AWS_IOT_PRIVATE_KEY_PATH, AWS_IOT_CERTIFICATE_PATH)
    myMQTTClient.configureOfflinePublishQueueing(-1)  # Infinite offline Publish queueing
    myMQTTClient.configureDrainingFrequency(2)  # Draining: 2 Hz
    myMQTTClient.configureConnectDisconnectTimeout(10)  # 10 sec
    myMQTTClient.configureMQTTOperationTimeout(5)  # 5 sec

    # Paho MQTT client
    paho_mqtt_client = mqtt.Client(client_id=PAHO_MQTT_BROKER_CLIENT_ID)
    paho_mqtt_client.on_connect = on_connect
    paho_mqtt_client.on_message = on_message

    # Start clients
    is_conn = myMQTTClient.connect()
    print("AWS connection: ",is_conn)
    paho_mqtt_client.connect(host=PAHO_MQTT_BROKER_IP, port=PAHO_MQTT_BROKER_PORT)
    # Blocking call that processes network traffic, dispatches callbacks and
    # handles reconnecting.
    paho_mqtt_client.loop_forever()