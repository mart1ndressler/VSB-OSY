#include "header.h"
using namespace std;

void output(int t_L, int t_N, bool use_hex) 
{
    srand(time(NULL));

    for(int i = 0; i < t_L; i++) 
    {
        int sum = 0, num;
        num = rand() % (1000 + 1000 + 1) - 1000;
        sum = num;

        (use_hex) ? cout << "(0x" << hex << uppercase << num << ")" : cout << "(" << num << ")";

        for(int j = 1; j < t_N; j++) 
        {
            num = rand() % (1000 + 1000 + 1) - 1000;
            int sign1 = rand() % 2;
            int sign2 = rand() % 2;

            if((sign1 == 0 && sign2 == 0) || (sign1 == 1 && sign2 == 1)) 
            {
                cout << " + ";
                sum += num;
            }
            else 
            {
                cout << " - ";
                sum -= num;
            }

            (use_hex) ? cout << "(0x" << hex << uppercase << num << ")" : cout << "(" << num << ")";

        }

        (use_hex) ? cout << " = 0x" << hex << uppercase << sum << endl : cout << " = " << sum << endl;
    }
}