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
    "�ȳ��ϼ��� Sangho's Free Chat �Դϴ�.\n\
    �����Ͻ� �г����� �����ôٸ� '1'�� �����ôٸ� '2'�� �Է����ּ���",
    "Sangho's Free Chat �̿��� ���� ������ ���� �Է��� �����ϵ��� �ϰڽ��ϴ�.\n\n\
    Sangho's Free Chat ���� ����Ͻ� �г����� �Է����ּ���(�ߺ��Ұ�)\n\
    ����Ͻ� �г��� : ",
    "����ڴ��� ������ �Է����ּ���\n\
    ���� : ",
    "����ڴ��� �����Ͻ� �г����� ���������� �����Ǿ����ϴ�.\n\
    �����Ͻ� �г����� ���� Sangho's Free Chat�� �̿��ϼ���.(��� �� �г��� �Է� ȭ������ ��ȯ)"
}; //���� ���� ���ڿ� 2���� �迭
std::map<SOCKET, std::string> map_SockNick; // ���ϰ� �г���
std::map<std::string, std::string> map_Nick_Name; //�г��� �̸�
std::list<std::string> list_Nick;
unsigned WINAPI HandleClnt(void* arg);
unsigned WINAPI WelcomeClnt(void* newUser);
void SendMsg(char * msg, int len);
void ErrorHandling(char* msg);
int ToFromMsg(SOCKET newSock);
int clntCnt = 0;
SOCKET clntSocks[MAX_CLNT]; //������ ��� �迭
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
            (HANDLE)_beginthreadex(NULL, 0, WelcomeClnt, (void*)&hClntSock, 0, NULL);
    }
    closesocket(hServSock);
    WSACleanup();
    return 0;
}

unsigned WINAPI WelcomeClnt(void* newUser)
{
    SOCKET ClntSock = *((SOCKET*)newUser);
    int caseNum = 1 , returnValue;
    char msgTo[BUF_SIZE];
    char msgFrom[BUF_SIZE];
    while(1)
    {
        switch(caseNum)
        {
        case 1:
            if((returnValue = ToFromMsg(ClntSock)) == 10)
                return 0;
            else if((returnValue = ToFromMsg(ClntSock)) == 2)
            {
                caseNum = 2;
                continue;
            }
            break;
        case 2:
            std::cout << "�α��� ������" << std::endl;
            break;
        }
    }

}

int ToFromMsg(SOCKET newSock)
{
    // SOCKET newSock 
    // = *((SOCKET*)sock);
    char msgFrom[BUF_SIZE];
    int strlen = 0, turnCnt = 0, duplication = 0;
    std::string name, nick;
    while(1)
    {
        send(newSock, QeArray[turnCnt], sizeof(QeArray[turnCnt]), 0);// �ְ�
        if((strlen = recv(newSock, msgFrom, sizeof(msgFrom), 0)) == -1)// �ް�
        {
            std::cerr << "can't receive a message" << std::endl;
            return 10;//�� �о��� �� ���� ó��
        }
        else if(turnCnt == 0 && msgFrom == "1") // �г����� �����ø� 1��
            return 2; // �г��� ���� ��
        else if (turnCnt == 0 && (msgFrom != "1" && msgFrom != "2"))//������ ��ȣ ������ �ʾ��� ��
            continue;
        else if (turnCnt == 1)//�Էµ� �г��� �˻�
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
            map_SockNick.insert({newSock, nick}); // ���ϰ� �г���
            map_Nick_Name.insert({nick, name});                        //�г��Ӱ� �̸�
            send(newSock, QeArray[turnCnt+1], sizeof(QeArray[turnCnt]), 0); //�г��� ���� �Ϸ�
            return 2;// �Ϸ� �ڵ�
        }
        turnCnt +=1;
    }
} 


unsigned WINAPI HandleClnt(void * arg)
{
	SOCKET hClntSock=*((SOCKET*)arg);
	int strLen=0, i;
	char msg[BUF_SIZE];
	
	while((strLen=recv(hClntSock, msg, sizeof(msg), 0))!=0)
		SendMsg(msg, strLen); 
	
	WaitForSingleObject(hMutex, INFINITE);
	for(i=0; i<clntCnt; i++)   // remove disconnected client
	{
		if(hClntSock==clntSocks[i])
		{
			while(i++<clntCnt-1)
				clntSocks[i]=clntSocks[i+1];
			break;
		}
	}
	clntCnt--;
	ReleaseMutex(hMutex);
	closesocket(hClntSock);
	return 0;
}
void SendMsg(char * msg, int len)   // send to all
{
	int i;
	WaitForSingleObject(hMutex, INFINITE);
	for(i=0; i<clntCnt; i++)
		send(clntSocks[i], msg, len, 0);

	ReleaseMutex(hMutex);
}

void ErrorHandling(char*msg)
{
    std::cerr << msg << std::endl;
    exit(1);
}
