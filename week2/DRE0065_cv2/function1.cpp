#include "header1.h"
using namespace std;

void input() 
{
    char line[1024];
    int row = 1;

    while(fgets(line, sizeof(line), stdin)) 
    {
        int index = 0, result = 0, number, move;
        char op = 0;
        bool first_num = true;

        while(sscanf(line + index, "%d%n", &number, &move) == 1) 
        {
            index = index + move;

            if(first_num == true) 
            {
                result = number;
                first_num = false;
            }
            else 
            {
                switch(op) 
                {
                    case '*': result = result * number; break;
                    case '/':
                        if(number != 0) result = result / number;
                        else 
                        {
                            cout << "Division by zero on line" << row << endl;
                            return;
                        } break;
                    case '%':
                        if(number != 0) result = result % number;
                        else 
                        {
                            cout << "Modulo by zero on line " << row << endl;
                            return;
                        } break;
                }
            }

            if(sscanf(line + index, " %c%n", &op, &move) == 1) 
            {
                index = index + move;
                if(op == '=') break;
            } 
            else break;
        }
        cout << "Result for line " << row << ": " << result << endl;
        row++;
    }
}
