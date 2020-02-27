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
