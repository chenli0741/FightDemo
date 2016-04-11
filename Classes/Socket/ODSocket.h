/*
* define file about portable socket class. 
* description:this sock is suit both windows and linux
* design:odison
* e-mail:odison@126.com>
* 
*/

#ifndef _ODSOCKET_H_
#define _ODSOCKET_H_


#if defined(_MSC_VER) || defined(__MINGW32__)
    #include <io.h>
    #include <WS2tcpip.h>
    #include <Winsock2.h>
    #define bzero(a, b) memset(a, 0, b);
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

#include <sys/types.h>
#include <string>
#include <fcntl.h>

#ifdef WIN32

typedef int socklen_t;
//tpedef int ssize_t;

#else

typedef unsigned int SOCKET;
typedef unsigned char BYTE;
typedef unsigned long DWORD;

#define FALSE 0

#define SOCKET_ERROR (-1)
#define INVALID_SOCKET  (-1)

#endif


/*#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1*/
//#pragma endregion

typedef signed char           int8;
typedef signed short          int16;
typedef signed int            int32;




class ODSocket {

public:
	ODSocket(SOCKET sock = INVALID_SOCKET);
	~ODSocket();

	// Create socket object for snd/recv data
	bool Create(int af, int type, int protocol = 0);

	// Connect socket
	bool Connect(const char* ip, unsigned short port);
	//#region server
	// Bind socket
	bool Bind(unsigned short port);

	// Listen socket
	bool Listen(int backlog = 5); 

	// Accept socket
	bool Accept(ODSocket& s, char* fromip = nullptr);
	//#endregion

	// Send socket
	int Send(const char* buf, int len, int flags = 0);

	// Recv socket
	int Recv(char* buf, int len, int flags = 0);

	// Close socket
	int Close();

	// Get errno
	int GetError();

	// write Int32ToByte
	int Int32ToByte(int iIn,char* sOut,int& _index);

	// write Int16ToByte
	int Int16ToByte(int iIn,char* sOut,int& _index);

	//write Int8ToByte
	int Int8ToByte(int iIn,char* sOut,int& _index);

	//write StringToByte
	int StringToByte(char* sInfo,char* sOut,int& _index);

	//read getInt32
	int32 getInt32(char* sIn,int& _index);

	//read getInt16
	int16 getInt16(char* sIn,int& _index);

	//read getInt8
	int8 getInt8(char* sIn,int& _index);



	//#pragma region just for win32
	// Init winsock DLL 
	static int Init();	
	// Clean winsock DLL
	static int Clean();
	//#pragma endregion

	// Domain parse
	static bool DnsParse(const char* domain, char* ip);

	ODSocket& operator = (SOCKET s);

	operator SOCKET ();

protected:
	SOCKET m_sock;

};

#endif
