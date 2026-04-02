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

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*) offsetof(Vertex,uv));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,1,GL_INT,GL_FALSE,sizeof(Vertex),(void*) offsetof(Vertex,tex_index));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,norm));
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4,1,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex, tint));
    glEnableVertexAttribArray(4);

    triangle_count = 0;
    texture_count = 0;

    //std::cout<<"[INFO]: initialized mesh with max triangle count: "<<max_triangle_count<<"\n";
}

void Mesh::Clear() {
    triangle_count = 0;
    texture_count = 0;
}

void Mesh::UpdeteMesh() {

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER,vbo);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex)*triangle_count*3,vertices);

    //std::cout<<"[INFO]: Updated mesh\n";
}

void Mesh::PushTriangle(
    vec3 p1,vec3 p2,vec3 p3,
    vec2 uv1,vec2 uv2,vec2 uv3,
    Texture tex,
    vec3 norm,
    float tint
) {

    if (triangle_count >= max_triangle_count) {
        std::cerr<<"[ERROR]: max mesh quad count exided : "<<max_triangle_count<<"\n";
        exit(-1);
        return;
    }

    int8_t texpos = -1;
    for (int i=0; i<texture_count; i++) {
        if (textures[i] == tex.id) {
            texpos = i;
            break;
        }
    }

    if (texpos == -1 && texture_count < 8) {
        texpos = texture_count;
        textures[texture_count++] = tex.id;
    }

    if (texpos == -1) {
        std::cerr<<"[ERROR]: max mesh texture count exided";
        exit(-1);
        return;
    }

    vertices[triangle_count*3].pos = p1;
    vertices[triangle_count*3 + 1].pos = p2;
    vertices[triangle_count*3 + 2].pos = p3;

    vertices[triangle_count*3].uv = uv1;
    vertices[triangle_count*3 + 1].uv = uv2;
    vertices[triangle_count*3 + 2].uv = uv3;

    vertices[triangle_count*3].tex_index = texpos;
    vertices[triangle_count*3 + 1].tex_index = texpos;
    vertices[triangle_count*3 + 2].tex_index = texpos;

    vertices[triangle_count*3].norm = norm;
    vertices[triangle_count*3 + 1].norm = norm;
    vertices[triangle_count*3 + 2].norm = norm;

    vertices[triangle_count*3].tint = tint;
    vertices[triangle_count*3 + 1].tint = tint;
    vertices[triangle_count*3 + 2].tint = tint;

    triangle_count++;
}

void Mesh::Draw() {

    for (int i=0; i<texture_count; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);

    glDrawArrays(GL_TRIANGLES,0,triangle_count*3);
}

void Mesh::Clean() {
    delete[] vertices;

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
