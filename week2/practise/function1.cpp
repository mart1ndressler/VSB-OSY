#include "header1.h"
using namespace std;

void input() 
{
    char line[1024];
    int row = 1;

    while(fgets(line, sizeof(line), stdin)) 
    {
        int index = 0, sum = 0, number, move, last_number;
        bool is_last = false;

        while(sscanf(line + index, "%d%n", &number, &move) == 1) 
        {
            index = index + move;
            if(sscanf(line + index, "%d%n", &last_number, &move) != 1) 
            {
                is_last = true;
                break;
            }

            sum = sum + number;
        }
        if(is_last) (number != sum) ? cout << "Error - line " << row << endl : cout << "Correct - line " << row << endl;
        row++;
    }
}