#include<iostream>
#include<stdlib.h>
using namespace std;

int main(){
    char *value;
    value = getenv("PATH");
    printf("PATH= %s", value);
}
