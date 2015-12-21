// parseing ps output

// This file run ps-a and shows us the output file along with the virtual size they are using.
// Used to get the list of running processes.

#include <bits/stdc++.h>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/times.h> 

using namespace std;

#define MAXBUFLEN 1000000
// Global defineations : -
long long int clock_ticks_per_second = sysconf(_SC_CLK_TCK);

struct {
	int pgrp;
	int pid;
	int virtual_size;
	char ucmd[50];
} ps_output[500];

int parse_ps(pid_t pid){
	long long int utime_ticks, stime_ticks, cum_utime_ticks, cum_stime_ticks;
	char source[12800];
	// char * command = "cat /proc/0000/stat 2>/dev/null";

    char * command = "ps -A -o pgrp= -o pid= -o vsz= -o ucmd= ";

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

		pclose(pipe);
		source[strlen(source)-1] = '\0';
		int inp;
		cout << source << endl;
		int n = 1;
		cout << endl << endl << endl;
		int offset = 0, bytesRead, count = 0;
		while(n != EOF){
			n = sscanf(source + offset, "%d %d %d %s\n%n" , &ps_output[count].pgrp, &ps_output[count].pid, &ps_output[count].virtual_size, ps_output[count].ucmd, &bytesRead );
			cout << "From Stream: " <<ps_output[count].pgrp << " "  << ps_output[count].pid << " "  
			     << ps_output[count].virtual_size << " "  << ps_output[count].ucmd << endl;
			// cin >> inp;
			offset += bytesRead;
			count++;
		}

	}

	cout << " Finished With Parsing Proc " << endl;
}

int main(int argc, char *argv[]){
	pid_t pid = fork();
	int temp;
	if(!pid){
		// system("");
		
	}
	else{
		while(1){
			parse_ps(pid);
			cin >> temp;
		}
	}
	return 0;

}