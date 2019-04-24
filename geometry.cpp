#include "geometry.h"

float MathHelp::toRadians(float angle)
{
     return angle*PI/180;
}

int MathHelp::solveQuadratic(const float &a, const float &b, const float &c, float& t1, float& t2)
{
    float delta = b*b - 4*a*c;
    if(delta<0)
        return 0;
    if(delta == 0)
    {
        t1 = b/a*(-0.5);
        return 1;
    }

    delta = sqrt(delta);
    t1 = ((-1)*b-delta)/a*0.5;
    t2 = ((-1)*b+delta)/a*0.5;

    if(t1>t2)
    {
        if(t1>=0)
        {
            float pom = t1;
            t1 = t2;
            t2 = pom;
        }
    }
    return 2;
}
////******************************************************************
//// BASIC GEOMETRY CLASSES
Quat::Quat(float xA, float yA, float zA, float angle)
{
    w = cos(angle/2);
    x = xA * sin(angle/2);
    y = yA * sin(angle/2);
    z = zA * sin(angle/2);
    int pom = 2;
}

Quat Quat::operator*(Quat& param) const
{
    Quat result;
    result.w = this->w*param.w - this->x*param.x - this->y*param.y - this->z*param.z;
    result.x = this->w*param.x + this->x*param.w + this->y*param.z - this->z*param.y;
    result.y = this->w*param.y - this->x*param.z + this->y*param.w + this->z*param.x;
    result.z = this->w*param.z + this->x*param.y - this->y*param.x + this->z*param.w;
    return result;
}

void Quat::toMatrix(float m[4][4])
{
    float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

    x2 = x + x; y2 = y + y;
    z2 = z + z;
    xx = x * x2; xy = x * y2; xz = x * z2;
    yy = y * y2; yz = y * z2; zz = z * z2;
    wx = w * x2; wy = w * y2; wz = w * z2;


    m[0][0] = 1.0 - (yy + zz);
    m[1][0] = xy - wz;
    m[2][0] = xz + wy;
    m[3][0] = 0.0;

    m[0][1] = xy + wz;
    m[1][1] = 1.0 - (xx + zz);
    m[2][1] = yz - wx;
    m[3][1] = 0.0;

    m[0][2] = xz - wy;
    m[1][2] = yz + wx;
    m[2][2] = 1.0 - (xx + yy);
    m[3][2] = 0.0;

    m[0][3] = 0;
    m[1][3] = 0;
    m[2][3] = 0;
    m[3][3] = 1;
}

float Quat::getMagni() const
{
    return sqrt(pow(this->x,2)+pow(this->y,2)+pow(this->z,2)+pow(this->w,2));
}

void Quat::normalize()
{
    float magni = this->getMagni();
    x /= magni;
    y /= magni;
    z /= magni;
    w /= magni;
}

float Quat::angle()
{
    return 2*acos(w);
}

Vec3 Vec3::operator+(Vec3& param) const
{
    return Vec3(this->x+param.x, this->y+param.y, this->z+param.z);
}

Vec3 Vec3::operator-(Vec3& param) const
{
    return Vec3(this->x-param.x, this->y-param.y, this->z-param.z);
}

Vec3 Vec3::mult(float& param) const
{
    return Vec3(this->x*param, this->y*param, this->z*param);
}

float Vec3::operator*(Vec3& param) const
{
    return this->x*param.x + this->y*param.y + this->z*param.z;
}

float Vec3::getLenght() const
{
   return sqrt(pow(this->x,2)+pow(this->y,2)+pow(this->z,2));
}

void Vec3::normalize()
{
    float len = getLenght();
    x/=len;
    y/=len;
    z/=len;
}

Vec3 Vec3::cross(Vec3& param) const
{
    Vec3 result;
    result.x = this->y*param.z - this->z*param.y;
    result.y = this->z*param.x - this->x*param.z;
    result.z = this->x*param.y - this->y*param.x;
    Vec3 pom = result;
    return result;
}

float Vec3::angle(Vec3& param) const
{
    float result;
    result = acos( *this*param/(this->getLenght()*param.getLenght()) ) * 180.0 / 3.14159265;
    return result;
}
////////////////////////////////////////////////////////////////////////////

bool Sphere::intersectionTest(const Vec3& ray, std::vector<Vec3>& points, bool acc)
{
    MathHelp mh;
    Vec3 L(-1*crds.x,-1*crds.y,-1*crds.z);
    float t0;
    float t1;

    float a = 1; // assuming ray vector comes normalized
    float b = ray*L*2;
    float c = L*L-r*r;

    int amount = mh.solveQuadratic(a, b, c, t0, t1);

    if(amount == 0)
        return false;
    if(!acc) return true;

    points.push_back(ray.mult(t0));
    if(amount == 2)
        points.push_back(ray.mult(t1));

    return true;
}

////******************************************************************
//// FUNCTIONALITY

// INTERSECTION

void IntersectHelp::setRay(Vec3& ray)
{
    this->ray = ray;
}

bool IntersectHelp::testObject(Object* obj, std::vector<Vec3>& points)
{
    if(obj->intersectionTest(ray, points, true))
    {
        return true;
    }
    return false;
}

void Orbit::rotateOrbital(Object* obj, Vec3& p1, Vec3& p2)
{
    // calculating vectors from center of sphere to points
    Vec3 center = obj->getCrds();
    Vec3 p1s = center-p1;
    Vec3 p2s = center-p2;

    float angle = p1s.angle(p2s);
    Vec3 axis = p1s.cross(p2s);

    // building temporal quaternion of that rotation
    Quat rotation(axis.x, axis.y, axis.z, angle);
    rotation.normalize();

    Quat actual = obj->getRotations();
    Quat total_rotation = rotation*actual;
    obj->setRotations(total_rotation);
    actual = obj->getRotations();
}








