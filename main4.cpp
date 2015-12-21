#include <bits/stdc++.h>
#include <unistd.h>

using namespace std;

int main(){
	pid_t pid = fork();
	if(!pid){
		// sleep(3);
		cout << "Inside main4.out :- Child's Pid : " << getpid() << " " <<  getpgid(0) << endl;
		// system("./main.out");
		char *args[] = { "./main.out" /*, /* other arguments */, NULL };
		execve("main.out", args, NULL);
	}
	else{
		cout << "Inside main4.out :- Parent's Pid : " << getpid() << " " <<  getpgid(0) << endl;
	}
}
