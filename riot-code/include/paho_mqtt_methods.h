int init_paho_mqtt(void);
void publish_message(int value, char* topic);
int cmd_unsub(int argc, char **argv);
int cmd_sub(int argc, char **argv);
int cmd_pub(int argc, char **argv);
int cmd_con(int argc, char **argv);