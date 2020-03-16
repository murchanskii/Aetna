#ifndef AETNA_UTILS_H
#define AETNA_UTILS_H

#include <glm/glm.hpp>
#include <string>
#include <vector>

class Utils {
public:
	static std::string readFile(const std::string &path);
	static std::string getPathToCore();
	static std::vector<std::string> splitString(const std::string &str, const std::string &delimiters);
	static int stringToInt(const char* str);
	static float stringToFloat(const char* str);
	static glm::vec3 stringToVec3(const char* str);
	static glm::vec4 stringToVec4(const char* str);
	static glm::mat4 stringToMat4(const char* str);
	static bool stringToBool(const char* str);
	static std::vector<float> stringToVecFloat(std::string str);
	static std::string intToString(int val);
	static std::string floatToString(float val);
	static std::string vec3ToString(glm::vec3 val);
	static std::string vec4ToString(glm::vec4 val);
	static std::string mat4ToString(glm::mat4 val);
	static std::string boolToString(bool val);
	static std::string vecFloatToString(std::vector<float> vec);
};

class Variable {
public:
	virtual bool isInt();
	virtual int getInt();
	virtual void setInt(int value);

	virtual bool isFloat();
	virtual float getFloat();
	virtual void setFloat(float value);

	virtual bool isVec3();
	virtual glm::vec3 getVec3();
	virtual void setVec3(glm::vec3 value);

	virtual bool isVec4();
	virtual glm::vec4 getVec4();
	virtual void setVec4(glm::vec4 value);

	virtual bool isMat4();
	virtual glm::mat4 getMat4();
	virtual void setMat4(glm::mat4 value);
};

class VariableInt : public Variable {
public:
	explicit VariableInt(int value);
	bool isInt() override;
	int getInt() override;
	void setInt(int value) override;

private:
	int data;
};

class VariableFloat : public Variable {
public:
	explicit VariableFloat(float value);
	bool isFloat() override;
	float getFloat() override;
	void setFloat(float value) override;

private:
	float data;
};

class VariableVec3 : public Variable {
public:
	explicit VariableVec3(glm::vec3 value);
	bool isVec3() override;
	glm::vec3 getVec3() override;
	void setVec3(glm::vec3 value) override;

private:
	glm::vec3 data;
};

class VariableVec4 : public Variable {
public:
	explicit VariableVec4(glm::vec4 value);
	bool isVec4() override;
	glm::vec4 getVec4() override;
	void setVec4(glm::vec4 value) override;

private:
	glm::vec4 data;
};

class VariableMat4 : public Variable {
public:
	explicit VariableMat4(glm::mat4 value);
	bool isMat4() override;
	glm::mat4 getMat4() override;
	void setMat4(glm::mat4 value) override;

private:
	glm::mat4 data;
};

#endif //AETNA_UTILS_H