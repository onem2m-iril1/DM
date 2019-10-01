
int mgmtObject(MQTTSN::Client<MQTTSNUDP, Countdown> *client, MQTTSNUDP* ipstack,  MQTTSN_topicid& topicid, string h_available, string h_total, string s_available, string s_total)
{
    //Create Container
    //Mendatory resources: Operation, To, From, Request Identifier ,Resource Type, Content 
    //Resource Specific Attributes [M]: App-ID, requestReachability, supportedReleaseVersions
    //                              [0]: PointofAccess, ResourceName

    //struct Mendatory m1; (to, fr, rqi, ty, op)
    Request R1;
    R1.To = "CSE1234\Hardware01"; 
    R1.From = "CSE01";
    R1.Request_Identifier = "1234";
    //R1.Resource_Type = 13;
    R1.Operation = 2;
    
    //struct Management Object;
    //MGO.contentInfo = "text/plains:0"; 
    //MGO.content = "854";
    //buffer = Create_Cnt();
    //lcl = false;
    buf = Create_Req(R1);
    //buf[0] = '\0'; 
        
    //PUBLISH
    if ((rc = publish(client, ipstack, topicid)) != 0)
         printf("rc from MQTT Publish is %d\n", rc);
    else
         printf("Published Buffer: %s to Topic %s\n",buf, MQTT_TOPIC);

    while(1)
    {     
        client->yield(4000);
        if(isMessageArrived) {
            isMessageArrived = false;
            printf("\r\nMessage arrived:\r\n%s\r\n", messageBuffer);
            //process_msg();
            process_msg(messageBuffer);
            free(messageBuffer);
        } 
        if(rsc != 0){ 
            //reg_resp = true;
            printf("rsc = %d\n",rsc);
            break;
        }
        else rsc = 0;
    }
    if(rsc == 2000 ){ 
        rsc = 0;
        return 0;
    }
    else {
        rsc = 0;
        return -1;
    }
    //wait for response
    return 0;
}
