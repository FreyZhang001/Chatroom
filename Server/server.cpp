#include "winsock2.h"
#include "stdio.h"
#include <windows.h>

//blog:zfblog.xyz
//author:���շ�

#define SERVER_PORP	8884

struct client_info{
	SOCKET sockConn;
	char name[100];
	client_info *next;
};//���ÿһ���û�����Ϣ

client_info * C_info_head;


DWORD WINAPI Threadmes(LPVOID pM)
{
	char sendInfo[100];
	sprintf(sendInfo,(char *)pM);
	for(int i=0;i<=c_info_num;i++){
		SOCKET sockConn=C_info[i].sockConn;
		//if(C_info[i].num!=-1){
		send(sockConn,sendInfo,strlen(sendInfo)+1,0);
		//}
	}
	return 0;
}

//��ĳ����ϢȺ�������пͻ���
void sendmessage(char *message)
{
	CreateThread(NULL, 0, Threadmes, (void *)message, 0, NULL);
}
//����ÿ���û�����Ϣ
DWORD WINAPI ThreadFun(LPVOID pM)  
{	
	client_info c_info = *(client_info *)pM;
	SOCKET sockConn=c_info.sockConn;

    char sendInfo[100];
	//inet_ntoa���ṹת��Ϊʮ���Ƶ�IP��ַ�ַ���
	//sprintf(sendInfo,"welcome %s to this Server!",inet_ntoa(addrClient.sin_addr));
	//�ɹ��������Ӻ���ͻ��˷������ݣ��������ʾ�ڿͻ�����
	//send(sockConn,sendInfo,strlen(sendInfo)+1,0);
	//�ӿͻ��˽������ݣ������ʾ�ڷ�������
	char recvInfo[100];
	recv(sockConn,recvInfo,100,0);
	sprintf(c_info.name,recvInfo);
	
	printf("��ӭ%s����������\n",recvInfo);
	sprintf(sendInfo,"��ӭ%s����������\n",recvInfo);
	sendmessage(sendInfo);

	
	while(1)
	{	
		if(recv(sockConn,recvInfo,100,0)<0)
			break;
		printf("[%s]:%s\n",c_info.name,recvInfo);
		sprintf(sendInfo,"[%s]:%s\n",c_info.name,recvInfo);
		sendmessage(sendInfo);
	}
	//�����ν��������еõ��׽��ֹر�
	closesocket(sockConn);
	return 0;
}


void main()
{
	//�����׽��֣�winsock���⣬������δ��뿽����MSDN��WSAStartup�Ľ���
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
	//��Ϣ�洢������
	

	//----------------------------------------------------
	//�����׽���
	SOCKET sockServer=socket(AF_INET,SOCK_STREAM,0); //SOCK_STREAM��������ΪTCP����
	SOCKADDR_IN addrServer; //���÷��������׽��ֵ��������
	addrServer.sin_addr.S_un.S_addr=htonl(INADDR_ANY); //����IP
	addrServer.sin_family=AF_INET;
	addrServer.sin_port=htons(SERVER_PORP); //���ö˿ں�
	//���׽��ְ󶨵����ص�ַ��ָ���˿���
	bind(sockServer,(SOCKADDR*)&addrServer,sizeof(SOCKADDR));
	//���׽�������Ϊ����ģʽ����������������������ó�5����������������ȫ������
	listen(sockServer,5);
	SOCKADDR_IN addrClient; //�������տͻ��˵����ã�����IP�Ͷ˿�
	int len=sizeof(SOCKADDR);
	
	while(1) //���ϼ���
	{
		//�õ��������Ӻ��һ���µ��׽��֣������Ϳͻ��˽��й�ͨ��ԭ�׽��ּ��������ͻ�����������
		SOCKET sockConn=accept(sockServer,(SOCKADDR*)&addrClient,&len);
		if(sockConn!=INVALID_SOCKET) //�����ɹ�
		{	
			c_info_num++;
			C_info[c_info_num].num=c_info_num;
			C_info[c_info_num].sockConn=sockConn;
			CreateThread(NULL, 0, ThreadFun, (void *)&C_info[c_info_num], 0, NULL);	
		}
		else
		{
			int errCode=WSAGetLastError();
			printf("the errcode is:%d\n",errCode);
		}
	}
	//�������������ѭ������ô�ڴ˴���Ӧ������´��룺
	closesocket(sockServer); //��һֱ���ڼ���״̬���׽��ֽ��йر�
	WSACleanup(); //��ֹ��winsocket���ʹ��
}
