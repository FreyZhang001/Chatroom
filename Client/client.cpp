#include "winsock2.h"
#include "stdio.h"

#define SERVER_IP	"10.80.167.248"
#define SERVER_PORP	8884

//blog:zfblog.xyz
//author:���շ�

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
	//�����׽��ֿ�
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 1, 1 ); //�汾��Ϊ1.1
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		return;
	}
	if ( LOBYTE( wsaData.wVersion ) != 1 ||
	HIBYTE( wsaData.wVersion ) != 1 ) {
		WSACleanup( );
		return;
	}
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0); //SOCK_STREAM��������ΪTCP����
	SOCKADDR_IN addrServer; //��������ַ�ṹ
	addrServer.sin_addr.S_un.S_addr=inet_addr(SERVER_IP); //��������ַ
	addrServer.sin_port=htons(SERVER_PORP); //�������˿ں�
	addrServer.sin_family=AF_INET;
	//��������˽������ӣ�����ͨ��
	char name[100];
	printf("����������:");
	scanf("%s",name);
	int connReult=connect(sockClient,(SOCKADDR*)&addrServer,sizeof(SOCKADDR));
	if(connReult!=WSAEADDRNOTAVAIL) //���ʳɹ�
	{
		CreateThread(NULL, 0, ThreadFun, (void *)&sockClient, 0, NULL);	
		printf("���ӳɹ�\n");
		//�ɹ��������Ӻ���������˷������ݣ��������ʾ�ڷ���������
		char sendInfo[100];
		sprintf(sendInfo,name);
		send(sockClient,sendInfo,strlen(sendInfo)+1,0);
		//�������Է������˷���������Ϣ
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
