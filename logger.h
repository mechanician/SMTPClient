/*
 *	For implementation of logger class design pattern 'singleton' is used.
 */
#ifndef LOGGER_H
#define LOGGER_H
#include <fstream>
#include <ctime>
template<typename T >
class Logger {
	public:
		static Logger* Instance();
		void write(T const * const, int );
		void write(T const * const ,T const * const, int );
	protected:
		Logger();
	private:
		static Logger *_instance;
		T *text;
		std::ofstream file;
		Logger(Logger const &);
		Logger& operator=(Logger const &);
		~Logger();
};


template <typename T >
Logger<T>*  Logger<T>::_instance = NULL;

template<typename T >
Logger<T>* Logger<T>::Instance(){
	if (_instance == NULL)
		_instance = new Logger();
	return _instance;
}

template<typename T >
Logger<T>::Logger(){
	file.open("logs");	
}

template<typename T >
void Logger<T>::write(T const * const text, int length) {
	time_t t = time(0);
	struct tm *now = localtime(&t); 
	file << now->tm_year + 1900 << '/'
	     << now->tm_mon + 1 << '/'
	     << now->tm_mday << '/'
	     << now->tm_hour << ':'
	     << now->tm_min << ':'
	     << now->tm_sec << '\t';
//	     << text << std::endl;
//		write(file,(const char *)text, length);
		file.write(text, length);
		file << std::endl;
		
}

template<typename T >
void Logger<T>::write(T const * const text1, T const * const text2, int length2) {
	time_t t = time(0);
	struct tm *now = localtime(&t); 
	file << now->tm_year + 1900 << '/'
	     << now->tm_mon + 1 << '/'
	     << now->tm_mday << '/'
	     << now->tm_hour << ':'
	     << now->tm_min << ':'
	     << now->tm_sec << '\t'
	     << text1;
		file.write(text2, length2);
		file << std::endl;
}
#endif //LOGGER_H
