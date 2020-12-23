

#include<stdio.h>
#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
#include "DVRalgo.hpp"

using namespace std;

int main()
{
    bool running;
    string name, answer;
    
    do{
    
        
        cout << "What is the topology file name, give in the following format: (filename.txt) :  ";
        cin >> name;
        
        runsimulation(name);
        
        cout << endl;
        
        cout << "Do you want to run another topology file? (Answer Exactly : YES or NO):  ";
        cin >> answer;
        
        cout << endl<< endl;
        
    }
    while(answer == "YES");
    
    return 0;
}

