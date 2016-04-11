#include "ODSocket.h"
#include <stdio.h>
#include <fcntl.h>

#ifdef WIN32
	#pragma comment(lib, "wsock32")
#else
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#endif

ODSocket::ODSocket(SOCKET sock)
{
	m_sock = sock;
}

ODSocket::~ODSocket()
{
}

int ODSocket::Init()
{
#ifdef WIN32
	/*
	http://msdn.microsoft.com/zh-cn/vstudio/ms741563(en-us,VS.85).aspx

	typedef struct WSAData { 
		WORD wVersion;								//winsock version
		WORD wHighVersion;							//The highest version of the Windows Sockets specification that the Ws2_32.dll can support
		char szDescription[WSADESCRIPTION_LEN+1]; 
		char szSystemStatus[WSASYSSTATUS_LEN+1]; 
		unsigned short iMaxSockets; 
		unsigned short iMaxUdpDg; 
		char FAR * lpVendorInfo; 
	}WSADATA, *LPWSADATA; 
	*/
	WSADATA wsaData;
	//#define MAKEWORD(a,b) ((WORD) (((BYTE) (a)) | ((WORD) ((BYTE) (b))) << 8)) 
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData);//win sock start up
	if ( ret ) {
//		cerr << "Initilize winsock error !" << endl;
		return -1;
	}
#endif
	
	return 0;
}
//this is just for windows
int ODSocket::Clean()
{
#ifdef WIN32
		return (WSACleanup());
#endif
		return 0;
}

ODSocket& ODSocket::operator = (SOCKET s)
{
	m_sock = s;
	return (*this);
}

ODSocket::operator SOCKET ()
{
	return m_sock;
}
//create a socket object win/lin is the same
// af:
bool ODSocket::Create(int af, int type, int protocol)
{
	m_sock = socket(af, type, protocol);
	if ( m_sock == INVALID_SOCKET ) {
		return false;
	}
    //在win上过不去
#ifdef WIN32
	//close(m_sock);
	//dup2(oldfd, newfd)；
#else
    fcntl(m_sock, F_SETFD, fcntl(m_sock, F_GETFD) | FD_CLOEXEC);
#endif
	return true;
}

bool ODSocket::Connect(const char* ip, unsigned short port)
{
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr(ip);
	svraddr.sin_port = htons(port);
	int ret = connect(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));
	if ( ret == SOCKET_ERROR ) {
		return false;
	}
	return true;
}

bool ODSocket::Bind(unsigned short port)
{
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = INADDR_ANY;
	svraddr.sin_port = htons(port);

	int opt =  1;
	if ( setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0 ) 
		return false;

	int ret = bind(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));
	if ( ret == SOCKET_ERROR ) {
		return false;
	}
	return true;
}
//for server
bool ODSocket::Listen(int backlog)
{
	int ret = listen(m_sock, backlog);
	if ( ret == SOCKET_ERROR ) {
		return false;
	}
	return true;
}

bool ODSocket::Accept(ODSocket& s, char* fromip)
{
	struct sockaddr_in cliaddr;
	socklen_t addrlen = sizeof(cliaddr);
	SOCKET sock = accept(m_sock, (struct sockaddr*)&cliaddr, &addrlen);
	if ( sock == SOCKET_ERROR ) {
		return false;
	}

	s = sock;
	if ( fromip != NULL )
		sprintf(fromip, "%s", inet_ntoa(cliaddr.sin_addr));

	return true;
}

int ODSocket::Send(const char* buf, int len, int flags)
{
	int bytes;
	int count = 0;

	while ( count < len ) {

		bytes = send(m_sock, buf + count, len - count, flags);
		if ( bytes == -1 || bytes == 0 )
			return -1;
		count += bytes;
	} 

	return count;
}

int ODSocket::Recv(char* buf, int len, int flags)
{
	return (recv(m_sock, buf, len, flags));
}

int ODSocket::Close()
{
#ifdef WIN32
	return (closesocket(m_sock));
#else
    /*
    int nRet = shutdown(m_sock, SHUT_RDWR);
    if (nRet == SOCKET_ERROR)
    {
        printf("Shutdown socket(%d) error: %d\n", m_sock, errno);
    }
     */
	return (close(m_sock));
#endif
}

int ODSocket::GetError()
{
#ifdef WIN32
	return (WSAGetLastError());
#else
	return (errno);
#endif
}


int ODSocket::Int32ToByte(int iIn,char* sOut,int& _index)
{
    int i = 3;
    while(i >=0)
    {
        sOut[i + _index] = iIn>>((3-i)*8) & 0xff;
        i--;
    }
    return (_index+=4);
}


int ODSocket::Int16ToByte(int iIn,char* sOut,int& _index)
{
    int i = 1;
    while (i>=0) {
        sOut[i+_index] = iIn>>((1-i)*8) & 0xff;
        i--;
    }
    return (_index+=2);
}


int ODSocket::Int8ToByte(int iIn,char* sOut,int& _index)
{
    sOut[_index] = iIn&0xff;
    return (++_index);
}


int ODSocket::StringToByte(char* sInfo,char* sOut,int& _index)
{
    long len = strlen(sInfo);
    memcpy(sOut+_index, sInfo, strlen(sInfo));
    return (_index+=len);
}


int32 ODSocket::getInt32(char* sIn,int& _index)
{
    int i = 3;
    int32 tempInt32 = 0;
    while(i>=0){
        tempInt32 |= (sIn[i+_index] & 0xff) << ((3-i)*8);
        i--;
    }
    _index+=4;
    return tempInt32;
}


int16 ODSocket::getInt16(char* sIn,int& _index)
{
    int i = 1;
    int32 tempInt16 = 0;
    while(i>=0){
        tempInt16 |= (sIn[i+_index] & 0xff) << ((1-i)*8);
        i--;
    }
    _index+=2;
    return tempInt16;
}


int8 ODSocket::getInt8(char* sIn,int& _index)
{
    int32 tempInt8 = 0;
    tempInt8 |= (sIn[_index] & 0xff);
    ++_index;
    return tempInt8;
}

//char* ODSocket::getCStr(char* )

bool ODSocket::DnsParse(const char* domain, char* ip)
{
	struct hostent* p;
	if ( (p = gethostbyname(domain)) == NULL )
		return false;
		
	sprintf(ip, 
		"%u.%u.%u.%u",
		(unsigned char)p->h_addr_list[0][0], 
		(unsigned char)p->h_addr_list[0][1], 
		(unsigned char)p->h_addr_list[0][2], 
		(unsigned char)p->h_addr_list[0][3]);
	
	return true;
}
