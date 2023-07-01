#ifndef PAHO_MQTT_H
#define PAHO_MQTT_H

#define MQTT_BRIDGE_IP "192.168.102.113"

int init_paho_mqtt (void);
int cmd_unsub(int argc, char **argv);
int cmd_sub(int argc, char **argv);
int cmd_pub(int argc, char **argv);
int cmd_con(int argc, char **argv);

#endif