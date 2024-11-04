#include "header.h"

void generate_numbers(int N, int L)
{
    srand(time(NULL)); 
    int line_number = 1;
    while(1)
    {
        printf("%d: ", line_number++);
        for(int j=0; j < N; j++)
        {
            int num = rand()% (100 + 100 + 1) - 100;
            printf("%d", num);
            if(j < N - 1) printf(" ");
        }
        printf("\n");
        fflush(stdout);
        usleep(60000000 / L);
    }
}

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
        if(options[j] == "-s") printf("%12lld ", (long long)file_stat.st_size);
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
            for(int k = 0; k < 9; k++) if(file_stat.st_mode & modes[k]) permissions[k] = rwx[k % 3];
            printf("%s ", permissions);
        }
    }
    printf("%s\n", filename);
}

void monitor(char* filename, off_t& last_size)
{
    struct stat file_stat;
    if(stat(filename, &file_stat) == 0) 
    {
        time_t now = time(0);
        char* dt = ctime(&now);
        printf("--- %sVelikost: %lld, Jméno souboru: %s ---\n", dt, (long long)file_stat.st_size, filename);

        if(file_stat.st_size > last_size)
        {
            FILE* file = fopen(filename, "r");
            if(!file) 
            {
                perror("Error opening file");
                return;
            }

            fseek(file, last_size, SEEK_SET);
            char buffer[1024];
            int bytes_read;
            printf("> výpis nových dat:\n");
            while((bytes_read = fread(buffer, 1, sizeof(buffer) - 1, file)) > 0) 
            {
                buffer[bytes_read] = '\0';  
                printf("%s", buffer);  
            }

            fclose(file);

            last_size = file_stat.st_size;
            printf("-----------------------------------------\n");
        }
    }
}