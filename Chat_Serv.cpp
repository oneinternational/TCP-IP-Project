#include <iostream>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <process.h>
#include <winsock2.h>
#include <map>
#include <list>
#define BUF_SIZE 1024
#define MAX_CLNT 256
char QeArray[4][400]={
    "안녕하세요 Sangho's Free Chat 입니다.\n생성하신 닉네임이 있으시다면 '1'을 없으시다면 '2'를 입력해주세요",
    "Sangho's Free Chat 이용을 위한 간단한 정보 입력을 진행하도록 하겠습니다.\n사용하실 닉네임을 입력해주세요(중복불가)",
    "사용자님의 성함을 입력해주세요",
    "사용자님이 선택하신 닉네임이 성공적으로 생성되었습니다.\n로그인 화면으로 전환을 원하시면 '1'번을 눌려주세요"
}; //질문 저장 문자열 2차원 배열
std::map<SOCKET, std::string> map_SockNick; // 소켓과 닉네임
std::map<std::string, std::string> map_Nick_Name; //닉네임 이름
std::list<std::string> list_Nick;//닉네임
unsigned WINAPI HandleClnt(void* arg);
unsigned WINAPI AllProcess(void* newUser);

void SendMsg(char * msg, int len);
void ErrorHandling(char* msg);
int NickSet(SOCKET newSock);
int clntCnt = 0;
SOCKET clntSocks[MAX_CLNT]; //소켓을 담는 배열
HANDLE hMutex;


int main(int argc, char *argv[])
{
    WSADATA wsaData; 
    SOCKET hServSock, hClntSock; 
    SOCKADDR_IN servAdr, clntAdr; 
    int clntAdrSz; 
    HANDLE hThread, hlogThread;
    if(argc!=2)
    {
        printf("Usag : %s <port>\n", argv[0]);
        exit(1);
    }
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    hMutex = CreateMutex(NULL, FALSE, NULL);

    hServSock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&servAdr, 0, sizeof(servAdr)); 
    servAdr.sin_family=AF_INET; 
    servAdr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servAdr.sin_port=htons(atoi(argv[1]));

    if(bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorHandling("bind() error");
    if(listen(hServSock,5) == SOCKET_ERROR)
        ErrorHandling("listen() error");


    while(1)
    {
        clntAdrSz = sizeof(clntAdr);
        hClntSock=accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSz); 
        WaitForSingleObject(hMutex, INFINITE);
        clntSocks[clntCnt++]=hClntSock;
        ReleaseMutex(hMutex);
        printf("Connected client IP : %s \n" ,inet_ntoa(clntAdr.sin_addr));
        hThread = 
            (HANDLE)_beginthreadex(NULL, 0, AllProcess, (void*)&hClntSock, 0, NULL);
    }
    closesocket(hServSock);
    WSACleanup();
    return 0;
}

unsigned WINAPI AllProcess(void* newUser)
{
    SOCKET ClntSock = *((SOCKET*)newUser);
    int caseNum = 1 , returnValue;
    char msgTo[BUF_SIZE];
    char msgFrom[BUF_SIZE];
    std::list<std::string>::iterator iter;
    int i = 0;
    while(1)
    {
        switch(caseNum)
        {
        case 1://닉네임 생성 process
            returnValue = NickSet(ClntSock);

            if(returnValue == 10)//오류처리
            {
                closesocket(ClntSock);
                return 0;
            }
            else if(returnValue == 2)//닉네임 존재 or 완료 리턴 값
            {
                caseNum = 2;
                continue;
            }
            break;
        case 2://닉네임으로 입장 process
            std::cout << "현재 서버에 등록된 닉네임" <<std::endl;
            for(iter = list_Nick.begin(); iter != list_Nick.end(); iter++)
            {
                std::cout << *iter << std::endl;
            }
            std::cout << "현재 접속한 닉네임" << std::endl;

            std::cout << map_SockNick.at(clntSocks[0]) << std::endl;
            Sleep(10000);
            break;
        case 3://입장 후 친구추가하기/접속 친구 보기/ 1:1 대화 걸기/ 랜덤 다중 채팅방 선택

        case 4://~~ 내일 
        }
    }

}

int NickSet(SOCKET newSock)
{
    char msgFrom[BUF_SIZE];
    int strlen = 0, turnCnt = 0, duplication = 0;
    std::string name, nick;
    while(1)
    {
        send(newSock, QeArray[turnCnt], sizeof(QeArray[turnCnt]), 0);// 주고
        strlen = recv(newSock, msgFrom, BUF_SIZE, 0);
        std::cout << turnCnt << std::endl;
        std::cout << strlen << std::endl;
        if(strlen  == -1)// 받고
        {
            std::cerr << "can't receive a message" << std::endl;
            return 10;//못 읽었을 때 에러 처리
        }
        // std::cout << msgFrom << std::endl;
        else if(turnCnt == 0 && std::string(msgFrom) == "1") // 닉네임이 있으시면 1번
            return 2; // 닉네임 있을 때
        else if (turnCnt == 0 && (std::string(msgFrom) == "1" && std::string(msgFrom) == "2"))//지정된 번호 누르지 않았을 때
            continue;
        else if (turnCnt == 1)//입력된 닉네임 검사
        {
            std::list<std::string>::iterator iter;
            std::string msgToString = std::string(msgFrom);
            std::cout << "msgToString : " << msgToString << "text count : " << msgToString.length() << std::endl;
            std::cout << "msgFrom : " << msgFrom << "text count : " << sizeof(msgFrom) << std::endl;

            for(iter = list_Nick.begin(); iter != list_Nick.end(); iter++)
            {
                if(msgToString == *iter)
                    duplication = 1;
            }
            if(duplication == 1)
                continue;
            nick = msgToString;
        } 
        else if (turnCnt == 2)
        {
            name = std::string(msgFrom);
            map_SockNick.insert({newSock, nick}); // 소켓과 닉네임
            map_Nick_Name.insert({nick, name});
            list_Nick.push_back(nick);                        //닉네임과 이름
            send(newSock, QeArray[turnCnt+1], sizeof(QeArray[turnCnt]), 0); //닉네임 생성 완료
            return 2;// 완료 코드
        }
        turnCnt +=1;
    }
    return 0;
} 


// unsigned WINAPI HandleClnt(void * arg)
// {
// 	SOCKET hClntSock=*((SOCKET*)arg);
// 	int strLen=0, i;
// 	char msg[BUF_SIZE];
	
// 	while((strLen=recv(hClntSock, msg, sizeof(msg), 0))!=0)
// 		SendMsg(msg, strLen); 
	
// 	WaitForSingleObject(hMutex, INFINITE);
// 	for(i=0; i<clntCnt; i++)   // remove disconnected client
// 	{
// 		if(hClntSock==clntSocks[i])
// 		{
// 			while(i++<clntCnt-1)
// 				clntSocks[i]=clntSocks[i+1];
// 			break;
// 		}
// 	}
// 	clntCnt--;
// 	ReleaseMutex(hMutex);
// 	closesocket(hClntSock);
// 	return 0;
// }
// void SendMsg(char * msg, int len)   // send to all
// {
// 	int i;
// 	WaitForSingleObject(hMutex, INFINITE);
// 	for(i=0; i<clntCnt; i++)
// 		send(clntSocks[i], msg, len, 0);

// 	ReleaseMutex(hMutex);
// }

void ErrorHandling(char*msg)
{
    std::cerr << msg << std::endl;
    exit(1);
}
