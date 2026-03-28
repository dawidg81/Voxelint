#include "renderer.hpp"
#include <math.h>

static const float COLOR_GRASS_TOP[3]  = { 67/255.f, 124/255.f,  37/255.f };
static const float COLOR_GRASS_SIDE[3] = { 97/255.f, 143/255.f,  72/255.f };
static const float COLOR_DIRT[3]       = {134/255.f,  96/255.f,  67/255.f };

void Renderer::Init(int w, int h){
    screenW = w;
    screenH = h;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glViewport(0, 0, w, h);

    // Projection matrix (fixed-function)
    float fov    = 60.f;
    float aspect = (float)w / (float)h;
    float near   = 0.1f;
    float far    = 500.f;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, aspect, near, far);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Renderer::BeginFrame(){
    glClearColor(135/255.f, 206/255.f, 235/255.f, 1.f); // sky blue
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static void DrawFace(
    float x0, float y0, float z0,
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    float x3, float y3, float z3,
    const float* color)
{
    glColor3f(color[0], color[1], color[2]);
    glBegin(GL_QUADS);
        glVertex3f(x0, y0, z0);
        glVertex3f(x1, y1, z1);
        glVertex3f(x2, y2, z2);
        glVertex3f(x3, y3, z3);
    glEnd();
}

void Renderer::DrawWorld(const World& world, const Camera& cam){
    // Load view matrix from camera
    Mat4 view = cam.GetViewMatrix();

    glMatrixMode(GL_MODELVIEW);
    // Mat4 is column-major already from our LookAt, GL expects column-major
    glLoadMatrixf(&view.m[0][0]);

    const float RENDER_DIST = 64.f;
    const float RD2 = RENDER_DIST * RENDER_DIST;

    for(int x = 0; x < WORLD_W; x++){
        for(int y = 0; y < WORLD_H; y++){
            for(int z = 0; z < WORLD_D; z++){
                if(world.Get(x,y,z) == BLOCK_AIR) continue;

                float dx = x + 0.5f - cam.pos.x;
                float dy = y + 0.5f - cam.pos.y;
                float dz = z + 0.5f - cam.pos.z;
                if(dx*dx + dy*dy + dz*dz > RD2) continue;

                bool isGrass = world.Get(x,y,z) == BLOCK_GRASS;
                float fx = (float)x, fy = (float)y, fz = (float)z;

                // TOP
                if(world.Get(x,y+1,z) == BLOCK_AIR)
                    DrawFace(fx,fy+1,fz,  fx,fy+1,fz+1,  fx+1,fy+1,fz+1,  fx+1,fy+1,fz,
                             isGrass ? COLOR_GRASS_TOP : COLOR_DIRT);

                // BOTTOM
                if(world.Get(x,y-1,z) == BLOCK_AIR)
                    DrawFace(fx,fy,fz,  fx+1,fy,fz,  fx+1,fy,fz+1,  fx,fy,fz+1,
                             COLOR_DIRT);

                // FRONT (z-)
                if(world.Get(x,y,z-1) == BLOCK_AIR)
                    DrawFace(fx,fy,fz,  fx,fy+1,fz,  fx+1,fy+1,fz,  fx+1,fy,fz,
                             isGrass ? COLOR_GRASS_SIDE : COLOR_DIRT);

                // BACK (z+)
                if(world.Get(x,y,z+1) == BLOCK_AIR)
                    DrawFace(fx+1,fy,fz+1,  fx+1,fy+1,fz+1,  fx,fy+1,fz+1,  fx,fy,fz+1,
                             isGrass ? COLOR_GRASS_SIDE : COLOR_DIRT);

                // LEFT (x-)
                if(world.Get(x-1,y,z) == BLOCK_AIR)
                    DrawFace(fx,fy,fz+1,  fx,fy+1,fz+1,  fx,fy+1,fz,  fx,fy,fz,
                             isGrass ? COLOR_GRASS_SIDE : COLOR_DIRT);

                // RIGHT (x+)
                if(world.Get(x+1,y,z) == BLOCK_AIR)
                    DrawFace(fx+1,fy,fz,  fx+1,fy+1,fz,  fx+1,fy+1,fz+1,  fx+1,fy,fz+1,
                             isGrass ? COLOR_GRASS_SIDE : COLOR_DIRT);
            }
        }
    }
}

void Renderer::BeginHUD(SDL_Window* win){
    // Switch to 2D orthographic for SDL HUD overlay
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, screenW, screenH, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
}

void Renderer::EndHUD(SDL_Window* win){
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
