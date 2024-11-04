#include "header1.h"
using namespace std;

void input() 
{
    char line[1024];
    int row = 1;

    while (fgets(line, sizeof(line), stdin)) 
    {
        int index = 0, result = 0, number, move;
        char op = 0;

        if(sscanf(line + index, "0x%X%n", &result, &move) == 1) 
        {
            index = index + move;

            while(sscanf(line + index, " %c 0x%X%n", &op, &number, &move) == 2) 
            {
                index = index + move;
                
                switch(op) 
                {
                    case '*': result = result * number; break;
                    case '/':
                        if(number != 0) result = result / number;
                        else 
                        {
                            cout << "Division by zero on line " << row << endl;
                            return;
                        } break;
                    case '%':
                        if(number != 0) result = result % number;
                        else 
                        {
                            cout << "Modulo by zero on line " << row << endl;
                            return;
                        } break;
                    case '=': break;
                }
            }
        }

        printf("Result for line %d: 0x%X\n", row, result);
        row++;
    }
}