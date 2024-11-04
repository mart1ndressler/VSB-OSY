#include "header.h"
using namespace std;

void output(int t_L, int t_N, bool use_float) 
{
    srand(time(NULL));

    for (int i = 0; i < t_L; i++) 
    {
        double sum = 0, num;
        (use_float) ? num = (rand() % (1000 + 1000 + 1) - 1000) / 10.0 : num = rand() % (1000 + 1000 + 1) - 1000;
        sum = num;
        cout << "(" << num << ")";

        for(int j = 1; j < t_N; j++) 
        {
            (use_float) ? num = (rand() % (1000 + 1000 + 1) - 1000) / 10.0 : num = rand() % (1000 + 1000 + 1) - 1000;
            int sign1 = rand() % 2;
            int sign2 = rand() % 2;

            if((sign1 == 0 && sign2 == 0) || (sign1 == 1 && sign2 == 1)) 
            {
                cout << " + ";
                sum = sum + num;
            }
            else 
            {
                cout << " - ";
                sum = sum - num;
            }

            cout << "(" << num << ")";
        }

        (use_float) ? cout << " = " << sum << endl : cout << " = " << (int)sum << endl;
    }
}