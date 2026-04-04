#include "utils.h"
#include <cmath>

vec2 vec2::operator+(const vec2 &b) {
    return {x+b.x,y+b.y};
}
vec2 vec2::operator-(const vec2 &b) {
    return {x-b.x,y-b.y};
}
vec2 vec2::operator *(float r) {
    return {x/r,y/r};
}
vec2 vec2::operator /(float r) {
    return {x/r,y/r};
}


vec2& vec2::operator +=(vec2 r) {
    x += r.x;
    y += r.y;
    return *this;
}
vec2& vec2::operator -=(vec2 r) {
    x -= r.x;
    y -= r.x;
    return *this;
}
vec2& vec2::operator *=(float r) {
    x *= r;
    y *= r;
    return *this;
}
vec2& vec2::operator /=(float r) {
    x /= r;
    y /= r;
    return *this;
}

std::ostream& operator<<(std::ostream &os,const vec2& val) {
    os << "["<<val.x<<","<<val.y<<"]";
    return os;
}

vec3 vec3::operator +(const vec3 &b) {
    return {x + b.x,y + b.y,z + b.z};
}
vec3 vec3::operator -(const vec3 &b) {
    return {x - b.x, y - b.y, z - b.z};
}
vec3 vec3::operator *(float r) {
    return {x * r,y * r,z * r};
}
vec3 vec3::operator /(float r) {
    return {x/r,y/r,z/r};
}

vec3& vec3::operator +=(vec3 r) {
    x += r.x;
    y += r.y;
    z += r.z;
    return *this;
}
vec3& vec3::operator -=(vec3 r) {
    x -= r.x;
    y -= r.y;
    z -= r.z;
    return *this;
}

vec3& vec3::operator *=(float r) {
    x *= r;
    y *= r;
    z *= r;
    return *this;
}
vec3& vec3::operator /=(float r) {
    x /= r;
    y /= r;
    z /= r;
    return *this;
}

std::ostream& operator<<(std::ostream &os,const vec3& val) {
    os << '[' << val.x << ','<< val.y << ',' << val.z << ']';
    return os;
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
