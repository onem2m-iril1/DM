#define MQTTSNCLIENT_QOS2 1

#include "mbed.h"
#include "stdio.h"
#include "string.h"
#include "MQTT_server_setting.h"
#include "MQTTSNUDP.h"
#include "MQTTSNClient.h"
#include "sensor.h"
#include "MbedJSONValue.h"
#include "Enumeration.h"
#include "easy-connect.h"
#include "MQTTSN_func.h"
#include "DMR.h"

NetworkInterface* network = NULL;
MQTTSNUDP* ipstack = NULL;
MQTTSN::Client<MQTTSNUDP, Countdown>* client = NULL;

MQTTSN_topicid topicid;
MQTTSN_topicid topicid2;
MQTTSN_topicid topicid3;
MQTTSN_topicid topicid4;

//////////------------- Memory Info Function Begins ----------//////////////

static uint32_t stack_available, stack_total;
static uint32_t heap_available, heap_total;

char h_available[6];
char h_total[6];
char s_available[6];
char s_total[6];

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

int main()
{

    float version = 1.0;

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

    ////////////////////////////////////////////////////////////////////////////////////////

    ipstack = new MQTTSNUDP(network);
    client = new MQTTSN::Client<MQTTSNUDP, Countdown>(*ipstack);

    //////////////////////////////////UDP Connect///////////////////////////////////////////
    printf("Connecting to %s:%d\n", MQTTSN_HOST, PORT);
    rc = ipstack->connect(MQTTSN_HOST, PORT);
    if (rc != 0)
        printf("rc from UDP connect is %d\n", rc);
    else
        printf("UDP connected\n");
    //////////////////////////////////MQTT Connect///////////////////////////////////////////
    attemptConnect(client, ipstack);
    //////////////////////////////MQTT-MN Subscribe//////////////////////////////////////////
    print_memory_info();
    ///////////////////// send resp to ADN02 //////////////////////////
    create_Topic("resp", "SAE02", "CSE_01");
    if ((rc = subscribe(client, ipstack, topicid)) != 0)
        printf("rc from MQTT subscribe is %d\n", rc);
    else
        printf("Subscribed to Topic %s\n", MQTT_TOPIC);
    ///////////////////// send req to ADN02 //////////////////////////
    //Set Topic to /oneM2M/req/SAE01/CSE_01
    create_Topic("req", "SAE02", "CSE_01");
    if ((rc = subscribe(client, ipstack, topicid2)) != 0)
        printf("rc from MQTT subscribe is %d\n", rc);
    else
        printf("Subscribed to Topic %s\n", MQTT_TOPIC);
    //////////////////////END_1////////////////////////////////////////

    /////////////////////// resp from ADN03 ///////////////////
    create_Topic("resp", "SSAE03", "CSE_01");
    if ((rc = subscribe(client, ipstack, topicid3)) != 0)
        printf("rc from MQTT subscribe is %d\n", rc);
    else
        printf("Subscribed to Topic %s\n", MQTT_TOPIC);
    ///////////////////// send req to ADN03 //////////////////////////
    //Set Topic to /oneM2M/req/SAE01/CSE_01
    create_Topic("req", "SSAE03", "CSE_01");
    if ((rc = subscribe(client, ipstack, topicid4)) != 0)
        printf("rc from MQTT subscribe is %d\n", rc);
    else
        printf("Subscribed to Topic %s\n", MQTT_TOPIC);
    /////////////////////////END_2////////////////////////////////

    while (1) {
        if (!client->isConnected()) // Check if the client is connected
            break;

        if (isMessageArrived) // Check for the received messages MQTT_func.h
        {
            isMessageArrived = false;

            //------------ extraction of ty ---------------
            printf("To: %s\r\n", to.c_str());
            const char* ret;
            const char* rett;
            int C;

            /////------------------- Memory
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

            printf("Resource Type: %d\n", ty);
            switch (C) {
            case 11:

                switch (ty) {
                case 13:

                    switch (op) {
                    case 2:
                        //retrive Registration
                        if (strcmp((const char*)rqi.c_str(), "1235") == 0) {
                            Request R1;
                            //RES.resourceName = rn;
                            R1.Resource_Type = ty;
                            //Resp.Request_Identifier = rqi;
                            R1.To = From;
                            R1.From = CSE_ID;
                            buf = Create_Req(R1);
                            create_Topic("req", "SSAE03", "CSE_01");
                            if ((rc = publish(client, ipstack, topicid4)) != 0)
                                printf("rc from MQTT Publish is %d\n", rc);
                            else
                                printf("Published Buffer: %s to Topic %s\n", buf, MQTT_TOPIC);
                        }
                        if (strcmp((const char*)rqi.c_str(), "1234") == 0) {
                            printf("/n Generating MGMT response /n");
                            create_Topic("resp", "SAE02", "CSE_01");
                            //Create response code for CREATE AE
                            Response Resp;
                            //DM_Resp Resp;
                            RES.resourceID = "CSE1234";
                            RES.parentID = "AB123";
                            RES.creationTime = "092019";
                            RES.expirationTime = "112019";
                            RES.Resource_Type = ty;
                            RES.memoryAvailabe = s_available;
                            RES.memoryTotal = s_total;
                            RES.resourceName = "ABC789";
                            buf = Create_Resp(Resp);

                            //Publish Registration Response

                            if ((rc = publish(client, ipstack, topicid)) != 0)
                                printf("rc from MQTT Publish is %d\n", rc);
                            else
                                printf("Published Buffer: %s to Topic %s\n", buf, MQTT_TOPIC);
                        }
                        break;
                    // update with ADN03 value to ADN02
                    case 3:
                        printf("/n Generating MGMT response other ADN /n");
                        create_Topic("resp", "SSAE03", "CSE_01");
                        //Create response code for CREATE AE
                        Response Resp1;

                        RES.resourceID = "CSE1234";
                        RES.parentID = "AB123";
                        RES.creationTime = "092019";
                        RES.expirationTime = "112019";
                        RES.Resource_Type = ty;
                        //RES.resourceName = rn;
                        Resp1.Request_Identifier = rqi;
                        Resp1.To = From;
                        Resp1.From = CSE_ID;
                        RES.memoryAvailabe = mma;
                        RES.memoryTotal = mmt;
                        RES.resourceName = "ABC7893";
                        buf = Create_Resp(Resp1);
                        //Publish Registration Response
                        if ((rc = publish(client, ipstack, topicid)) != 0)
                            printf("rc from MQTT Publish is %d\n", rc);
                        else
                            printf("Published Buffer: %s to Topic %s\n", buf, MQTT_TOPIC);
                        break;
                    }
                    break;
                }
                break;

            case 22:
                NVIC_SystemReset();
                break;

            case 33:

                switch (ty) {
                case 13:

                    switch (op) {
                    case 2:
                        //retrive Registration
                        if (strcmp((const char*)rqi.c_str(), "1235") == 0) {
                            Request R1;
                            //RES.resourceName = rn;
                            R1.Resource_Type = ty;
                            //Resp.Request_Identifier = rqi;
                            R1.To = From;
                            R1.From = CSE_ID;
                            buf = Create_Req(R1);
                            create_Topic("req", "SSAE03", "CSE_01");
                            if ((rc = publish(client, ipstack, topicid4)) != 0)
                                printf("rc from MQTT Publish is %d\n", rc);
                            else
                                printf("Published Buffer: %s to Topic %s\n", buf, MQTT_TOPIC);
                        }
                        if (strcmp((const char*)rqi.c_str(), "1234") == 0) {
                            printf("/n Generating MGMT response /n");
                            create_Topic("resp", "SAE02", "CSE_01");
                            //Create response code for CREATE AE
                            Response Resp;

                            RES.resourceID = "CSE1234";
                            RES.parentID = "AB123";
                            RES.creationTime = "092019";
                            RES.expirationTime = "112019";
                            RES.Resource_Type = ty;
                            //RES.resourceName = rn;
                            Resp.Request_Identifier = rqi;
                            Resp.To = From;
                            Resp.From = CSE_ID;
                            RES.FirmwareVersion = "5.12";
                            RES.SoftwareVersion = "MbedOS";
                            RES.resourceName = "ABC789";
                            buf = DM_Resp(Resp);

                            //Publish Registration Response

                            if ((rc = publish(client, ipstack, topicid)) != 0)
                                printf("rc from MQTT Publish is %d\n", rc);
                            else
                                printf("Published Buffer: %s to Topic %s\n", buf, MQTT_TOPIC);
                        }
                        break;
                    // update with ADN03 value to ADN02
                    case 3:
                        printf("/n Generating MGMT response other ADN /n");
                        create_Topic("resp", "SSAE03", "CSE_01");
                        //Create response code for CREATE AE
                        Response Resp1;

                        RES.resourceID = "CSE1234";
                        RES.parentID = "AB123";
                        RES.creationTime = "092019";
                        RES.expirationTime = "112019";
                        RES.Resource_Type = ty;
                        Resp1.Request_Identifier = rqi;
                        Resp1.To = From;
                        Resp1.From = CSE_ID;
                        RES.FirmwareVersion = FwV;
                        RES.SoftwareVersion = SwV;
                        RES.resourceName = "ABC7893";
                        buf = DM_Resp(Resp1);
                        //Publish Registration Response
                        if ((rc = publish(client, ipstack, topicid)) != 0)
                            printf("rc from MQTT Publish is %d\n", rc);
                        else
                            printf("Published Buffer: %s to Topic %s\n", buf, MQTT_TOPIC);
                        break;
                    }
                    break;
                }

                break;

            case 44:

                switch (ty) {
                case 13:

                    switch (op) {
                    case 2:
                        //retrive Registration
                        if (strcmp((const char*)rqi.c_str(), "1235") == 0) {
                            Request R1;
                            //RES.resourceName = rn;
                            R1.Resource_Type = ty;
                            //Resp.Request_Identifier = rqi;
                            R1.To = From;
                            R1.From = CSE_ID;
                            buf = Create_Req(R1);
                            create_Topic("req", "SSAE03", "CSE_01");
                            if ((rc = publish(client, ipstack, topicid4)) != 0)
                                printf("rc from MQTT Publish is %d\n", rc);
                            else
                                printf("Published Buffer: %s to Topic %s\n", buf, MQTT_TOPIC);
                        }
                        if (strcmp((const char*)rqi.c_str(), "1234") == 0) {
                            printf("/n Generating MGMT response /n");
                            create_Topic("resp", "SAE02", "CSE_01");
                            //Create response code for CREATE AE
                            Response Resp;

                            RES.resourceID = "CSE1234";
                            RES.parentID = "AB123";
                            RES.creationTime = "092019";
                            RES.expirationTime = "112019";
                            RES.Resource_Type = ty;
                            RES.resourceName = rn;
                            Resp.Request_Identifier = rqi;
                            Resp.To = From;
                            Resp.From = CSE_ID;
                            //RES.resourceName = "RN";
                            Resp.Request_Identifier = rqi;
                            Resp.To = From;
                            Resp.From = CSE_ID;
                            //RES.DevID =
                            RES.Manufacturer = "RPI";
                            RES.Model = "3 B+";
                            RES.DeviceType = "ARM";
                            RES.HardwareVersion = "Cortex";
                            RES.resourceName = "ABC789";
                            buf = DM_Resp2(Resp);
                            //Publish Registration Response
                            if ((rc = publish(client, ipstack, topicid)) != 0)
                                printf("rc from MQTT Publish is %d\n", rc);
                            else
                                printf("Published Buffer: %s to Topic %s\n", buf, MQTT_TOPIC);
                        }
                        break;
                    // update with ADN03 value to ADN02
                    case 3:
                        printf("/n Generating MGMT response other ADN /n");
                        create_Topic("resp", "SSAE03", "CSE_01");
                        //Create response code for CREATE AE
                        Response Resp1;
                        RES.resourceID = "CSE1234";
                        RES.parentID = "AB123";
                        RES.creationTime = "092019";
                        RES.expirationTime = "112019";
                        RES.Resource_Type = ty;
                        //RES.resourceName = rn;
                        Resp1.Request_Identifier = rqi;
                        Resp1.To = From;
                        Resp1.From = CSE_ID;
                        RES.Manufacturer = Man;
                        RES.Model = Mod;
                        RES.DeviceType = DevType;
                        RES.HardwareVersion = HwV;
                        RES.resourceName = "ABC7893";
                        buf = DM_Resp2(Resp1);
                        //Publish Registration Response
                        if ((rc = publish(client, ipstack, topicid)) != 0)
                            printf("rc from MQTT Publish is %d\n", rc);
                        else
                            printf("Published Buffer: %s to Topic %s\n", buf, MQTT_TOPIC);
                        break;
                    }
                    break;
                }

                break;
            }
            //---------- first case ends
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
