#include "stdafx.h"
#include "..\NetLib\server\ServLink.h"
#include "Player.h"
#include "..\msg\TestMsg.h"
#include "Buffer\NetPack.h"

NetPack& Player::_backBuffer = NetPack(0);

Player::Player(ServLink* p)
    : _clientNetLink(p)
{

}
void Player::SetServLink(ServLink* p)
{
    _clientNetLink = p;
}
void Player::SendMsg(const stMsg& msg, uint16 size)
{
    _clientNetLink->SendMsg(&msg, size);
}
void Player::SendPack(const NetPack& pack)
{
    _clientNetLink->SendMsg(pack.Buffer(), pack.Size());
}
void Player::CallRpc(uint16 opCode, const WriteRpcParam& func)
{
    static NetPack msg(0);
    msg.ClearBody();
    msg.SetOpCode(opCode);
    func(msg);
    SendPack(msg);
}
void Player::CallRpc(uint16 opCode, const WriteRpcParam& fun1, const ReadRpcBack& fun2)
{
    CallRpc(opCode, fun1);

    sRpcQueue.RegistResponse(opCode, fun2);
}
//////////////////////////////////////////////////////////////////////////
// msg ��Ӧ����ʵ��
Msg_Realize(C2S_Login)
{
    printf("C2S_Login\n");
}
Msg_Realize(C2S_ReConnect)
{
    printf("C2S_ReConnect\n");
}
Msg_Realize(C2S_Echo)
{
    TestMsg& msg = (TestMsg&)req;
    char* str = msg.data;
    SendMsg(msg, msg.size());
    printf("Echo: %s\n", str);
}


//////////////////////////////////////////////////////////////////////////
// rpc
Rpc_Realize(rpc_login)
{
    printf("rpc_login\n");
    NetPack& backBuffer = BackBuffer();
    backBuffer << m_index;
}
Rpc_Realize(rpc_reconnect) { printf("rpc_reconnect\n"); }
Rpc_Realize(rpc_echo)
{
    string str = recvBuf.ReadString();
    printf("Echo: %s\n", str.c_str());

    NetPack& backBuffer = BackBuffer();
    backBuffer << str;
}
