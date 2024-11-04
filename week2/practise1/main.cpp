#include "header.h"
using namespace std;

int main(int argc, char *argv[]) 
{
    int L = atoi(argv[1]), N = atoi(argv[2]);
    bool use_float = (argc == 4 && strcmp(argv[3], "-f") == 0);
    srand(time(NULL));

    if(argc < 3 || argc > 4) 
    {
        cout << "Wrong number of arguments!";
        return -1;
    }

    output(L, N, use_float);
    return 0;
}