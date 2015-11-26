#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h> 
#include <sys/stat.h>
#include <dirent.h> 
#include <signal.h>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define LISTENQ 8 /*maximum number of client connections*/

char dir_buf[5000];

void sigchld_handler(int s)
{

    	while(waitpid(-1, NULL, WNOHANG) > 0);

}


int main (int argc, char **argv)
{
	int listenfd, connfd, n;
	pid_t childpid;
	socklen_t clilen;
	char buf[MAXLINE];
	struct sockaddr_in cliaddr, servaddr;
	struct sigaction sa; /* deal with signals from dying children! */
	

	//Create a socket for the soclet
	//If sockfd<0 there was an error in the creation of the socket
	if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
		perror("Problem in creating the socket");
		exit(2);
	}

	//preparation of the socket address
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	//bind the socket
	bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	//listen to the socket by creating a connection queue, then wait for clients
	listen (listenfd, LISTENQ);
	printf("%s\n","Server running...waiting for connections.");

	while(1) {
		clilen = sizeof(cliaddr);
		//accept a connection
		connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);
		printf("%s\n","Received request...");
		if ( (childpid = fork ()) == 0 ) 
		{//if it’s 0, it’s child process
			printf ("%s\n","Child created for dealing with client requests");
			//close listening socket
			close (listenfd);
			getcwd(dir_buf, 5000);
			while ( (n = recv(connfd, buf, MAXLINE,0)) > 0)  {
				printf("%s","String received from and resent to the client:");
				puts(buf);
				char* command = strtok(buf, " ");
				printf("Command: %s\n", command);
				if (strcmp("LIST", command) == 0)
				{
					char response[10000];
					DIR *d;
					struct dirent *dir;
					d = opendir(dir_buf);
					if (d)
					{
						strcpy(response, "");
						while ((dir = readdir(d)) != NULL)
						{
							strcat(response, dir->d_name);
							strcat(response, "\n");
						}
						closedir(d);
					}
					write(connfd, response , strlen(response));
				}
				else
				{
					write(connfd, "Unknown command" , strlen("Unknown command"));	
				}
				
			}
			if (n < 0)
				printf("%s\n", "Read error");
			exit(0);
		}
		else
		{
		 	sa.sa_handler = sigchld_handler; // reap all dead processes
			sigemptyset(&sa.sa_mask);
			sa.sa_flags = SA_RESTART;
			if (sigaction(SIGCHLD, &sa, NULL) == -1) {
				perror("sigaction");
				exit(1);
			}
		}
		
		//close socket of the server
		close(connfd);
	}
}
