#include "camera.hpp"
#include <math.h>

void Camera::Init(Vec3 startPos){
    pos         = startPos;
    yaw         = 0.f;
    pitch       = 0.f;
    speed       = 10.f;
    sensitivity = 0.002f;
}

Vec3 Camera::GetForward() const {
    return {
        cosf(pitch) * sinf(yaw),
        sinf(pitch),
        cosf(pitch) * -cosf(yaw)
    };
}

Vec3 Camera::GetRight() const {
    return Mat4::Vec3Norm(Mat4::Vec3Cross(GetForward(), {0,1,0}));
}

void Camera::HandleInput(const Uint8* keys, float dt){
    Vec3 fwd   = GetForward();
    Vec3 right = GetRight();

    if(keys[SDL_SCANCODE_W]){ pos.x+=fwd.x*speed*dt; pos.y+=fwd.y*speed*dt; pos.z+=fwd.z*speed*dt; }
    if(keys[SDL_SCANCODE_S]){ pos.x-=fwd.x*speed*dt; pos.y-=fwd.y*speed*dt; pos.z-=fwd.z*speed*dt; }
    if(keys[SDL_SCANCODE_A]){ pos.x+=right.x*speed*dt; pos.y+=right.y*speed*dt; pos.z+=right.z*speed*dt; }
    if(keys[SDL_SCANCODE_D]){ pos.x-=right.x*speed*dt; pos.y-=right.y*speed*dt; pos.z-=right.z*speed*dt; }
    if(keys[SDL_SCANCODE_SPACE])     pos.y += speed * dt;
    if(keys[SDL_SCANCODE_LSHIFT])    pos.y -= speed * dt;
}

void Camera::HandleMouse(int dx, int dy){
    yaw   -= dx * sensitivity;
    pitch -= dy * sensitivity;
    if(pitch >  1.5f) pitch =  1.5f;
    if(pitch < -1.5f) pitch = -1.5f;
}

Mat4 Camera::GetViewMatrix() const {
    Vec3 fwd = GetForward();
    Vec3 target = { pos.x+fwd.x, pos.y+fwd.y, pos.z+fwd.z };
    return Mat4::LookAt(pos, target, {0,1,0});
}
