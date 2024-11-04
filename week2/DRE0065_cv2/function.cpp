#include "header.h"
using namespace std;

void output(int t_L, int t_N, bool use_hex) 
{
    srand(time(NULL)); 

    for(int i = 0; i < t_L; i++) 
    {
        for(int j = 0; j < t_N; j++) 
        {
            int num = rand()% (100 + 100 + 1) - 100;
            //SAME LIKE printf("0x%X", (num < 0 ? -num : num));
            (use_hex) ? cout << "0x" << hex << uppercase << ((num < 0) ? (-num) : num) : cout << "(" << num << ")";
            
            if(j < t_N - 1) 
            {
                int operation = rand() % 3;
                switch(operation) 
                {
                    case 0: cout << " * "; break;
                    case 1: cout << " / "; break;
                    case 2: cout << " % "; break;
                }
            }
        }
        cout << " = " << endl;
    }
}