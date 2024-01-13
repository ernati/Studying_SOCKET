//#pragma once
//
//#include "PracticeTCPServer_vector.h"
//
//int main(void) {
//	vector<int> v(10, 0);
//
//	//통신 시도
//	thread t1(TCPServerStart_vector, &v);
//
//	while (true) {
//		Sleep(333);
//
//		for (int& num : v) {
//			num++;
//		}
//		//cout << num << "\n";
//	}
//
//
//	t1.join();
//	return 0;
//}