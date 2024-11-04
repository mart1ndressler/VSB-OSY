#include "header.h"
using namespace std;

void output(int t_L, int t_N) 
{
    for(int i=0; i < t_L; i++) 
    {
        int sum = 0;
        for(int j=0; j < t_N; j++) 
        {
            int num = rand()%(1000+1000+1) - 1000;
            cout << num << " ";
            sum = sum + num;
        }
        cout << sum << endl;
    }
}