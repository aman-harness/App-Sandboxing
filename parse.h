#ifndef parse_header
#define parse_header

#include <bits/stdc++.h>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/times.h> 
#include <signal.h>
#include <sys/time.h>
#include <getopt.h>
#include <sstream>
#include <sys/types.h> 
#include <sys/wait.h> 

int signal_to_kill(pid_t , int , int );
int reading_command_line_arguments(int , char *[], char** );
void init_sigaction(int );
void init_time(void);
void sig_handler(int );

#endif