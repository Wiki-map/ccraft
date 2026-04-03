#pragma once
#include <stdint.h>
#include <string>

#include "glm/glm.hpp"

struct Shader {
    Shader() = default;
    Shader(std::string vertex_path,std::string fragment_path);

    void SetVector2Uniform(std::string name,glm::vec2 val);
    void SetVector3Uniform(std::string name,glm::vec3 val);
    void SetVector4Uniform(std::string name,glm::vec4 val);

    void SetMatrix4Uniform(std::string name,glm::mat4 val);

    void SetIntArrayUniform(std::string name,int32_t* val,size_t count);
    void SetFloatArrayUniform(std::string name,float* val,size_t count);

    int32_t GetShaderID();

    void Use();

private:
    int32_t id;
};
