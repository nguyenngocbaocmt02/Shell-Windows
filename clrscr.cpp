#include <bits/stdc++.h>
#include <windows.h>
using namespace std;
void myCreateProcess1(char* duongDan) {
	STARTUPINFO si;
    PROCESS_INFORMATION pi;
	DWORD dwProcessID=0;
	BOOL bProcess;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	CreateProcess( NULL,  
        duongDan,       
        NULL,          
        NULL,           
        FALSE,          
        0,            
        NULL,          
        NULL,       
        &si,           
        &pi ) ;
	if (bProcess ==FALSE) {
		cout<<"Failed to create Process "<< GetLastError()<<endl;
		cout<<"Please enter 'help' for more command"<<endl;
	}
	WaitForSingleObject(pi.hProcess,INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}
int main() {
	system("cls");
	myCreateProcess1("welcome.exe");
	myCreateProcess1("help.exe");
}
