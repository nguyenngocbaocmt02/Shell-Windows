#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <processthreadsapi.h>
using namespace std;
int main() {
	cout<<"------------------PROCESS LISTING------------------"<<endl;
	HANDLE hSnapShot =INVALID_HANDLE_VALUE;
	PROCESSENTRY32 ProcessInfo ={0};
	ProcessInfo.dwSize =sizeof(PROCESSENTRY32);
	int count =0;
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(INVALID_HANDLE_VALUE == hSnapShot) {
		cout<<"CreatToolhelp32SnapShot Function Failed" <<endl;
		cout<< "Error No - "<<GetLastError()<<endl;
	}
	cout<<"CreateToolhelp32Snapshot Function Success"<< endl;
	DWORD exitCode=0;
	while(Process32Next(hSnapShot, &ProcessInfo) !=FALSE) {
		cout<<"---------------------------------------"<<endl;
		cout<<"\t PROCESS NO - "<<++count<<endl;
		cout<<"---------------------------------------"<<endl;
		cout<<"NO. OF THREAD - "<< ProcessInfo.cntThreads<<endl;
		cout<<"SIZE - "<< ProcessInfo.dwSize<<endl;
		cout<<"BASE PRIORITY - "<< ProcessInfo.pcPriClassBase<<endl;
		wcout<<"EXECUTABLE FILE - "<< ProcessInfo.szExeFile<<endl;
		cout<<"PPID - "<< ProcessInfo.th32ParentProcessID<<endl;
		cout<<"PPID - "<< ProcessInfo.th32ProcessID<<endl;
		if(ProcessInfo.th32ParentProcessID==15912) {
			cout<<"dm";
			CloseHandle(&ProcessInfo);
		}
	}
	CloseHandle(hSnapShot);
	cout<<"-------------------------------";
	
	
}
