#include "SDLHelp.h"

SDLHelp::SDLHelp()
{
    surf_display = NULL;
    running = true;
    width = 1000;
    height = 800;
    sphereX = 20;
    sphereY = 30;
    genSphere(1,sphereX,sphereY);
    objects.push_back(new Sphere(Vec3(0,0,-5),1));
    orbitActive = false;
}

void SDLHelp::genRay(int mouseX, int mouseY)
{
    double matModelView[16], matProjection[16];
    int viewport[4];

    glGetDoublev( GL_MODELVIEW_MATRIX, matModelView );
    glGetDoublev( GL_PROJECTION_MATRIX, matProjection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    double winX = (double)mouseX;
    double winY = viewport[3] - (double)mouseY;

    GLdouble startP[3];
    GLdouble endP[3];

    gluUnProject(winX, winY, 0.0, matModelView, matProjection,
         viewport, &startP[0], &startP[1], &startP[2]);

    gluUnProject(winX, winY, 1.0, matModelView, matProjection,
         viewport, &endP[0], &endP[1], &endP[2]);

    ray.x = endP[0]-startP[0];
    ray.y = endP[1]-startP[1];
    ray.z = endP[2]-startP[2];
}

void SDLHelp::genSphere(float r, int countY, int countX)
{
    MathHelp mh;
    float stepI = countY;
    float stepJ = 360/float(countX);
    for(int i = -stepI/2; i<=stepI/2; i++)
    {
        float pom = pow(r,2)-pow(2*r*(i/stepI),2);
        float rN = sqrt(pom);
        float z = ((2*r)/stepI*i);
        for(float j = 0; j<360; j+=stepJ)
        {
            float x = rN*cos(mh.toRadians(j));
            float y = rN*sin(mh.toRadians(j));
            sphereCrds.push_back(Vec3(x,z,y));
        }
    }
    sphereCrds.push_back(Vec3(0,-r,0));
    sphereCrds.push_back(Vec3(0,r,0));
}

void SDLHelp::draw()
{
    for(int i = 0; i<objects.size(); i++)
    {
        Vec3 crds = objects[i]->getCrds();
        //Vec3 rot = objects[i]->getRotations();
        Sphere* sp = dynamic_cast< Sphere* >( objects[i] );
        float scale = sp->getR();
        float quat[4][4];
        objects[0]->getRotations().toMatrix(quat);

        glPushMatrix();

        glColor3f(1,1,1);

        glTranslatef(crds.x, crds.y, crds.z);
        glScalef(scale, scale, scale);
        glMultMatrixf((GLfloat*)quat);
        drawSphere(sphereX,sphereY);

        glPopMatrix();
        //delete quat;
    }
}

void SDLHelp::drawSphere(int countY, int countX)
{
    for(int i = 0; i<countY; i++)
    {
        //glColor3f(1,1,1);
        glBegin(GL_LINE_STRIP);
        for(int j = 0; j<countX; j++)
        {
            glVertex3f(sphereCrds[i*countX+j].x, sphereCrds[i*countX+j].y, sphereCrds[i*countX+j].z);
        }
        glVertex3f(sphereCrds[i*countX].x, sphereCrds[i*countX].y, sphereCrds[i*countX].z);
        glEnd();
    }

    for(int i = 0; i<countX; i++)
    {
        //glColor3f(0,1,0);
        glBegin(GL_LINE_STRIP);
        glVertex3f(sphereCrds[sphereCrds.size()-2].x, sphereCrds[sphereCrds.size()-2].y, sphereCrds[sphereCrds.size()-2].z);

        for(int j = 0; j<countY; j++)
        {
            glVertex3f(sphereCrds[j*countX+i].x, sphereCrds[j*countX+i].y, sphereCrds[j*countX+i].z);
        }

        glVertex3f(sphereCrds[sphereCrds.size()-1].x, sphereCrds[sphereCrds.size()-1].y, sphereCrds[sphereCrds.size()-1].z);
        glEnd();
    }

    glBegin(GL_LINE_STRIP);
            glVertex3f(axis.x*-5, axis.y*-5, axis.z*-5);
            glVertex3f(axis.x*5, axis.y*5, axis.z*5);
    glEnd();
}

void SDLHelp::drawQuad(GLfloat x, GLfloat y)
{
    glBegin(GL_QUADS);
        glVertex2f(-x/2,-y/2);
        glVertex2f(x/2,-y/2);
        glVertex2f(x/2,y/2);
        glVertex2f(-x/2,y/2);
        glEnd();
}

int SDLHelp::OnExecute()
{
    // initiate
    if(OnInit() == false)
    {
        return -1;
    }

    SDL_Event event;

    // main loop
    while(running)
    {
        // catch events
        while(SDL_PollEvent(&event))
        {
            // make use of them
            OnEvent(&event);
        }

        OnLoop();
        OnRender();
    }
    // if loop was terminated
    // we are cleaning the leftovers
    OnCleanup();

    return 0;
}

bool SDLHelp::OnInit()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return false;

    if((surf_display = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL)) == NULL)
        return false;

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,width/height,1.0,500.0);
    //gluOrtho2D(0,width,height,0);
    //glTranslatef(0,0,-15);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    return true;
}

void SDLHelp::OnEvent(SDL_Event* Event)
{
    switch (Event->type)
    {
    case SDL_QUIT:
        running = false;
        break;

    case SDL_MOUSEBUTTONDOWN:
        switch(Event->button.button)
        {
        case SDL_BUTTON_LEFT:
            {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX,&mouseY);

                genRay(mouseX, mouseY);
                ray.normalize();
                IntersectHelp ih;
                ih.setRay(ray);

                std::vector<Vec3> inter;
                if(ih.testObject(objects[0], inter))
                {
                    p1 = inter[0];
                    orbitActive = true;
                }
            }
            break;
        }
        break;

    case SDL_MOUSEBUTTONUP:
        switch(Event->button.button)
        {
        case SDL_BUTTON_LEFT:
            {
                orbitActive = false;
            }
            break;
        }
        break;

    case SDL_MOUSEMOTION:
        {
            if(!orbitActive)
                return;

            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX,&mouseY);

            genRay(mouseX, mouseY);
            ray.normalize();
            IntersectHelp ih;
            ih.setRay(ray);

            std::vector<Vec3> inter;
            if(ih.testObject(objects[0], inter))
            {
                p2 = inter[0];
                Orbit orbit;
                orbit.rotateOrbital(objects[0], p1, p2);
                p1 = p2;
            }
        }

    default:
        {
        }
        break;
    }

}

void SDLHelp::OnLoop()
{

}

void SDLHelp::OnRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //
    draw();
    //

    SDL_GL_SwapBuffers();
}

void SDLHelp::OnCleanup()
{
    SDL_Quit();
}
