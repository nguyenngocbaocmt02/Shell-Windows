#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>

using namespace std;
int main() {
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
	cout<<"CreateToolhelp32Snapshot Function Success"<< endl;
	while(Thread32Next(hSnapShot, &ThreadInfo) !=FALSE) {
		cout<<"---------------------------------------"<<endl;
		cout<<"\t THREAD NO - "<<++count<<endl;
		cout<<"---------------------------------------"<<endl;
		cout<<"COUNT USAGE - "<< ThreadInfo.cntUsage<<endl;
		cout<<"FLAG - "<< ThreadInfo.dwFlags<<endl;
		cout<<"SIZE - "<< ThreadInfo.dwSize<<endl;
		cout<<"OWNER PROCESS ID - "<< ThreadInfo.th32OwnerProcessID<<endl;
		cout<<"THREAD ID - "<< ThreadInfo.th32ThreadID<<endl;
		cout<<"BASE PRIORITY - "<< ThreadInfo.tpBasePri<<endl;
		cout<<"DELTA PRIORITY - "<< ThreadInfo.tpDeltaPri<<endl;
	}
	CloseHandle(hSnapShot);
	cout<<"-------------------------------";
}
