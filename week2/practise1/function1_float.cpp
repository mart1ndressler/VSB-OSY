#include "header1.h"
using namespace std;

void input() {
    char line[1024];
    int row = 1;

    while (fgets(line, sizeof(line), stdin)) 
    {
        int index = 0, read_chars;
        float sum = 0, num;
        char op = '+';
        bool first_num = true;

        while(sscanf(line + index, "%f%n", &num, &read_chars) == 1) 
        {
            index = index + read_chars;

            if(first_num) 
            {
                sum = num;
                first_num = false;
            } 
            else 
            {
                if(op == '+') sum = sum + num;
                else if(op == '-') sum = sum - num;
            }

            if(sscanf(line + index, " %c%n", &op, &read_chars) == 1) 
            {
                index += read_chars;
                if(op == '=') break;
            } 
            else break;
        }

        char *equal_pos = strchr(line, '=');
        if(equal_pos != nullptr) 
        {
            float expected_sum;
            if(sscanf(equal_pos + 1, "%f", &expected_sum) == 1) 
            {
                if(sum == expected_sum) cout << "Correct - line " << row << endl;
                else cout << "Error - line " << row << endl;
            }
        } 
        else cout << "Missing '=' symbol." << endl;
        row++;
    }
}