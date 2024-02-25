#include <iostream>
#include <windows.h> 
#include <process.h> /*_beginthreadex, endthreadex*/
unsigned WINAPI ThreadFunc(void* arg);

int main(int argc, char* argv[])
{
    HANDLE hThread;
    unsigned threadID;
    int param = 5;
    //ThreadFunc에 변수 param의 주소 값을 전달하면서 쓰레드의 생성을 요구하고 있다
    hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, (void*)&param, 0, &threadID);
    if(hThread ==0)
    {
        puts("_beginthreaddex() error");
        return 1;
    }
    Sleep(3000); // 3초
    puts("end of main");
    return 0;
}

unsigned WINAPI ThreadFunc(void* arg) // beginthreadex 함수가 요구하는 호출규약을 지키기 위해 삽입
{
    int i;
    int cnt = *((int*)arg);
    for(i=0; i<cnt; i++)
    {
        Sleep(1000); puts("running thread");
    }
    return 0;
}
// main함수의 반환으로 인해 프로세스가 종료되면, 그 안에 담겨 있는 모든 쓰레드들도 함께 종료된다.
// 핸들의 정수 값은 프로세스가 달라지면 중복될 수 있다.
//그러나, 쓰레드의 ID는 프로세스의 영역을 넘어서서 중복되지 않는다.

//