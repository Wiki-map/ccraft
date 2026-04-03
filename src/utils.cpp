#include "utils.h"

inline vec2 operator +(const vec2 &x,const vec2 &y) {
    return {x.x + y.x, x.y + y.y};
}

inline const vec2& operator +=(vec2 &x,const vec2 &y) {
    x.x += y.x;
    x.y += y.y;
    return x;
}

inline vec2 operator -(const vec2 &x,const vec2 &y) {
    return {x.x - y.x, x.y - y.y};
}

inline const vec2& operator -=(vec2 &x,const vec2 &y) {
    x.x -= y.x;
    x.y -= y.y;
    return x;
}



inline vec3 operator +(const vec3 &x,const vec3 &y) {
    return {x.x + y.x, x.y + y.y, x.z + y.z};
}

inline const vec3& operator +=(vec3 &x,const vec3 &y) {
    x.x += y.x;
    x.y += y.y;
    x.z += y.z;
    return x;
}

inline vec3 operator -(const vec3 &x,const vec3 &y) {
    return {x.x - y.x, x.y - y.y, x.z - y.z};
}

inline const vec3& operator -=(vec3 &x,const vec3 &y) {
    x.x -= y.x;
    x.y -= y.y;
    x.z -= y.z;
    return x;
}



inline vec4 operator +(const vec4 &x,const vec4 &y) {
    return {x.x + y.x, x.y + y.y, x.z + y.z,x.w + y.w};
}

inline const vec4& operator +=(vec4 &x,const vec4 &y) {
    x.x += y.x;
    x.y += y.y;
    x.z += y.z;
    x.w += y.w;
    return x;
}

inline vec4 operator -(const vec4 &x,const vec4 &y) {
    return {x.x - y.x, x.y - y.y, x.z - y.z,x.w - y.w};
}

inline const vec4 &operator -=(vec4 &x,const vec4 &y) {
    x.x -= y.x;
    x.y -= y.y;
    x.z -= y.z;
    x.w -= y.w;
    return x;
}

vec2 vec2Normalize(vec2 val) {
    float lenght = std::sqrt(val.x*val.x + val.y*val.y);
    if (lenght == 0) return val;

    val.x /= lenght;
    val.y /= lenght;

    return val;
}

vec3 vec3Normalize(vec3 val) {
    float lenght = std::sqrt(val.x*val.x + val.y*val.y + val.z*val.z);
    if (lenght == 0) return val;

    val.x /= lenght;
    val.y /= lenght;
    val.z /= lenght;

    return val;
}

vec4 vec4Normalize(vec4 val) {
    float lenght = std::sqrt(val.x*val.x + val.y*val.y + val.z*val.z + val.w*val.w);
    if (lenght == 0) return val;

    val.x /= lenght;
    val.y /= lenght;
    val.z /= lenght;
    val.w /= lenght;

    return val;
}

vec3 vec3Cross(vec3 v1,vec3 v2) {
    return { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };
}

float vec3Dot(vec3 x,vec3 y) {
    return x.x*y.x+x.y*y.y+x.z*y.z;
}

vec2 WorldToScreen(vec2 pos,float height,float width) {
    float wW = (float)width / 2,wH = (float)height/2;
    float distX = pos.x - wW;
    float distY = wH - pos.y;
    return {distX / wW,distY / wH};
}
