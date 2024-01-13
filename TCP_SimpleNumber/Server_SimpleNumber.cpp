//#pragma once
//
//#include "PracticeTCPServer.h"
//
//int num = 1;
//
//int main(void) {
//	//통신 시도
//	thread t1(TCPServerStart_SimpleNumber, &num);
//
//	while (true) {
//		num++;
//		//cout << num << "\n";
//		Sleep(333);
//	}
//
//	t1.join();
//	return 0;
//}