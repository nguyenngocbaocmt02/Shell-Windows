#include <iostream>
#include <windows.h>
#include <cstring>
#include "tchar.h"

using namespace std;

void SetUserVariablePath(){
    HKEY hkey;
    long regOpenResult;
    const char key_name[] = "Environment";
    cout << "Enter value path: ";
    string new_value;
    cin >> new_value;
    const char *path=new_value.c_str();                                               //new_value path need to update 
    regOpenResult = RegOpenKeyEx(HKEY_CURRENT_USER,key_name, 0, KEY_ALL_ACCESS, &hkey);
    cout << "Enter variable name: ";
    string var;
    cin >> var;
    LPCSTR stuff = var.c_str();                                                   //Variable Name 
    RegSetValueEx(hkey,stuff,0,REG_SZ,(BYTE*) path, strlen(path));
    RegCloseKey(hkey);
}



void GetUserVariablePath(){
    static const char path[] = "VVS_LOGGING_PATH" ;                                      //Variable Name 
    static BYTE buffer1[1000000] ;
    DWORD buffsz1 = sizeof(buffer1) ;
    {
        //HKEY_CURRENT_USER\Environment
        const char key_name[] = "Environment";
        HKEY key ;

        if( RegOpenKeyExA( HKEY_CURRENT_USER, key_name, 0, KEY_QUERY_VALUE, std::addressof(key) ) == 0 &&
            RegQueryValueExA( key, path, nullptr, nullptr, buffer1, std::addressof(buffsz1) ) == 0 )
        {
            std::cout << "The updated value of the user variable is :  " << reinterpret_cast<const char*>(buffer1) << '\n' ;
        }
    }
}

int main()
{   
    SetUserVariablePath();
    //GetUserVariablePath();
    return 0;
}