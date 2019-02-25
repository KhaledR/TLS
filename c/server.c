#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

void error(char *msg);
int create_listner(uint16_t port);
int read_from_client(int fd);

void error(char *msg) {
	perror(msg);    
     	exit(1);
}

int create_listner(uint16_t port) {
	int sockfd;
	struct sockaddr_in serv_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		error("socket");
	}
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            error("bind");
        }
	return sockfd;
}

int read_from_client(int fd) {
	char buffer[256];
        int nbytes;
	printf("Reading from client\n");
        nbytes = read(fd, buffer, 256);
	printf("The Data in the buffer from client:\n%s",buffer);
        if (nbytes < 0) {
                error("read");
        }
        else if (nbytes == 0) {
                return -1;
        }
        else {
        //      fprintf(stderr, "Server: got message: %s\n", buffer);
		printf("Buffer content in HEXDEC:\n");
	        for (int i = 0; i < sizeof(buffer) && buffer[i] != 10; i++) {
			printf("%02x ",buffer[i]);//hexdecimal
		}

        //      fprintf(stderr, "%s", buffer);
		printf("Send this Answer to the client: \n%s",buffer);     
                write(fd, buffer, strlen(buffer));
		return 0;
        }
}

int main(int argc, char *argv[]) {
	int sockfd, clilen;
	uint16_t port;

        if (argc < 2) {
                fprintf(stderr,"ERROR, no port provided\n");
                exit(1);
        }
        port = atoi(argv[1]);
        /* Create the socket and set it up to accept connections. */
        sockfd = create_listner(port);
        if (listen(sockfd, 10) < 0) {
                error("listen"); 
        }


         /* Connection request on original socket. */
	int new_fd;
	struct sockaddr_in cli_addr;
	clilen=sizeof(cli_addr);
	new_fd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (new_fd < 0) {
		error("accept");
	}
	read_from_client(new_fd);
#if 0
	int sockfd, newsockfd, portno, clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	
	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
	/* create socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		error("ERROR opening socket");
	}
	/* place n zero-valued bytes in the area pointed to by s */
	bzero((char *) &serv_addr, sizeof(serv_addr));
	/* Convert string to integer */
	portno = atoi(argv[1]); 
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	/* int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen); */
	if (bind(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)	{
		error("ERROR on binding");
	}
	/* int listen(int sockfd, int backlog); */	
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) {
		error("ERROR on accept");
	}	
	bzero(buffer,256);
	n = read(newsockfd,buffer,256);
	if (n < 0) {
		error("ERROR reading from socket");
	}
	printf("Here is the message: %s\n",buffer);
	/* return the file.bin to client */
	for (int i = 0; i < sizeof(buffer) && buffer[i] != 10; i++) {
		//printf("%d",buffer[i]);//decimal
		printf("%02x ",buffer[i]);//hexdecimal
	}
	printf("\n");
	
	FILE* tmp = tmpfile(); 
	int i = 0;
	if (tmp == NULL) { 
	    puts("Unable to create temp file"); 
	    return 0; 
	} 
	puts("Temporary file is created\n"); 
	while (buffer[i] != '\0') { 
	    fputc(buffer[i], tmp); 
	    i++; 
	} 	
	rewind(tmp);
	printf("My tmp file: ");
	while (!feof(tmp)) {
		putchar(fgetc(tmp)); 
	}
	n = write(newsockfd,"I got your message",18);
	if (n < 0) {
		error("ERROR writing to socket");
	}
#endif
	printf("\n");
	return 0;
}
