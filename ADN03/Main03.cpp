
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

#if !defined(MBED_SYS_STATS_ENABLED)
#error[NOT_SUPPORTED] test not supported
#endif

static uint32_t stack_available, stack_total;
static uint32_t heap_available, heap_total;

char h_available[6];
char h_total[6];
char s_available[6];
char s_total[6];

NetworkInterface* network = NULL;

//////////------------- Memory Info Function Begins ----------//////////////

void print_memory_info()
{
    // allocate enough room for every thread's stack statistics
    int cnt = osThreadGetCount();
    mbed_stats_stack_t* stats = (mbed_stats_stack_t*)malloc(cnt * sizeof(mbed_stats_stack_t));

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

    sprintf(h_available, "%lu", heap_available);
    sprintf(h_total, "%lu", heap_total);
    sprintf(s_available, "%lu", stack_available);
    sprintf(s_total, "%lu", stack_total);

    printf("string is %s: ", h_available);
    printf("string is %s: ", h_total);
    printf("string is %s: ", s_available);
    printf("string is %s: ", s_total);
}

//////////------------- Memory Info Function Ends ----------//////////////

int main()
{
    const char* ret;
    const char* rett;
    int C;
    mbed_stats_sys_t stats;
    mbed_stats_sys_get(&stats);

    float version = 1.0;
    MQTTSNUDP* ipstack = NULL;
    MQTTSN::Client<MQTTSNUDP, Countdown>* client = NULL;

    /////////////////////////////////////////////////////////////////////////////////////////
    printf("Opening network interface...\r\n");
    {
        network = easy_connect(true); // If true, prints out connection details.
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

    /////////////////////////////////Print Memory//////////////////////////////////////////

    //////////////////////////////////MQTT Connect/////////////////////////////////////////
    attemptConnect(client, ipstack);
    //////////////////////////////////MQTT Subscribe///////////////////////////////////////

    create_Topic("resp", "SSAE03", "CSE_01");
    MQTTSN_topicid topicid;
    if ((rc = subscribe(client, ipstack, topicid)) != 0)
        printf("rc from MQTT subscribe is %d\n", rc);
    else
        printf("Subscribed to Topic %s\n", MQTT_TOPIC);

    //Set Topic to /oneM2M/reg_req/Sensor01/CSE_01
    create_Topic("req", "SSAE03", "CSE_01");
    MQTTSN_topicid topicid2;
    if ((rc = subscribe(client, ipstack, topicid2)) != 0)
        printf("rc from MQTT subscribe is %d\n", rc);
    else
        printf("Subscribed to Topic %s\n", MQTT_TOPIC);

    print_memory_info();
    while (1) {
        if (!client->isConnected()) {
            break;
        }
        if (isMessageArrived) {
            isMessageArrived = false;
            printf("\r\nMessage arrived:\r\n%s\r\n", messageBuffer);

            ret = strstr((const char*)to.c_str(), "Memory01");
            if (ret != NULL) {
                printf("res:%s\n", ret);
                C = 11;
                ty = 13;
            }
            /////------------------- Restart
            rett = strstr((const char*)to.c_str(), "Reboot01");
            if (rett != NULL) {
                printf("res:%s\n", rett);
                C = 22;
            }
            /////------------------- Software
            ret = strstr((const char*)to.c_str(), "Software01");
            if (ret != NULL) {
                printf("res:%s\n", ret);
                C = 33;
                ty = 13;
            }
            ////------------------- Hardware
            ret = strstr((const char*)to.c_str(), "Hardware01");
            if (ret != NULL) {
                printf("res:%s\n", ret);
                C = 44;
                ty = 13;
            }
            printf("To: %s\r\n", to.c_str());
            
            Response Resp;
            Response Resp2;
            Response Resp3;
            switch (C) {
            case 11:
                
                RES.memoryAvailabe = s_available;
                RES.memoryTotal = s_total;
                RES.Resource_Type = 13;
                RES.Request_Identifier = "1235";
                RES.Operation = 3;
                RES.responseStatusCode = 2000;
                buf = Create_Resp(Resp);
                create_Topic("resp", "SSAE03", "CSE_01");
                if ((rc = publish(client, ipstack, topicid)) != 0)
                    printf("rc from MQTT Publish is %d\n", rc);
                else
                    printf("Published Buffer: %s to Topic %s\n", buf, MQTT_TOPIC);
                break;
            case 22:
                NVIC_SystemReset();
                break;
            case 33:
                
                RES.FirmwareVersion = "5.12";
                RES.SoftwareVersion = "MbedOS";
                RES.resourceName = "ABC789";
                RES.Resource_Type = 13;
                RES.Request_Identifier = "1235";
                RES.Operation = 3;
                RES.responseStatusCode = 2000;
                buf = DM_Resp(Resp2);
                create_Topic("resp", "SSAE03", "CSE_01");
                if ((rc = publish(client, ipstack, topicid)) != 0)
                    printf("rc from MQTT Publish is %d\n", rc);
                else
                    printf("Published Buffer: %s to Topic %s\n", buf, MQTT_TOPIC);
                break;
            case 44:
                
                if (stats.compiler_id == 1) {
                    RES.HardwareVersion = "ARM";
                }
                RES.Manufacturer = "STM32";
                RES.Model = "L476RG";
                RES.DeviceType = "Nucleo";
                RES.resourceName = "ABC789";
                RES.Resource_Type = 13;
                RES.Request_Identifier = "1235";
                RES.Operation = 3;
                RES.responseStatusCode = 2000;
                buf = DM_Resp2(Resp3);
                create_Topic("resp", "SSAE03", "CSE_01");
                if ((rc = publish(client, ipstack, topicid)) != 0)
                    printf("rc from MQTT Publish is %d\n", rc);
                else
                    printf("Published Buffer: %s to Topic %s\n", buf, MQTT_TOPIC);
                break;
                default:
                    printf("Something's Wrong\n");
                break;
            }
        }
        wait(2);
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