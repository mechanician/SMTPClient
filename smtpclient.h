#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <openssl/ssl.h>
#include "base64.h"
#include "logger.h"

class SmtpClient{
	private:
		char buf[513]; 
		struct addrinfo *servinfo;
		SSL_CTX *context;
		SSL *ssl;
		BIO *bio;
	public:
		SmtpClient(char const *const HOST, char const * PORT );
		int auth(char *mail_address, char *password);
		int startSession (char const * sender, char const * recipient, char const * file);
		int endSession();
		private:
			void sendRequest(char const * const text) const;
			int readResponse(char * ans);

};
