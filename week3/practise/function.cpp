#include "header.h"

void print_info(char* filename, vector<string> options) 
{
    struct stat file_stat;
    if(stat(filename, &file_stat) == -1) 
    {
        perror(filename);
        return;
    }

    for(int j=0; j < options.size(); j++) 
    {
        if(options[j] == "-s") printf("%12d ", (int)file_stat.st_size);
        else if(options[j] == "-t") 
        {
            struct tm *timeinfo = localtime(&file_stat.st_ctime);
            char time_str[100];
            strncpy(time_str, asctime(timeinfo), sizeof(time_str) - 1);
            time_str[strlen(time_str) - 1] = '\0';
            printf("%s ", time_str);
        } 
        else if(options[j] == "-r") 
        {
            char permissions[10] = "---------";
            int modes[] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};
            char rwx[] = {'r', 'w', 'x'};
            for(int k=0; k < 9; k++) if(file_stat.st_mode & modes[k]) permissions[k] = rwx[k % 3];
            printf("%s ", permissions);
        }
    }
    printf("%s\n", filename);
}

void generate_numbers(int N, int L) 
{
    srand(time(NULL)); 
    while(1)
    {
        for(int i=0; i < L; i++)
        {
            for(int j=0; j < N; j++)
            {
                int num = rand()% (100 + 100 + 1) - 100;
                cout << num;
                if(j < N - 1) cout << " ";
            }
            cout << endl;
        }
        usleep(60000000);
    }
}

void monitor(char* filename) 
{
    struct stat file_stat;
    time_t last_mod_time = 0;
    off_t last_size = 0;

    while(1) 
    {
        if(stat(filename, &file_stat) == 0) 
        {
            if(file_stat.st_size != last_size || file_stat.st_mtime != last_mod_time) 
            {
                time_t now = time(0);
                char* dt = ctime(&now);
                printf("File size changed at %sOld size: %lld, New size: %lld\n", dt, last_size, file_stat.st_size);
                last_size = file_stat.st_size;
                last_mod_time = file_stat.st_mtime;
            }
        }
        usleep(500000);
    }
}