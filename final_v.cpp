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
#include <signal.h>
#include <sys/time.h>

using namespace std;

int end_program = 1;

pid_t pid;
// Global definations : -
long long int clock_ticks_per_second = sysconf(_SC_CLK_TCK);


// parse proc should be called for each process.


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                For all Process                                       //
//////////////////////////////////////////////////////////////////////////////////////////////////////////

get_all_process(pid_t group_pid){

    char buffer[1280];
    const char * command = "ps -A -o pgrp= -o pid= -o vsz= -o ucmd=";

    FILE* pipe = popen(command, "r");
    {if (pipe){
        while(!feof(pipe)){
            if(fgets(buffer, 1280, pipe) != NULL){}
        }
        pclose(pipe);
        buffer[strlen(buffer)-1] = '\0';
    
       

        
        
    }


}



//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                  The Parse Proc                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////

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

int call_parse_proc(){
	long long int utime_ticks, stime_ticks, cum_utime_ticks, cum_stime_ticks;
	char buffer[1280];
	// char * command = "cat /proc/0000/stat 2>/dev/null";

	string call = "cat /proc/";
	call += to_string((int) pid);
	call += "/stat 2>/dev/null";
	const char *command = call.c_str();

	FILE* pipe = popen(command, "r");
    {if (pipe){
        while(!feof(pipe)){
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
        
            cout << "utime_ticks -- " <<utime_ticks << " " << "stime_ticks -- " <<stime_ticks << " \n" << 
            "cum_utime_ticks -- " <<cum_utime_ticks <<  " " << "cum_stime_ticks -- " <<cum_stime_ticks << endl;
            free(tokens);
        }
    }	
    cout << " Finished With Parsing Proc " << endl;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   The functions related to signal handling                               //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct itimerval val;
int limit = 5;
/* signal process */
void timeout_info(int signo)
{
   if(limit == 0)
   {
       printf("Sorry, time limit reached.\n");
       // Original Code // exit(0);
       
       // To disable the alarm
       val.it_interval.tv_sec = 0;
       val.it_interval.tv_usec = 0;
       val.it_value.tv_sec = 0;
       end_program = 0;
       setitimer(ITIMER_PROF, &val, NULL);
   }
   printf("only %d senconds left.\n", limit--);
   
   // there should first be a call to all processes.
   for_all_process();
   call_parse_proc();
}

/* init sigaction */
void init_sigaction(void)
{
    struct sigaction act;

    act.sa_handler = timeout_info;
    act.sa_flags   = 0;
    sigemptyset(&act.sa_mask);
    // sigaction(SIGPROF, &act, NULL);
    if (signal(SIGALRM, (void (*)(int)) timeout_info) == SIG_ERR) {
      perror("Unable to catch SIGALRM");
      exit(1);
    }

} 

/* init */
void init_time(void)
{
    struct itimerval val;

    val.it_value.tv_sec = 1;
    val.it_value.tv_usec = 0;
    val.it_interval = val.it_value;
    // val.it_value.tv_sec = 

    // Time we need is absolute.
    setitimer(ITIMER_REAL, &val, NULL);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  int main(int argc, char *argv[]){
int main(){

	pid = fork();

	if(!pid){

		// system("./trial_1.out");
        char *args[] = { "./busy_wait.out" /*, /* other arguments */, NULL };
        execve("busy_wait.out", args, NULL);

	}
	else{

		init_sigaction();
        init_time();
        printf("You have only 10 seconds for thinking.\n");

        // Uncomment this part to get the details of the proc manually:-
        /*
        while(1){
			call_parse_proc(pid);
            cout << "Press a key to get the details again: ";
			cin >> temp;
		}
        */

        printf("The timer has started \n");
        printf("You have only %d seconds for thinking.\n", limit);
        while(end_program);
        cout << "Amazingly reached here! \n";

    }
	return 0;
}