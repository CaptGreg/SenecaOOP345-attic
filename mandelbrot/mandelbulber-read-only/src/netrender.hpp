/*********************************************************
 /                   MANDELBULBER
 / class for rendering with several network clients
 /
 /
 / author: Krzysztof Marczak
 / contact: buddhi1980@gmail.com
 / licence: GNU GPL v3.0
 /
 ********************************************************/

#ifndef NETRENDER_HPP_
#define NETRENDER_HPP_

#include <iostream>
#include <cstring>
#ifdef WIN32
	#undef WINVER
	#define WINVER WindowsXP
	#define _WIN32_WINNT 0xFFFF
	//#include <windows.h>
	#include <ws2tcpip.h>
	#include <winsock2.h>
	#define INET6_ADDRSTRLEN 46
#else
	#include <sys/socket.h>
	#include <netdb.h>
#endif

#include <errno.h>
#include <unistd.h>
#include <vector>

using namespace std;


class CNetRender
{
public:
	CNetRender(int myVersion, int CPUs);
	~CNetRender();
	bool SetServer(char *portNo, char *statusOut);
	void DeleteServer(void);
	bool SetClient(char *portNo, char*name, char *statusOut);
	void DeleteClient(void);
	bool WaitForClient(char *statusOut);
	bool IsServer() {return isServer;}
	bool IsClient() {return isClient;}
	int getNoOfClients() {return clientIndex;};
	int getCpuCount(int index) {return clients[index].noOfCPU;};
	bool sendDataToClient(void *data, size_t size, char *command, int index, int32_t identifier);
	bool sendDataToServer(void *data, size_t size, char *command);
	size_t receiveDataFromServer(char *command);
	size_t receiveDataFromClient(char *command, int index, int32_t reqIdentifier);
	void GetData(void *data);

	struct sClients
	{
		int socketfd;
		char ipstr[INET6_ADDRSTRLEN];
		int port;
		int noOfCPU;
	};

	struct sHeader
	{
		char command[4];
		int32_t identifier;
		uint64_t size64;
	};

private:
	uint16_t CRC_Fletcher16( uint8_t const *data, size_t bytes);
  int status;
  struct addrinfo host_info;       // The struct that getaddrinfo() fills up with data.
  struct addrinfo *host_info_list; // Pointer to the to the linked list of host_info's.
  bool isServer;
  bool isClient;
  int socketfd ; // The socket descripter
  int clientIndex;
  vector<sClients> clients;
  int version;
  char *dataBuffer;
  size_t dataSize;
  int noOfCPUs;
  int32_t lastIdentifier;
};



extern CNetRender *netRender;

#endif /* NETRENDER_HPP_ */
