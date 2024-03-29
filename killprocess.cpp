#include <windows.h> //include windows.h header file (needed for windows API)
#include <tlhelp32.h> //include tlhelp32.h
#include <tchar.h>	//tchar.h header file	
#include <iostream> //include iostream header file for input/output operations
#include <bits/stdc++.h> //include bits/stdc++.h header file (imports standard operations for c++)
#include <string> //include string header (needed for some string operations)
#include <vector> // include vector header file (need for creating vectors)
#include <algorithm>    // include algorithm header file (needed for operations such as sort)
using namespace std; //use std for namespace
bool compareFunction (std::string a, std::string b); // call to function which sorts strings in alphabetic order (where sorting is applied as if inputs are all lower-case)
void SetTeColor(WORD color)
{ 
    HANDLE hConsoleOutput;
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
    WORD wAttributes = screen_buffer_info.wAttributes;
    color &= 0x000f;
    wAttributes &= 0xfff0; wAttributes |= color;
    SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}
int main( ){ // main program @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// INTRODUCTION STRING #############################################################################################
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
	cout<<"################################################################################"<<endl; // string added for ui purposes
	cout<<"######################### Task Process Terminator ##############################"<<endl; // string added for ui purposes
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
		//##################################################################################
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
		std::sort(process_name_unique.begin(),process_name_unique.end(),compareFunction); // sortvector using compare function (output is that process_name_unique is sorted alphabetically using function "compareFunction")
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
		return 0;
}// end of main program @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


bool compareFunction (std::string a, std::string b){ //function for sorting strings (of vector) in alphabetic order [considering them as lower-case]
	for(int k=0;k<=a.size()-1;k++){ // convert each element in vector a (string) to lower case
		a[k]=tolower(a[k]); //convert kth element of vector a to lower case
	};
	for(int k=0;k<=b.size()-1;k++){ // convert each element in vector b (string) to lower case
		b[k]=tolower(b[k]); //convert kth element of vector b to lower case
	};
	return a<b; // return a<b (sorted)
};
