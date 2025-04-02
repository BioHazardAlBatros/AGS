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

	char errorMsg[1024]{ 0 };
	glGetShaderInfoLog(shaderID, 1024, nullptr, errorMsg);
	std::string crashLogFilename = "crash_" + std::to_string(time(nullptr)) + ".log";
	std::ofstream crash_log(crashLogFilename);
	printf("Failed to compile %s:\n%s", sourcePath.c_str(), errorMsg);
	crash_log << "Compilation error: sourceFile - " << sourcePath << " " << "\nLOG:\n" << errorMsg;
	printf("A crash has occured. Saved crash info in file: %s.", crashLogFilename.c_str());
	crash_log.close();
	return 0;
}

bool Shader::load(std::string vertexShaderName, std::string fragmentShaderName)
{
	GLuint vertexShader = createShaderObject(GL_VERTEX_SHADER, vertexShaderName);
	if (!vertexShader)
		return false;
	GLuint fragmentShader = createShaderObject(GL_FRAGMENT_SHADER, fragmentShaderName);
	if (!fragmentShader)
	{
		glDeleteShader(vertexShader);
		return false;
	}

	this->program = glCreateProgram();
	glAttachShader(this->program, fragmentShader);
	glAttachShader(this->program, vertexShader);

	glLinkProgram(this->program);

	GLint linkResult;
	glGetProgramiv(this->program, GL_LINK_STATUS, &linkResult);
	
	if (linkResult) //GL_TRUE
	{
		GLint uniformCount, uniformSize; 
		GLenum uniformType;
		GLchar uniformName[30];
		GLsizei length;

		printf("Shader compiled. Seeking uniform variables...\n");
		glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniformCount);
		
		if(uniformCount==0)
			printf("No uniforms found.\n");
		for (GLint i = 0; i < uniformCount; i++)
		{
			glGetActiveUniform(program, (GLuint)i, (const GLsizei)30, &length, &uniformSize, &uniformType, uniformName);

			printf("Found new uniform #%d TypeID: %u Name: %s\n", i, uniformType, (char*)uniformName);
			this->uniforms.insert({ uniformName,glGetUniformLocation(this->program, uniformName) });
        }

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		printf("Program ID - %d\n", this->program);
		return true;
	}

	char errorMsg[1024];
	std::string crashLogFilename = "crash_" + std::to_string(time(nullptr)) + ".log";
	glGetProgramInfoLog(this->program, 1024, nullptr, errorMsg);
	std::ofstream crash_log(crashLogFilename);
	printf("Failed to link shaders to program, LOG:\n%s", errorMsg);
	crash_log << "Linking error: sourceFiles - " << fragmentShaderName << "," << vertexShaderName<< "\nLOG:" << errorMsg;
	printf("A crash has occured. Saved crash info in file: %s.", crashLogFilename.c_str());
	crash_log.close();
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(this->program);
	return 0;
}

GLuint Shader::currentProgram = 0;

void Shader::deactivate()
{	
	Shader::currentProgram = 0;
	glUseProgram(0);
}

void Shader::activate()
{
	Shader::currentProgram = this->program;
	glUseProgram(this->program);
}

GLuint Shader::getUniformLocation(std::string name) 
{
	std::map<std::string, GLuint>::iterator position = this->uniforms.find(name);
	if (position != this->uniforms.end())
		return this->uniforms.find(name)->second;
	else
		return 0;
}

void Shader::setUniform(std::string name, int value)
{
	glUniform1i(getUniformLocation(name), value);
	return;
}

void Shader::setUniform(std::string name, float value)
{
	glUniform1f(getUniformLocation(name), value);
	return;
}

void Shader::setUniform(std::string name, glm::vec2 value)
{
	glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value));
	return;
}

void Shader::setUniform(std::string name, glm::vec4 value)
{
	glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
	return;
}

void Shader::setUniform(std::string name, glm::mat4 value)
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	return;
}

void Shader::setUniform(std::string name, std::vector<glm::mat4> &value)
{
	glUniformMatrix4fv(getUniformLocation(name), value.size(), GL_FALSE, (GLfloat*)value.data());
	return;
}