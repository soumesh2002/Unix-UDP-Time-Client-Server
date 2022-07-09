#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

int main(int argc, char *argv[]) {
	int net_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (net_socket < 0)
		perror("socket creation error...");
	else
		printf("socket creation completed...\n");

	struct sockaddr_in serv_addr;
	serv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
	serv_addr.sin_port = htons(atoi(argv[2]));
	serv_addr.sin_family = AF_INET;

	int len = sizeof(struct sockaddr_in);
	int br = bind(net_socket, (struct sockaddr *)&serv_addr, len);
	if (br < 0) {
		perror("bind error");
	} else {
		printf("bind done");
		len = sizeof(struct sockaddr *);
		getsockname(net_socket, (struct sockaddr *)&serv_addr, &len);
		printf("local ip: %s\n", inet_ntoa(serv_addr.sin_addr));
		printf("local port: %d\n", htons(serv_addr.sin_port));
	}

	int n;
	time_t tick;
	char buffer[200];

	while (1) {
		n = recvfrom(net_socket, buffer, 200, 0, (struct sockaddr *)&serv_addr, &len);
		if (n < 0) {
			perror("read failed");
		} else {
			buffer[n] = '\0';
			tick = time(NULL);
			snprintf(buffer, sizeof(buffer), "%s", ctime(&tick));		
			n = sendto(net_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, len);

			if (n < 0) {
				perror("write failed");
			}
		}
	}
	close(net_socket);
	return 0;
}
