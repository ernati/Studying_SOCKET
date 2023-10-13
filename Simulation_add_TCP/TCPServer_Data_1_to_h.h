﻿#pragma once

#pragma comment( lib, "ws2_32" )
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SERVERPORT 9000
#define BUFSIZE 512


//소켓 함수 오류 출력 후 종료
void err_quit(const char* msg) 	//VS 2017에서는 에디터에서 상수 문자열을 일반 문자열로 형변환할 수 없다는 컴파일러 오류가 발생한다
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_ICONERROR); // 	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

//소켓 함수 오류 출력
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf); // 	printf("[%s] %s", msg, (char *)lpMsgBuf); 
	LocalFree(lpMsgBuf);
}

//int와 std::Binder 를 호환하기 위한 코드 (because of C++)
SOCKET BindFunction(SOCKET socket, const SOCKADDR* addr, int addrlen) {
	return bind(socket, addr, addrlen);
}



void TCP_mine()
{
	printf("Server 시작\n");

	int retval;

	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) //	retval = WSAStartup(MAKEWORD(2, 2), &wsa); 
		return; // return 1;

	//socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket");


	//bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	//retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	retval = static_cast<int>( BindFunction(listen_sock, reinterpret_cast<SOCKADDR*>(&serveraddr), sizeof(serveraddr)) );
	if (retval == SOCKET_ERROR) err_quit("bind()");

	//listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;

	/*char buf[BUFSIZE + 1];
	int len;*/

	//데이터 통신에 사용할 변수
	char buf[BUFSIZE];
	int len;

	while (1)
	{
		//accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			continue;
		}


		//접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		//클라이언트와 데이터 통신
		while (1)
		{

			//데이터 입력
			printf("\n[보낼 데이터] ");
			if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
				break;

			// '\n' 문자 제거
			len = strlen(buf);
			if (buf[len - 1] == '\n')
				buf[len - 1] = '\0';
			if (strlen(buf) == 0)
				break;


			//데이터 보내기
			retval = send(client_sock, (char*)&len, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			//데이터 보내기
			retval = send(client_sock, buf, len, 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			printf("[TCP 서버] %d바이트를 보냈습니다.\n", retval);

		}

		// closesocket()
		closesocket(client_sock);
		printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트번호=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// closesocket()
	closesocket(listen_sock);

	//윈속 종료
	WSACleanup();
	return; //return 0;
}