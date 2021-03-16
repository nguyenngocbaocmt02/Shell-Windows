#include <stdio.h>
#include <fstream>
#include <bits/stdc++.h>
#include <iostream>
#include <windows.h>

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
int main ()
{
    map < string, char*>  demand;
	demand.insert( std::make_pair<string, char*>("checkModule", "checkmodule.exe" ) );
	demand.insert( std::make_pair<string, char*>("checkThread", "checkthread.exe" ) );
	demand.insert( std::make_pair<string, char*>( "checkProcess", "checkprocess.exe" ) );
	demand.insert( std::make_pair<string, char*>( "killProcess", "killprocess.exe" ) );
	demand.insert( std::make_pair<string, char*>( "help", "help.exe" ) );
	demand.insert( std::make_pair<string, char*>( "welcome", "welcome.exe" ) );
	demand.insert( std::make_pair<string, char*>( "calculator", "calc.exe" ) );
	demand.insert( std::make_pair<string, char*>( "clear", "clrscr.exe" ) );
	 
   char data[100];

   // mo mot file trong che do read.
   ifstream infile; 
   infile.open("Shell.bat"); 
 
   do {
   	
   	infile >> data; 
   	if (infile.eof()) break;
        myCreateProcess(demand[data]);  
	cout << "#############################################" << endl; 
   
   } while (!infile.eof());
   

   // dong file da mo.
   infile.close();


   return 0;
}
