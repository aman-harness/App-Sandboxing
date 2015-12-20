// As this link says: 
// This implementation is not foolproof: http://stackoverflow.com/questions/8714016/c-calling-a-function-at-exact-every-second
// This can be improved using timer_create() functions.
// If the called functions don't end up in allocated time, ther could be weird results.

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <bits/stdc++.h>

using namespace std;
struct itimerval val;
int limit = 10;
/* signal process */
void timeout_info(int signo)
{
   if(limit == 0)
   {
       printf("Sorry, time limit reached.\n");
       // exit(0);
       val.it_interval.tv_sec = 0;
   }
   printf("only %d senconds left.\n", limit--);
}

/* init sigaction */
void init_sigaction(void)
{
    struct sigaction act;

    act.sa_handler = timeout_info;
    act.sa_flags   = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGPROF, &act, NULL);
} 

/* init */
void init_time(void)
{
    struct itimerval val;

    val.it_value.tv_sec = 1;
    val.it_value.tv_usec = 0;
    val.it_interval = val.it_value;
    val.it_value.tv_sec = 1;
    setitimer(ITIMER_PROF, &val, NULL);
}

int main(void)
{
    init_sigaction();
    init_time();
    printf("You have only 10 seconds for thinking.\n");

    while(limit);
    // exit(0);
    cout << "Amazingly reached here! \n";
}