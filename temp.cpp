// http://stackoverflow.com/questions/6014391/want-the-excutable-run-by-execve-to-use-my-preloaded-library

// Shows how we can executea file with some environment variables.

#include <bits/stdc++.h>
#include <bits/stdc++.h>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h> 
#include <signal.h>
#include <sys/time.h>
#include <getopt.h>
#include <sstream>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <fcntl.h> 
#include <unistd.h> 


using namespace std;

int main(int argc, char *argv[]){

	// char *const args[] = {"./run",NULL};
	int file_read = open("input.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	int file_write = open("out.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

	dup2(file_read, 0);
	dup2(file_write, 1);   // make stdout go to file
	dup2(file_write, 2);   // make stderr go to file - you may choose to not do this
	               // or perhaps send stderr to another file

	close(file_write);     // file_write no longer needed - the dup'ed handles are sufficient

	char *const args[] = {argv[1] , NULL};
    char *const envs[] = {"LD_PRELOAD=./EasySandbox.so",NULL};

    execve(argv[1], args, envs);


}