#include "header.h"

int main(int argc, char *argv[])
{
    if(argc != 3) 
    {
        cout << "Wrong number of arguments!" << endl;
        return -1;
    }

    int N = atoi(argv[1]), L = atoi(argv[2]);
    generate_numbers(N, L); 
    return 0;
}