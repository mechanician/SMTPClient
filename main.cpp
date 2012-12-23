#include <iostream>
#include <termios.h>
#include "smtpclient.h"
using namespace std;

void hideInput(){
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);
	t.c_lflag = t.c_lflag&~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void showInput(){
	struct termios t;
	tcgetattr(STDIN_FILENO, &t);
	t.c_lflag = t.c_lflag|ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

int main(int argc, char **argv){
	char mail_address[254];
	char recipient[254];
	char password[254];
	char path[300];
	char ans;
	SmtpClient client(argv[1], argv[2]);
	while (1){
		cout << "Enter your email address:\n";
		cin >> mail_address;
		cout << "Enter password:\n";
		hideInput();
		cin >> password;
		showInput();
		if (client.auth(mail_address, password) == 235)
			break;
		cout << "Uncorrect password or email address. Try again?(Y/N):";
		cin >> ans;
		if (ans != 'y' && ans != 'Y')
			return 0;
		}
	cout << "Enter recipient address: ";
	cin >> recipient;
	cout << "Enter path to file, which contains message:";
	cin >> path;
	client.startSession(mail_address, recipient, path);
	client.endSession();
	return 0;
}
