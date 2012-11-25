
#include <assert.h>
#include "codec.h"
#include "commu.h"
#include "finger.pb.h"
#include <Poco/Timer.h>

class AbstractMessageCallback
{
public:
    AbstractMessageCallback()
    {

    }
    AbstractMessageCallback(const AbstractMessageCallback& callback)
    {

    }
    virtual ~AbstractMessageCallback()
    {

    }
    virtual void invoke(google::protobuf::Message* msg) const = 0;
    AbstractMessageCallback& operator = (const AbstractMessageCallback& callback)
    {
        return *this;
    }
    virtual AbstractMessageCallback* clone() const = 0;
};

template <class C>
class MessageCallback:public AbstractMessageCallback
{
public:
    typedef void (C::*Callback)(google::protobuf::Message*);

    MessageCallback(C& object, Callback method): _pObject(&object), _method(method)
    {
    }

    MessageCallback(const MessageCallback& callback): _pObject(callback._pObject), _method(callback._method)
    {
    }

    ~MessageCallback()
    {
    }

    MessageCallback& operator = (const MessageCallback& callback)
    {
        if (&callback != this)
        {
            _pObject = callback._pObject;
            _method  = callback._method;
        }
        return *this;
    }
    void invoke(google::protobuf::Message* message) const
    {
        (_pObject->*_method)(message);
    }
    AbstractMessageCallback* clone() const
    {
        return new MessageCallback(*this);
    }

private:
    MessageCallback();

    C*       _pObject;
    Callback _method;
};



#if 0
#include <map>
class MessageDispatcher
{
public:
    MessageDispatcher(){}
    virtual ~MessageDispatcher(){}
    void Register(const google::protobuf::Descriptor* desc,const AbstractMessageCallback& callback)
    {
        callbackmaps[desc] = callback.clone ();
    }
    void Run(google::protobuf::Message* msg)
    {

        TMsgMap::const_iterator it = callbackmaps.find (msg->GetDescriptor ());

        if(it != callbackmaps.end ())
        {
            it->second->invoke(msg);
        }
        else
        {
            printf("error msg[%s]\n",msg->GetTypeName ().c_str ());
        }

    }

private:
    typedef std::map<const google::protobuf::Descriptor* ,AbstractMessageCallback*> TMsgMap;
    TMsgMap callbackmaps;
};
MessageDispatcher md;
#endif

Commu::Commu()
{

    //MessageCallback<Commu> tc(*this,&Commu::Test1);
    //md.Register (FingerLoginRequest::descriptor (),tc);


}
void Commu::Test1(google::protobuf::Message* req)
{
    FingerLoginRequest *freq = dynamic_cast<FingerLoginRequest*>(req);

#if 0
    std::string str = encode(rsp);
    zframe_t* frame_req = zframe_new(str.data (),str.length ());
    zframe_send (&frame_req,finger_socket,0);
#endif
}
/*!
1.登录消息，获取登录人员ID
2.更新人员ID和指纹位置和指纹模板数据
3.检测设备故障
*/
//message type --> OnMessage(google::message *msg)
bool Commu::Start(CFingerMgr *mgr)
{
    this->mgr = mgr;
    finger_context = zctx_new ();

    assert(finger_context!=NULL);

    finger_socket = zsocket_new (finger_context,ZMQ_REP);

    zsocket_bind (finger_socket, "tcp://%s:%d","*",5555);

    while(1)
    {
        zframe_t* msg = zframe_recv (finger_socket);

        byte* d = zframe_data (msg);
        std::string transport;
        transport.append ((const char*)d,zframe_size(msg));

        std::string buf = transport.substr(sizeof(int32_t));
        google::protobuf::Message *pMsg = decode(buf);

        zframe_destroy(&msg);

        delete pMsg;
    }


    return true;
}
