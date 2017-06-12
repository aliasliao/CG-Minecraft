#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <cstdio>

class Shader {
	GLuint program;

	GLchar *loadText(const std::string &fileName) {
		const std::string fullName = "shaders/" + fileName;
		const char *name = fullName.c_str();

		FILE *fp;
		GLchar *content = NULL;
		int count = 0;

		fp = fopen(name, "rt");
		if (fp == NULL) {
			std::cout << "can not open shader file" << fullName << std::endl;
			return NULL;
		}

		fseek(fp, 0, SEEK_END);
		count = ftell(fp);
		rewind(fp);

		if (count > 0) {
			content = (GLchar *)malloc((count + 1) * sizeof(char));
			if (content != NULL) {
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
		}

		fclose(fp);
		return content;
	}

	void checkCompileErrors(GLuint shader, const std::string &type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}

public:
	Shader(const std::string &vertFile, const std::string &fragFile) {
		const GLchar *vertSource = this->loadText(vertFile);
		const GLchar *fragSource = this->loadText(fragFile);

		GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertShader, 1, &vertSource, NULL);
		free((void *)vertSource);
		glCompileShader(vertShader);
		this->checkCompileErrors(vertShader, "VERTEX");

		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShader, 1, &fragSource, NULL);
		free((void *)fragSource);
		glCompileShader(fragShader);
		this->checkCompileErrors(fragShader, "FRAGMENT");

		this->program = glCreateProgram();
		glAttachShader(this->program, vertShader);
		glAttachShader(this->program, fragShader);
		glLinkProgram(this->program);
		checkCompileErrors(this->program, "PROGRAM");

		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
	}

	// activate the shader
	// ------------------------------------------------------------------------
	void use()
	{
		glUseProgram(this->program);
	}
	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(this->program, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(this->program, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(this->program, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string &name, const glm::vec2 &value) const
	{
		glUniform2fv(glGetUniformLocation(this->program, name.c_str()), 1, glm::value_ptr(value));
	}
	void setVec2(const std::string &name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(this->program, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, const glm::vec3 &value) const
	{
		glUniform3fv(glGetUniformLocation(this->program, name.c_str()), 1, glm::value_ptr(value));
	}
	void setVec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(this->program, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string &name, const glm::vec4 &value) const
	{
		glUniform4fv(glGetUniformLocation(this->program, name.c_str()), 1, glm::value_ptr(value));
	}
	void setVec4(const std::string &name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(this->program, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string &name, const glm::mat2 &mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(this->program, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string &name, const glm::mat3 &mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(this->program, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string &name, const glm::mat4 &mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(this->program, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}
};