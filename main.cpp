#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <openssl/ssl.h>
using namespace std;

/*
 *	While using this function user should delete allocated memory 
 *  	(it is alocated inside function during every calling).
 */

char *toBase64(char const * const);

class SmtpClient{
	private:
		int sock;
		struct sockaddr_in addr;
		char buf[1024]; 
		struct addrinfo *servinfo;
		SSL_CTX *context;
		SSL *ssl;
		BIO *bio;
		FILE* logFile;
	public:
		SmtpClient(char const *const HOST, char const * PORT )
		{
			logFile = fopen("log","w");
			getaddrinfo(HOST, PORT, NULL, &servinfo);
			sockaddr_in *t =(sockaddr_in*)servinfo->ai_addr;
			char* servIP =(char*)malloc(INET_ADDRSTRLEN + 1 + strlen(PORT));
			inet_ntop(AF_INET, &(t->sin_addr.s_addr), servIP, INET_ADDRSTRLEN); 
			SSL_library_init();
			context = SSL_CTX_new(TLSv1_client_method());
			ssl = SSL_new(context);
			strcat(servIP, ":");
			bio = BIO_new_connect(strcat(servIP, PORT));
			if (BIO_do_connect(bio) <= 0){
				cerr << "BIO_do_connect is failed\n";
				exit(EXIT_FAILURE);
			} else cout << "BIO connection was succesfull!\n";
			SSL_set_bio(ssl, bio, bio);  			
			if (SSL_connect(ssl) != 1){
				cout << "ERROR in SSL_connect()\n";
				exit(EXIT_FAILURE);
			}
			else 
				cout << "Connection is made and secure!\n";

			readAnswer(buf);

			SSL_write(ssl, "HELO\r\n", 6);
			readAnswer(buf);

			SSL_write(ssl, "AUTH LOGIN\r\n",12);
			readAnswer(buf);
			char * login = toBase64("user@gmail.com");
			char * pass = toBase64("userspassword");
			login = strcat(login,"\r\n");
			pass = strcat(pass,"\r\n");

			cout << SSL_write(ssl,login, strlen(login)) << endl;
			readAnswer(buf);

			SSL_write(ssl, pass, strlen(pass));
			readAnswer(buf);


			SSL_write(ssl, "MAIL FROM: <vladislav.balashov@gmail.com>\r\n",
					strlen("MAIL FROM: <vladislav.balashov@gmail.com>\r\n"));
			readAnswer(buf);

			SSL_write(ssl, "RCPT TO: <supertester2013@yandex.ru>\r\n", 
				 	strlen("RCPT TO: <supertester2013@yandex.ru>\r\n"));
			readAnswer(buf);

			SSL_write(ssl, "DATA\r\n", 6);
			readAnswer(buf);

			SSL_write(ssl, "Subject: TEST!!!\n", strlen("Subject: TEST!!!\n"));
			SSL_write(ssl, "I DID IT!!! ( almost)) )\r\n", strlen("I DID IT!!! ( almost)) )\r\n"));

			SSL_write(ssl, ".\r\n", 3);
			readAnswer(buf);

			SSL_write(ssl, "QUIT\r\n", 6);
			readAnswer(buf);

			free(pass);
			free(login);
		}
		private:
			void readAnswer(char * ans){
				char buff[1024];
				int r = SSL_read(ssl, buff, 1024);
				int i = 0;
				for (i = 0; i < r; ++i)
					ans[i] = buff[i];
				ans[r] = '\0';
				fputs(ans, logFile);					
			}

		
};

int main(int argc, char **argv){
	SmtpClient client(argv[1], argv[2]);
	return 0;
}
