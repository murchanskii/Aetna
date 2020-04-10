#ifndef AETNA_UTILS_H
#define AETNA_UTILS_H

#include <pugixml.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Variable;

class Utils {
public:
	static std::string readFile(const std::string &path);
	static void saveFile(const std::string &path, const std::string &contents, bool binary);
	static std::string getAbsolutePathFromXML(const std::string &path);
	static std::string getPathXMLFromAbsolute(const std::string& path);
	static std::vector<std::string> splitString(const std::string &str, const std::string &delimiters);
	static void parseXmlVariable(pugi::xml_node variable_node, Variable** var);
	static std::string getStringVariable(Variable* variable);
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
	Variable();
	Variable(const Variable& other);
	Variable& operator=(Variable other);

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
	VariableInt();
	VariableInt(const VariableInt& other);
	VariableInt& operator=(VariableInt other);

	explicit VariableInt(int value);
	bool isInt() override;
	int getInt() override;
	void setInt(int value) override;

private:
	int data;
};

class VariableFloat : public Variable {
public:
	VariableFloat();
	VariableFloat(const VariableFloat& other);
	VariableFloat& operator=(VariableFloat other);

	explicit VariableFloat(float value);
	bool isFloat() override;
	float getFloat() override;
	void setFloat(float value) override;

private:
	float data;
};

class VariableVec3 : public Variable {
public:
	VariableVec3();
	VariableVec3(const VariableVec3& other);
	VariableVec3& operator=(VariableVec3 other);

	explicit VariableVec3(glm::vec3 value);
	bool isVec3() override;
	glm::vec3 getVec3() override;
	void setVec3(glm::vec3 value) override;

private:
	glm::vec3 data;
};

class VariableVec4 : public Variable {
public:
	VariableVec4();
	VariableVec4(const VariableVec4& other);
	VariableVec4& operator=(VariableVec4 other);

	explicit VariableVec4(glm::vec4 value);
	bool isVec4() override;
	glm::vec4 getVec4() override;
	void setVec4(glm::vec4 value) override;

private:
	glm::vec4 data;
};

class VariableMat4 : public Variable {
public:
	VariableMat4();
	VariableMat4(const VariableMat4& other);
	VariableMat4& operator=(VariableMat4 other);

	explicit VariableMat4(glm::mat4 value);
	bool isMat4() override;
	glm::mat4 getMat4() override;
	void setMat4(glm::mat4 value) override;

private:
	glm::mat4 data;
};

#endif //AETNA_UTILS_H