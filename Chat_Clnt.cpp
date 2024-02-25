#include <iostream>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <process.h>
#define BUF_SIZE 1024
unsigned WINAPI SendMsg(void* arg);
unsigned WINAPI RecvMsg(void* arg);
void ErrorHandling(char* msg);
typedef enum ColorType{
BLACK = 0,
darkBLUE = 1,
DarkGreen = 2, 
darkSkyBlue = 3, 
DarkRed = 4,
DarkPurple = 5,
DarkYellow = 6,
GRAY = 7,		
DarkGray = 8,	
BLUE = 9,		
GREEN = 10,		
SkyBlue = 11,	
RED = 12,		
PURPLE = 13,		
YELLOW = 14,		
WHITE = 15		
} COLOR;

void textcolor(int colorNum);


std::string name = "[DEFAULT]";
std::string msg;

int main(int argc, char* argv[])
{
    WSADATA wasData;
    SOCKET hSock;
    SOCKADDR_IN servAdr;
    HANDLE hSndThread, hRcvThread;
    if(argc!=4)
    {
        printf("Usage : %s <IP> <PORT> <NAME>\n",argv[0]);
        exit(1);
    }
    if(WSAStartup(MAKEWORD(2,2), &wasData)!= 0)
        ErrorHandling("WSAStartup() error!");
    name=("[%s]", argv[3]);
    hSock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family=AF_INET;
    servAdr.sin_addr.s_addr=inet_addr(argv[1]);
    servAdr.sin_port=htons(atoi(argv[2]));

    if(connect(hSock, (SOCKADDR*)&servAdr, sizeof(servAdr))==SOCKET_ERROR)
        ErrorHandling("connect() error!");

    hSndThread=
        (HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)&hSock, 0, NULL);
    hRcvThread=
        (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)&hSock, 0, NULL);

    WaitForSingleObject(hSndThread, INFINITE);// 쓰레드 종료 감지 (INFINITE->함수가 종료될 때 까지 값을 반환 하지 않음)
    WaitForSingleObject(hRcvThread, INFINITE);

    closesocket(hSock);
    WSACleanup();
    return 0;
}

unsigned WINAPI SendMsg(void* arg)
{
    SOCKET hSock=*((SOCKET*)arg);
    char msgWithName[BUF_SIZE];
    while(1)
    {
        getline(std::cin, msg);
        if(msg == "q" || msg == "Q")
        {
            closesocket(hSock);
            exit(0);
        }
        std::string totalMsg = "[" + name + "]" + msg;
        int size = totalMsg.length();
        std::cout << "보낸 메시지 : " << totalMsg << std::endl;
        strcpy(msgWithName, totalMsg.c_str());
        send(hSock, msgWithName, size, 0);

    }
    return 0;
}

unsigned WINAPI RecvMsg(void* arg)
{
    int hSock = *((SOCKET*)arg);
    char msgWithName[BUF_SIZE];
    int strLen;
    while(1)
    {
        strLen = recv(hSock, msgWithName, BUF_SIZE, 0);
        if(strLen == -1)
            return -1;
        msgWithName[strLen]=0;
        std::cout << "받은 메시지 : " << msgWithName << std::endl;
    }
    return 0;
}

void ErrorHandling(char *msg)
{
    std::cerr << msg << std::endl;
    exit(1);
}

void textcolor(int colorNum)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

