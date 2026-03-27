#pragma once
#include "math.hpp"
#include <SDL2/SDL.h>

struct Camera {
    Vec3  pos;
    float yaw;    // horizontal angle (radians)
    float pitch;  // vertical angle (radians)
    float speed;
    float sensitivity;

    void Init(Vec3 startPos);
    void HandleInput(const Uint8* keys, float dt);
    void HandleMouse(int dx, int dy);
    Mat4 GetViewMatrix() const;
    Vec3 GetForward() const;
    Vec3 GetRight() const;
};
