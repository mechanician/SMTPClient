#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <openssl/ssl.h>
using namespace std;

//char *toBase64(char const * const);

class SmtpClient{
	private:
		int sock;
		struct sockaddr_in addr;
		char buf[1024];
		int r;
	public:
		SmtpClient(char const *const HOST, int const PORT = 25){
			sock = socket(AF_INET, SOCK_STREAM, 0);
			memset(&addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(PORT);
			in_addr_t a = inet_addr(HOST);
			if (a == INADDR_NONE){
				cerr << "ERROR: HOST IP is invalid\n";
				exit(EXIT_FAILURE);
			}
			addr.sin_addr.s_addr = a;
			if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0){
				cerr<< "ERROR: "<< errno <<endl;
				exit(EXIT_FAILURE);
			} 
			else 
				cout << "Connection established!\n";
				r = recv(sock, buf, 1024, 0);
				write(1, buf, r);
		}

		
};

int main(int argc, char **argv){
	// argv[1] refers to SMTP-server IP
	if (argc == 2)
		SmtpClient client(argv[1]);
	if (argc == 3)
		SmtpClient client(argv[1], atoi(argv[2]));
	
	return 0;
}
