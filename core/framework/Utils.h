#ifndef AETNA_UTILS_H
#define AETNA_UTILS_H

#include <string>

class Utils {
public:
	static std::string readFile(const std::string &path);
	static std::string getPathToCore();
};

#endif //AETNA_UTILS_H