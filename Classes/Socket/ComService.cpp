#include "ComService.h"
//#include <iterator>
//#include "aes.h"
//#include <stdlib.h>
//#include "str_fix.h"
//#include <string.h>

//#pragma comment(lib, "pthreadVCE2")

ComService*    ComService::m_pComService = NULL;

//// 等待发送消息队列
//struct node {
//    const char* m_message;
//    size_t m_msgLength;
//    struct node *m_next;
//    node(){
//        m_message = 0;
//        m_msgLength = 0;
//    }
//} *head = NULL;


ComService::ComService(void)
:m_bConnect(false)
,m_bQuit(false)
{
	//m_RecPtr = NULL;
}

ComService::~ComService(void)
{
	if(m_pComService)
	{
		delete m_pComService;
	}
}

ComService* ComService::getInstance()
{
	if(!m_pComService)
	{
		m_pComService = new ComService();
        m_pComService->retain();
	}
	return m_pComService;
}


bool ComService::startComm(const char *pszHostName, const int nPort)
{
    /*
     if (m_bConnect && !m_bQuit)
     {
     if (strcmp(m_sHost.c_str(), pszHostName) == 0 && m_nPort == nPort)
     {
     return true;
     }
     }
     */
#if DEBUG
    printf("--->开始连接服务器\n");
#endif
	m_sHost = pszHostName;
	m_nPort = nPort;
	ODSocket::Init();
	if (!InitNetwork())
    {
#if DEBUG
        printf("----->创建连接失败.\n");
#endif
        return false;
    }
    m_bQuit = false;
	pthread_mutex_init(&m_mutex_send, NULL);
	pthread_cond_init(&m_cond_send, NULL);
	pthread_mutex_init(&m_mutex_recv, NULL);
    //	pthread_t tid;
    
    int rc = 0;
    rc = pthread_create(&ptRecv, NULL, RecvThread, this);
	rc += pthread_create(&ptSend, NULL, SendThread, this);
#if DEBUG
    printf("----->服务器连接成功.\n");
#endif
	return (rc == 0);
}

bool ComService::resumeComm()
{
    ODSocket::Init();
	if (!InitNetwork())
    {
        printf("服务器连接失败.\n");
    }
    m_bQuit = false;
    
	pthread_mutex_init(&m_mutex_send, NULL);
	pthread_cond_init(&m_cond_send, NULL);
	pthread_mutex_init(&m_mutex_recv, NULL);
    //	pthread_t tid;
    int rc = pthread_create(&ptRecv, NULL, RecvThread, this);
	rc += pthread_create(&ptSend, NULL, SendThread, this);
    
	return (rc == 0);
}

void ComService::stopComm()
{
	m_bQuit = true;
	Close();
    
    if (m_vec_msgdata.size())
	{
        m_vec_msgdata.erase(m_vec_msgdata.begin());
	}
    
#if defined(__ANDROID__)
#else
    pthread_cancel(ptSend);
    pthread_cancel(ptRecv);
#endif
	pthread_mutex_destroy(&m_mutex_recv);
	pthread_mutex_destroy(&m_mutex_send);
	pthread_cond_destroy(&m_cond_send);
}

// 直接消息
void ComService::sendData(LQSocketBuffer *m_buffer)
{
    
    // body length
    short length = m_buffer->size() + sizeof(short);
    // message data
    LQSocketBuffer *buffer = LQSocketBuffer::createWithCapacity(length);
    // append length
    buffer->writeShort(length);
    // append message body
    buffer->writeBytes(m_buffer->bodyData(),m_buffer->size());
    
    SendData(buffer->bodyData(), buffer->size());
    
#if DEBUG
    buffer->resetPosition();
    short blength = buffer->readShort();
    int bmsgcode  = buffer->readInt();
    CCLOG("----->准备发送:  msgCode:%d , length:%u",bmsgcode,blength);
#endif
}

// 消息体  消息长度
void ComService::SendData(const char *message, size_t msgLength)
{
    char* data = (char*)malloc(msgLength);
    memcpy(data, message, msgLength);
    
    //    struct node *p = new node;
    //    p->m_message = data;
    //    p->m_msgLength = msgLength;
    //    p->m_next = head;
    //    head = p;
    //copy(data.begin(), data->cend(), back_inserter(m_vec_send));
    
    pthread_mutex_lock(&m_mutex_send); //需要操作head这个临界资源，先加锁，
    
    std::copy(data, data+msgLength, back_inserter(m_vec_send));
    
    //    std::copy(head, head, back_inserter(m_vec_msg));
    
    //m_vec_send.push_back(p);
    pthread_cond_signal(&m_cond_send);
	pthread_mutex_unlock(&m_mutex_send);
    
    //-----------------------------
    //    char* cdata = (char*)malloc(msgLength);
    //    memcpy(cdata, message, msgLength);
    //
    //    char szSend[128] = {0};
    //    memset(szSend, 0, sizeof(char)*128);
    //
    //    string data = string(szSend);
    //    data.append(cdata);
    //
    //    printf("will send data: %s , size:%lu \n", data.data() , sizeof(msgLength));
    //
    //	pthread_mutex_lock(&m_mutex_send);
    //	//copy(pszData, pszData + nTotal, back_inserter(m_vec_send));
    //    copy(data.cbegin(), data.cend(), back_inserter(m_vec_send));
    //
    //	pthread_cond_signal(&m_cond_send);
    //	pthread_mutex_unlock(&m_mutex_send);
    
}

void ComService::SendData(const int cmd ,const char *pszData)
{
}

bool ComService::InitNetwork(void)
{
	if (m_bConnect)
	{
		Close();
	}
	bool bRet = m_odsSock.Create(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (!bRet)
	{
#if DEBUG
        printf("Create socket failure\n");
#endif
		return bRet;
	}
	char szIP[20] = {0};
	bRet = ODSocket::DnsParse(m_sHost.c_str(), szIP);
	if (!bRet)
	{
#if DEBUG
        printf("Parse DNS of address[%s(%s)] failure\n", m_sHost.c_str(), szIP);
#endif
		return bRet;
	}
	m_bConnect = m_odsSock.Connect(szIP, m_nPort);
#if DEBUG
    if (!m_bConnect)
    {
        printf("Connect to %s:%d failure.\n", szIP, m_nPort);
    }
#endif
	return m_bConnect;
}

void ComService::Close()
{
    int n_ret = m_odsSock.Close();
    if (n_ret == SOCKET_ERROR)
    {
        printf("Close connection failure, err: %d\n", errno);
    }
	m_bConnect = false;
#if DEBUG
    printf("Connection closed.\n");
#endif
}

void* ComService::RecvThread(LPVOID param)
{
#if defined(__ANDROID__)
    CCLOG("This is from android RecvThread");
#else
    //    pthread_setname_np("NET RECV");
#endif
	ComService *pchgt = (ComService *)param;
	pchgt->RecvFunc();
	return NULL;
}

void* ComService::SendThread(LPVOID param)
{
#if defined(__ANDROID__)
    CCLOG("This is from android SendThread");
#else
    //    pthread_setname_np("NET SEND");
#endif
	ComService *pchgt = (ComService *)param;
	pchgt->SendFunc();
	return NULL;
}

void ComService::RecvFunc()
{
	pthread_detach(pthread_self());
	ODSocket::Init();
	while (!m_bQuit)
	{
		if (!m_bConnect)
		{
			InitNetwork();
		}
        
        char szReLeng[2] = {0};
        memset(szReLeng,0, sizeof(szReLeng));
        
		while (1)
		{
            int nr = m_odsSock.Recv(szReLeng, 2);
            CCLOG("nr=%d",nr);
            if (nr <= 0)
			{
				if (m_bQuit)
				{
					break;
				}
				Close();
#ifdef WIN32
				Sleep(1000);
#else
                sleep(1000);
#endif
				break;
			}
            if(nr > 0){
                LQSocketBuffer *b = LQSocketBuffer::create();
                b->writeBytes(szReLeng, nr);
                b->resetPosition();
                short length = b->readShort();
                
                //int sdd = 23;
                
                //char szRecv[length];// = {0};
                char* szRecv = new char[length];
                memset(szRecv, 0, sizeof(szRecv));
                
                //int nr = m_odsSock.Recv(szRecv, 2);
                //memset(szRecv, 0, sizeof(szRecv));
                int step = nr;
                while (step < length) {
                    nr = m_odsSock.Recv(szRecv, length-step);
                    if (nr <= 0)
					{
						Close();
						if (m_bQuit)
						{
							break;
						}
#ifdef WIN32
                        Sleep(1000);
#else
                        sleep(1000);
#endif
                        printf("Receive data but no data\n");
						break;
					}
                    
                    step+=nr;
                    //CCLOG(" nr=%d",nr);
                    //m_vec_recv.clear();
                    
                    std::copy(szRecv, szRecv + nr, back_inserter(m_vec_recv));
                    if(step < length)
                        CCLOG("step %d, length %d", step, length);
                    //copy(szRecv, szRecv + nr,  m_vec_recv);
                    //m_vec_recv.push_back(*szRecv);
                }
                
                //                std::vector<string> tttttvvvv;
                //                copy("11", &"22" [ nr], back_inserter(tttttvvvv));
                
                int nSend = step-2;//m_vec_recv.size();
                char *pszSend = new char[nSend];
                std::copy(m_vec_recv.begin(), m_vec_recv.end(), pszSend);
                m_vec_recv.clear();
                
                LQSocketBuffer *buf = LQSocketBuffer::create();
                buf->writeBytes(pszSend, nSend);
                
                buf->resetPosition();
                //                CCLOG("buf->readInt()=%d",buf->readInt());
                //                CCLOG("buf->readbyte()=%hd",buf->readByte());
                //                CCLOG("buf->readString()=%s",buf->readString().c_str());
                //                CCLOG("buf->readInt()=%d",buf->readInt());
                int msgcode = buf->readInt();
                MSG_RECVDATA msg = {msgcode, buf};
#if DEBUG
                CCLOG("----->接收数据完成 messageCode=%d length=%hd",msgcode,length);
#endif
                
                //                PUT_DATA data = {msgcode, buf};
                //                pthread_mutex_lock(&m_stack_recv);
                //                m_stack.push(data);
                //                pthread_mutex_unlock(&m_stack_recv);
                
                pthread_mutex_lock(&m_mutex_recv);
                m_vec_msgdata.push_back(msg);
                pthread_mutex_unlock(&m_mutex_recv);
                
                delete[] pszSend;
                
                break;
            }
		}
	}
	ODSocket::Clean();
}

int ComService::getReceiveCode()
{
    if (m_bQuit || !m_bConnect) return 0;
    
    int nRet = 0;
	pthread_mutex_lock(&m_mutex_recv);
    //    CCLOG("m_vec_msgdata.size() = %lu",m_vec_msgdata.size());
	if (m_vec_msgdata.size())
	{
        nRet = m_vec_msgdata[0].nCmd;
	}
	pthread_mutex_unlock(&m_mutex_recv);
	return nRet;
}

LQSocketBuffer* ComService::getReceiveData()
{
    //string psRet = "";
    LQSocketBuffer *buf = NULL;
    
    if (m_bQuit || !m_bConnect) return NULL;
    
	pthread_mutex_lock(&m_mutex_recv);
	if (m_vec_msgdata.size())
	{
        buf = (LQSocketBuffer*)m_vec_msgdata[0].buffer;
        m_vec_msgdata.erase(m_vec_msgdata.begin());
	}
	pthread_mutex_unlock(&m_mutex_recv);
	return buf;
}


void ComService::SendFunc()
{
	pthread_detach(pthread_self());
	ODSocket::Init();
	while (!m_bQuit)
	{
        ////////////////////
        //        struct node *p = NULL;
        ///////////////////
		char *pszSend = NULL;
		int nSend = 0;
        
		pthread_mutex_lock(&m_mutex_send);
		if (!m_vec_send.size())
		{
			pthread_cond_wait(&m_cond_send, &m_mutex_send);
		}
		nSend = m_vec_send.size();
		pszSend = new char[nSend];
		std::copy(m_vec_send.begin(), m_vec_send.end(), pszSend);
		m_vec_send.clear();
        
        ///////////////////
        //        p = head;
        //        head = head->m_next;
        ///////////////////
        
        
		pthread_mutex_unlock(&m_mutex_send);
        
        //        nSend = 6;
#if DEBUG
        
#endif
        int nRet = m_odsSock.Send(pszSend, nSend);
#if DEBUG
        CCLOG("----->发送完成 length:%d",nRet);
#endif
		//int nRet = m_odsSock.Send(pszSend, nSend);
		if (nRet <= 0)
		{
			if (m_bQuit)
			{
				delete[] pszSend;
                
                //                delete p->m_message;
                //                free(p);
                //
				break;
			}
#ifdef WIN32
            Sleep(1000);
#else
            sleep(1000);
#endif
			pthread_mutex_lock(&m_mutex_send);
			m_vec_send.insert(m_vec_send.begin(), pszSend, pszSend + nSend);
			pthread_mutex_unlock(&m_mutex_send);
		}
		delete[] pszSend;
        
        //        delete p->m_message;
        //        free(p);
	}
	ODSocket::Clean();
}

//bool ComService::DecodePDUHead(char *pszPack, PDU_HEAD_STRUCT &head)
//{
//    return true;
//}

int ComService::GetValue(char *pszSrc, const char *pszAtr)
{
	char *pszFind = strstr(pszSrc, pszAtr);
	if (!pszFind)
	{
		return 0;
	}
	pszFind += strlen(pszAtr);
	return atoi(pszFind);
    
}