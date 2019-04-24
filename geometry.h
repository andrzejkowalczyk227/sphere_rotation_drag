#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "math.h"
#include <vector>

class MathHelp
{
private:
    const static double PI = 3.14159265;
public:
    float toRadians(float angle);
    int solveQuadratic(const float &a, const float &b, const float &c, float& t1, float& t2);
};

////******************************************************************
//// BASIC GEOMETRY CLASSES

class Quat
{
public:
    float x;
    float y;
    float z;
    float w;

    Quat() : x(0), y(0), z(0), w(1){};
    Quat(float xA, float yA, float zA, float angle);
    Quat operator*(Quat& param) const;
    void toMatrix(float m[4][4]);
    float getMagni() const;
    void normalize();
    float angle();
};

class Vec3
{
public:
    Vec3() : x(0), y(0), z(0){};
    Vec3(float x, float y, float z) : x(x), y(y), z(z){};

    void normalize();
    float getLenght() const;
    Vec3 operator+(Vec3& param) const;
    Vec3 operator-(Vec3& param) const;
    Vec3 mult(float& param) const;
    float operator*(Vec3& param) const; // dot product
    Vec3 cross(Vec3& param) const;
    float angle(Vec3& param) const;

    float x;
    float y;
    float z;
};

class Object
{
protected:
    Vec3 crds;
    Quat rotations;
public:
    Object(Vec3 crds) : crds(crds){};
    Object(Vec3 crds, Quat rotations) : crds(crds), rotations(rotations) {};
    //virtual ~Object() = 0;

    Quat& getRotations(){return rotations;};
    void setRotations(Quat rotations){this->rotations=rotations;};
    Vec3& getCrds(){return crds;};
    virtual bool intersectionTest(const Vec3& ray, std::vector<Vec3>& points, bool acc) = 0;
};

class Sphere : public Object
{
private:
    float r;
public:
    Sphere(Vec3 crds, float r) : r(r), Object(crds){};
    Sphere(Vec3 crds, Quat rotations, float r) : Object(crds, rotations), r(r) {};
    //~Sphere(){};

    float getR(){return r;};
    bool intersectionTest(const Vec3& ray, std::vector<Vec3>& points, bool acc);
};
////******************************************************************
//// FUNCTIONALITY

// INTERSECTION

class IntersectHelp
{
private:
    Vec3 ray;

public:
    IntersectHelp(){};
    ~IntersectHelp(){};

    void setRay(Vec3& ray);
    bool testObject(Object* obj, std::vector<Vec3>& points);
};

// ORBITAL MOVEMENT

class Orbit
{
public:
    void rotateOrbital(Object* obj, Vec3& p1, Vec3& p2);
};
#endif







