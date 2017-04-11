/***********************************************************************
* @ 消息池
* @ brief
    1、转接网络层buffer中的数据，缓存，以待主逻辑循环处理
    2、直接处理网络buffer的数据，那就是在IO线程做逻辑了，很可能出现性能风险

* @ Notice
    1、Insert()接口，供网络层调用，是多线程的，所以消息池必须线程安全
    2、避免野指针：队列中缓存了Player*，应在主逻辑Handle()后才做 delete player

* @ author zhoumf
* @ date 2016-12-12
************************************************************************/
#pragma once
#include "tool/SafeQueue.h"
class Player;
class NetPack;

typedef std::function<void(NetPack&)> SendRpcParam;
typedef std::function<void(NetPack&)> RecvRpcParam;

class RpcQueue {
    typedef void(Player::*RpcFunc)(NetPack&);
    typedef std::pair<Player*, NetPack*> RpcPair;

    std::map<int, RpcFunc>      _rpc;       //自己实现的rpc
    std::map<int, RecvRpcParam>  _response;  //rpc远端的回复
    SafeQueue<RpcPair>          _queue; //Notice：为避免缓存指针野掉，主循环HandleMsg之后，处理登出逻辑
public:
    static RpcQueue& Instance(){ static RpcQueue T; return T; }
    RpcQueue();

    void Insert(Player* player, const void* pData, uint size); //Notice：须考虑线程安全
    void Update(); //主循环，每帧调一次
    void _Handle(Player* player, NetPack& buf);
    void RegistResponse(int opCode, const RecvRpcParam& func);
    static int RpcNameToId(const char* name);
};
#define sRpcQueue RpcQueue::Instance()
