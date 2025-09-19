#pragma once
class Vector3D
{
public:
	Vector3D(float x, float y, float z) noexcept : _x(x), _y(y), _z(z) {} ;
	Vector3D& operator = (const Vector3D& other);

	~Vector3D() = default;
	//sobrecarga operadores
	//sumas y restas
	Vector3D operator +(const Vector3D& other) const;
	Vector3D operator -(const Vector3D& other) const;
	void operator +=(const Vector3D& other) noexcept;
	void operator -=(const Vector3D& other) noexcept;
	
	//multiplicacion por un factor
	Vector3D operator *(float factor) const;
	Vector3D operator /(float factor) const;

	//producto escalar
	float dot(const Vector3D& other) const;
	//producto vectorial
	Vector3D cross(const Vector3D& other) const;
	//modulo
	float norma()const;
	//modulo al cuadrado
	float norma2()const;
	//normalizar el vector
	float normalize();
protected:
	float _x;
	float _y;
	float _z;
};

