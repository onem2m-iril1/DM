#pragma once
#include <string>
#include <iostream>

using namespace std;

extern int ty, op, cst, rsc;
extern std::string resourceType, cseType, operation, content, response;
extern bool RequestReachability;  
extern bool rr, local;   
extern std::string From, csi, api, poa, to, rqi, aei, rn, mma, mmt, mmd, et, ct;
extern std::string DevID, Man, Mod, DevType, FwV, SwV, HwV;
extern char URI[40];
extern char* c_aei;
extern std::string con, cnf;
extern std::string nu;
extern std::string sur;
extern int net, nct;

extern void resource_type();
extern void Operation_Type();
extern void Response_Type();
extern void CSE_Type();



struct Resource 
{
    string resourceName;            //rn     1
    string memoryAvailabe;             //mma    1
    string memoryTotal;                //mmt    1
    int Resource_Type;              //ty     1
    int responseStatusCode;
    string Request_Identifier;
    string resourceID;              //ri     1
    string parentID;                //pi     1
    string expirationTime;          //et
    string creationTime;            //ct     1
    string lastModifiedTime;        //lt     1
    string DeviceID;                //DevID 
    string Manufacturer;            //Man
    string Model;                   //Mod
    string DeviceType;              //DevType 
    string FirmwareVersion;          //FwV
    string SoftwareVersion;         //SwV
    string HardwareVersion;         //HwV
    string labels;                  //      0-1
};

struct regularResource 
{ 
    //struct RES;
    string accessControlPolicyIDs;                  //
    string expirationTime;                          //et    1
    string dynamicAuthorizationConsultationIDs;     //
};

struct announceableResource 
{
    //struct regRES;                          
    string announceTo;              //
    string announcedAttribute;      //
};

struct announcedResource 
{
    //struct RES;
    string accessControlPolicyIDs;                          //      1
    string expirationTime;                                  //et    1
    string link;                                            //      1
    string dynamicAuthorizationConsultationIDs;             //   0-1
};

struct subordinateResource 
{
    //struct RES;
    string expirationTime;          //et    1
};

struct announceableSubordinateResource 
{
    //struct RES;
    string expirationTime;          //et    1
    string announceTo;              //      0-1
    string announcedAttribute;      //      0-1
};

struct announcedSubordinateResource
{   
    //struct RES;
    string expirationTime;          //et        1
    string link;                    //          1
};

class Response
{
public:
    int responseStatusCode;
    string Request_Identifier;
    string To;
    string From;
    int releaseVersion;
    string resourceName;    //rn
    string creationTime;    //ct
    string expirationTime;  //et
    string parentID;        //pi
    string memoryAvailabe;     //mma 
    string memoryTotal;        //mma     
    int Resource_Type;      //rt    
    string DeviceID;                //DevID 
    string Manufacturer;            //Man
    string Model;                   //Mod
    string DeviceType;              //DevType 
    string FirmwareVersion;          //FwV
    string SoftwareVersion;         //SwV
    string HardwareVersion;         //HwV 
};

class Request{
public:
    int Operation;  
    string To;
    string From;
    string Request_Identifier;
    int Resource_Type;
};

struct CreateMGO
{
    string resourceName;    //rn
    string creationTime;    //ct
    string expirationTime;  //et
    string parentID;        //pi
    
};

struct CreateAE
{
    string resourceName; 
    string App_ID; 
    string pointOfAccess; 
    bool requestReachability;
};

struct CreateCIN
{
    string resourceName;                //rn
    string contentInfo;                 //cnf 
    string content;                     //con
};

struct CreateSub
{
    string resourceName;                //rn
    string notificationURI;             //nu
    int notificationContentType;        //nct
    int notificationEventType;          //net
};

struct respAE
{
    string App_ID;
    bool requestReachability;
    string AE_ID;
    string pointOfAccess;
};

struct respCnt
{
    int stateTag;
    int CurrentNrOfInstances;
    int CurrentByteSize;
};
struct respCin
{
    string contentInfo;
    int contentSize;
    int stateTag;
    string content;
};

struct respmgo
{
    string resourceName;     //rn
    string creationTime;     //ct
    string expirationTime;   //et
    string parentID;         //pi
    string memoryAvailable;  //mma
    string memoryTotal;      //mmt
};

struct respSub
{
    string notificationURI;
    int notificationContentType;
    int notificationEventType;
};

const char* Create_Req(Request Req);

const char* Delete_Req(Request Req);

const char* Create_Resp(Response Resp);

const char* Notify_Resp(Response Resp);

const char* DM_Resp(Response Resp);

void process_msg(const char* Buffer);