#ifndef COMMU_H
#define COMMU_H
#include <czmq.h>
#include "FingerMgr.h"
#include <google/protobuf/message.h>
class FingerLoginRequest;
class FingerResponse;
class Commu
{
public:
    Commu();
    bool Start(CFingerMgr *mgr);
    void ParseCmd(FingerLoginRequest& req);
private:
    void Test1(google::protobuf::Message* req);
    void Login(FingerResponse* resp);
    void UpdateFingerData(FingerLoginRequest& req,FingerResponse* resp);
    zctx_t* finger_context;
    void* finger_socket;
    CFingerMgr *mgr;
};

#endif // COMMU_H
