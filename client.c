#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <poll.h>
#include <arpa/inet.h>

void sig_handel(int sig) {
	printf("time out has occured\n");
	exit(0);
}

int main(int argc, char *argv[]) {
	signal(SIGCHLD, sig_handel);
	int net_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (net_socket < 0) {
		perror("socket creation failed");
	} else {
		printf("socket created succesfully\n");
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));
	serv_addr.sin_family = AF_INET;


	int n, len;
	time_t tick;
	char buffer[200];
	while(fgets(buffer, 200, stdin) != NULL) {
		if (strcmp(buffer, "1\n") == 0) {
			len = sizeof(struct sockaddr_in);
			n = sendto(net_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, len);

			if (n < 0) { perror("write error"); }
			
			len = sizeof(struct sockaddr_in);
			alarm(5);
			n = recvfrom(net_socket, buffer, 200, 0, (struct sockaddr *)&serv_addr, &len);
			if (n < 0) perror("failed");
		} else {
			alarm(0);
			buffer[n] = '\0';
			printf("server sent: %s\n", buffer);
		}
	}
	close(net_socket);
	return 0;
}
