#pragma once

#include "glm/glm.hpp"

struct vec2 {
    float x,y;
};

struct vec3 {
    float x,y,z;
};

struct vec4 {
    float x,y,z,w;
};

#define Vec2ToGLM(v) {v.x,v.y}
#define Vec3ToGLM(v) {v.x,v.y,v.z}
#define Vec4ToGLM(v) {v.x,v.y,v.z,v.w}

inline vec2 operator +(const vec2 &x,const vec2 &y);
inline const vec2& operator +=(vec2 &x,const vec2 &y);

inline vec2 operator -(const vec2 &x,const vec2 &y);
inline const vec2& operator -=(vec2 &x,const vec2 &y);


inline vec3 operator +(const vec3 &x,const vec3 &y);
inline const vec3& operator +=(vec3 &x,const vec3 &y);

inline vec3 operator -(const vec3 &x,const vec3 &y);
inline const vec3& operator -=(vec3 &x,const vec3 &y);


inline vec4 operator +(const vec4 &x,const vec4 &y);
inline const vec4& operator +=(vec4 &x,const vec4 &y);

inline vec4 operator -(const vec4 &x,const vec4 &y);
inline const vec4 &operator -=(vec4 &x,const vec4 &y);

vec2 vec2Normalize(vec2 val);
vec3 vec3Normalize(vec3 val);
vec4 vec4Normalize(vec4 val);

float vec3Dot(vec3 x,vec3 y);

vec3 vec3Cross(vec3 v1,vec3 v2);

vec2 WorldToScreen(vec2 pos,float height,float width);
