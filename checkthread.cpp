#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <bits/stdc++.h>
using namespace std;
map < int ,string> ppip;
void checkProcess() {
	set <string>  processList;
	processList.insert( "ourShell.exe" ); 
	 processList.insert( "checkmodule.exe" );  
	 processList.insert(  "checkthread.exe" );
	 processList.insert( "checkprocess.exe" );
	 processList.insert( "killprocess.exe" );
	 processList.insert( "help.exe" );
	 processList.insert(  "Calculator.exe" );
	 processList.insert("clrscr.exe"  );
	 processList.insert( "clock.exe" );
	HANDLE hSnapShot =INVALID_HANDLE_VALUE;
	PROCESSENTRY32 ProcessInfo ={0};
	ProcessInfo.dwSize =sizeof(PROCESSENTRY32);
	int count =0;
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(INVALID_HANDLE_VALUE == hSnapShot) {
		cout<<"CreatToolhelp32SnapShot Function Failed" <<endl;
		cout<< "Error No - "<<GetLastError()<<endl;
	}
	while(Process32Next(hSnapShot, &ProcessInfo) !=FALSE) {
		if(processList.find(ProcessInfo.szExeFile)==processList.end()) continue;
		ppip[ProcessInfo.th32ProcessID]=ProcessInfo.szExeFile;
		}
	CloseHandle(hSnapShot);
}
int main() {
	checkProcess();
	cout<<"------------------THREAD LISTING------------------"<<endl;
	HANDLE hSnapShot =INVALID_HANDLE_VALUE;
	THREADENTRY32 ThreadInfo ={0};
	ThreadInfo.dwSize =sizeof(THREADENTRY32);
	int count =0;
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);
	if(INVALID_HANDLE_VALUE == hSnapShot) {
		cout<<"CreatToolhelp32SnapShot Function Failed" <<endl;
		cout<< "Error No - "<<GetLastError()<<endl;
	}
	map<int,string> ::iterator it;
	while(Thread32Next(hSnapShot, &ThreadInfo) !=FALSE) {
		if(ppip.find(ThreadInfo.th32OwnerProcessID)==ppip.end()) continue;
		cout<<"---------------------------------------"<<endl;
		cout<<"\t THREAD NO - "<<++count<<endl;
		cout<<"---------------------------------------"<<endl;
		cout<<"COUNT USAGE - "<< ThreadInfo.cntUsage<<endl;
		cout<<"FLAG - "<< ThreadInfo.dwFlags<<endl;
		cout<<"SIZE - "<< ThreadInfo.dwSize<<endl;
		cout<<"OWNER PROCESS ID - "<< ThreadInfo.th32OwnerProcessID<<endl;
		cout<<"OWNER PROCESS - "<<ppip[ThreadInfo.th32OwnerProcessID]<<endl;
		cout<<"THREAD ID - "<< ThreadInfo.th32ThreadID<<endl;
		cout<<"BASE PRIORITY - "<< ThreadInfo.tpBasePri<<endl;
		cout<<"DELTA PRIORITY - "<< ThreadInfo.tpDeltaPri<<endl;
	}
	CloseHandle(hSnapShot);
	return 0;
}
