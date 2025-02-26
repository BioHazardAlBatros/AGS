#include "Shader.h"

GLuint Shader::createShaderObject(GLenum type, std::string sourcePath)
{
	std::ifstream shaderFile(sourcePath);
	if (!shaderFile)
	{
		return 0;
	}

	shaderFile.seekg(0, shaderFile.end);
	std::vector<char> buffer(shaderFile.tellg());
	shaderFile.seekg(0, shaderFile.beg);
	shaderFile.read(buffer.data(), buffer.size());

	GLuint shaderID = glCreateShader(type);
	
	GLchar const* pointerToBuffer =  buffer.data() ;
	glShaderSource(shaderID, 1,&pointerToBuffer, nullptr);
	glCompileShader(shaderID);

	GLint compileResult;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileResult);
	if (compileResult) //GL_TRUE
		return shaderID;

	char errorMsg[1024];
	glGetShaderInfoLog(shaderID, 1024, nullptr, errorMsg);
	std::ofstream crash_log("crash_" + std::to_string(time(nullptr)) + ".log");
	crash_log << "Compilation error: sourceFile - " << sourcePath << " " << "\nLOG:" << errorMsg;
	crash_log.close();
	return 0;
}

bool Shader::load(std::string vertexShaderName, std::string fragmentShaderName)
{
	GLuint vertexShader = createShaderObject(GL_VERTEX_SHADER, vertexShaderName);
	if (!vertexShader)
	{
		std::ofstream crash_log("crash_" + std::to_string(time(nullptr)) + ".log");
		crash_log << "Compilation error: vertex shader sourceFile - " << vertexShaderName;
		crash_log.close();
		return false;
	}

	GLuint fragmentShader = createShaderObject(GL_FRAGMENT_SHADER, fragmentShaderName);
	if (!fragmentShader)
	{
		std::ofstream crash_log("crash_" + std::to_string(time(nullptr)) + ".log");
		crash_log << "Compilation error: fragment shader sourceFile - " << fragmentShaderName;
		crash_log.close();
		glDeleteShader(vertexShader);
		return false;
	}

	this->program = glCreateProgram();
	glAttachShader(this->program, fragmentShader);
	glAttachShader(this->program, vertexShader);

	glLinkProgram(this->program);

	GLint linkResult;
	glGetProgramiv(this->program, GL_LINK_STATUS, &linkResult);
	if(linkResult) //GL_TRUE
		return true;

	char errorMsg[1024];
	glGetProgramInfoLog(this->program, 1024, nullptr, errorMsg);
	std::ofstream crash_log("crash_" + std::to_string(time(nullptr))+".log");
	crash_log << "Linking error: sourceFiles - " << fragmentShaderName << "," << vertexShaderName<< "\nLOG:" << errorMsg;
	crash_log.close();
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(this->program);
	return 0;
}

void Shader::deactivate()
{	
	glUseProgram(0);
}

void Shader::activate()
{
	glUseProgram(this->program);
}