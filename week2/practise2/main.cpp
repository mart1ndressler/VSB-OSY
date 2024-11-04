#include "header.h"
using namespace std;

int main(int argc, char *argv[]) 
{
    if(argc < 3 || argc > 4) 
    {
        cout << "Wrong number of arguments!";
        return -1;
    }

    int L = atoi(argv[1]), N = atoi(argv[2]);
    bool use_hex = false;

    for(int i = 3; i < argc; i++) if(strcmp(argv[i], "-x") == 0) use_hex = true;

    output(L, N, use_hex);
    return 0;
}