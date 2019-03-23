//
// Created by Maxim Morgunov on 23.03.2019.
//

#ifndef SUFFERING2_SHADER_H
#define SUFFERING2_SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    //Program ID
    unsigned ID;

    //Constructor reads and builds the shader
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {

        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        //Open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        //File's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        //Close the files
        vShaderFile.close();
        fShaderFile.close();

        //Convert streams into strings
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        //Shader compilation
        GLuint vertex, fragment;
        int success;
        char infoLog[512];

        //Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);

        //Print compilation errors if they pop up
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
            std::cout << "Vertex shader compilation failure\n" << infoLog << std::endl;
        } else
            std::cout << "Vertex shader compiled" << std::endl;

        //Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);

        //Print compilation errors if they pop up
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
            std::cout << "Fragment shader compilation failure\n" << infoLog << std::endl;
        } else
            std::cout << "Fragment shader compiled" << std::endl;

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        //Print linking errors if they pop up
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(ID, 512, nullptr, infoLog);
            std::cout << "Shader program linking error\n" << std::endl;
        } else
            std::cout << "Shader program linked" << std::endl;

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    //Use/activate the shader
    void use() {
        glUseProgram(ID);
    }
    //Useful uniform funcs
    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }
    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    void set4Floats(const std::string &name, float value1, float value2, float value3, float value4) {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
    }
};

#endif //SUFFERING2_SHADER_H
