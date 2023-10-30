# 참고 자료
TCP/IP 윈도우 소켓 프로그래밍 - 김선우 저, 한빛미디어

## 파일 및 폴더 구조
TCP_Studying : 예제 문제들을 학습.
   - TCPClient , TCPServer - 에코서버와 클라이언트를 TCP 통신으로구현
     - TCPClient_Data, TCPServer_Data - 클라이언트가 고정길이 데이터를 서버로 송신
     - TCPClient_Variable, TCPServer_Variable - 클라이언트가 가변길이 데이터를 서버로 송신
     - TCPClient_FixedVariable, TCPServer_FixedVariable - 클라이언트가 고정+가변길이 데이터를 서버로 송신
     - TCPClient_CloseOnTransfer, TCPServer_CloseOnTransfer - 클라이언트가 데이터를 한 번 송신 후 소켓을 닫음.
   - MultithreadTCPServer - 서버가 스레드를 통해 클라이언트와 소켓 통신 - 여러 클라이언트와 동시에 통신가능
Simulation_add_TCP : TCP 통신과 시뮬레이션을 결합
     - Serialization.h - vector의 데이터를 char* 타입의 버퍼로 옮기는 직렬화와, char* 버퍼의 데이터를 vector로 
                                   다시 옮기는 역직렬화 함수를 구현해놓은 헤더파일
     - TCPClient_Data_1, TCPServer_Data_1 - 서버가 데이터를 보내고, 클라이언트가 받음.- Server에서 0을 보내면, 						   Client와의 통신이 끊긴다.
                                          하지만 server는 남아있으며, 또 다른 Client가 다시 실행하면 또 통신이                  					   이어진다.
     - TCPClient_Data_2, TCPServer_Data_2 - 서버는 시뮬레이션을 진행함과 동시에 클라이언트와 통신한다.
   - TCPServer_Data_1_to_h.h : Data_2 버전을 구현하기 위해, 통신함수를 하나의 함수로 정의해놓은 헤더파일
