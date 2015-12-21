#include <bits/stdc++.h>
#include <unistd.h>

using namespace std;

int main(){
	pid_t pid = fork();
	if(!pid){
		// sleep(3);
		cout << "Child's Pid : " << getpid() << " " <<  getpgid(0) << endl;
		system("./main.out");
	}
	else{
		cout << "Parent's Pid : " << getpid() << " " <<  getpgid(0) << endl;
	}
}
