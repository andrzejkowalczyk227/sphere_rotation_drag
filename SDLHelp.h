#ifndef _SDLHELP_H_
#define _SDLHELP_H_

#include<SDL/SDL.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<vector>
#include"geometry.h"

class SDLHelp
{
private:
    bool running;
    SDL_Surface* surf_display;
    float width;
    float height;
    std::vector<Vec3> sphereCrds;
    std::vector<Object*> objects;
    Vec3 axis;
    int sphereX;
    int sphereY;
    Vec3 ray;

    Vec3 p1;
    Vec3 p2;

    bool orbitActive;

public:
    SDLHelp();
    int OnExecute();
    bool OnInit();
    void OnEvent(SDL_Event* Event);
    void OnLoop();
    void OnRender();
    void OnCleanup();

    void drawQuad(GLfloat x, GLfloat y);
    void genSphere(float r, int countY, int countX);
    void drawSphere(int countY, int countX);
    void genRay(int mouseX, int mouseY);
    void draw();
};

#endif // _SDLHELP_H_
