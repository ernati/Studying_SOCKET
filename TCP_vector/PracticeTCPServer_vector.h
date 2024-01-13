#pragma once
#pragma comment( lib, "ws2_32" )
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
//#include <stdlib.h>
//#include <stdio.h>
#include <string.h>

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
#include <iostream>
#include <Windows.h>
#include <thread>
#include <mutex>

#define SERVERPORT 9000
#define BUFSIZE 70000

using namespace std;

void err_quit_vector(const char* msg);
void err_display_vector(const char* msg);
int TCPServerStart_vector(vector<int>* v);