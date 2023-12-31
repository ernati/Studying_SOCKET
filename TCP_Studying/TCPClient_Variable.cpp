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
////소켓 함수 오류 출력 후 종료
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
////소켓 함수 오류 출력
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
//
//int main()
//{
//	printf("Client 시작\n");
//
//	int retval;
//
//	//윈속 초기화
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
//	//데이터 통신에 사용할 변수
//	char buf[BUFSIZE];
//	char *testdata[] = { "안녕하세요", "반갑습니다", "또 만나요" };
//	int len;
//
//	//서버와 데이터 통신
//	for (int i = 0; i < 3; i++) {
//		//데이터 입력(시뮬레이션)
//		len = strlen(testdata[i]);
//		strncpy(buf, testdata[i], strlen(testdata[i]));
//		buf[len++] = '\n';
//
//		//데이터 보내기
//		retval = send(sock, buf, len, 0);
//		if (retval == SOCKET_ERROR) {
//			err_display("send()");
//			break;
//		}
//		printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);
//	}
//
//	Sleep(1000);
//
//	//closesocket()
//	closesocket(sock);
//
//	//윈속 종료
//	WSACleanup();
//	return 0;
//}