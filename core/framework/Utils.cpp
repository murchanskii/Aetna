#include "Utils.h"
#include "Engine.h"

#include <fstream>
#include <vector>

std::string Utils::readFile(const std::string &path) {
    std::ifstream ifs(path.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
    std::ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    std::vector<char> bytes(fileSize);
    ifs.read(&bytes[0], fileSize);
    return std::string(&bytes[0], fileSize);
}

std::string Utils::getPathToCore() {
    std::string path_to_core;
    std::string path_to_exe = Engine::get()->getExecutablePath();
    int proj_name_index = path_to_exe.find("Aetna");
    if (proj_name_index >= 0) {
        path_to_core = path_to_exe.substr(0, proj_name_index + strlen("Aetna") + 1) + "core\\";
    }
    return path_to_core;
}

std::vector<std::string> Utils::splitString(const std::string& str, const std::string& delimiters) {
    std::vector<std::string> result;
    size_t last = 0; 
    size_t next = 0; 
    while ((next = str.find(delimiters, last)) != std::string::npos) {
        result.push_back(str.substr(last, next - last));
        last = next + 1;
    } 
    result.push_back(str.substr(last));
	return result;
}

int Utils::stringToInt(const char* str) {
	return std::stoi(str);
}

float Utils::stringToFloat(const char* str) {
	float result = 0.0f;
	if ((str[strlen(str) - 1] >= '0' &&
		str[strlen(str) - 1] <= '9') ||
		str[strlen(str) - 1] == 'f') {
		if (str[strlen(str) - 1] == 'f') {
			result = std::stof(std::string(str).substr(0, strlen(str) - 1));
		}
		else {
			result = std::stof(str);
		}
	}
	return result;
}

glm::vec3 Utils::stringToVec3(const char* str) {
	glm::vec3 result = glm::vec3(0.0f);
	std::vector<float> digits = stringToVecFloat(str);
	if (digits.size() != 3) {
		return result;
	}

	for (int i = 0; i < digits.size(); ++i) {
		result[i] = digits[i];
	}
	return result;
}

glm::vec4 Utils::stringToVec4(const char* str) {
	glm::vec4 result = glm::vec4(0.0f);
	std::vector<float> digits = stringToVecFloat(str);
	if (digits.size() != 4) {
		return result;
	}

	for (int i = 0; i < digits.size(); ++i) {
		result[i] = digits[i];
	}
	return result;
}

glm::mat4 Utils::stringToMat4(const char* str)
{
	glm::mat4 result = glm::mat4(0.0f);
	std::vector<float> digits = stringToVecFloat(str);
	if (digits.size() != 4 * 4) {
		return result;
	}

	for (int i = 0; i < 4; ++i) {
		result[i][0] = digits[i * 4 + 0];
		result[i][1] = digits[i * 4 + 1];
		result[i][2] = digits[i * 4 + 2];
		result[i][3] = digits[i * 4 + 3];
	}

	return result;
}

bool Utils::stringToBool(const char* str) {
	if (strcmp(str, "true") == 0 ||
		strcmp(str, "1") == 0) {
		return true;
	}
	return false;
}

std::vector<float> Utils::stringToVecFloat(std::string str) {
	std::vector<float> result;
	size_t last = 0;
	size_t next = 0;
	while ((next = str.find(" ", last)) != std::string::npos) {
		result.push_back(Utils::stringToFloat(str.substr(last, next - last).c_str()));
		last = next + 1;
	}
	result.push_back(Utils::stringToFloat(str.substr(last).c_str()));
	return result;
}

std::string Utils::intToString(int val)
{
	return std::to_string(val);
}

std::string Utils::floatToString(float val)
{
	return std::to_string(val);
}

std::string Utils::vec3ToString(glm::vec3 val)
{
	return std::to_string(val.x) + " " + std::to_string(val.y) + " " + std::to_string(val.z);
}

std::string Utils::vec4ToString(glm::vec4 val)
{
	return std::to_string(val.x) + " " + std::to_string(val.y) + " " + std::to_string(val.z) + " " + std::to_string(val.w);
}

std::string Utils::mat4ToString(glm::mat4 val)
{
	return std::to_string(val[0].x) + " " +
		std::to_string(val[0].y) + " " +
		std::to_string(val[0].z) + " " +
		std::to_string(val[0].w) + " " +

		std::to_string(val[1].x) + " " +
		std::to_string(val[1].y) + " " +
		std::to_string(val[1].z) + " " +
		std::to_string(val[1].w) + " " +

		std::to_string(val[2].x) + " " +
		std::to_string(val[2].y) + " " +
		std::to_string(val[2].z) + " " +
		std::to_string(val[2].w) + " " +

		std::to_string(val[3].x) + " " +
		std::to_string(val[3].y) + " " +
		std::to_string(val[3].z) + " " +
		std::to_string(val[3].w);
}

std::string Utils::boolToString(bool val) {
	if (val) {
		return "1";
	}
	return "0";
}

std::string Utils::vecFloatToString(std::vector<float> vec) {
	std::string result;
	for (int i = 0; i < vec.size(); ++i) {
		result += Utils::floatToString(vec[i]) + " ";
	}
	if (!result.empty()) {
		result = result.erase(result.length() - 1);
	}
	return result;
}

bool Variable::isInt() {
	return false;
}

int Variable::getInt() {
	return 0;
}

void Variable::setInt(int value) {
}

bool Variable::isFloat() {
	return false;
}

float Variable::getFloat() {
	return 0.0f;
}

void Variable::setFloat(float value) {
}

bool Variable::isVec3() {
    return false;
}

glm::vec3 Variable::getVec3() {
    return glm::vec3();
}

void Variable::setVec3(glm::vec3 value) {
}

bool Variable::isVec4() {
    return false;
}

glm::vec4 Variable::getVec4() {
    return glm::vec4();
}

void Variable::setVec4(glm::vec4 value) {
}

bool Variable::isMat4() {
    return false;
}

glm::mat4 Variable::getMat4() {
    return glm::mat4();
}

void Variable::setMat4(glm::mat4 value) {
}

VariableInt::VariableInt(int value) {
    setInt(value);
}

bool VariableInt::isInt() {
    return true;
}

int VariableInt::getInt() {
    return data;
}

void VariableInt::setInt(int value) {
    data = value;
}

VariableFloat::VariableFloat(float value) {
    setFloat(value);
}

bool VariableFloat::isFloat() {
    return true;
}

float VariableFloat::getFloat() {
    return data;
}

void VariableFloat::setFloat(float value) {
    data = value;
}

VariableVec3::VariableVec3(glm::vec3 value) {
    setVec3(value);
}

bool VariableVec3::isVec3() {
    return true;
}

glm::vec3 VariableVec3::getVec3() {
    return data;
}

void VariableVec3::setVec3(glm::vec3 value) {
    data = value;
}

VariableVec4::VariableVec4(glm::vec4 value) {
    setVec4(value);
}

bool VariableVec4::isVec4() {
    return true;
}

glm::vec4 VariableVec4::getVec4() {
    return data;
}

void VariableVec4::setVec4(glm::vec4 value) {
    data = value;
}

VariableMat4::VariableMat4(glm::mat4 value) {
    setMat4(value);
}

bool VariableMat4::isMat4() {
    return true;
}

glm::mat4 VariableMat4::getMat4() {
    return data;
}

void VariableMat4::setMat4(glm::mat4 value) {
    data = value;
}
