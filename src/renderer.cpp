#include "renderer.hpp"
#include <math.h>
#include <algorithm>

static float edgeFunc(Vec2 a, Vec2 b, Vec2 p){
    return (p.x - a.x)*(b.y - a.y) - (p.y - a.y)*(b.x - a.x);
}

void Renderer::Init(int w, int h){
    screenW = w;
    screenH = h;
    depthBuffer.assign(w * h, 1.f);
    float aspect = (float)w / (float)h;
    proj = Mat4::Perspective(1.0472f, aspect, 0.1f, 500.f); // 60 deg fov
}

void Renderer::Clear(){
    std::fill(depthBuffer.begin(), depthBuffer.end(), 1.f);
}

bool Renderer::ProjectVertex(Vec4 clip, Vec3& out) const {
    if(clip.w <= 0.001f) return false;
    float invW = 1.f / clip.w;
    out.x = clip.x * invW;
    out.y = clip.y * invW;
    out.z = clip.z * invW;
    return true; // let rasterizer handle screen bounds, don't cull on z here
}

void Renderer::RasterTriangle(SDL_Renderer* sdl,
    Vec2 p0, Vec2 p1, Vec2 p2,
    float z0, float z1, float z2,
    Uint8 r, Uint8 g, Uint8 b)
{
    int minX = (int)std::max(0.f,       std::min({p0.x, p1.x, p2.x}));
    int maxX = (int)std::min((float)screenW-1, std::max({p0.x, p1.x, p2.x}));
    int minY = (int)std::max(0.f,       std::min({p0.y, p1.y, p2.y}));
    int maxY = (int)std::min((float)screenH-1, std::max({p0.y, p1.y, p2.y}));

    float area = edgeFunc(p0, p1, p2);
    if(area >= 0) return; // back-face cull

    for(int py = minY; py <= maxY; py++){
        for(int px = minX; px <= maxX; px++){
            Vec2 p = {(float)px + 0.5f, (float)py + 0.5f};
            float w0 = edgeFunc(p1, p2, p);
            float w1 = edgeFunc(p2, p0, p);
            float w2 = edgeFunc(p0, p1, p);
            if(w0 > 0 || w1 > 0 || w2 > 0) continue;

            float invArea = 1.f / area;
            float bc0 = w0 * invArea;
            float bc1 = w1 * invArea;
            float bc2 = w2 * invArea;
            float depth = bc0*z0 + bc1*z1 + bc2*z2;

            int idx = py * screenW + px;
            if(depth < depthBuffer[idx]){
                depthBuffer[idx] = depth;
                SDL_SetRenderDrawColor(sdl, r, g, b, 255);
                SDL_RenderDrawPoint(sdl, px, py);
            }
        }
    }
}

void Renderer::DrawTriangle(SDL_Renderer* sdl,
    Vec3 a, Vec3 b, Vec3 c,
    Uint8 r, Uint8 g, Uint8 b_,
    const Mat4& vp)
{
    auto xform = [&](Vec3 v) -> Vec4 {
        Vec4 v4 = {v.x, v.y, v.z, 1.f};
        return Mat4MulVec4(vp, v4);
    };

    Vec4 ca = xform(a), cb = xform(b), cc = xform(c);
    if(ca.w <= 0.f && cb.w <= 0.f && cc.w <= 0.f) return;

    Vec3 na, nb, nc;
    if(!ProjectVertex(ca, na)) return;
    if(!ProjectVertex(cb, nb)) return;
    if(!ProjectVertex(cc, nc)) return;

    if(na.x < -1 && nb.x < -1 && nc.x < -1) return;
    if(na.x >  1 && nb.x >  1 && nc.x >  1) return;
    if(na.y < -1 && nb.y < -1 && nc.y < -1) return;
    if(na.y >  1 && nb.y >  1 && nc.y >  1) return;
    if(na.z < -1 && nb.z < -1 && nc.z < -1) return;
    if(na.z >  1 && nb.z >  1 && nc.z >  1) return;

    auto toScreen = [&](Vec3 ndc) -> Vec2 {
        return {
            (ndc.x * 0.5f + 0.5f) * screenW,
            (1.f - (ndc.y * 0.5f + 0.5f)) * screenH
        };
    };

    Vec2 sa = toScreen(na), sb = toScreen(nb), sc = toScreen(nc);

    SDL_SetRenderDrawColor(sdl, r, g, b_, 255);
    SDL_RenderDrawLine(sdl, (int)sa.x, (int)sa.y, (int)sb.x, (int)sb.y);
    SDL_RenderDrawLine(sdl, (int)sb.x, (int)sb.y, (int)sc.x, (int)sc.y);
    SDL_RenderDrawLine(sdl, (int)sc.x, (int)sc.y, (int)sa.x, (int)sa.y);
}

// Face colors
static const Uint8 COLOR_GRASS_TOP[3]  = {  67, 124,  37 };
static const Uint8 COLOR_GRASS_SIDE[3] = {  97, 143,  72 };
static const Uint8 COLOR_DIRT[3]       = { 134,  96,  67 };

// Simple face visibility: only draw face if neighbor in that direction is air
void Renderer::DrawWorld(SDL_Renderer* sdl, const World& world, const Camera& cam){
    Mat4 view = cam.GetViewMatrix();
    Mat4 vp   = Mat4Mul(proj, view);

    const float RENDER_DIST = 48.f;

    for(int x = 0; x < WORLD_W; x++){
        for(int y = 0; y < WORLD_H; y++){
            for(int z = 0; z < WORLD_D; z++){
                if(world.Get(x,y,z) == BLOCK_AIR) continue;

		float dx = x + 0.5f - cam.pos.x;
                float dy = y + 0.5f - cam.pos.y;
                float dz = z + 0.5f - cam.pos.z;
                // if(dx*dx + dy*dy + dz*dz > RENDER_DIST*RENDER_DIST) continue;
                
		bool isGrass = world.Get(x,y,z) == BLOCK_GRASS;

                float fx = (float)x, fy = (float)y, fz = (float)z;

                // -- TOP face (y+1 is air) --
                if(world.Get(x,y+1,z) == BLOCK_AIR){
                    const Uint8* c = isGrass ? COLOR_GRASS_TOP : COLOR_DIRT;
                    Vec3 v0={fx,   fy+1,fz  }, v1={fx+1,fy+1,fz  },
                         v2={fx+1,fy+1,fz+1}, v3={fx,   fy+1,fz+1};
                    DrawTriangle(sdl,v0,v1,v2,c[0],c[1],c[2],vp);
                    DrawTriangle(sdl,v0,v2,v3,c[0],c[1],c[2],vp);
                }
                // -- BOTTOM face (y-1 is air) --
                if(world.Get(x,y-1,z) == BLOCK_AIR){
                    const Uint8* c = COLOR_DIRT;
                    Vec3 v0={fx,   fy,fz  }, v1={fx,   fy,fz+1},
                         v2={fx+1,fy,fz+1}, v3={fx+1,fy,fz  };
                    DrawTriangle(sdl,v0,v1,v2,c[0],c[1],c[2],vp);
                    DrawTriangle(sdl,v0,v2,v3,c[0],c[1],c[2],vp);
                }
                // -- FRONT face (z-1 is air) --
                if(world.Get(x,y,z-1) == BLOCK_AIR){
                    const Uint8* c = isGrass ? COLOR_GRASS_SIDE : COLOR_DIRT;
                    Vec3 v0={fx,   fy,  fz}, v1={fx+1,fy,  fz},
                         v2={fx+1,fy+1,fz}, v3={fx,   fy+1,fz};
                    DrawTriangle(sdl,v0,v1,v2,c[0],c[1],c[2],vp);
                    DrawTriangle(sdl,v0,v2,v3,c[0],c[1],c[2],vp);
                }
                // -- BACK face (z+1 is air) --
                if(world.Get(x,y,z+1) == BLOCK_AIR){
                    const Uint8* c = isGrass ? COLOR_GRASS_SIDE : COLOR_DIRT;
                    Vec3 v0={fx,   fy,  fz+1}, v1={fx,   fy+1,fz+1},
                         v2={fx+1,fy+1,fz+1}, v3={fx+1,fy,  fz+1};
                    DrawTriangle(sdl,v0,v1,v2,c[0],c[1],c[2],vp);
                    DrawTriangle(sdl,v0,v2,v3,c[0],c[1],c[2],vp);
                }
                // -- LEFT face (x-1 is air) --
                if(world.Get(x-1,y,z) == BLOCK_AIR){
                    const Uint8* c = isGrass ? COLOR_GRASS_SIDE : COLOR_DIRT;
                    Vec3 v0={fx,fy,  fz  }, v1={fx,fy+1,fz  },
                         v2={fx,fy+1,fz+1}, v3={fx,fy,  fz+1};
                    DrawTriangle(sdl,v0,v1,v2,c[0],c[1],c[2],vp);
                    DrawTriangle(sdl,v0,v2,v3,c[0],c[1],c[2],vp);
                }
                // -- RIGHT face (x+1 is air) --
                if(world.Get(x+1,y,z) == BLOCK_AIR){
                    const Uint8* c = isGrass ? COLOR_GRASS_SIDE : COLOR_DIRT;
                    Vec3 v0={fx+1,fy,  fz  }, v1={fx+1,fy,  fz+1},
                         v2={fx+1,fy+1,fz+1}, v3={fx+1,fy+1,fz  };
                    DrawTriangle(sdl,v0,v1,v2,c[0],c[1],c[2],vp);
                    DrawTriangle(sdl,v0,v2,v3,c[0],c[1],c[2],vp);
                }
            }
        }
    }
}
