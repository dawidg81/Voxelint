#pragma once
#include <math.h>

struct Vec2 { float x, y; };
struct Vec3 { float x, y, z; };
struct Vec4 { float x, y, z, w; };

struct Mat4 {
    float m[4][4] = {};

    static Mat4 Identity(){
        Mat4 r;
        r.m[0][0] = r.m[1][1] = r.m[2][2] = r.m[3][3] = 1.f;
        return r;
    }

    static Mat4 Perspective(float fovY, float aspect, float nearZ, float farZ){
    Mat4 m = {};

    float f = 1.0f / tanf(fovY * 0.5f);

    m.m[0][0] = f / aspect;
    m.m[1][1] = f;

    m.m[2][2] = (farZ + nearZ) / (nearZ - farZ);
    m.m[2][3] = (2 * farZ * nearZ) / (nearZ - farZ);

    m.m[3][2] = -1.0f;

    return m;
}

    static Mat4 LookAt(Vec3 eye, Vec3 center, Vec3 up){
    Vec3 f = Vec3Norm({ center.x-eye.x, center.y-eye.y, center.z-eye.z });
    Vec3 r = Vec3Norm(Vec3Cross(up, f));   // correct: right = up × forward
    Vec3 u = Vec3Cross(f, r);

    Mat4 m = Mat4::Identity();

    // Row 0
    m.m[0][0] = r.x;
    m.m[0][1] = r.y;
    m.m[0][2] = r.z;
    m.m[0][3] = -Vec3Dot(r, eye);

    // Row 1
    m.m[1][0] = u.x;
    m.m[1][1] = u.y;
    m.m[1][2] = u.z;
    m.m[1][3] = -Vec3Dot(u, eye);

    // Row 2
    m.m[2][0] = -f.x;
    m.m[2][1] = -f.y;
    m.m[2][2] = -f.z;
    m.m[2][3] = Vec3Dot(f, eye);

    return m;
}

    static float Vec3Dot(Vec3 a, Vec3 b){ return a.x*b.x + a.y*b.y + a.z*b.z; }
    static Vec3  Vec3Cross(Vec3 a, Vec3 b){
        return { a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x };
    }
    static Vec3  Vec3Norm(Vec3 v){
        float l = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
        if(l == 0) return {0,0,0};
        return { v.x/l, v.y/l, v.z/l };
    }
};

inline Vec4 Mat4MulVec4(const Mat4& m, Vec4 v){
    return {
        m.m[0][0]*v.x + m.m[0][1]*v.y + m.m[0][2]*v.z + m.m[0][3]*v.w,
        m.m[1][0]*v.x + m.m[1][1]*v.y + m.m[1][2]*v.z + m.m[1][3]*v.w,
        m.m[2][0]*v.x + m.m[2][1]*v.y + m.m[2][2]*v.z + m.m[2][3]*v.w,
        m.m[3][0]*v.x + m.m[3][1]*v.y + m.m[3][2]*v.z + m.m[3][3]*v.w
    };
}

inline Mat4 Mat4Mul(const Mat4& a, const Mat4& b){
    Mat4 r;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            for(int k=0;k<4;k++)
                r.m[i][j] += a.m[i][k] * b.m[k][j];
    return r;
}
