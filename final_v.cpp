#include <bits/stdc++.h>
#include "parse.h"
using namespace std;



//////////////////////////////////////////////////////////////////////////////////////////////////////////////

 int main(int argc, char *argv[]){

    char* command_to_execute; 
    reading_command_line_arguments(argc, argv, &command_to_execute);
	  pid_t pid = -1;
    pid = fork();

	if(!pid){
        // char* tt = "firefox";
        // cout << command_to_execute << endl;
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

		init_sigaction(pid);
        init_time();
        // printf("You have only 10 seconds for thinking.\n");

        // Uncomment this part to get the details of the proc manually:-
        /*
        while(1){
			parse_proc(pid);
            cout << "Press a key to get the details again: ";
			cin >> temp;
		}
        */

        printf("The timer has started \n");
        // printf("You have only %d seconds for thinking.\n", limitt);
        while(1);
        cout << "Amazingly reached here! \n";

    }
	return 0;
}