#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "ShaderLoader.h"

using namespace std;

void LoadFile(string* data, string fileName)
{
	// Create input stream
	string line;
	ifstream myFile(fileName);

	// If the file is open, retrieve lines from the input stream
	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			(*data) += line + "\n";
		}

		myFile.close();
	}
}

GLuint LoadShaders(const string vertexFilePath, const string fragmentFilePath)
{
	// Create shader references
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Load shader files
	string vertexShaderString;
	string fragmentShaderString;

	LoadFile(&vertexShaderString, vertexFilePath);
	LoadFile(&fragmentShaderString, fragmentFilePath);

	int lengthOfErrorLog;

	// Compile vertex shader
	cout << "Shader: Compiling vertex shader" << endl;
	const char* vertexPointer = vertexShaderString.c_str();
	glShaderSource(vertexShader, 1, &vertexPointer, NULL);
	glCompileShader(vertexShader);

	// Check compile status for vertex shader
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &lengthOfErrorLog);

	// If an error log exists, retrieve and print it
	if (lengthOfErrorLog > 0) {
		vector<char> vertexShaderErrorMessage(lengthOfErrorLog + 1);
		glGetShaderInfoLog(vertexShader, lengthOfErrorLog, NULL, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
	}

	// Compile fragment shader
	cout << "Shader: Compiling fragment shader" << endl;
	const char* fragmentPointer = fragmentShaderString.c_str();
	glShaderSource(fragmentShader, 1, &fragmentPointer, NULL);
	glCompileShader(fragmentShader);

	// Check compile status for fragment shader
	glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &lengthOfErrorLog);
	
	// If an error log exists, retrieve and print it
	if (lengthOfErrorLog > 0) {
		vector<char> fragmentShaderErrorMessage(lengthOfErrorLog + 1);
		glGetShaderInfoLog(fragmentShader, lengthOfErrorLog, NULL, &fragmentShaderErrorMessage[0]);
		printf("%s\n", &fragmentShaderErrorMessage[0]);
	}

	// Link program
	cout << "Shader: Linking program" << endl;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	// Clean up
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}