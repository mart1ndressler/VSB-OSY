#include "header.h"

int main(int argc, char* argv[]) 
{
    if(argc != 2) 
    {
        cout << "Wrong number of arguments!" << endl;
        return -1;
    }
    monitor(argv[1]);
    return 0;
}