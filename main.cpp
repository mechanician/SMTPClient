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
//		SSL_METHOD const *smethod;
		SSL_CTX *context;
		SSL *ssl;
	public:
		SmtpClient(char const *const HOST, int const PORT = 25)
//		smethod(TLSv1_client_method())
		{

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
			SSL_library_init();
			context = SSL_CTX_new(TLSv1_client_method());
			ssl = SSL_new(context);
			SSL_set_fd(ssl,sock);
			write(1,"test12\n",7);
			int v = SSL_connect(ssl);
			FILE *fp = fopen("log","w");
			fprintf(fp,"testtest\n");
			if (v <= 0){
				cout << "ERROR in SSL_connect()\n";
				exit(EXIT_FAILURE);
			}
			else 
				cout << "Connection is made and secure!\n";
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
