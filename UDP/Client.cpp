#include <WinSock2.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib,"ws2_32.lib")

using namespace std;

int main()
{
	WORD wVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	char sendData[BUFSIZ] = "";
	char beginData[BUFSIZ] = "Begin\n";
	char overData[BUFSIZ] = "Over\n";
	char recBuf[BUFSIZ] = "";
	char okBuf[BUFSIZ] = "OK\n";

	if (WSAStartup(wVersion, &wsaData) != 0)
	{
		cout << "WSAStsrtup failed with error: " << WSAGetLastError() << endl;
		return 0;
	}

	sockaddr_in addrSer;
	addrSer.sin_family = AF_INET;
	addrSer.sin_port = htons(4999);
	addrSer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int length = sizeof(addrSer);

	while (true)
	{
		SOCKET sClient = socket(AF_INET, SOCK_DGRAM, 0);
		recvfrom(sClient, recBuf, BUFSIZ, 0, (sockaddr*)&addrSer, &length);
		cout << recBuf;
		FILE* fp;
		char filename[20] = { 0 };
		cout << "please input the filename: ";
		cin >> filename;
		fp = fopen(filename, "rb");
		if (NULL == fp)
		{
			cout<<"file cannot find!"<<endl;
			continue;
		}
		sendto(sClient, beginData, BUFSIZ, 0, (sockaddr*)&addrSer, sizeof(addrSer));
		int count = 0;
		cout << "transfering: ";
		while ((count = fread(sendData, 1, BUFSIZ, fp)) > 0)
		{
			//Sleep(1);
			//cout << "#";
			sendto(sClient, sendData, BUFSIZ, 0, (sockaddr*)&addrSer, sizeof(addrSer));


		}
		sendto(sClient, overData, BUFSIZ, 0, (sockaddr*)&addrSer, sizeof(addrSer));
		recvfrom(sClient, recBuf, BUFSIZ, 0, (sockaddr*)&addrSer, &length);
		if (strcmp(recBuf, okBuf) == 0)
		{
			cout << "transfer successful!" << endl;
		}
		closesocket(sClient);
	}

	WSACleanup();
	return 0;
	
}