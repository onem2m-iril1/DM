#include "sensor.h"
#include "MbedJSONValue.h"

void process_rsc(MbedJSONValue &MSG);

struct Resource RES;
struct regularResource regRES;
struct announceableResource ancRES;
struct announcedResource ancdRES;
struct subordinateResource subRES;
struct announceableSubordinateResource ancsubRES;
struct announcedSubordinateResource ancdsubRES;

struct CreateAE CAE;
struct CreateMGO MGO;
struct CreateCIN CIN;
struct CreateSub CSub;
struct respAE RAE;
struct respCnt RCnt;
struct respCin RCin;
struct respSub RSub;
struct respmgo RMG;

//extern const char* CSE_ID;
extern char* AE_ID;
extern bool lcl;

//////////////////////////////////////////////
//      Func: Create_Req                    //
//      returns a json string               //
//      input parameters Class Request      //
//////////////////////////////////////////////
const char* Create_Req(Request Req)
{
    MbedJSONValue demo, demo2, demo3, demo4;
    Req.Operation = 2;
    Req.Request_Identifier="1234";     
    std::string s;
    switch(Req.Resource_Type){
        case 0:
            break;
        case 1:
            break;
        case 2:
            demo3["api"] = CAE.App_ID;
            demo3["rn"] = CAE.resourceName;
            demo3["rr"] = CAE.requestReachability; 
            demo2["m2m:ae"] = demo3;
        break;
        case 3:
            demo3["rn"] = CAE.resourceName;
            demo2["m2m:cnt"] = demo3;
        break;
        case 4:
            demo3["cnf"] = CIN.contentInfo;
            demo3["con"] = CIN.content;
            demo2["m2m:cin"] = demo3;
        break;
        case 13:
            //demo2["ri"] = MGO.resourceID;
            //demo2["rn"] = MGO.resourceNam e;
        break;
            
        case 23:
            demo3["rn"] = CSub.resourceName;          
            demo4["net"] = CSub.notificationEventType;
            demo3["enc"] = demo4; 
            demo3["nu"] = CSub.notificationURI;             
            demo3["nct"] = CSub.notificationContentType;      
            demo2["m2m:sub"] = demo3;
        break;
    }
    demo["fr"] = Req.From;
    demo["op"] = Req.Operation;
    //demo["pc"] = demo2;
    //demo["m2m:mgo"] = demo2;
    demo["rqi"] = Req.Request_Identifier;
    demo["to"] = Req.To;
    //demo["ty"] = Req.Resource_Type;
    //demo["lcl"] = lcl;
    
    //serialize it into a JSON string
    s = demo.serialize();
    printf("\nMSG SIZE: %d\n", s.length());
    printf("Req JSON: %s\r\n", s.c_str());

    return s.c_str();
}

//////////////////////////////////////////////
//      Func: Delete_Req                    //
//      returns a json string               //
//      input parameters Class Request      //
//////////////////////////////////////////////

const char* Delete_Req(Request Req)
{
    MbedJSONValue demo, demo2, demo3, demo4;
         
    std::string s;
   
    demo["fr"] = Req.From;
    demo["op"] = Req.Operation;
    demo["rqi"] = Req.Request_Identifier;
    demo["to"] = "memory001";
    demo["lcl"] = lcl;
    
    //serialize it into a JSON string
    s = demo.serialize();
    printf("\nMSG SIZE: %d\n", s.length());
    printf("Req JSON: %s\r\n", s.c_str());

    return s.c_str();
}
//////////////////////////////////////////////
//      Func: Create_Resp                   //
//      returns a json string               //
//      input parameters Class Response     //
//////////////////////////////////////////////
const char* Create_Resp(Response Resp)
{
    MbedJSONValue demo, demo2, demo3, demo4;
    demo3["ty"] = RES.Resource_Type;
    demo3["rsc"] = RES.responseStatusCode;
    //demo3["ri"] = RES.resourceID;
    //demo3["pi"] = RES.parentID;
    //demo3["ct"] = RES.creationTime;         //ct     1
    //demo3["et"] = RES.expirationTime;     //et     1  
    //demo3["rn"] = RES.resourceName;
    //demo3["mmt"] = RES.memoryTotal;
    //demo3["mma"] = RES.memoryAvailabe;
    std::string s;
    //RES.Resource_Type = 13;
    printf("RES.Resource_Type = %d",RES.Resource_Type);
    switch(RES.Resource_Type){
        case 0:
            demo3["api"] = RAE.App_ID;
            demo3["rr"] = RAE.requestReachability;
            demo3["aei"] = RAE.AE_ID;
            demo3["poa"] = RAE.pointOfAccess; 
            demo2["m2m:ae"] = demo3;
        break;
        case 1:
        break;
        case 2:
            demo3["api"] = RAE.App_ID;
            demo3["rr"] = RAE.requestReachability;
            demo3["aei"] = RAE.AE_ID;
            demo3["poa"] = RAE.pointOfAccess; 
            demo2["m2m:ae"] = demo3;
        break;
        case 3:
            demo3["st"] = RCnt.stateTag;
            demo3["cni"] = RCnt.CurrentNrOfInstances;
            demo3["cbs"] = RCnt.CurrentByteSize;
            demo2["m2m:cnt"] = demo3;
        break;
        case 4:
            demo3["et"] = ancsubRES.expirationTime;
            demo3["st"] = RCin.stateTag;
            demo3["cnf"] = RCin.contentInfo;
            demo3["cs"] = RCin.contentSize;
            demo3["con"] = RCin.content;
            demo2["m2m:cin"] = demo3;
        break;
        case 13:
            demo2["m2m:mgo"] = demo3;
            break;
        case 23:
            demo3["et"] = ancsubRES.expirationTime;
            demo4["net"] = RSub.notificationEventType;
            demo3["enc"] = demo4;
            demo3["nct"] = RSub.notificationContentType;
            demo3["nu"] = RSub.notificationURI;
            demo2["m2m:sub"] = demo3;
        break;
        
    }
    //demo["rsc"] = Resp.responseStatusCode;
    //demo["rqi"] = Resp.Request_Identifier;
    //demo["pc"] = demo2;
    //demo["to"] = Resp.To;
    //demo["fr"] = Resp.From;
    //demo["lcl"] = lcl;
    
    //serialize it into a JSON string
    s = demo2.serialize();
    printf("\nMSG SIZE: %d\n", s.length());
    printf("RESP JSON: %s\r\n", s.c_str());
    return s.c_str();
}

const char* Notify_Resp(Response Resp)
{
    MbedJSONValue demo;
    std::string s;
    demo["rsc"] = Resp.responseStatusCode;
    demo["rqi"] = Resp.Request_Identifier;
    demo["fr"] = Resp.From;
    demo["to"] = Resp.To;
    demo["lcl"] = lcl;
    //serialize it into a JSON string
    s = demo.serialize();
    printf("\nMSG SIZE: %d\n", s.length());
    printf("RESP JSON: %s\r\n", s.c_str());
    return s.c_str();
}

const char* DM_Resp(Response Resp) // DevID, Man, Mod, DevType, FwV, SwV, HwV
{
    MbedJSONValue demo, demo2, demo3, demo4;
    std::string s;
    switch(RES.Resource_Type){
        case 0:
        break;
        case 13:
            demo2["Man"] = Resp.Manufacturer;
            demo2["Mod"] = Resp.Model;
            demo2["DevType"] = Resp.DeviceType;
            demo2["FwV"] = Resp.FirmwareVersion; 
            demo2["SwV"] = Resp.SoftwareVersion;
            demo2["HwV"] = Resp.HardwareVersion;
            demo2["ri"] = RES.resourceID;
            demo2["pi"] = RES.parentID;
            demo2["ct"] = RES.creationTime;       //ct     1
            demo2["et"] = RES.expirationTime;     //et     1  
            demo2["rn"] = RES.resourceName;
        break;
    }
    demo["rsc"] = Resp.responseStatusCode;
    demo["rqi"] = Resp.Request_Identifier;
    demo["fr"] = Resp.From;
    demo["to"] = Resp.To;
    demo["m2m:mgo"] = demo2;
    
    //serialize it into a JSON string
    s = demo.serialize();
    printf("\nMSG SIZE: %d\n", s.length());
    printf("RESP JSON: %s\r\n", s.c_str());
    return s.c_str();
}

void process_msg(const char* Buffer)
{
    MbedJSONValue MSG;

    parse(MSG, Buffer); 

    ////////////////////////Local Parameter (Additional parameter)//////////////////////
    if(MSG.hasMember("lcl"))
    {
        local = MSG["lcl"].get<bool>();
        printf("Local: %s\r\n", local? "true" : "false");
    }
     
    ////////////////////////resource Type Parameter (Mendatory parameter)//////////////////////
    if(MSG.hasMember("rsc"))
    {
        rsc = MSG["rsc"].get<int>();
        printf("Response Status Code: %d\r\n", rsc);
        Response_Type();
        printf("Response Status: %s\r\n", response.c_str());
        process_rsc(MSG);
        return;
    }
    
    ////////////////////////From parameter (Mendatory parameter[optional for Create AE])//////////
    if(MSG.hasMember("fr"))
    {
       From = MSG["fr"].get<std::string>();
       printf("From: %s\r\n", From.c_str());
    }
    else     {
    //add Response Status Code for no mendatory parameter BAD_REQUEST 
    return;  }
    
    ////////////////////////Operation Parameter (Mendatory parameter)//////////////////////
    if(MSG.hasMember("op"))
    {
       op = MSG["op"].get<int>();
       printf("Operation: %d\r\n", op);
       Operation_Type();
       printf("Operation: %s\r\n", operation.c_str());
       /* Process Notification*/
       if(op == 5)
       {
            MbedJSONValue &MSG_1 = MSG["pc"]["m2m:sgn"];
            sur = MSG_1["sur"].get<std::string>();
            printf("Subscription Reference: %s\r\n", sur.c_str());
            MbedJSONValue &MSG_2 = MSG_1["m2m:sgn"]["nev"];
            net = MSG_2["net"].get<int>();
            printf("NotificationEventType: %d\r\n", net); 
            MbedJSONValue &MSG_3 = MSG_2["nev"]["rep"]["cin"];
            con = MSG_3["con"].get<std::string>();
            printf("Content: %s\r\n", con.c_str());
            cnf = MSG_3["cnf"].get<std::string>(); 
            printf("Content Info: %s\r\n", cnf.c_str());
            to = MSG["to"].get<std::string>();
            printf("To: %s\r\n", to.c_str());     
            rqi = MSG["rqi"].get<std::string>();
            printf("Request Identifier: %s\r\n", rqi.c_str());   
            MSG.~MbedJSONValue();
            MSG_1.~MbedJSONValue();
            MSG_2.~MbedJSONValue();
            MSG_3.~MbedJSONValue();
            return;
       }
    }
    else     {
    //add response code for no mendatory parameter
    return;  }
    
    ////////////////////////To Parameter (Mendatory parameter)//////////////////////
    if(MSG.hasMember("to"))
    {
       to = MSG["to"].get<std::string>();
       printf("To: %s\r\n", to.c_str());
    }
    else     {
    //add response code for no mendatory parameter
    return;  } 
         
    ////////////////////////Request Identifier (Mendatory parameter)//////////////////////
    if(MSG.hasMember("rqi"))
    {
       rqi = MSG["rqi"].get<std::string>();
       printf("Request Identifier: %s\r\n", rqi.c_str());
    }
    else     {
    //add response code for no mendatory parameter
    return;   }        
    
    ////////////////////////resource Type Parameter (Mendatory parameter)//////////////////////
    if(MSG.hasMember("ty"))
    {
       ty = MSG["ty"].get<int>();
       printf("ResourceType: %d\r\n", ty);
       resource_type();
       printf("ResourceType: %s\r\n", resourceType.c_str());
    }
    else     {
    //add response code for no mendatory parameter
    return;   }
    
    ////////////////////////Response Status Code
    if(MSG.hasMember("rsc"))
    {
       rsc = MSG["rsc"].get<int>();
       printf("Response Status Code: %d\r\n", rsc);
       Response_Type();
       printf("Response Status: %s\r\n", response.c_str());
    }
    MbedJSONValue &MSG_1 = MSG["pc"][content]; //content value depends on the resource type e.g. m2m:ae for resource type 2
    
    //////////////////////// Resource specific attributes CSE-ID
    if(MSG_1.hasMember("csi"))
    {
       csi = MSG_1["csi"].get<std::string>();
       printf("CSI: %s\r\n", csi.c_str());
    }
       
    //////////////////////// Resource specific attributes Point of Access
    if(MSG_1.hasMember("poa"))
    {
       poa = MSG_1["poa"].get<std::string>();
       printf("POA: %s\r\n", poa.c_str());
    }  
     
    /////////////////////// Resource specific attributes CSE Type
    if(MSG_1.hasMember("cst"))
    {
       cst = MSG_1["cst"].get<int>();
       printf("CSE Type: %d\r\n", cst);
       CSE_Type();
       printf("CSE Type: %s\r\n", cseType.c_str());
    }
       
    /////////////////////// Resource specific attributes RequestReachability
    if(MSG_1.hasMember("rr"))
    {
       RequestReachability = MSG_1["rr"].get<bool>();
       printf("RR: %s\r\n", RequestReachability? "true" : "false");
    }
       
    /////////////////////// Resource specific attributes App-ID
    if(MSG_1.hasMember("api"))
    {
       api = MSG_1["api"].get<std::string>();
       printf("App-ID: %s\r\n", api.c_str());
    } 
    /////////////////////// Resource specific attributes resourceName
    if(MSG_1.hasMember("rn"))
    {
        rn = MSG_1["rn"].get<std::string>();
        printf("resourceName : %s\r\n", rn.c_str());
    } 
    /////////////////////// Resource specific attributes Content
    if(MSG_1.hasMember("con"))
    {
        con = MSG_1["con"].get<std::string>();
        printf("Content: %s\r\n", con.c_str());
    } 
    /////////////////////// Resource specific attributes ContentInfo
    if(MSG_1.hasMember("cnf"))
    {
        cnf = MSG_1["cnf"].get<std::string>();
        printf("Content Info: %s\r\n", cnf.c_str());
    }  
    /////////////////////// Resource specific attributes NotificationURI nu
    if(MSG_1.hasMember("nu"))
    {
        nu = MSG_1["nu"].get<std::string>();
        printf("Notification URI: %s\r\n", nu.c_str());
    } 
    
    /////////////////////----------Device Management--------//////////////////////////
    
    
     MbedJSONValue &MSG_4 = MSG["m2m:mgo"][content];
    ////////////////////////
    if(MSG_4.hasMember("mma"))
    {
       mma = MSG_4["mma"].get<std::string>();
       printf("memoryAvailable: %s\r\n", mma.c_str());
    }
  
    
    ////////////////////////
    if(MSG_4.hasMember("mmt"))
    {
       mmt = MSG_4["mmt"].get<std::string>();
       printf("memoryTotal: %s\r\n", mmt.c_str());
    }
 
    
    if(MSG_4.hasMember("Man"))
    {
       Man = MSG_4["Man"].get<std::string>();
       printf("Manufacturer: %s\r\n", Man.c_str());
    }
  
    
    ////////////////////////
    if(MSG_4.hasMember("Mod"))
    {
       Mod = MSG_4["Mod"].get<std::string>();
       printf("Model: %s\r\n", Mod.c_str());
    }
    
    if(MSG_4.hasMember("DevType"))
    {
       DevType = MSG_4["DevType"].get<std::string>();
       printf("DeviceType: %s\r\n", DevType.c_str());
    }
  
    
    ////////////////////////
    if(MSG_4.hasMember("FwV"))
    {
       FwV = MSG_4["FwV"].get<std::string>();
       printf("Firmware Version: %s\r\n", FwV.c_str());
    }
 
    
    if(MSG_4.hasMember("SwV"))
    {
       SwV = MSG_4["SwV"].get<std::string>();
       printf("Software Version: %s\r\n", SwV.c_str());
    }
  
    if(MSG_4.hasMember("HwV"))
    {
       HwV = MSG_4["HwV"].get<std::string>();
       printf("Hardware Version: %s\r\n", HwV.c_str());
    }
    
    ////////////////////----------- END DM -------------/////////////////////////// 
    
    
    /////////////////////// Resource specific attributes NotificationContentType
    if(MSG_1.hasMember("nct"))
    {
        nct = MSG_1["nct"].get<int>();
        printf("NotificationContentType: %d\r\n", nct);
        MbedJSONValue &MSG_2 = MSG_1["m2m:sub"]["enc"];
        if(MSG_2.hasMember("net"))
        {
            net = MSG_2["net"].get<int>();  
            printf("NotificationEventType: %d\r\n", net);  
        }
        MSG_2.~MbedJSONValue();
    }   
    
    MSG.~MbedJSONValue();
    MSG_1.~MbedJSONValue();
    return;
}

void process_rsc(MbedJSONValue &MSG)
{
    if(MSG.hasMember("rqi"))
    {
       rqi = MSG["rqi"].get<std::string>();
       printf("Request Identifier: %s\r\n", rqi.c_str());
    }
    else     {
    //add response code for no mendatory parameter
    return;   }  
    if(MSG.hasMember("to"))
    {
       to = MSG["to"].get<std::string>();
       printf("To: %s\r\n", to.c_str());
    }
    else     {
    //add response code for no mendatory parameter
    return;   } 
    if(MSG.hasMember("fr"))
    {
       From = MSG["fr"].get<std::string>();
       printf("From: %s\r\n", From.c_str());
    }
    else     {
    //add response code for no mendatory parameter
    return;   }
    
    MbedJSONValue &MSG_1 = MSG["pc"][content];
    
    if(MSG_1.hasMember("ty"))
    {
        ty = MSG_1["ty"].get<int>();
        printf("ResourceType in response: %d\r\n", ty);
        //resource_type();
        //printf("ResourceType in response: %s\r\n", resourceType.c_str());
    }
    else     {
    //add response code for no mendatory parameter
    return;   }
    
    if(MSG_1.hasMember("rr"))
    {
       RequestReachability = MSG_1["rr"].get<bool>();
       printf("RR: %s\r\n", RequestReachability? "true" : "false");
    } 
    if(MSG_1.hasMember("aei"))
    {
       aei = MSG_1["aei"].get<std::string>();
       printf("AE_ID: %s\r\n", aei.c_str());
    }         
    if(MSG_1.hasMember("poa"))
    {
       poa = MSG_1["poa"].get<std::string>();
       printf("POA: %s\r\n", poa.c_str());
    } 
    if(MSG_1.hasMember("rn"))
    {
       rn = MSG_1["rn"].get<std::string>();
       printf("Resource Name: %s\r\n", rn.c_str());
    }
    if(MSG_1.hasMember("api"))
    {
       api = MSG_1["api"].get<std::string>();
       printf("App-ID: %s\r\n", api.c_str());
    } 
    /////////////////////// Resource specific attributes Content
    if(MSG_1.hasMember("con"))
    {
        con = MSG_1["con"].get<std::string>();
        printf("Content: %s\r\n", con.c_str());
    } 
    /////////////////////// Resource specific attributes ContentInfo
    if(MSG_1.hasMember("cnf"))
    {
        cnf = MSG_1["cnf"].get<std::string>();
        printf("Content Info: %s\r\n", cnf.c_str());
    }  
    
    return;
}
