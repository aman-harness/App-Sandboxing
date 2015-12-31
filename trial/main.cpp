// # A script which keeps measuring the memory usage of a process:-

#include <bits/stdc++.h>
#include <unistd.h>

using namespace std;

int main(int argc, char* argv[]){
	pid_t xx;

	// process_id = fork();

	cout << " The pid of the main process : "  << (int) getpid()  << " " << getpgid(0) << endl;
	xx = fork();
	if(!xx){
		cout << "Inside File :- Child's : - " << getpid() << " " << getpgid(0) << endl;
	}
	else{
		cout << "Inside File :- Parent's : - " << getpid() << " " << getpgid(0) << endl;
		while(1);
	}
	return 0;
}
