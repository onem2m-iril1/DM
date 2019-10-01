#ifndef __MQTT_SERVER_SETTING_H__
#define __MQTT_SERVER_SETTING_H__

#define MQTT_MAX_PAYLOAD_SIZE 700 
#define MQTT_CLIENT_ID "gateway_ae"
 
//char *buf;
const char * ssid = "KICS-IOT-DHCP";
const char * seckey = "AAAFFFBBBC";  
bool PUB_REG = false;
/* Buffer size for a receiving message. */
const int MESSAGE_BUFFER_SIZE = 512;
/* Buffer for a receiving message. */
char messageBuffer[MESSAGE_BUFFER_SIZE];

static volatile bool isMessageArrived = false;
int arrivedcount = 0;

//const char *buffer = NULL;
const char* buf;
//char buf[MESSAGE_BUFFER_SIZE];

char *MQTTSN_HOST = "192.168.0.126";
short MQTT_Atmpt = 0;
int PORT = 10000;

char* tpc_ty;
char* orignator; 
char* receiver;
//const char MQTT_SERVER_HOST_NAME[] = "iot.eclipse.org";
//char *MQTT_CLIENT_ID = "Sensor02";

char MQTT_TOPIC[40];
//const unsigned char* kpsa = "123";
//const unsigned char* kpsaID = "bridge";

#endif /* __MQTT_SERVER_SETTING_H__ */
