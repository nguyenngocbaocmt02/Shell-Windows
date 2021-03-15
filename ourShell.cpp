#include <windows.h>
#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;
void myCreateProcess(char* duongDan) {
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
	 map < string, char*>  demand;
	 demand.insert( std::make_pair<string, char*>("checkModule", "checkmodule.exe" ) );
	 demand.insert( std::make_pair<string, char*>("checkThread", "checkthread.exe" ) );
	 demand.insert( std::make_pair<string, char*>( "checkProcess", "checkprocess.exe" ) );
	 demand.insert( std::make_pair<string, char*>( "killProcess", "killprocess.exe" ) );
	 demand.insert( std::make_pair<string, char*>( "help", "help.exe" ) );
	 demand.insert( std::make_pair<string, char*>( "welcome", "welcome.exe" ) );
	 demand.insert( std::make_pair<string, char*>( "calculator", "calc.exe" ) );
	 demand.insert( std::make_pair<string, char*>( "clear", "clrscr.exe" ) );
	 string dm;
	 int t=6;
	 myCreateProcess(demand["clear"]);
	 while(t--) {
	 	cout<<endl<<"Xin moi nhap yeu cau cua ban: ";
	 	cin>>dm;
	 	if(demand[dm]== NULL) {
	 		cout<<"The command isn't available";
	 		continue;
	 	}
	 	cout<<"The command is accepted!"<<endl;
	 	myCreateProcess(demand[dm]);
	 }
	 return(0);
}
