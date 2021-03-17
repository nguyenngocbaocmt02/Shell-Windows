#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <bits/stdc++.h>
using namespace std;
int main() {
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
	cout<<"------------------MODULE LISTING------------------"<<endl;
	HANDLE hSnapShot =INVALID_HANDLE_VALUE;
	MODULEENTRY32 ModuleInfo ={0};
	ModuleInfo.dwSize =sizeof(MODULEENTRY32);
	int count =0;
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,0);
	if(INVALID_HANDLE_VALUE == hSnapShot) {
		cout<<"CreatToolhelp32SnapShot Function Failed" <<endl;
		cout<< "Error No - "<<GetLastError()<<endl;
	}
	while(Module32Next(hSnapShot, &ModuleInfo) !=FALSE) {
		if(processList.find(ModuleInfo.szExePath)==processList.end()) continue;
		cout<<"---------------------------------------"<<endl;
		cout<<"\t MODULE NO - "<<++count<<endl;
		cout<<"---------------------------------------"<<endl;
		cout<<"SIZE - "<< ModuleInfo.dwSize<<endl;
		cout<<"GLOBAL COUNT USAGE - "<< ModuleInfo.GlblcntUsage<<endl;
		cout<<"MODULE - "<< ModuleInfo.hModule<<endl;
		cout<<"MODULE BASE ADDRESS - "<< ModuleInfo.modBaseAddr<<endl;
		cout<<"MODULE BASE SIZE - "<< ModuleInfo.modBaseSize<<endl;
		cout<<"LOAD COUT USAGE - "<< ModuleInfo.ProccntUsage<<endl;
		cout<<"EXECUTABLE PATH - "<< ModuleInfo.szExePath<<endl;
		cout<<"MODULE NAME - "<< ModuleInfo.szModule<<endl;
		cout<<"THREAD MODULE ID - "<<ModuleInfo.th32ModuleID<<endl;
		cout<<"PROCESS MODULE ID - "<<ModuleInfo.th32ProcessID<<endl;
	}
	CloseHandle(hSnapShot);
	return 0;

}
