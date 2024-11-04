#include "header.h"
using namespace std;

int main(int argc, char *argv[]) 
{
    int t_L = atoi(argv[1]), t_N = atoi(argv[2]);
    bool use_hex = false;
    
    if(argc == 4 && strcmp(argv[3], "-x") == 0) use_hex = true;

    output(t_L, t_N, use_hex);
    return 0;
}
