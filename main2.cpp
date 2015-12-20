// Since system calls don't return something, they can't be uswed to read result of 
// commands on terminal. Exec() family functions also can't be used because they eat-up 
// the whole process which is required for further operations.:-

#include <bits/stdc++.h>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/times.h> 

using namespace std;

// Global defineations : -
long long int clock_ticks_per_second = sysconf(_SC_CLK_TCK);

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = (char ** )malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

int call_parse_proc(pid_t pid){
		long long int utime_ticks, stime_ticks, cum_utime_ticks, cum_stime_ticks;
	char buffer[1280];
	// char * command = "cat /proc/0000/stat 2>/dev/null";

	string call = "cat /proc/";
	call += to_string((int) pid);
	call += "/stat 2>/dev/null";
	const char *command = call.c_str();

	FILE* pipe = popen(command, "r");
if (pipe)
{
while(!feof(pipe))
{
if(fgets(buffer, 1280, pipe) != NULL){}
}
pclose(pipe);
buffer[strlen(buffer)-1] = '\0';

// cout << buffer << endl;

}

char ** tokens = str_split(buffer, ' ');
if (tokens)
{
    int i;

    // for (i = 13; *(tokens + i); i++)
    // {
    //     printf("month= %d - [%s]\n",i,  *(tokens + i));
    //     // free(*(tokens + i));
    // }

    utime_ticks = atoi(*(tokens + 13));
    stime_ticks = atoi(*(tokens + 14));
    cum_utime_ticks = atoi(*(tokens + 15));
    cum_stime_ticks = atoi(*(tokens + 16));

    cout << utime_ticks << " " << stime_ticks << " " << cum_utime_ticks <<  " " << cum_stime_ticks << endl;
    free(tokens);
}

	cout << " Finished With ParsingProc " << endl;
}


int main(int argc, char *argv[]){
	pid_t pid = fork();
	int temp;
	if(!pid){
		system("./trial_1.out");
	}
	else{
		while(1){
			call_parse_proc(pid);
			cin >> temp;
		}
	}
	return 0;

}