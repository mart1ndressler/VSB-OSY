#include "header.h"

int main(int argc, char* argv[]) 
{
    vector<string> files;
    vector<off_t> last_sizes;
    int interval = 1000;

    if(argc > 2 && strcmp(argv[1], "-t") == 0) 
    {
        interval = atoi(argv[2]);
        if(interval <= 0) 
        {
            printf("Invalid interval!\n");
            return -1;
        }
        for(int i = 3; i < argc; i++) 
        {
            files.push_back(argv[i]);
            last_sizes.push_back(0);
        }
    }
    else 
    {
        for(int i = 1; i < argc; i++) 
        {
            files.push_back(argv[i]);
            last_sizes.push_back(0);
        }
    }

    if(files.empty()) 
    {
        printf("File is empty!\n");
        return -1;
    }

    while(1) 
    {
        for(int i = 0; i < files.size(); i++) 
        {
            char filename[256];
            strncpy(filename, files[i].c_str(), sizeof(filename) - 1);
            if(access(filename, W_OK) == 0) monitor(filename, last_sizes[i]);
            else if(access(filename, X_OK) != 0) monitor(filename, last_sizes[i]);
            else if(access(filename, W_OK) != 0 && access(filename, X_OK) == 0) printf("File %s does not have the required permissions.\n", filename);
        }
        usleep(interval * 1000);
    }
    return 0;
}