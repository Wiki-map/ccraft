#pragma once

#include <ostream>

struct vec2 {
    float x,y;

    vec2 operator +(const vec2 &b);
    vec2 operator -(const vec2 &b);
    vec2 operator *(float r);
    vec2 operator /(float r);

    vec2& operator +=(vec2 r);
    vec2& operator -=(vec2 r);

    vec2& operator *=(float r);
    vec2& operator /=(float r);

    friend std::ostream& operator<<(std::ostream &os,const vec2& val);
};

struct vec3 {
    float x,y,z;

    vec3 operator +(const vec3 &b);
    vec3 operator -(const vec3 &b);
    vec3 operator *(float r);
    vec3 operator /(float r);

    vec3& operator +=(vec3 r);
    vec3& operator -=(vec3 r);

    vec3& operator *=(float r);
    vec3& operator /=(float r);

    friend std::ostream& operator<<(std::ostream &os,const vec3& val);
};

// TODO: add vec4 operator overloding
struct vec4 {
    float x,y,z,w;
};

#define Vec2ToGLM(v) {v.x,v.y}
#define Vec3ToGLM(v) {v.x,v.y,v.z}
#define Vec4ToGLM(v) {v.x,v.y,v.z,v.w}

vec2 vec2Normalize(vec2 val);
vec3 vec3Normalize(vec3 val);
vec4 vec4Normalize(vec4 val);

float vec3Dot(vec3 x,vec3 y);

vec3 vec3Cross(vec3 v1,vec3 v2);

vec2 WorldToScreen(vec2 pos,float height,float width);
