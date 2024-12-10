#pragma once
#ifndef SHADER_H

#define SHADER_H

#include<GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
	unsigned int programID;

	// constructor receives the path for the source code of the shaders

	Shader(std::string vertexShaderPath, std::string fragmentShaderPath);

	void UseProgram();
	void DeleteProgram();
	void SetUniformBool(const std::string& name, bool value) const;
	void SetUniformInt(const std::string& name, int value) const;
	void SetUniformFloat(const std::string& name, float value) const;

private:
	std::string RetrieveSource(std::string filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	void CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	void CheckErrors(unsigned int id, std::string type);
};

#endif