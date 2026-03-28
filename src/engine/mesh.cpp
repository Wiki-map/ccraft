#include "engine/mesh.h"

#include "glad/gl.h"

Mesh::Mesh(int32_t max_size) {

    max_triangle_count = max_size;

    vertices = new Vertex[max_size+1];

    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);

    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    glBufferData(GL_ARRAY_BUFFER,sizeof(Vertex)*max_size*3,NULL,GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*) offsetof(Vertex,pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*) offsetof(Vertex,uv));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,1,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*) offsetof(Vertex,tex_index));
    glEnableVertexAttribArray(2);
}

void Mesh::Clear() {
    triangle_count = 0;
    texture_count = 0;
}

void Mesh::UpdeteMesh() {

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER,vbo);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex)*triangle_count*3,vertices);
}

void Mesh::PushTriangle(
    glm::vec3 p1,glm::vec3 p2,glm::vec3 p3,
    glm::vec2 uv1,glm::vec2 uv2,glm::vec2 uv3,
    Texture tex
) {

    if (triangle_count == max_triangle_count) {
        fprintf(stderr, "[ERROR]: max mesh quad count exided");
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
        fprintf(stderr, "[ERROR]: max mesh texture count exided");
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

Mesh::~Mesh() {
    delete vertices;
}