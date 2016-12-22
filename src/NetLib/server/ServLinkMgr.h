/***********************************************************************
* @ IOCP�����
* @ brief
	1��Client�����Ͽ����ܱ�֤�յ�socket close��
		���ͻ�������close���ؽ��̣���DoneIO�ص����ҡ�dwNumberOfBytesTransferred = 0��
	2��Clientǿɱ���̣��������
		���ֶ�ɱ���̣�����ϵͳTCPģ��ᴥ���رգ��Զ˿��յ�socket close
		��ֱ�ӹػ����Զ�ʲô����֪���ˣ��뿿������
	3������Ч����֤(��С����ʽ������)
	4������Client����Ƶ��
	5��ĳ������Ϣ�ѻ��������߳�
	6�����粨����������ʱ�����ͻ���ѻ����ڴ���
	7����Ϳ�����ۺ������������TCP_NODELAY����Nagle�㷨��������Լ��������ݷ���
		�������Ż�����Ϣ���۳���������ŵ��ײ�API
		�������߳�(��)����(50ms)��������buffer
	8��SYN��������ֹ�������ӱ���Чռ��
	9���м�����Ӳ�������socket close������·����ը�ˡ����߱��������
		�����ڷ�ҵ���߼��쳣��һ��ʱ���ᴥ��TCP��RST���ã���Ӧ��socketҲ�������ˣ�����DoneIO�ص�
		���Ƿ�ͬ���������������غϣ��������ĶϿ�ʱ�޳�Щ(60s)�������쳣������̺ܶ�(5s)
	*�������������ϣ�
* @ author zhoumf
* @ date 2016-7-15
************************************************************************/
#pragma once
//////////////////////////////////////////////////////////////////////////
// ʹ��winsock2  ����ͬwinsock��ͻ
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winsock2.h>
//////////////////////////////////////////////////////////////////////////

class ServLink;
class Thread;
struct ServerConfig;

typedef void(*HandleMsgFunc)(void* player, void* pMsg, DWORD size);
typedef void(*BindLinkFunc)(void*& refPlayer, ServLink* p, void* pMsg);
typedef void(*ReportErrorFunc)(void* player, int InvalidEnum, int nErrorCode, int nParam);

class ServLinkMgr{
public:
	const ServerConfig& _config;
	time_t              _timeNow = 0;
    BindLinkFunc        _BindLinkAndPlayer = NULL;
    HandleMsgFunc       _HandleClientMsg = NULL;
    ReportErrorFunc     _ReportErrorMsg = NULL;
public:
    ServLinkMgr(const ServerConfig& info);
    ~ServLinkMgr();

	static bool InitWinsock();
	static bool CleanWinsock();
	static bool IsValidIP(LPCSTR szIP){ return true; }

	// ��������socket������Ͷ�ݼ���AcceptEx
    bool CreateServer(BindLinkFunc bindPlayer, HandleMsgFunc handleClientMsg, ReportErrorFunc reportErrorMsg); //Notice������Ϣʱ��playerָ�����ΪNULL
	bool Close();

	bool AssistThreadLoop();
	bool _AssistLoop();
	void BroadcastMsg(void* pMsg, DWORD msgSize);

	// ���sClient(ServerLink)����Accept��������(��������socketʱԤ��Ͷ���˼���Accept)����������
	void Maintain(time_t timenow);

	SOCKET GetListener(){ return _sListener; }

	// ԭ���Բ��������connect��accept��invalid��������ԭ�Ӳ����м��о�̬�ģ��᲻���Bug��
	void LinkOnCreate(int id){
		InterlockedIncrement(&_nAccept);
		InterlockedDecrement(&_nInvalid);
	}
	void LinkOnConnect(int id){
		InterlockedIncrement(&_nConnect);
		InterlockedDecrement(&_nAccept);
	}
	void LinkOnAcceptClose(int id){
		InterlockedIncrement(&_nInvalid);
		InterlockedDecrement(&_nAccept);
	}
	void LinkOnClose(int id){
		InterlockedIncrement(&_nInvalid);
		InterlockedDecrement(&_nConnect);
	}
private:
	LONG _nInvalid;
	LONG _nAccept;     // ���ڵȴ����ӵ�socket����
	LONG _nConnect;    // �����ӵ�socket����

    Thread* _pThread = NULL; //����c++11��ָ���Ա�������ʱ��ָ��NULL����ֹctor��©��������Ұָ��

	SOCKET	_sListener;
	std::vector<ServLink*> _vecLink;
	//const int c_nMaxLink = 2/*50000*/;
	//ServLink* _arrLink[c_nMaxLink];
};