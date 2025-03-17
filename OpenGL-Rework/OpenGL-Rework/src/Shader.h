#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Shader {
public:
	// program ID
	unsigned int id;

	// contructor
	Shader(const char* vertexShaderPath, const char* fragShaderPath);
	// activate shader
	void activate();

	// utility functions
	std::string loadShaderSrc(const char* filePath);
	GLuint compileShader(const char* filePath, GLuint type);

	// uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void set4Float(const std::string& name, float v1, float v2, float v3, float v4) const;
	void setMat4(const std::string& name, glm::mat4 val) const;
};

#endif // !SHADER_H