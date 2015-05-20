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

#include "netrender.hpp"
#include <stdio.h>
#ifndef WIN32
#include <arpa/inet.h>
#endif

CNetRender *netRender;

//---------------- Constructor ---------------------
CNetRender::CNetRender(int myVersion, int CPUs)
{
  memset(&host_info, 0, sizeof host_info);
  host_info_list = NULL;
  status = 0;
  isServer = false;
  isClient = false;
  socketfd = 0;
  clientIndex = 0;
  version = myVersion;
  dataBuffer = NULL;
  dataSize = 0;
  noOfCPUs = CPUs;
  lastIdentifier = 0;
#ifdef WIN32  
  WORD wVersionRequested;
  WSADATA wsaData;
  int err;

/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        printf("WSAStartup failed with error: %d\n", err);
    }
#endif
}

//------------------ Destructor ---------------------
CNetRender::~CNetRender()
{
	if (dataBuffer) delete [] dataBuffer;
}
//------------------- Set Server ---------------------
bool CNetRender::SetServer(char *portNo, char *statusOut)
{
  memset(&host_info, 0, sizeof host_info);

  host_info.ai_family = AF_INET;     // IP version not specified. Can be both.
  host_info.ai_socktype = SOCK_STREAM; // Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.
  host_info.ai_flags = AI_PASSIVE;     // IP Wildcard

  status = getaddrinfo(NULL, portNo, &host_info, &host_info_list);
  if (status != 0)  std::cout << "getaddrinfo error" << gai_strerror(status) ;

  std::cout << "Creating a socket..."  << std::endl;
  socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
  if (socketfd == -1)  std::cout << "socket error " << strerror(errno);

  std::cout << "Binding socket..."  << std::endl;
  // we use to make the setsockopt() function to make sure the port is not in use
  // by a previous execution of our code. (see man page for more information)
  char yes = 1;
  status = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

#ifndef WIN32
  struct timeval timeout;
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;
#else
  DWORD timeout = 1000;
#endif

  if (setsockopt (socketfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
              sizeof(timeout)) < 0)
  	std::cout << "socket options error " << strerror(errno);

  if (setsockopt (socketfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
              sizeof(timeout)) < 0)
  	std::cout << "socket options error " << strerror(errno);

  status = bind(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
  if (status == -1)
  {
  	std::cout << "bind error" << std::endl ;
  	return false;
  }
  else
  {
  	strcpy(statusOut,"status: server enabled");
  	printf("Server enabled\n");
  	isServer = true;
  	return true;
  }
}

//------------------- Delete Server ----------------------
void CNetRender::DeleteServer(void)
{
	if(host_info_list) freeaddrinfo(host_info_list);
	if(socketfd > 0) close(socketfd);
	if(clients.size() > 0)
	{
		for(unsigned int i=0; i<clients.size(); i++)
		{
			close(clients[0].socketfd);
		}
		clients.clear();
	}
	isServer = false;
}

//-------------------- Set client ---------------------------
bool CNetRender::SetClient(char *portNo, char*name, char *statusOut)
{
	memset(&host_info, 0, sizeof host_info);

	host_info.ai_family = AF_INET;     // IP version not specified. Can be both.
	host_info.ai_socktype = SOCK_STREAM; // Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.

	status = getaddrinfo(name, portNo, &host_info, &host_info_list);
	if (status != 0)  std::cout << "getaddrinfo error" << gai_strerror(status) ;

	std::cout << "Creating a socket..."  << std::endl;

	socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype,
	host_info_list->ai_protocol);
	if (socketfd == -1)  std::cout << "socket error " << strerror(errno);

#ifndef WIN32
  struct timeval timeout;
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;
#else
  DWORD timeout = 1000;
#endif

  if (setsockopt (socketfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
              sizeof(timeout)) < 0)
  	std::cout << "socket options error " << strerror(errno);

  if (setsockopt (socketfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
              sizeof(timeout)) < 0)
  	std::cout << "socket options error " << strerror(errno);

	//connecting to server
  std::cout << "Connect()ing..."  << std::endl;
  status = connect(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
  if (status == -1)
  {
  	std::cout << "connect error" << strerror(errno);
  	strcpy(statusOut, strerror(errno));
  	return false;
  }
  else
  {
  	strcpy(statusOut,"status: client connected to server");
  	printf("Client connected to server\n");

  	//sending version number

		char command[4];
		size_t recvd_bytes = receiveDataFromServer(command);
		if(!strcmp("ver", command))
		{
	  	size_t len = sizeof(version);
	  	sendDataToServer(&version, len, (char*)"VER");
		}

  	//checking aswer regarding version
		recvd_bytes = receiveDataFromServer(command);
		if(!strcmp("ok.", command))
		{
    	printf("Client version approved\n");
  		isClient = true;

  		recvd_bytes = receiveDataFromServer(command);
  		if(!strcmp("cpu", command))
  		{
  			sendDataToServer(&noOfCPUs, sizeof(noOfCPUs), (char*)"CPU");
  		}

    	return true;
		}
		else if(!strcmp("bad", command))
		{
    	int serverVersion = 0;
    	if(recvd_bytes == sizeof(int)) GetData(&serverVersion);
			printf("Client version refused. Server version is %f\n", serverVersion/1000.0);
  		isClient = false;
  		sprintf(statusOut,"status: client has wrong version. Server is %f", serverVersion/1000.0);
  		printf("Client disconnected from server because client version is wrong\n");
    	return false;
		}
		else
		{
			sprintf(statusOut,"status: communication error during version verification");
			printf("communication error during version verification");
			return false;
		}
  }
}

//---------------- Delete Client ------------------------

void CNetRender::DeleteClient(void)
{
	if(host_info_list) freeaddrinfo(host_info_list);
	if(socketfd > 0) close(socketfd);
}

//------------------ Wait for client -------------------
bool CNetRender::WaitForClient(char *statusOut)
{
	//std::cout << "Listen()ing for connections..." << std::endl;
	status = listen(socketfd, 5);
	if (status == -1)
	{
		//std::cout << "listen error" << std::endl;
		strcpy(statusOut,"status: listen error");
		return false;
	}

	//accepting donnection
	struct sockaddr_storage their_addr;
	socklen_t addr_size = sizeof(their_addr);
	sClients newClient;
	memset(&newClient, 0, sizeof(newClient));

	fd_set set;
	struct timeval timeout;
	int rv;
	FD_ZERO(&set); // clear the set
	FD_SET(socketfd, &set); // add file descriptor to the set

	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	rv = select(socketfd + 1, &set, NULL, NULL, &timeout);
	if (rv == -1)
	{
		perror("select"); // an error accured
		strcpy(statusOut,"status: socket error");
		return false;
	}
	else if (rv == 0)
	{
		//printf("timeout occurred (1 second) \n"); // a timeout occured
		newClient.socketfd  = -1;
	}
	else newClient.socketfd = accept(socketfd, (struct sockaddr *) &their_addr, &addr_size);

	if (newClient.socketfd  == -1)
	{
		//std::cout << "listen error" << std::endl;
		strcpy(statusOut,"status: client not found");
		return false;
	}
	else
	{
		std::cout << "Connection accepted. Using new socketfd : " << newClient.socketfd << std::endl;

#ifndef WIN32
		//getting IP address
		socklen_t len;
		struct sockaddr_storage addr;
		char ipstr[INET6_ADDRSTRLEN];
		int port;

		len = sizeof addr;
		getpeername(newClient.socketfd, (struct sockaddr*)&addr, &len);

		// deal with both IPv4 and IPv6:
		if (addr.ss_family == AF_INET) {
		    struct sockaddr_in *saddr = (struct sockaddr_in *)&addr;
		    port = ntohs(saddr->sin_port);
		    inet_ntop(AF_INET, &saddr->sin_addr, ipstr, sizeof ipstr);
		} else { // AF_INET6
		    struct sockaddr_in6 *saddr = (struct sockaddr_in6 *)&addr;
		    port = ntohs(saddr->sin6_port);
		    inet_ntop(AF_INET6, &saddr->sin6_addr, ipstr, sizeof ipstr);
		}

		strcpy(newClient.ipstr, ipstr);
		newClient.port = port;
#else
		strcpy(newClient.ipstr, "IP unknown");
#endif
		

		//remembering new client
		clients.push_back(newClient);
		clientIndex = clients.size();

		//preparing status
		char stat[1000];
#ifndef WIN32
		sprintf(stat,"status: Client #%d has IP address: %s, port %d\n", clientIndex, ipstr, port);
#else
		sprintf(stat,"status: Client #%d connected\n", clientIndex);
#endif
		strcpy(statusOut, stat);
		std::cout << stat;

		//checking client version
		int clientVersion = 0;
		sendDataToClient(NULL, 0, (char*)"ver", clientIndex-1, 0);
		char command[4];
		size_t recvd_bytes = receiveDataFromClient(command, clientIndex-1, 0);
		if(!strcmp("VER", command) && recvd_bytes == sizeof(int))
		{
			GetData(&clientVersion);
		}

		if(clientVersion == version)
		{
			printf("Client version is correct\n");

			//sending answer
			sendDataToClient(NULL, 0, (char*)"ok.", clientIndex-1, 0);

			//ask for number of CPUs
			sendDataToClient(NULL, 0, (char*)"cpu", clientIndex-1, 0);
			recvd_bytes = receiveDataFromClient(command, clientIndex-1, 0);
			if(!strcmp("CPU", command) && recvd_bytes == sizeof(int))
			{
				int noOfCpu;
				GetData(&noOfCpu);
				clients[clientIndex-1].noOfCPU = noOfCpu;
				printf("Client #%d has %d CPUs\n", clientIndex, noOfCpu);
			}

			return true;
		}
		else
		{
			printf("Wrong client version\n");

			//sending answer
			sendDataToClient(&version, sizeof(version), (char*)"bad", clientIndex-1, 0);

			//deleting client
			clients.erase(clients.end()-1);
			return false;
		}
	}
}

//--------------- Send data to client ----------------------
bool CNetRender::sendDataToClient(void *data, size_t size, char *command, int index, int32_t identifier)
{
	//printf("Sending %d bytes data with command %s...\n", size, command);
	uint64_t size64 = size;
	send(clients[index].socketfd, command, 4, 0);

#ifdef WIN32
	send(clients[index].socketfd, (const char*)&identifier, sizeof(identifier), 0);
	send(clients[index].socketfd, (const char*)&size64, sizeof(size64), 0);
#else
	send(clients[index].socketfd, &identifier, sizeof(identifier), 0);
	send(clients[index].socketfd, &size64, sizeof(size64), 0);
#endif

	sHeader header;
	memset(&header,0,sizeof(header));
	memcpy(header.command, command, sizeof(char[4]));
	header.size64 = size64;
	header.identifier = identifier;
	uint16_t crc = CRC_Fletcher16((uint8_t*)&header, sizeof(header));
	
#ifdef WIN32
	send(clients[index].socketfd, (const char*)&crc, sizeof(crc), 0);
#else
	send(clients[index].socketfd, &crc, sizeof(crc), 0);
#endif

	size_t send_left = size;
	char *dataPointer = (char*)data;

	while(send_left > 0)
	{
		ssize_t bytes_send = send(clients[index].socketfd, dataPointer, send_left, 0);
		if (bytes_send == -1) return false;
		send_left -= bytes_send;
		dataPointer += bytes_send;
		//printf("Sent %d bytes\n", bytes_send);
	}

	if(size > 0)
	{
		uint16_t crcData = CRC_Fletcher16((uint8_t*)data, size);

#ifdef WIN32
		send(clients[index].socketfd, (const char*)&crcData, sizeof(crcData), 0);
#else
		send(clients[index].socketfd, &crcData, sizeof(crcData), 0);
#endif
	}

	return true;
}

//-------------- Send data to server --------------------
bool CNetRender::sendDataToServer(void *data, size_t size, char *command)
{
	//printf("Sending %d bytes data with command %s...\n", size, command);
	uint64_t size64 = size;
	send(socketfd, command, 4, 0);
	
	int32_t identifier = lastIdentifier;

#ifdef WIN32
	send(socketfd, (const char*)&identifier, sizeof(identifier), 0);
	send(socketfd, (const char*)&size64, sizeof(size64), 0);
#else
	send(socketfd, &identifier, sizeof(identifier), 0);
	send(socketfd, &size64, sizeof(size64), 0);
#endif

	sHeader header;
	memset(&header,0,sizeof(header));
	memcpy(header.command, command, sizeof(char[4]));
	header.size64 = size64;
	header.identifier = identifier;
	uint16_t crc = CRC_Fletcher16((uint8_t*)&header, sizeof(header));
	
#ifdef WIN32	
	send(socketfd, (const char*)&crc, sizeof(crc), 0);
#else
	send(socketfd, &crc, sizeof(crc), 0);
#endif

	size_t send_left = size;
	char *dataPointer = (char*)data;

	while(send_left > 0)
	{
		ssize_t bytes_send = send(socketfd, dataPointer, send_left, 0);
		if (bytes_send == -1) return false;
		send_left -= bytes_send;
		dataPointer += bytes_send;
		//printf("Sent %d bytes\n", bytes_send);
	}

	if(size > 0)
	{
		uint16_t crcData = CRC_Fletcher16((uint8_t*)data, size);

#ifdef WIN32	
		send(socketfd, (const char*)&crcData, sizeof(crcData), 0);
#else	
		send(socketfd, &crcData, sizeof(crcData), 0);
#endif
	}

	return true;
}

//------------------- Receive data from server -----------------------
size_t CNetRender::receiveDataFromServer(char *command)
{
	dataSize = 0;

	//printf("Waiting for data...\n");
	memset(command,0,4);
	ssize_t bytes_recvd = recv(socketfd, command, 4, 0);

	if (bytes_recvd <= 0)
	{
		if(errno != 11)
		{
			std::cout << errno << strerror(errno) << endl;
		}
		return 0;
	}

	//printf("Received command: %s\n", command);

	uint64_t size64 = 0;
	int32_t identifier;

#ifdef WIN32
	recv(socketfd, (char*)&identifier, sizeof(identifier), 0);
	recv(socketfd, (char*)&size64, sizeof(size64), 0);
#else
	recv(socketfd, &identifier, sizeof(identifier), 0);
	recv(socketfd, &size64, sizeof(size64), 0);
#endif

	sHeader header;
	memset(&header,0,sizeof(header));
	memcpy(header.command, command, sizeof(char[4]));
	header.size64 = size64;
	header.identifier = identifier;

	uint16_t crc = CRC_Fletcher16((uint8_t*)&header, sizeof(header));
	uint16_t crc2 = 0;
	
#ifdef WIN32
	recv(socketfd, (char*)&crc2, sizeof(crc2), 0);
#else
	recv(socketfd, &crc2, sizeof(crc2), 0);
#endif

	if(crc != crc2)
	{
		printf("Data header crc error\n");
		char scrapBuffer[1000];
		do
		{
			bytes_recvd = recv(socketfd, scrapBuffer, 1000, 0);
		}
		while(bytes_recvd > 0);
		return 0;
	}

	lastIdentifier = identifier;
	//printf("Will be received %d bytes\n", size);

	if (size64 > 0)
	{
		if(dataBuffer) delete [] dataBuffer;
		dataBuffer = new char[size64];

		char *dataPointer = dataBuffer;
		size_t rcv_left = size64;

		while(rcv_left > 0)
		{
			bytes_recvd = recv(socketfd, dataPointer, rcv_left, 0);
			//printf("%d bytes received\n", bytes_recvd);
			if(bytes_recvd == -1)
			{
				printf("Data receive error\n");
				return 0;
			}

			rcv_left -= bytes_recvd;
			dataPointer += bytes_recvd;
		}

		uint16_t crcData = CRC_Fletcher16((uint8_t*)dataBuffer, size64);
		uint16_t crcData2 = 0;
		
#ifdef WIN32
		recv(socketfd, (char*)&crcData2, sizeof(crcData2), 0);
#else		
		recv(socketfd, &crcData2, sizeof(crcData2), 0);
#endif

		if(crcData != crcData2)
		{
			printf("Received data crc error\n");
			char scrapBuffer[1000];
			do
			{
				bytes_recvd = recv(socketfd, scrapBuffer, 1000, 0);
			}
			while(bytes_recvd > 0);
			return 0;
		}

		dataSize = size64;
	}
	return size64;
}

//--------------- receive data from client --------------------

size_t CNetRender::receiveDataFromClient(char *command, int index, int32_t reqIdentifier)
{
	//printf("Waiting for data...\n");
	memset(command,0,4);
	ssize_t bytes_recvd = recv(clients[index].socketfd, command, 4, 0);

	if (bytes_recvd <= 0)
	{
		if(errno != 11)
		{
			std::cout << errno << strerror(errno) << endl;
		}
		return 0;
	}

	//printf("Received command: %s\n", command);

	uint64_t size64 = 0;
	int32_t identifier;
	
#ifdef WIN32
	recv(clients[index].socketfd, (char*)&identifier, sizeof(identifier), 0);
	recv(clients[index].socketfd, (char*)&size64, sizeof(size64), 0);
#else
	recv(clients[index].socketfd, &identifier, sizeof(identifier), 0);
	recv(clients[index].socketfd, &size64, sizeof(size64), 0);
#endif

	//printf("Will be received %d bytes\n", size);

	sHeader header;
	memset(&header,0,sizeof(header));
	memcpy(header.command, command, sizeof(char[4]));
	header.size64 = size64;
	header.identifier = identifier;
	uint16_t crc = CRC_Fletcher16((uint8_t*)&header, sizeof(header));
	uint16_t crc2 = 0;
	
#ifdef WIN32
	recv(clients[index].socketfd, (char*)&crc2, sizeof(crc2), 0);
#else
	recv(clients[index].socketfd, &crc2, sizeof(crc2), 0);
#endif

	if(crc != crc2)
	{
		printf("Data header crc error\n");
		char scrapBuffer[1000];
		do
		{
			bytes_recvd = recv(clients[index].socketfd, scrapBuffer, 1000, 0);
		}
		while(bytes_recvd > 0);
		return 0;
	}

	if(identifier != reqIdentifier)
	{
		printf("Data identifier error (lost synchronization)\n");
		char scrapBuffer[1000];
		do
		{
			bytes_recvd = recv(clients[index].socketfd, scrapBuffer, 1000, 0);
		}
		while (bytes_recvd > 0);
		return 0;
	}

	if (size64 > 0)
	{
		if(dataBuffer) delete [] dataBuffer;
		dataBuffer = new char[size64];

		char *dataPointer = dataBuffer;
		size_t rcv_left = size64;

		while(rcv_left > 0)
		{
			bytes_recvd = recv(clients[index].socketfd, dataPointer, rcv_left, 0);
			//printf("%d bytes received\n", bytes_recvd);
			if(bytes_recvd == -1)
			{
				printf("Data receive error\n");
				return 0;
			}

			rcv_left -= bytes_recvd;
			dataPointer += bytes_recvd;
		}

		uint16_t crcData = CRC_Fletcher16((uint8_t*)dataBuffer, size64);
		uint16_t crcData2 = 0;
		
#ifdef WIN32
		recv(clients[index].socketfd, (char*)&crcData2, sizeof(crcData2), 0);
#else
		recv(clients[index].socketfd, &crcData2, sizeof(crcData2), 0);
#endif

		if(crcData != crcData2)
		{
			printf("Received data crc error\n");
			char scrapBuffer[1000];
			do
			{
				bytes_recvd = recv(socketfd, scrapBuffer, 1000, 0);
			}
			while(bytes_recvd > 0);
			return 0;
		}

		dataSize = size64;
	}
	return size64;
}

//--------------- Get data -----------------

void CNetRender::GetData(void *data)
{
	memcpy(data, dataBuffer, dataSize);
}

//------------------------ Fletchers checsum -----------------
//source: http://en.wikipedia.org/wiki/Fletcher's 16-bit checksum
uint16_t CNetRender::CRC_Fletcher16(uint8_t const *data, size_t bytes)
{
	uint16_t sum1 = 0xff, sum2 = 0xff;

	while (bytes)
	{
		size_t tlen = bytes > 20 ? 20 : bytes;
		bytes -= tlen;
		do
		{
			sum2 += sum1 += *data++;
		} while (--tlen);
		sum1 = (sum1 & 0xff) + (sum1 >> 8);
		sum2 = (sum2 & 0xff) + (sum2 >> 8);
	}
	/* Second reduction step to reduce sums to 8 bits */
	sum1 = (sum1 & 0xff) + (sum1 >> 8);
	sum2 = (sum2 & 0xff) + (sum2 >> 8);
	return sum2 << 8 | sum1;
}
