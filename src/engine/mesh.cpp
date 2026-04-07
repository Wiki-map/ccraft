#include "engine/mesh.h"

#include "glad/gl.h"
#include <cstddef>
#include <iostream>
#include <stdlib.h>

static int mesh_id = 0;

Mesh::Mesh(int32_t max_size) {

    max_triangle_count = max_size;

    vertices = new Vertex[max_size*3];
    mesh_id++;

    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*) offsetof(Vertex,pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,1,GL_UNSIGNED_INT,GL_FALSE,sizeof(Vertex),(void*) offsetof(Vertex,color));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,1,GL_UNSIGNED_BYTE,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,norm));
    glEnableVertexAttribArray(2);

    triangle_count = 0;

    //std::cout<<"[INFO]: initialized mesh with max triangle count: "<<max_triangle_count<<"\n";
}

void Mesh::Clear() {
    triangle_count = 0;
}

void Mesh::UpdeteMesh() {

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER,vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*triangle_count*3,vertices,GL_STATIC_DRAW);

    //std::cout<<"[INFO]: Updated mesh\n";
}

void Mesh::PushTriangle(
    vec3 p1,vec3 p2,vec3 p3,
    Color color,
    NormalDir norm
) {
    vertices[triangle_count*3].pos = p1;
    vertices[triangle_count*3 + 1].pos = p2;
    vertices[triangle_count*3 + 2].pos = p3;

    vertices[triangle_count*3].color = rgba32(color);
    vertices[triangle_count*3+1].color = rgba32(color);
    vertices[triangle_count*3+2].color = rgba32(color);

    vertices[triangle_count*3].norm = (uint8_t)norm;
    vertices[triangle_count*3 + 1].norm = (uint8_t)norm;
    vertices[triangle_count*3 + 2].norm = (uint8_t)norm;

    triangle_count++;
}

void Mesh::PushQuad(
    vec3 top,vec3 right, vec3 left, vec3 bot,
    Color color,
    NormalDir norm
) {
    PushTriangle(top,right,left,color,norm);
    PushTriangle(left,right,bot,color,norm);
}


void Mesh::Draw() {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);

    glDrawArrays(GL_TRIANGLES,0,triangle_count*3);
}

void Mesh::Clean() {
    delete[] vertices;

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
