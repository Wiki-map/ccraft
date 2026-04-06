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

    glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex)*max_size*3,nullptr,GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*) offsetof(Vertex,pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*) offsetof(Vertex,color));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,norm));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3,1,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex, tint));
    glEnableVertexAttribArray(3);

    triangle_count = 0;

    //std::cout<<"[INFO]: initialized mesh with max triangle count: "<<max_triangle_count<<"\n";
}

void Mesh::Clear() {
    triangle_count = 0;
}

void Mesh::UpdeteMesh() {

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER,vbo);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex)*triangle_count*3,vertices);

    //std::cout<<"[INFO]: Updated mesh\n";
}

void Mesh::PushTriangle(
    vec3 p1,vec3 p2,vec3 p3,
    Color color,
    vec3 norm,
    float tint
) {

    if (triangle_count >= max_triangle_count) {
        std::cerr<<"[ERROR]: max mesh quad count exided : "<<max_triangle_count<<"\n";
        exit(-1);
        return;
    }
    vertices[triangle_count*3].pos = p1;
    vertices[triangle_count*3 + 1].pos = p2;
    vertices[triangle_count*3 + 2].pos = p3;

    vertices[triangle_count*3].color = frgba(color);
    vertices[triangle_count*3+1].color = frgba(color);
    vertices[triangle_count*3+2].color = frgba(color);

    vertices[triangle_count*3].norm = norm;
    vertices[triangle_count*3 + 1].norm = norm;
    vertices[triangle_count*3 + 2].norm = norm;

    vertices[triangle_count*3].tint = tint;
    vertices[triangle_count*3 + 1].tint = tint;
    vertices[triangle_count*3 + 2].tint = tint;

    triangle_count++;
}

void Mesh::PushQuad(
    vec3 top,vec3 right, vec3 left, vec3 bot,
    Color color,
    vec3 norm,
    float tint
) {
    PushTriangle(top,right,left,color,norm,tint);
    PushTriangle(left,right,bot,color,norm,tint);
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
