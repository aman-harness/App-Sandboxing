#include <bits/stdc++.h>
#include <unistd.h>

using namespace std;

// Use this time for setting the time of busy wait. 
#define TIME 10

void  ALARMhandler(int sig)

{
  signal(SIGALRM, SIG_IGN);          /* ignore this signal       */
  printf("In Busy_wait: - Alarm recieved \n");
  signal(SIGALRM, ALARMhandler);     /* reinstall the handler    */
  exit(0);
}

int main(){
	signal(SIGALRM, ALARMhandler);
	cout << "In Busy_wait: - My PID is : " << getpid() << endl;
	alarm(TIME);

	while(1);
}
