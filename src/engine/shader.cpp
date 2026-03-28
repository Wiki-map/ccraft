#include "engine/shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "glad/gl.h"

static std::string GetFileContend(std::string path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

static void CheckError(uint32_t shader, int32_t type) {
    int32_t succes;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &succes);

    if (!succes) {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        if (type == GL_VERTEX_SHADER) {
            std::cerr<<"[ERROR]: vertex shader error: "<<info_log<<"\n";
        }
        else {
            std::cerr<<"[ERROR]: fragment shader error: "<<info_log<<"\n";
        }
        exit(-1);
    }
}

Shader::Shader(std::string vertex_path,std::string fragment_path) {
    std::string vertex_source = GetFileContend(vertex_path).c_str();
    std::string fragment_source = GetFileContend(fragment_path).c_str();

    const char* vertex_source_cstr = vertex_source.c_str();
    const char* fragment_source_cstr = fragment_source.c_str();

    uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader,1,&vertex_source_cstr,NULL);
    glCompileShader(vertex_shader);

    CheckError(vertex_shader,GL_VERTEX_SHADER);


    uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader,1,&fragment_source_cstr,NULL);
    glCompileShader(fragment_shader);

    CheckError(fragment_shader,GL_FRAGMENT_SHADER);

    this->id = glCreateProgram();
    glAttachShader(this->id,vertex_shader);
    glAttachShader(this->id,fragment_shader);
    glLinkProgram(this->id);

    int32_t error;
    glGetProgramiv(this->id,GL_LINK_STATUS,&error);
    if (error == GL_FALSE) {
        std::cout<<"[ERROR]: program linking failed\n";
        char buffer[512];
        glGetProgramInfoLog(this->id,512,NULL,buffer);
        std::cout<<buffer<<"\n";
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Shader::SetVector2Uniform(std::string name,glm::vec2 val) {
    int32_t location = glGetUniformLocation(this->id,name.c_str());
    glUniform2fv(location,1,&val[0]);
}

void Shader::SetVector3Uniform(std::string name,glm::vec3 val) {
    int32_t location = glGetUniformLocation(this->id,name.c_str());
    glUniform3fv(location,1,&val[0]);
}

void Shader::SetVector4Uniform(std::string name,glm::vec4 val) {
    int32_t location = glGetUniformLocation(this->id,name.c_str());
    glUniform4fv(location,1,&val[0]);
}


void Shader::SetMatrix4Uniform(std::string name,glm::mat4 val) {
    int32_t location = glGetUniformLocation(this->id,name.c_str());
    glUniformMatrix4fv(location,1,GL_FALSE,&val[0][0]);
}

void Shader::SetIntArrayUniform(std::string name,int32_t* val,size_t count) {
    int32_t location = glGetUniformLocation(this->id,name.c_str());
    glUniform1iv(location,count,val);
}

void Shader::SetFloatArrayUniform(std::string name,float* val,size_t count) {
    int32_t location = glGetUniformLocation(this->id,name.c_str());
    glUniform1fv(location,count,val);
}

int32_t Shader::GetShaderID() {
    return this->id;
}