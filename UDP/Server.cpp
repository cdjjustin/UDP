#pragma comment(lib,"Ws2_32.lib")

#include <WinSock2.h>
#include <stdio.h>
#include <iostream>

using namespace std;

int main()
{
	WORD wVersion = MAKEWORD(2, 2);
	WSADATA wsaData;

	if (WSAStartup(wVersion, &wsaData)!=0)
	{
		cout << "WSAStartup failed with error " << WSAGetLastError() << endl;
		return 0;
	}

	SOCKET sServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sockaddr_in addrSer;
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons(4999);
	addrSer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	bind(sServer, (sockaddr*)&addrSer, sizeof(addrSer));

	sockaddr_in addrClient;
	int lenght = sizeof(addrClient);
	char recvBuf[BUFSIZ] = { 0 };
	int rev = 0;
	int i = 0;

	while (true)
	{
		char sendData[BUFSIZ] = "hello!\n";
		char beginData[BUFSIZ] = "Begin\n";
		char overData[BUFSIZ] = "Over\n";
		char okBuf[BUFSIZ] = "OK\n";
		char filename[BUFSIZ]= "d://";
		sprintf(filename, "d://%d", i++);

		FILE* fp;
		sendto(sServer, sendData, BUFSIZ, 0, (sockaddr*)&addrClient, lenght);
		recvfrom(sServer, recvBuf, BUFSIZ, 0, (sockaddr*)&addrClient, &lenght);
		if (strcmp(recvBuf, beginData) == 0)
		{
			cout << "ready to receive file: ";
			fp = fopen(filename, "wb");
			if (NULL == fp)
			{
				cout << "cannot write file!" << endl;
				continue;
			}
		}

		while ((rev = recvfrom(sServer, recvBuf, BUFSIZ, 0, (sockaddr*)&addrClient, &lenght)) > 0)
		{
			//cout << "#";
			if (strcmp(overData, recvBuf) == 0)
			{
				cout << " recieve successful!" << endl;
				fclose(fp);
				sendto(sServer, okBuf, BUFSIZ, 0, (sockaddr*)&addrClient, lenght);
				break;
			}
			if (fwrite(recvBuf, 1, rev, fp) < 0)
			{
				cout << "write failed!" << endl;
			}
		}

		if (rev < 0 || strcmp(overData, recvBuf) != 0)
		{
			cout << "transfer failed!";
			fclose(fp);
			remove(filename);
		}


	}
	closesocket(sServer);
	WSACleanup();
	return 0;
}