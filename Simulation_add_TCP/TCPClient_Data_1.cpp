//#pragma comment( lib, "ws2_32" )
//
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#include <WinSock2.h>
//#include <Windows.h>
//#include <stdlib.h>
//#include <stdio.h>
//
//
//#define SERVERIP "127.0.0.1"
//#define SERVERPORT 9000
//#define BUFSIZE 50
//
//
////���� �Լ� ���� ��� �� ����
//void err_quit(const char* msg)
//{
//	LPVOID lpMsgBuf;
//	FormatMessage(
//		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//		NULL, WSAGetLastError(),
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		(LPTSTR)&lpMsgBuf, 0, NULL);
//
//	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_ICONERROR); // 	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
//	LocalFree(lpMsgBuf);
//	exit(1);
//}
//
////���� �Լ� ���� ���
//void err_display(const char* msg)
//{
//	LPVOID lpMsgBuf;
//	FormatMessage(
//		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
//		NULL, WSAGetLastError(),
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		(LPTSTR)&lpMsgBuf, 0, NULL);
//
//	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf); // 	printf("[%s] %s", msg, (char *)lpMsgBuf); 
//	LocalFree(lpMsgBuf);
//}
//
//// ����� ���� ������ ���� �Լ�
//int recvn(SOCKET s, char* buf, int len, int flags)
//{
//	int received;
//	char* ptr = buf;
//	int left = len;
//
//	while (left > 0)
//	{
//		received = recv(s, ptr, left, flags);
//		if (received == SOCKET_ERROR)
//			return SOCKET_ERROR;
//		else if (received == 0)
//			break;
//		left -= received;
//		ptr += received;
//	}
//
//	return (len - left);
//}
//
//
//int main()
//{
//	printf("Client ����\n");
//
//	int retval;
//
//	//���� �ʱ�ȭ
//	WSADATA wsa;
//	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) //	retval = WSAStartup(MAKEWORD(2, 2), &wsa); 
//		return 1;
//
//	//socket()
//	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
//	if (sock == INVALID_SOCKET) err_quit("socket");
//
//	//connect()
//	SOCKADDR_IN serveraddr;
//	ZeroMemory(&serveraddr, sizeof(serveraddr));
//	serveraddr.sin_family = AF_INET;
//	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
//	serveraddr.sin_port = htons(SERVERPORT);
//	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
//	if (retval == SOCKET_ERROR) err_quit("connect");
//
//	//������ ��ſ� ����� ����
//	char buf[BUFSIZE];
//	char* testdata[] = { "�ȳ��ϼ���", "�ݰ����ϴ�", "�� ������" };
//	int len;
//
//	while (1)
//	{
//		//������ �ޱ� - ���� ����
//		retval = recvn(sock, (char*)&len, sizeof(int), 0);
//		if (retval == SOCKET_ERROR)
//		{
//			err_display("recv()");
//			break;
//		}
//		else if (retval == 0)
//			break;
//
//		//�����͹ޱ� - ���� ����
//		retval = recvn(sock, buf, len, 0);
//		if (retval == SOCKET_ERROR)
//		{
//			err_display("recv()");
//			break;
//		}
//		else if (retval == 0)
//			break;
//
//		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
//		printf("[���� ������] %s\n", buf);
//	}
//
//
//	//closesocket()
//	closesocket(sock);
//
//	//���� ����
//	WSACleanup();
//	return 0;
//}