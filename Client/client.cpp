#include "winsock2.h"
#include "stdio.h"

#define SERVER_IP	"10.80.167.248"
#define SERVER_PORP	8884

//blog:zfblog.xyz
//author:昔日风

DWORD WINAPI ThreadFun(LPVOID pM)  
{	
	SOCKET sockClient=*(SOCKET *)pM;
	char recvInfo[100];
	while(1)
	{
		if(recv(sockClient,recvInfo,100,0)>0)
		{
			printf("%s",recvInfo);
		}
	}
	return 0;
}

void main()
{
	//加载套接字库
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 1, 1 ); //版本好为1.1
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return;
	}
	if ( LOBYTE( wsaData.wVersion ) != 1 ||
	HIBYTE( wsaData.wVersion ) != 1 ) {
		WSACleanup( );
		return;
	}
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0); //SOCK_STREAM参数设置为TCP连接
	SOCKADDR_IN addrServer; //服务器地址结构
	addrServer.sin_addr.S_un.S_addr=inet_addr(SERVER_IP); //服务器地址
	addrServer.sin_port=htons(SERVER_PORP); //服务器端口号
	addrServer.sin_family=AF_INET;
	//与服务器端建立连接，进行通信
	char name[100];
	printf("请输入姓名:");
	scanf("%s",name);
	int connReult=connect(sockClient,(SOCKADDR*)&addrServer,sizeof(SOCKADDR));
	if(connReult!=WSAEADDRNOTAVAIL) //访问成功
	{
		CreateThread(NULL, 0, ThreadFun, (void *)&sockClient, 0, NULL);	
		printf("连接成功\n");
		//成功建立连接后向服务器端发送数据，结果将显示在服务器端上
		char sendInfo[100];
		sprintf(sendInfo,name);
		send(sockClient,sendInfo,strlen(sendInfo)+1,0);
		//接收来自服务器端发送来的信息
		//char recvInfo[100];
		//recv(sockClient,recvInfo,100,0);
		//printf("%s\n",recvInfo);
		while(1)
		{
			scanf("%s",sendInfo);
			send(sockClient,sendInfo,strlen(sendInfo)+1,0);
		}
	}
	else
	{
		int errCode=WSAGetLastError();
		printf("the errcode is:%d\n",errCode);
	}
	closesocket(sockClient);
	WSACleanup();
}
