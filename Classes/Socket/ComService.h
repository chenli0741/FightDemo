// #pragma once

#include "ODSocket.h"
#include <string>
#include <vector>

#ifdef WIN32
	#pragma comment(lib, "pthreadVCE2")
#endif

#include "pthread.h"
//#include <unistd.h>
#include "cocos2d.h"
#include <stdint.h>

#include "LQSocketBuffer.h"

typedef void* LPVOID;

using namespace std;
using namespace cocos2d;

struct MSG_RECV{
	int nCmd;
	string sData;
};

struct MSG_RECVDATA{
    int nCmd;
    LQSocketBuffer *buffer;
};

class ComService : public Ref
{
public: // 看好下面如果不是public的就不要调用 ... 应为lua必须好像是
//private:
	void SendData(int cmd ,const char *pszData);
    void SendData(const char *message, size_t msgLength);

//public:
	ComService(void);
	~ComService(void);

	bool startComm(const char *pszHostName, int nPort);
    bool resumeComm();
	void stopComm(void);

    void sendData(LQSocketBuffer *m_buffer);
    
    bool getIsConnect(){return this->m_bConnect ;}
    
    int getReceiveCode();
    //std::string ReceiveData();
    LQSocketBuffer *getReceiveData();

	static ComService*    getInstance();
	static ComService*    m_pComService;
//protected:
	static void* RecvThread(LPVOID param);
	static void* SendThread(LPVOID param);

	void RecvFunc(void);
	void SendFunc(void);

	void Close();
	bool InitNetwork(void);
	int GetValue(char *pszSrc, const char *pszAtr);
//	bool DecodePDUHead(char *pszPack, PDU_HEAD_STRUCT &head);

	std::string m_sHost;
	int m_nPort;
	bool m_bConnect;
	bool m_bQuit;
    
    pthread_t ptSend;
    pthread_t ptRecv;

	std::vector<char> m_vec_recv;

	vector<MSG_RECV> m_vec_msg;
    vector<MSG_RECVDATA> m_vec_msgdata;
	pthread_mutex_t m_mutex_recv;

	std::vector<char> m_vec_send;
	pthread_mutex_t m_mutex_send;
	pthread_cond_t m_cond_send;

	ODSocket m_odsSock;
};

