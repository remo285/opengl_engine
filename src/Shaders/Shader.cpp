#include "Shader.h"
#include <filesystem>


Shader::Shader(std::string vertexShaderPath, std::string fragmentShaderPath)
{
    std::string vertexSource = RetrieveSource(vertexShaderPath);
    std::string fragmentSource = RetrieveSource(fragmentShaderPath);

    CreateShader(vertexSource, fragmentSource);
}

std::string Shader::RetrieveSource(std::string filePath)
{
    std::string source;
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shaderFile.open(filePath);
        std::stringstream fileStream;

        fileStream << shaderFile.rdbuf();
        shaderFile.close();

        source = fileStream.str();
    }
    catch (std::ifstream::failure error)
    {
        std::cout << "Error reading shader file, path:" << filePath << "\n" 
                  << "Error: " << error.what() << std::endl;
    };

    return source;
};

void Shader::UseProgram()
{ 
    glUseProgram(programID);
}

void Shader::DeleteProgram()
{
    glDeleteProgram(programID);
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int shaderId = glCreateShader(type); // we create the shader and we get an identifier for it
    const char* src = source.c_str();  //source code for the shader
    glShaderSource(shaderId, 1, &src, nullptr); // we provide the source code of the shader
    glCompileShader(shaderId); // we compile the source code of the shader

    CheckErrors(shaderId, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMEN");

    return shaderId;
};

void Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    programID = glCreateProgram(); // creates a program to wich we can attach the shaders
    unsigned int vertex = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(programID, vertex);  // attach shaders to the program
    glAttachShader(programID, fragment);

    glLinkProgram(programID);  // basically just creates the executable for the program by using the shaders we provided
    glValidateProgram(programID);

    CheckErrors(programID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
};

void Shader::CheckErrors(unsigned int id, std::string type)
{
    int success;
    if (type != "PROGRAM") {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);  // we ask for the compile status of our shader
        if (success == GL_FALSE) {  // we check if it's GL_FALSE (wich is 0)
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); // we get the length of the error message log

            char* message = (char*)alloca(length * sizeof(char)); // we allocate memory for it
            glGetShaderInfoLog(id, length, &length, message); // we get the actual log

            std::cout << "Failed to compile " << type;
            std::cout << message << std::endl;

            glDeleteShader(id); // we delete the shader that failed to compile
        }
    }
    else
    {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (success == GL_FALSE)
        {
            int length;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);

            char* message = (char*)alloca(length * sizeof(char));
            glGetProgramInfoLog(id, 1024, NULL, message);

            std::cout << "Program link error:" << "\n" << message << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void Shader::SetUniformBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
};

void Shader::SetUniformInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
};

void Shader::SetUniformFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
};



//int uniformLocation = glGetUniformLocation(shader, "dynamicColor");

//glUniform4f(uniformLocation, rgb[0], rgb[1], rgb[2], 1.0f);*/