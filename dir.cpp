#include<bits/stdc++.h>
#include<dirent.h>
#include<iostream>
using namespace std;

int main(){
    cout << "Input path: ";
    string path;
    getline(cin, path);
    cout << path;
    DIR* dir;
    dirent* pdir;
    vector<string> files;
    dir = opendir(path.c_str());
    while (pdir = readdir(dir)) {
        files.push_back(pdir->d_name);
    }
    for(int i = 0; i < files.size(); ++i){
        cout << files[i] << '\n';
    }
    system("pause");
    return 0;
}
