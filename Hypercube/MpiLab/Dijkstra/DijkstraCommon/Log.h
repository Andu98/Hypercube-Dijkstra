
#pragma once

#include <iostream>
#include <string>
#include <sstream>
using namespace std;

template <bool shouldLog>
class Log final
{
public:

	//processRank = numarul ce va fi printat inainte de fiecare mesaj. Indica nr de procese care printeaza mesajul.
	Log(ostream& stream, int processRank = 0) : stream(stream), processRank(processRank) {}


	
	template<typename... Ts>
	void logMessage(const Ts& ... informations) {
		ostringstream strs;
	       	strs << "[Procesul " << processRank << "] ";
		logMessageBackend(strs, informations...);
		strs << "\n";
		stream << strs.str();
	}

private:


	template<typename T>
	void logMessageBackend(ostringstream& strs, const T& information) {
		strs << information;
	}

	template<typename T, typename... Ts>
	void logMessageBackend(ostringstream& strs, const T& information, const Ts& ... informations) {
		strs << information;
		logMessageBackend(strs, informations...);
	}

	ostream& stream;
	int processRank;

};

template <>
class Log<false> final
{
public:

	Log(ostream& stream, int processRank = 0) : stream(stream), processRank(processRank) {}

	template<typename... Ts>
	void logMessage(const Ts& ... informations) {}

private:

	ostream& stream;
	int processRank;

};
