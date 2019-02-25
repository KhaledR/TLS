/*
 * $ ./client.out IpAddress Port InputFile
 */

#include <stdio.h>
//#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void error(char *msg);

void error(char *msg){
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[]){
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char bufferIN[256];
	char bufferOUT[256];

	if (argc < 4){
		fprintf(stderr,"usage %s hostname port filename\n", argv[0]);
		exit(0);
	}
	
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sockfd < 0){
		error("ERROR opening socket");
	}
	
	server = gethostbyname(argv[1]);/* webDomain = gethostbyname(websiteConnect); */
	if (server == NULL){
		fprintf(stderr,"ERROR, no such host \n");
		exit(0);
	}
	
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
	
	serv_addr.sin_port = htons(portno);
	
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		error("ERROR cant connect to server");
	}

//	printf("Please enter the message: ");
	bzero(bufferIN,256);
	FILE *f = fopen(argv[3], "rb");
	fread(bufferIN, 1, 256, f);
	n = write(sockfd,bufferIN,strlen(bufferIN));
		if (n < 0)
	{
		error("ERROR writing to socket");
	}
	bzero(bufferOUT,256);
	printf("Answer from server:\n");
	n = read(sockfd,bufferOUT,256);
	
	FILE *ff = fopen ("result.bin", "wb");
	if ( f == NULL) {
		error("ERROR opening file");
	}
	char result[256];
	fwrite(bufferOUT, 1, 256,ff);

	if (n < 0)
	{
		error("ERROR reading from socket");
	}
	printf("%s\n",bufferOUT);
	
	return 0;
}
