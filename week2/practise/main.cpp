#include "header.h"
using namespace std;

int main(int argc, char *argv[]) 
{
    int L = atoi(argv[1]), N = atoi(argv[2]);
    srand(time(NULL));

    if(argc != 3) 
    {
        cout << "Wrong number of arguments!";
        return -1;
    }

    output(L, N);
    return 0;
}