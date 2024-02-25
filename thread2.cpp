#include <iostream>
#include <windows.h> 
#include <process.h> /*_beginthreadex, endthreadex*/
unsigned WINAPI ThreadFunc(void* arg);

int main(int argc, char* argv[])
{
    HANDLE hThread;
    DWORD wr;
    unsigned threadID;
    int param =5;

    hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, (void*)&param, 0, &threadID);   
    if(hThread == 0)
    {
        puts( "_beginthreadex() error‘"); 
        return -1;
    }
    
    if((wr=WaitForSingleObject(hThread, INFINITE))==WAIT_FAILED)//하나의 커널 오브젝트에 대해서 signaled 상태인지 확인하기 위해 호출 INFINITE전달시 signaled 상태가 되기 전에는 반환하지 않음
    
    {
        puts("thread wait eorrr");
        return -1;
    }
    printf("wait result: %s \n", (wr==WAIT_OBJECT_0)? "signaled" : "time-out");
    puts("end of main");
    return 0;  
}

unsigned WINAPI ThreadFunc(void* arg)
{
    int i;
    int cnt=*((int*)arg);
    for(i = 0 ; i < cnt ; i++)
    {
        Sleep(1000); puts("running thread");

    }
    return 0;
}

HANDLE CreateMutex(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCTSTR lpName);
