#ifndef LOGGER_H
#define LOGGER_H
#include <vector>
#include <fstream>

void static Log(std::vector<std::ostream*> streams, std::string log) {
	for (int i = 0; i < streams.size(); ++i) {
		*streams[i] << log.c_str();
	}
}
void static LogLine(std::vector<std::ostream*> streams, std::string log) {
	for (int i = 0; i < streams.size(); ++i) {
		*streams[i] << log.c_str() << std::endl;
	}
}

#endif
