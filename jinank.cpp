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
#include <getopt.h>
#include <sstream>

using namespace std;

// Reasons for the ending of programme.
#define EXCEED_LIMIT 1
#define SIG_INT 2
#define SIG_TERM 3
#define SIG_QUIT 4

struct {
    int time;       // Timing in seconds
    float frequency;  // No of queries per second
    int memory;     // Memory Limit
} limit;

int counter = 0;
// For defining the arguments of getopt_long
int getopt(int argc, char * const argv[],
           const char *optstring);

extern char *optarg;
extern int optind, opterr, optopt;

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
//            The Functions Related to Killing the processes in case of limit violations                //
//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Note: `int kill(pid_t <process_group>, int SIGTERM);` can't be directly used.
// See: http://stackoverflow.com/questions/3219001/should-i-be-worried-about-the-order-in-which-processes-in-a-process-goup-receiv


int signal_to_kill(pid_t pgrp, int total_memory, int total_time){
    cout << "Killing Process of group: " << pgrp << endl;
    // Do error handling here. On successful execution kill returns 0;
    kill(pgrp, SIGSTOP);
    kill(pgrp, SIGKILL);
    kill(pgrp, SIGCONT);
}

int exit_gracefully(pid_t pgrp, int total_memory, int total_time, int reason){

    switch (reason) {    
        case EXCEED_LIMIT:
            if(total_time > limit.time)
                fprintf (stderr, "Programme terminating due because of exceeding time limits(%d)", total_time);
            else if(total_memory > limit.memory)
                fprintf (stderr, "Programme terminating due because of exceeding memory limits(%d)", total_memory);
            break;

        case SIG_TERM:
            fprintf (stderr, "Received Terminations signal. Terminating Itself and Blackbox Process");
            break;
        case SIG_QUIT:
            fprintf (stderr, "Received Dump Core signal. Terminating Itself and Blackbox Process");
            break;
        case SIG_INT:
            fprintf (stderr, "Received Interrupt signal. Terminating Itself and Blackbox Process");
            break;

        default:
            fprintf (stderr, "Exiting due to unknown reasons. Terminating Itself and Blackbox Process");
    }
    signal_to_kill(pgrp,  total_memory,  total_time);

    // The periodic memory and time checking needs to be stopped.

    exit(0);
}

void sig_handler(int signo)
{
  pid_t pgrp = blackbox_pid; 
  if (signo == SIGINT)
    exit_gracefully(blackbox_pid, 0, 0, 2);
  if(signo == SIGTERM)
    exit_gracefully(blackbox_pid, 0, 0, 3);
  if(signo == SIGQUIT)
    exit_gracefully(blackbox_pid ,0 , 0, 4);
}

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
            // if(idx >= count) return NULL;

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
    int i;
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
        cout << " Proc length: " << strlen(buffer) << "With Counter " << counter << endl;
    
        // cout << buffer << endl;
        }
        else cout << "Error \n" ;

        char ** tokens = str_split(buffer, ' ');
        if(tokens == NULL) {
            cout << "Token Empty\n";
            // sleep(1);
            // Exit gracefully.
            // return -1;
        }

        for (i = 0; *(tokens + i); i++);

        cout << "Token Size : " << i << endl;

        if(i != 52) {
            printf(" Proc Not Yet Ready. (%d), counter --  %d \n", i, counter);
            // sleep(1);
            // cout << buffer << endl;
            // return -1;
        }

        else{
            int i; 
            printf(" Proc Ready. (%d), counter --  %d \n", i, counter);
            utime_ticks = atoi(*(tokens + 13));
            stime_ticks = atoi(*(tokens + 14));
            cum_utime_ticks = atoi(*(tokens + 15));
            cum_stime_ticks = atoi(*(tokens + 16));
            
            for (i = 1; *(tokens + i); i++)
            {
                // printf("month= %d - [%s]\n",i,  *(tokens + i));
                free(*(tokens + i));
            }
            // cout << "utime_ticks -- " <<utime_ticks << " " << "stime_ticks -- " <<stime_ticks << " \n" << 
            // "cum_utime_ticks -- " <<cum_utime_ticks <<  " " << "cum_stime_ticks -- " <<cum_stime_ticks << endl;

            free(tokens);
        }
        // else cout << "Token Failed :/" << endl;
    }	

    // cout << " Finished With Parsing Proc " << endl;

    return utime_ticks + stime_ticks;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                             For Selecting all Processes                              //
//////////////////////////////////////////////////////////////////////////////////////////////////////////


int parse_ps(pid_t pgid){

    counter++;
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
        // cout << "Pgid " << pgid << endl;
        pclose(pipe);
        source[strlen(source)-1] = '\0';
        int inp;
        // cout << source << endl;
        int n = 1;

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
                int val = -1;
                while(val == -1){
                    cout << "Calling parse_proc With counter :" << counter << endl;
                    val = parse_proc(ps_output[count].pid);
                }
                total_time += val;
                // total_time += parse_proc(ps_output[count].pid);
                total_memory += ps_output[count].virtual_size;
                match++;
            }
        }

        cout << "No of process in ps: "<< count << endl << "match " << match << endl;
        cout << "Total time: - " << total_time << "Total Memory:- " << total_memory << endl;


cout << "----------------------------------------------------------------" << endl << endl;

        if(total_memory > limit.memory || total_time > limit.time){
            exit_gracefully(pgid, total_memory, total_time, EXCEED_LIMIT);
            // Disable the counter and exit gracefully

        }
    }
    // cout << " Finished With Parsing Ps " << endl;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   The functions related to signal handling                               //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct itimerval val;
int limitt = 500;
/* signal process */
void timeout_info(int signo)
{


    // IMPORTANT:- Apply Signal Masking :)
    // IMPORTANT
   if(limitt == 0)
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
   printf("only %d senconds left.\n", limitt--);
   
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
    int clocks = limit.frequency * 1000000;

    val.it_value.tv_sec = clocks / 1000000;
    val.it_value.tv_usec = clocks % 1000000;
    val.it_interval = val.it_value;
    // val.it_value.tv_sec = 1;
    // val.it_value.tv_sec = 

    // Time we need is absolute.
    setitimer(ITIMER_REAL, &val, NULL);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Interpreating the Command Line Arguments                               //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


int reading_command_line_arguments(int argc, char *argv[], char** command_to_execute){
      

   // Setting default options : 
  limit.time = 100; // Timing in seconds
  limit.memory = 1000000; // In KB.
  limit.frequency = 1; // Time interval after which the prpgramme should be monitored.

  string command;
  int c;
  static int tree_flag;
  while (1)
    {
      
      static struct option long_options[] =
        {
          /* These options set a flag. */
          {"tree",      no_argument,  &tree_flag, 1},
          {"group",     no_argument,  &tree_flag, 0},
          /* These options don’t set a flag.
             We distinguish them by their indices. */ 
          // {"add",       no_argument,       0, 'a'},
          // {"append",    no_argument,       0, 'b'},
          {"time",      required_argument, 0, 't'},
          {"memory",    required_argument, 0, 'm'},
          {"frequency", required_argument, 0, 'f'},
          {"help",      no_argument      , 0, 'h'},
          {0, 0, 0, 0}
        };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      // no colon afterwards ->  No arguments Required.
      // : -> compulsory argument.
      // :: -> Optional Arguments.
      c = getopt_long (argc, argv, "ht:m:f:",
                       long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;

      switch (c)
        {
        case 0:
          /* If this option set a flag, do nothing else now. */
          if (long_options[option_index].flag != 0)
            break;
          printf ("option %s", long_options[option_index].name);
          if (optarg)
            printf (" with arg %s", optarg);
          printf ("\n");
          break;

        case 'a':
          puts ("option -a\n");
          break;

        case 'b':
          puts ("option -b\n");
          break;

        case 't':
          printf ("option -t with value `%s'\n", optarg);
          limit.time = atoi(optarg);
          break;

        case 'm':
          printf ("option -m with value `%s'\n", optarg);
          limit.memory = atoi(optarg);
          break;

        case 'f':
          printf ("option -f with value `%s'\n", optarg);
          limit.frequency = atof(optarg);
          break;

        case 'h':
          printf ("Required Help.\n");
          break;

        case '?':
          /* getopt_long already printed an error message. */
          break;

        default:
          abort ();
        }
    }

  /* Instead of reporting ‘--tree’
     and ‘--group’ as they are encountered,
     we report the final status resulting from them. */
  if (tree_flag)
    puts ("tree flag is set");

  /* Print any remaining command line arguments (not options). */
  if (optind < argc)
    {
      printf ("non-option ARGV-elements: ");
      while (optind < argc){
            printf ("-- %s \n", argv[optind]);
            if(argv[optind] != NULL){
              cout << argv[optind] <<  " In\n";
              string temp(argv[optind]);
              command += temp;
              // command += ' ';
            }
            optind++;
      }
      putchar ('\n');
    }
    // A great function :  Converts Const char * to char *
    // http://stackoverflow.com/questions/12862739/convert-string-to-char
    *command_to_execute = strdup(command.c_str());
    cout << " -" << *command_to_execute << "-" << endl;

    // exit(0); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

 int main(int argc, char *argv[]){

    char* command_to_execute; 
    reading_command_line_arguments(argc, argv, &command_to_execute);
	  pid_t pid = -1;
    pid = fork();
    blackbox_pid = pid;

	if(!pid){
        cout << command_to_execute << endl;
        // char *args[] = { "./busy_wait.out" /*, /* other arguments */, NULL };
        setpgid(0, 0);
        // execvp("firefox", NULL);
        execvp(command_to_execute, NULL);
        // execve("busy_wait.out", args, NULL);

	}
	else{
        while(pid == -1);
        
        if (signal(SIGINT, sig_handler) == SIG_ERR)
            printf("\ncan't catch SIGINT\n");

        if (signal(SIGQUIT, sig_handler) == SIG_ERR)
         printf("\ncan't catch SIGINT\n");

        if (signal(SIGTERM, sig_handler) == SIG_ERR)
         printf("\ncan't catch SIGINT\n");

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
        printf("You have only %d seconds for thinking.\n", limitt);
        while(end_program);
        cout << "Amazingly reached here! \n";

    }
	return 0;
}