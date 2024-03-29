#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include<iostream>
#include <tlhelp32.h> 
#include<vector>
#include <tchar.h>
#include <algorithm>
#include<debugapi.h>
#include<set>
#include<map>
#include <processthreadsapi.h>
#pragma GCC diagnostic ignored "-Wwrite-strings"
#define MAX_PROCESS_RUNNING 100
using namespace std;
PROCESS_INFORMATION pi[MAX_PROCESS_RUNNING];
vector <PROCESSENTRY32> PE;
set<string>  processList;
int countProc=0;
bool compareFunction (std::string a, std::string b){ 
	for(int k=0;k<=a.size()-1;k++){ 
		a[k]=tolower(a[k]); 
	};
	for(int k=0;k<=b.size()-1;k++){
		b[k]=tolower(b[k]); 
	};
	return a<b; 
};
void kill(string p2t) {
		string namep = p2t; 
		string cmmdtsk="taskkill /IM "+namep+" /F"; 
		system(cmmdtsk.c_str());
}
void myCreateProcessParallel(char* path) {
	STARTUPINFO si;
    PROCESS_INFORMATION pi;
	DWORD dwProcessID=0;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	if (!CreateProcess(   
        path,       
        NULL,          
        NULL, 
		NULL,          
        FALSE,          
        CREATE_NEW_CONSOLE,            
        NULL,          
        NULL,       
        &si,           
        &pi ) ) {
		cout<<"Failed to create process. Error code: "<< GetLastError()<<endl;
		
	}
	else{
	cout<<"Process ID = "<<pi.dwProcessId<<endl;
	cout<<"Thread ID = "<< pi.dwThreadId<<endl;
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}
void myCreateProcessOBO(char* path) {
	STARTUPINFO si;
    PROCESS_INFORMATION pi;
	DWORD dwProcessID=0;
	BOOL bProcess;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	if (!CreateProcess(   
        path,       
        NULL,          
        NULL, 
		NULL,          
        FALSE,          
        CREATE_NEW_CONSOLE,             
        NULL,          
        NULL,       
        &si,           
        &pi ) ) {
		cout<<"Failed to create process. Error code: "<< GetLastError()<<endl;
	}
	else{
	cout<<"Process ID = "<<pi.dwProcessId<<endl;
	cout<<"Thread ID = "<< pi.dwThreadId<<endl;
	}
	char a;
	while(1) {
		a=getch();
		if (a!=27 && a!=115)
			cout<<"In OBO mode, you can press Esc to exit current process or press 's' to switch to Parallel mode!"<<endl;
		else break;
	} 
	if (a==27)
		kill(path);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	
}

bool find(PROCESSENTRY32 p){
	for (int i=0;i<PE.size();i++){
		if (PE[i].th32ProcessID==p.th32ProcessID)
			return true;
	return false;
	}
}

void checkProcess() {
	cout<<"------------------------------------------PROCESS LISTING-------------------------------------------"<<endl;
	HANDLE hSnapShot =INVALID_HANDLE_VALUE;
	PROCESSENTRY32 ProcessInfo ={0};
	ProcessInfo.dwSize =sizeof(PROCESSENTRY32);
	int count =0;
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(INVALID_HANDLE_VALUE == hSnapShot) {
		cout<<"CreatToolhelp32SnapShot Function Failed" <<endl;
		cout<< "Error No - "<<GetLastError()<<endl;
	}
	DWORD exitCode=0;
	cout<<"No.\t Number of Threads\tExecution File\t\tPPID\t\t PID\t\tStatus"<<endl;
	while(Process32Next(hSnapShot, &ProcessInfo) !=FALSE) {
		if(processList.find(ProcessInfo.szExeFile)==processList.end()) continue;
		cout<<"----------------------------------------------------------------------------------------------------"<<endl;
		if (find(ProcessInfo))
			cout<<++count<<".\t\t"<<ProcessInfo.cntThreads<<"\t\t"<<ProcessInfo.szExeFile<<"\t\t"<<ProcessInfo.th32ParentProcessID<<"\t\t"<<ProcessInfo.th32ProcessID<<"\t\tPending"<<endl;
		else
			cout<<++count<<".\t\t"<<ProcessInfo.cntThreads<<"\t\t"<<ProcessInfo.szExeFile<<"\t\t"<<ProcessInfo.th32ParentProcessID<<"\t\t"<<ProcessInfo.th32ProcessID<<"\t\tRunning"<<endl;
		cout<<"----------------------------------------------------------------------------------------------------"<<endl;

		}
	CloseHandle(hSnapShot);
}
void resume(){
	int n=PE.size();
	char pname[100];
	if (n==0){
	cout<<"No pending processes!"<<endl;
	return;
	}
	cout<<"---------------PENDING CHILD PROCESSES--------------"<<endl;
	for (int i=0;i<n;i++){
		cout<<i+1<<". "<<PE[i].szExeFile<<" - PID: "<< PE[i].th32ProcessID<<endl;
	}
	cout<<"Enter the process name to resume: ";
	fflush(stdin);
	gets(pname);
	for (int i=0;i<n;i++){
		if (!strcmp(PE[i].szExeFile,pname)){
			DebugActiveProcessStop(PE[i].th32ProcessID);
			PE.erase(PE.begin()+i);
		}
	}
}
void pause(){
	char pname[100];
	string name="ourShell.exe";
	HANDLE hSnapShot ;
	PROCESSENTRY32 ProcessInfo ;
	ProcessInfo.dwSize =sizeof(PROCESSENTRY32);
	int count =0;
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(INVALID_HANDLE_VALUE == hSnapShot) {
		cout<<"CreateToolhelp32SnapShot Function Failed" <<endl;
		cout<< "Error No - "<<GetLastError()<<endl;
	}
	DWORD exitCode=0;
	cout<<"-------------RUNNING CHILD PROCESSES-------------"<<endl; 
	if( Process32First( hSnapShot, &ProcessInfo ) )
    {
        do{
             	if(processList.find(ProcessInfo.szExeFile)==processList.end() || find(ProcessInfo)) continue;
             	if(ProcessInfo.szExeFile==name) continue;
				cout<<++count<<". "<<ProcessInfo.szExeFile<<" - PID: "<< ProcessInfo.th32ProcessID<<endl;
        } while( Process32Next( hSnapShot, &ProcessInfo ) );
    }
    if (count==0) cout<<"No running child processes!"<<endl;
	else {
	cout<<"Enter the process name to pause: ";
	fflush(stdin);
	gets(pname);
	if( Process32First( hSnapShot, &ProcessInfo ) )
    {
        do{
            if(!strcmp(ProcessInfo.szExeFile,pname))
            {
				PE.push_back(ProcessInfo);
				countProc++;
				DebugActiveProcess(ProcessInfo.th32ProcessID);
				break;
            }
        }while( Process32Next( hSnapShot, &ProcessInfo ) );
    }
	}
    CloseHandle(hSnapShot);
	CloseHandle(&ProcessInfo);
}
void help() {
	cout<<"LIST OF COMMAND: "<<endl;
	cout<<"1. help"<<endl;
	cout<<"2. checkProcess"<<endl;
	cout<<"3. checkThread"<<endl;
	cout<<"4. calculator"<<endl;
	cout<<"5. clear"<<endl;
	cout<<"6. killProcess"<<endl;
	cout<<"7. clock"<<endl;
	cout<<"8. exit"<<endl;
	cout<<"9. pause"<<endl;
	cout<<"10. resume"<<endl;
	cout<<"11. dir"<<endl;
}
void clear() {
	system("cls");
	cout<<"                      SHELL PROJECT                      "<<endl;
	cout<<"---------------------------------------------------------"<<endl;
	cout<<"AUTHOR: Ngoc Bao Nguyen, Hao NQ, Bang Nguyen , Truong Vu "<<endl;
	cout<<"---------------------------------------------------------\n"<<endl;
	help();
}
void checkThread() {
	map < int ,string> ppip;
	HANDLE hhSnapShot =INVALID_HANDLE_VALUE;
	PROCESSENTRY32 hProcessInfo ={0};
	hProcessInfo.dwSize =sizeof(PROCESSENTRY32);
	int count =0;
	hhSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(INVALID_HANDLE_VALUE == hhSnapShot) {
		cout<<"CreatToolhelp32SnapShot Function Failed" <<endl;
		cout<< "Error No - "<<GetLastError()<<endl;
	}
	while(Process32Next(hhSnapShot, &hProcessInfo) !=FALSE) {
		if(processList.find(hProcessInfo.szExeFile)==processList.end()) continue;
		ppip[hProcessInfo.th32ProcessID]=hProcessInfo.szExeFile;
		}
	CloseHandle(hhSnapShot);
	cout<<"------------------THREAD LISTING------------------"<<endl;
	HANDLE hSnapShot =INVALID_HANDLE_VALUE;
	THREADENTRY32 ThreadInfo ={0};
	ThreadInfo.dwSize =sizeof(THREADENTRY32);
	count =0;
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
}
void SetTeColor(WORD color){ 
    HANDLE hConsoleOutput;
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
    WORD wAttributes = screen_buffer_info.wAttributes;
    color &= 0x000f;
    wAttributes &= 0xfff0; wAttributes |= color;
    SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}
void killProcess(){ 
	cout<<"################################################################################"<<endl; 
	cout<<"######################### Task Process Terminator ##############################"<<endl; 
	cout<<"################################################################################"<<endl; // string added for ui purposes
	cout<<""<<endl;
	cout<<""<<endl;
	cout<<""<<endl;
	int run_this_program=1; //parameter which determines if program will keep running
	// while program is running (this section is intended to repeat until user exits program)
		//initialize objects@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		HANDLE hProcessSnap; //create handle identifier for object hprocesssnap
	  	HANDLE hProcess; //create handle identifier for object hprocess
	 	PROCESSENTRY32 pe32; //Describes an entry from a list of the processes residing in the system address space when a snapshot was taken.
	 	DWORD dwPriorityClass; //create dword class dwPriorityClass
	 	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 ); //take "snapshot" of processes running
	 	pe32.dwSize = sizeof( PROCESSENTRY32 ); //size of PROCESSENTRY32 [unused]
	 	vector <string> process_name; //initialize vector named "process_name" where each element will be a string (process name)
	 	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	
		// record all detetected running processes #######################################
		while( Process32Next( hProcessSnap, &pe32 ) ){ //for each object (pe32) in hprocesssnap [Process32Next runs through processes]
			process_name.push_back(pe32.szExeFile); // add process name to list of processes		
		};
		// Get list of unique process names and number of times that process name has process$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		vector <string> process_name_unique; //initialize vector of string elements (each element will be unique process name)
		vector <int> process_num_occur; //initialize vector of integer elements (each element will be count that unique process has occured)
		process_name_unique.push_back(process_name[3]); //initialise first process; start from 3 [0=blank 1='sys' var 2=another title 3=first process listed]
		process_num_occur.push_back(1); //initialise count for first process
		for(int k=3;k<=process_name.size()-1;k++){ // for each kth process name in complete list
			int add_unique=1; //initialise parameter that tells if proccess is unique (=1)
			for(int j=0;j<=process_name_unique.size()-1;j++){ // for each jth process in list of unique process names
				string st1=process_name[k]; //string 1 = process name from complete list (k)
				string st2=process_name_unique[j]; // process name from unique list (j)
				if(strcmp(st1.c_str(),st2.c_str())==0 && add_unique==1 ){ //if process names j&k match and if unique parameter=1 [c_str() converts cmmdtsk to c string, needed for strcmp]
																		  // note: str(a,b)=0 if string a and b are identical match
					process_num_occur[j]+=1; //add to count for re-occurance of process name
					add_unique=0; //set unique process parameter to 0 [process name from list k is already in list j]
				}; // if loop
			};  // for loop (j)
			if(add_unique==1){ //if process name from list k was unique (to be added to list)
				process_name_unique.push_back(process_name[k]); //add process name from list k to list of unique process
				process_num_occur.push_back(1);	 //initialise count of occurance for newly added process name
			}; // if loop
		};// for loop (k)
		//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		
		// sort process name alphabetically and then also sort "num occurance" accordingly&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
		vector <string> process_name_unique_unsorted; //initialize vector that will be unsorted vector of process names;
		vector <int> process_num_occur_sorted; //initialize vector that will be unsorted vector of process names;
		for(int k=0;k!=process_name_unique.size();k++) process_name_unique_unsorted.push_back(process_name_unique[k]); //add elements from original list to new list [for loop in one line]
		sort(process_name_unique.begin(),process_name_unique.end(),compareFunction); // sortvector using compare function (output is that process_name_unique is sorted alphabetically using function "compareFunction")
		// sort process_num_occur alphabetically and as new vector "process_num_occur_sorted" (previously initialized but is still empty@@@@@@@@@@@@@@@@@@@@@@@@@@@
		for(int k=0;k!=process_name_unique.size();k++) for(int j=0;j!=process_name_unique.size();j++){ // for each element of sorted(k) and unsorted (j) for process name [Double for loop defined in one line]
			if(strcmp(process_name_unique[k].c_str(),process_name_unique_unsorted[j].c_str())==0) process_num_occur_sorted.push_back(process_num_occur[j]); //if string of kth string (sorted) and jth string (unsorted) match;
		};                                                                                                                                                  //then, add jth process_num_occur element to new list
		//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
		
		// Print unique processes %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		SetTeColor(2);
		cout<<"---------------Processes that are running------------------"<<endl; // string added for UI purposes
		cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<endl;
		cout<<"PROCESS NAME || Number of processes of same name                                "<<endl; // string added for UI purposes
		for(int k=0;k<=(process_name_unique.size()-1);k++){ // for each kth element in sorted list of process names
		    	if(processList.find(process_name_unique[k])==processList.end()) continue;
			cout<<process_name_unique[k]; // print kth process name
			cout<<" || "<<process_num_occur_sorted[k]<<endl; // print number of occurances for kth process name (also previously assorted correspondingly with vector above)
		};
		cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<endl; // string added for UI purposes
		SetTeColor(15);
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	    string p2t;
	    cout<<endl<<endl;
		cout<<"For example, enter 'chrome.exe' to terminate all processes of that handle name."<<endl; // string added for UI purposes
		cout<<"(exit the program if you do not wish to progress)"<<endl; // string added for UI purposes
		cout<<""<<endl; // string added for UI purposes
		cout<<"Process (name) to terminate:"; // string added for UI purposes
		cin>>p2t; // [USER INPUT FOR PROCESS NAME HANDLE OF PROCESSES/S TO TERMINATE]
		std::string namep = p2t; //convert p2t to string for use below (had to this for system() to work, yet to be optimized from this)
		std::string cmmdtsk="taskkill /IM "+namep+" /F"; //create full string to run in line below (had to this for system() to work, yet to be optimized from this)
		system(cmmdtsk.c_str()); //c_str() converts cmmdtsk to c string
		cout<<""<<endl; // string added for UI purposes
		cout<<""<<endl; // string added for UI purposes
}
void killAll() {
	  for (std::set<string>::iterator it=processList.begin(); it!=processList.end(); ++it){
		if(*it=="ourShell.exe" ) continue;
		string r= * it;
		kill(r);
		}
}
int main() {
	  map < string, char*>  demand; 
	 demand.insert( std::make_pair<string, char*>("checkThread", "" ) );
	 demand.insert( std::make_pair<string, char*>( "checkProcess", "" ) );
	 demand.insert( std::make_pair<string, char*>( "killProcess", "" ) );
	 demand.insert( std::make_pair<string, char*>( "help", "" ) );
	 demand.insert( std::make_pair<string, char*>( "calculator", "calculator.exe" ) );
	 demand.insert( std::make_pair<string, char*>( "clear", "" ) );
	 demand.insert( std::make_pair<string, char*>( "clock", "clock.exe" ) );
	 demand.insert( std::make_pair<string, char*>( "exit", "" ) );
	 demand.insert( std::make_pair<string, char*>( "resume", "" ) );
	 demand.insert( std::make_pair<string, char*>( "pause", "" ) );
	 demand.insert( std::make_pair<string, char*>( "dir", "dir.exe" ) );
	 processList.insert( "ourShell.exe" ); 
	 processList.insert(  "Calculator.exe" );
	 processList.insert( "clock.exe" );
	 processList.insert( "dir.exe" );
	 string dm;
	 clear();
	 char a;
	 while(true) {
	 	cout<<endl<<"Enter command: ";
	 	cin>>dm;
	 	if(dm=="exit") break;
	 	if(demand.find(dm)==demand.end()) {
	 		cout<<"The demand isn't available!"<<endl;
	 		continue;
	 	}
	 	if (dm=="pause"){
	 		pause();
	 		continue;
	 	}
	 	if (dm=="resume"){
	 		resume();
	 		continue;
	 	}
	 	if (dm=="help"){
	 		help();
	 		continue;
	 	}
	 	if (dm=="clear"){
	 		clear();
	 		continue;
	 	}
	 	if (dm=="checkProcess"){
	 		checkProcess();
	 		continue;
	 	}
	 	if (dm=="checkThread"){
	 		checkThread();
	 		continue;
	 	}
	 	if(dm=="killProcess") {
	 		    killProcess();
	 			continue;
	 	}
	    cout<<"1. Parallel (Processes run in parallel)"<<endl;
	    cout<<"2. One By One (Processes run one by one)"<<endl;
	    do {
		cout<<"Choose mode (1/2): ";
		cin>>a;
		} while (a!='1'&&a!='2');
	    if (a=='1') {
			char a;
			myCreateProcessParallel(demand[dm]);
	    }
	    else {
			char a;
			myCreateProcessOBO(demand[dm]);
	    }
	}
	killAll();
	return 0;
}
