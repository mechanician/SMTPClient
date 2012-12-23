#include "smtpclient.h"
using namespace std;

SmtpClient::SmtpClient(char const *const HOST, char const * PORT )
{
	char* servIP =(char*)malloc(INET_ADDRSTRLEN + 1 + strlen(PORT));
	getaddrinfo(HOST, PORT, NULL, &servinfo);
	sockaddr_in *addr =(sockaddr_in*)servinfo->ai_addr;
	inet_ntop(AF_INET, &(addr->sin_addr.s_addr), servIP, INET_ADDRSTRLEN);
	Logger<char>::Instance()->write("Connection to ", servIP, INET_ADDRSTRLEN + 1 + strlen(PORT));
	SSL_library_init();
	context = SSL_CTX_new(TLSv1_client_method());
	ssl = SSL_new(context);
	strcat(servIP, ":");
	char * address = strcat(servIP, PORT);
	bio = BIO_new_connect(address);
	if (BIO_do_connect(bio) <= 0){
		cerr << "Connection is failed\n";
		exit(EXIT_FAILURE);
	} else cout << "Connection was succesfull!\n";
	SSL_set_bio(ssl, bio, bio);  			
	if (SSL_connect(ssl) != 1){
		cout << "Conection is failed\n";
		exit(EXIT_FAILURE);
	}
	else 
		cout << "Connection is made and secure!\n";

	readResponse(buf);
}

int SmtpClient::auth(char *mail_address, char *password) { 
	int status;
	char * login = toBase64(mail_address);
	char * pass = toBase64(password);
	login = strcat(login,"\r\n");
	pass = strcat(pass,"\r\n");

	sendRequest("HELO\r\n");
	if (readResponse(buf) != 250)
		exit(EXIT_FAILURE);

	sendRequest("AUTH LOGIN\r\n");
	readResponse(buf);

	sendRequest(login);
	readResponse(buf);

	SSL_write(ssl, pass, strlen(pass));
	Logger<char>::Instance()->write("Request: **********", 19);

	status = readResponse(buf);

	free(pass);
	free(login);
	return status;
}

int SmtpClient::startSession (char const * sender, char const * recipient, char const * file){
	ifstream in;
	char s[300];
	memset(s, '\0', 300);
	strcat(s, "MAIL FROM: <");
	strcat(s, sender);
	strcat(s, ">\r\n");
	sendRequest(s);
	readResponse(buf);
	
	memset(s, '\0', 300);
	strcat(s, "RCPT TO: <");
	strcat(s, recipient);
	strcat(s, ">\r\n");
	sendRequest(s);
	readResponse(buf);
	memset(s, '\0', 300);
	
	in.open(file, ios_base::in);
	if(!in.good()){
		cerr << "There is problem to open file " << file << endl;
		return -1;
	}

	sendRequest("DATA\r\n");
	readResponse(buf);

	while(!in.eof()){
		in.read(s, 300);
		sendRequest(s);
		memset(s, '\0', 300);
	}
	sendRequest("\r\n");

	sendRequest(".\r\n");
	readResponse(buf);

	sendRequest("QUIT\r\n");
	readResponse(buf);

};

void SmtpClient::sendRequest(char const * const text) const{
	SSL_write(ssl, text, strlen(text)); 
	Logger<char>::Instance()->write("Request: ", text, strlen(text) - 2);
};

int SmtpClient::readResponse(char * ans){
	int bytes; 
	char num[3];
	/*	From RFC 5321:
	 *		The maximum total length of a reply line including 
	 *		the reply code and the <CRLF> is 512 octets.				
	 */
	bytes = SSL_read(ssl, ans, 512);
	ans[bytes - 2] = '\0';
	Logger<char>::Instance()->write("Response: ", ans, strlen(ans));
	num[0] = ans[0];
	num[1] = ans[1];
	num[2] = ans[2];
	return atoi(num);
}

int SmtpClient::endSession(){
	SSL_free(ssl);
//	BIO_free(bio);
	SSL_CTX_free(context);
}
