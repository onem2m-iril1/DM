#define MQTTSNCLIENT_QOS2 1

#include "mbed_mem_trace.h"
#include "mbed.h"
#include "stdio.h"
#include "MQTT_server_setting.h"
#include "MQTTSNUDP.h"
#include "MQTTSNClient.h"
#include "sensor.h"
#include "Enumeration.h"
#include "easy-connect.h"
#include "MQTTSN_func.h"
#include "mgmtObject.h"
#include "string.h"

static uint32_t stack_available, stack_total;
static uint32_t heap_available, heap_total;

char h_available[6];
char h_total[6];
char s_available[6];
char s_total[6];

NetworkInterface* network = NULL;
//////////------------- Memory Info Function Begins ----------//////////////

void print_memory_info() {
    // allocate enough room for every thread's stack statistics
    int cnt = osThreadGetCount();
    mbed_stats_stack_t *stats = (mbed_stats_stack_t*) malloc(cnt * sizeof(mbed_stats_stack_t));
 
    cnt = mbed_stats_stack_get_each(stats, cnt);
    for (int i = 0; i < cnt; i++) {
        stack_available = stats[i].reserved_size - stats[i].max_size;
        stack_total = stats[i].reserved_size;
        printf("Thread: 0x%lX, Stack size: %lu / %lu\r\n", stats[i].thread_id, stack_available, stack_total);
    }
    free(stats);
 
    mbed_stats_heap_t heap_stats;
    mbed_stats_heap_get(&heap_stats);
    heap_available = heap_stats.reserved_size - heap_stats.current_size;
    heap_total = heap_stats.reserved_size;
    printf("Heap size: %lu / %lu bytes\r\n", heap_available, heap_total);
    
    
   // sprintf(h_available,"%lu", heap_available);
   // sprintf(h_total,"%lu", heap_total);
   // sprintf(s_available,"%lu", stack_available);
   // sprintf(s_total,"%lu", stack_total);
    
    printf("string is %s: ", h_available);
    printf("string is %s: ", s_available);   

}

//////////------------- Memory Info Function Ends ----------//////////////

int main() {
       
    float version = 1.0;
    MQTTSNUDP* ipstack = NULL;
    MQTTSN::Client<MQTTSNUDP, Countdown> *client = NULL;
   
    
    /////////////////////////////////////////////////////////////////////////////////////////
    printf("Opening network interface...\r\n");
    {
        network = easy_connect(true);    // If true, prints out connection details.
        if (!network) {
            printf("Unable to open network interface.\r\n");
            return -1;
        }
    }
    printf("Network interface opened successfully.\r\n");
    printf("\r\n");

    const char* Local_IP = network->get_ip_address();
    printf("IP Address of Device is: %s\n", Local_IP);
    
    const char* Local_MAC = network->get_mac_address();
    printf("MAC Address of Device is: %s\n", Local_MAC);
    /////////////////////////////////////////////////////////////////////////////////////////
    ipstack = new MQTTSNUDP(network);
    client = new MQTTSN::Client<MQTTSNUDP, Countdown>(*ipstack);
    
    //////////////////////////////////UDP Connect///////////////////////////////////////////    
    printf("Connecting to %s:%d\n", MQTT_SERVER_HOST_NAME, PORT);
    rc = ipstack->connect(MQTT_SERVER_HOST_NAME, PORT);
    if (rc != 0)
        printf("rc from UDP connect is %d\n", rc);
    else
        printf("UDP connected\n"); 
      
    //////////////////////////////////MQTT Connect///////////////////////////////////////////
    
    attemptConnect(client, ipstack);
    
    //////////////////////////////////MQTT Subscribe//////////////////////////////////////////

    create_Topic("resp", "SAE02", "CSE_01");
    MQTTSN_topicid topicid;
    if ((rc = subscribe(client, ipstack, topicid)) != 0)
        printf("rc from MQTT subscribe is %d\n", rc);
    else
        printf("Subscribed to Topic %s\n", MQTT_TOPIC);
        
    //Set Topic to /oneM2M/reg_req/Sensor01/CSE_01
    create_Topic("req", "SAE02", "CSE_01");
     MQTTSN_topicid topicid2;
    if ((rc = subscribe(client, ipstack, topicid2)) != 0)
        printf("rc from MQTT subscribe is %d\n", rc);
    else
        printf("Subscribed to Topic %s\n", MQTT_TOPIC);

    //////////////////////////////////mgmtObject///////////////////////////////////////////
   
    if(mgmtObject(client, ipstack, topicid2, h_available, h_total, s_available, s_total) != 0 )
        
        printf("Failed with response: %s\n", response.c_str());
    else
        printf("Done wth response: %s\n", response.c_str());
  
    ////////////////////////////////Req-Resp Topics//////////////////////////////////////////
    
        Request R1;
        RES.resourceName = rn;
        R1.Request_Identifier = "1234";
        R1.To = From;
        R1.From = CSE_ID;
        buf = Create_Req(R1); 
        if ((rc = publish(client, ipstack, topicid2)) != 0)
            printf("rc from MQTT Publish is %d\n", rc);
        else
            printf("Published Buffer: %s to Topic %s\n",buf, MQTT_TOPIC); 

    while(1) 
    {
        // Check connection //
        if(!client->isConnected()){
            break;
        }
        // Received a control message. //
        if(isMessageArrived) {
            isMessageArrived = false;
            // Just print it out here.
            printf("\r\nMessage arrived:\r\n%s\r\n", messageBuffer);
               
                //printf("Mem Available %s\n", mma.c_str());
                //printf("Request Identifier %s\n", rqi.c_str()); 
        }          
            client->yield(4000);
    } 
    
    if ((rc = client->disconnect()) != 0)
        printf("rc from disconnect was %d\n", rc);
 
    ipstack->disconnect();
    
    delete ipstack;
    delete client;
    
    printf("Version %.2f: finish %d msgs\n", version, arrivedcount);
    printf("Finishing with %d messages received\n", arrivedcount);
    
    return 0;
}