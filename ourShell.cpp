#include <windows.h>
#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;
void myCreateProcess(char* duongDan) {
	HANDLE hProcess=0;
	HANDLE hThread=0;
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
		cout<<"Ban da nhap sai cu phap duoc ho tro"<<endl;
	}
	cout<<"Success to create"<<endl;
	cout<<"Process ID= "<<pi.dwProcessId<<endl;
	cout<<"Thread ID= "<< pi.dwThreadId<<endl;
	WaitForSingleObject(pi.hProcess,INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}
int main() {
	 map < string, char*>  demand;
	 demand.insert( std::make_pair<string, char*>("checkModule", "checkmodule.exe" ) );
	 demand.insert( std::make_pair<string, char*>("checkThread", "checkthread.exe" ) );
	 demand.insert( std::make_pair<string, char*>( "checkProcess", "checkprocess.exe" ) );
	 demand.insert( std::make_pair<string, char*>( "untitled1", "Untitled1.exe" ) );
	 demand.insert( std::make_pair<string, char*>( "help", "help.exe" ) );
	 demand.insert( std::make_pair<string, char*>( "welcome", "welcome.exe" ) );
	 string dm;
	 int t=3;
	 myCreateProcess(demand["welcome"]);
	 while(t--) {
	 	cout<<endl<<"Xin moi nhap yeu cau cua ban: ";
	 	cin>>dm;
	 	myCreateProcess(demand[dm]);
	 }
	 return(0);
}
