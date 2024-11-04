#include "header.h"

int main(int argc, char *argv[]) 
{
    vector<string> options, missing_files;
    struct stat file_stat;
    char *files[argc];
    int file_count=0;

    if(argc < 2)
    {
        cout << "Wrong number of arguments!" << endl;
        return -1;
    }

    for(int i=1; i < argc; i++)
    {
        if(strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "-r") == 0) options.push_back(argv[i]);
        else files[file_count++] = argv[i];
    }

    for(int i=0; i < file_count; i++) 
    {
        if(stat(files[i], &file_stat) == -1) missing_files.push_back(string(files[i]));
        else print_info(files[i], options);
    }

    if(!missing_files.empty()) 
    {
        cout << "\nMissing files:\n";
        for(int i=0; i < missing_files.size(); i++) cout << missing_files[i] << endl;
    }
    return 0;
}