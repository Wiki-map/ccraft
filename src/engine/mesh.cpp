#include "engine/mesh.h"

#include "glad/gl.h"
#include <cstddef>


void Mesh::Init() {
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*) offsetof(Vertex,pos));
    glEnableVertexAttribArray(0);

    glVertexAttribIPointer(1,1,GL_UNSIGNED_INT,sizeof(Vertex),(void*) offsetof(Vertex,color));
    glEnableVertexAttribArray(1);

    glVertexAttribIPointer(2,1,GL_UNSIGNED_BYTE,sizeof(Vertex),(void*) offsetof(Vertex,norm));
    glEnableVertexAttribArray(2);

    triangle_count = 0;
}

void Mesh::Clear() {
    triangle_count = 0;
    vertices.clear();
}

void Mesh::UpdeteMesh() {
    #ifdef __APPLE__
        glBindBuffer(GL_ARRAY_BUFFER,vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(),vertices.data(),GL_STATIC_DRAW);
    #else
        glNamedBufferData(vbo,sizeof(Vertex)*vertices.size(),vertices.data(),GL_STATIC_DRAW);
    #endif
}

void Mesh::PushTriangle(
    vec3 p1,vec3 p2,vec3 p3,
    Color color,
    NormalDir norm
) {
    vertices.push_back({p1,rgba32(color),(uint8_t)norm});
    vertices.push_back({p2,rgba32(color),(uint8_t)norm});
    vertices.push_back({p3,rgba32(color),(uint8_t)norm});

    triangle_count++;
}

void Mesh::PushQuad(
    vec3 top,vec3 right, vec3 left, vec3 bot,
    Color color,
    NormalDir norm
) {
    PushTriangle(top,left,right,color,norm);
    PushTriangle(right,left,bot,color,norm);
}


void Mesh::Draw() {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);

    glDrawArrays(GL_TRIANGLES,0,triangle_count*3);
}

void Mesh::Clean() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    std::vector<Vertex>().swap(vertices);
}
