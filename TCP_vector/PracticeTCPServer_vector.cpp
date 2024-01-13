#pragma once

#include "PracticeTCPServer_vector.h"

//==================================================================================TCP Server==========================================================


//소켓 함수 오류 출력 후 종료
void err_quit_vector(const char* msg) 	//VS 2017에서는 에디터에서 상수 문자열을 일반 문자열로 형변환할 수 없다는 컴파일러 오류가 발생한다
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
void err_display_vector(const char* msg)
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

int TCPServerStart_vector(vector<int>* v)
{
	printf("Server 시작\n");

	int retval;

	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) //	retval = WSAStartup(MAKEWORD(2, 2), &wsa); 
		return 1;

	//socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit_vector("socket");


	//bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = ::bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit_vector("bind()");

	//listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit_vector("listen");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char* buf;
	buf = new char[BUFSIZE + 1];

	vector<int> check;

	while (1)
	{
		//accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display_vector("accept()");
			continue;
		}


		//접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		//클라이언트와 데이터 통신
		while (1)
		{
			////데이터 받기
			//retval = recv(client_sock, buf, BUFSIZE, 0);
			//if (retval == SOCKET_ERROR)
			//{
			//	err_display_vector("recv()");
			//	break;
			//}
			//else if (retval == 0)
			//	break;

			////받은 데이터 출력
			//buf[retval] = '\0';
			//printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
			//	ntohs(clientaddr.sin_port), buf);

			Sleep(333);

			memcpy(buf, v->data(), sizeof(int)*( v->size() ) );
			

			/*check.resize(v->size());
			memcpy(check.data(), buf, sizeof(int) * (v->size()));

			for (int a : check) {
				cout << a << " ";
			} cout << "\n";

			if (check.size() != v->size()) {
				cout << "v.size() : " << v->size() << "\n";
				cout << "check.size() : " << check.size() << "\n";
			}*/
				

			//데이터 보내기
			retval = send(client_sock, buf, sizeof(int) * ( v->size() ), 0);
			if (retval == SOCKET_ERROR)
			{
				err_display_vector("send()");
				break;
			}
			else {
				printf("[TCP 서버] %d바이트를 보냈습니다.\n", sizeof(int) * (v->size()) ); 
			}
		}

		// closesocket()
		closesocket(client_sock);
		printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트번호=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	// closesocket()
	closesocket(listen_sock);

	delete buf;

	//윈속 종료
	WSACleanup();
	return 0;
}