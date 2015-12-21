// Run the command using exec in line 297 and this process keeps track of the memory consumed
// and time taaken by that pocess. You can also verify it from the terminal by running command 
// `top -p <pid_of_your_process> -d 1
// d sets the refresh rate of the display.

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
#define MAXBUFLEN 1000000

pid_t blackbox_pid;

// Global definations : -
long long int clock_ticks_per_second = sysconf(_SC_CLK_TCK);
struct {
    int pgrp;
    int pid;
    int virtual_size;
    char ucmd[50];
} ps_output[500];


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                    The Parse Proc for calculating time                               //
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

long long int parse_proc(pid_t pid){
	long long int utime_ticks, stime_ticks, cum_utime_ticks, cum_stime_ticks;
	char buffer[1280];
	// char * command = "cat /proc/0000/stat 2>/dev/null";

	string call = "cat /proc/";
	call += to_string((int) pid);
	call += "/stat 2>/dev/null";
	const char *command = call.c_str();

	FILE* pipe = popen(command, "r");{

    if (pipe){
        while(!feof(pipe)){
            if(fgets(buffer, 1280, pipe) != NULL){}
        }
        pclose(pipe);
        buffer[strlen(buffer)-1] = '\0';
        cout << " Buffer length: " << strlen(buffer) << endl;
    
        // cout << buffer << endl;
        }
        else cout << "Error \n" ;

        char ** tokens = str_split(buffer, ' ');
        if (tokens)
        {
            cout << "In token :- ";
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
        
            // cout << "utime_ticks -- " <<utime_ticks << " " << "stime_ticks -- " <<stime_ticks << " \n" << 
            // "cum_utime_ticks -- " <<cum_utime_ticks <<  " " << "cum_stime_ticks -- " <<cum_stime_ticks << endl;

            free(tokens);
        }
        else cout << "Token Failed :/" << endl;
        
    }	

    cout << " Finished With Parsing Proc " << endl;

    return utime_ticks + stime_ticks;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                For Selecting all Processes                                       //
//////////////////////////////////////////////////////////////////////////////////////////////////////////


int parse_ps(pid_t pgid){

    int time_limit = 5000;

    long long int utime_ticks, stime_ticks, cum_utime_ticks, cum_stime_ticks;
    char source[100000];
    // char * command = "cat /proc/0000/stat 2>/dev/null";
    stack <pid_t> process_of_blackbox;
    char * command = "ps -A -o pgrp= -o pid= -o vsz= -o ucmd= ";

    long long int total_time = 0, total_memory = 0;
    FILE* pipe = popen(command, "r");
    if (pipe){
        

        // Gets wouldn't work :/  So rewrote it.
        if (pipe != NULL) {
            size_t newLen = fread(source, sizeof(char), MAXBUFLEN, pipe);
            if (newLen == 0) {
                fputs("Error reading file", stderr);
            } else {
                source[newLen++] = '\0'; /* Just to be safe. */
            }
        }
        cout << "Pgid " << pgid << endl;
        pclose(pipe);
        source[strlen(source)-1] = '\0';
        int inp;
        // cout << source << endl;
        int n = 1;
        cout << endl << endl << endl;
        int offset = 0, bytesRead, count = 0, match = 0;
        while(n != EOF){
            n = sscanf(source + offset, "%d %d %d %s\n%n" , &ps_output[count].pgrp, &ps_output[count].pid, &ps_output[count].virtual_size, ps_output[count].ucmd, &bytesRead );
            // cout << "From Stream: " <<ps_output[count].pgrp << " "  << ps_output[count].pid << " "  
            //      << ps_output[count].virtual_size << " "  << ps_output[count].ucmd << endl;
            // cin >> inp;
            offset += bytesRead;
            count++;

            if(pgid == ps_output[count].pgrp){ 
                process_of_blackbox.push(ps_output[count].pid);
                total_time += parse_proc(ps_output[count].pid);
                total_memory += ps_output[count].virtual_size;
                match++;
            }
        }

        cout << "No of process in ps: "<< count << endl << "match " << match << endl;
        cout << "Total time: - " << total_time << "Total Memory:- " << total_memory << endl;

    }
    cout << " Finished With Parsing Ps " << endl;
}






//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   The functions related to signal handling                               //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct itimerval val;
int limit = 500;
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
   pid_t pid = blackbox_pid;
   // pid_t pid = getpid();

   parse_ps(pid);
   // for_all_process();
   // parse_proc();
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

	pid_t pid = fork();
    blackbox_pid = pid;

	if(!pid){

        // char *args[] = { "./busy_wait.out" /*, /* other arguments */, NULL };
        setpgid(0, 0);
        execvp("/usr/bin/firefox", NULL);
        // execve("busy_wait.out", args, NULL);

	}
	else{

		init_sigaction();
        init_time();
        printf("You have only 10 seconds for thinking.\n");

        // Uncomment this part to get the details of the proc manually:-
        /*
        while(1){
			parse_proc(pid);
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