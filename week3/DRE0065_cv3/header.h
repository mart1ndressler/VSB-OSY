#ifndef HEADER_H
#define HEADER_H

#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <vector> 
#include <iostream>
#include <fcntl.h>

using namespace std;
void print_info(char* filename, vector<string> options);
void generate_numbers(int N, int L);
void monitor(char* filename, off_t& last_size);

#endif