// # Clears the doubt if why cutime should be added whle calculating the running time.

#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char* argv[]){

	// process_id = fork();
	int xx;
	// cin >> xx;
	// int *r  = (int *) malloc(sizeof(int) * 100000);
	long long int count;
	cout << "Hello Aman Singh" << endl;

	pid_t pid = fork();
	if(!pid){
		cout << " The pid of the child process : "  << (int) getpid()  << " " << getpgid(0) << endl;
		while(1) {
			count++;
			if (count >300000000000) break;
			// if (count >100000000000) break;
		}
	}

	else{
		cout << " The pid of the main process : "  << (int) getpid()  << " " << getpgid(0) << endl;
		int *x;
		// wait(x);
		while(1) 
			{
				count++;
				if (count >1500000000000) break;
			}
		cout << "Came out \n";
		cin >> count;
		// while(1);
		}
	return 0;
}
