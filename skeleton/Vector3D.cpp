#include "Vector3D.h"
#include <cmath> 
Vector3D& Vector3D::operator=(const Vector3D& otro) {
    if (this != &otro) { // Evitar autoasignación
        this->_x = otro._x;
        this->_y = otro._y;
        this->_z = otro._z;
    }
    return *this;
}
// Para usar sqrt
Vector3D Vector3D::operator+(const Vector3D& other) const
{
    return Vector3D(_x + other._x, _y + other._y, _z + other._z);
}

Vector3D Vector3D::operator-(const Vector3D& other) const
{
    return Vector3D(_x - other._x, _y - other._y, _z - other._z);
}

Vector3D Vector3D::operator*(float factor) const
{
    return Vector3D(_x*factor, _y * factor, _z * factor);
}

Vector3D Vector3D::operator/(float factor) const
{
    return Vector3D(_x / factor, _y / factor, _z / factor);
}

float Vector3D::dot(const Vector3D& other) const
{
    return (_x * other._x + _y * other._y + _z * other._z);
}

Vector3D Vector3D::cross(const Vector3D& other) const
{
    return {
        (_y*other._z - _z * other._y),
        (_z * other._x - _x * other._z)
        , (_x * other._y - _y * other._x) };
}

float Vector3D::norma2() const
{
    return _x * _x + _y * _y + _z * _z;
}
float Vector3D::norma() const
{
    float sol = norma2();
    return sqrt(sol);
}

float Vector3D::normalize()
{
    float mod = norma();
    _x = _x / mod;
    _y = _y / mod;
    _z = _z / mod;
    return mod;
}
