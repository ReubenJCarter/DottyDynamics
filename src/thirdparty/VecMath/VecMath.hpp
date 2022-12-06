#pragma once

#include <math.h>

//
//Defs
//

#define PI 3.14159265359
#define TWO_PI 6.28318530718

class Vec2; 
class Vec3; 
class Vec4; 
class Mat2; 
class Mat3; 
class Mat4; 
class Quat; 

class Vec2{
public:
    float x, y;

	Vec2();
    Vec2(float xNew, float yNew);
	Vec2& set(float xNew, float yNew); 
	Vec2& set(Vec2& v); 
	float len();
	float len2();
	float dot(Vec2& v);
	Vec2& add(Vec2& v);
	Vec2& sub(Vec2& v);
	Vec2& mult(Vec2& v);
	Vec2& div(Vec2& v);
	Vec2& norm();
	Vec2& neg();
	Vec2& multm(Mat2& m);
	Vec2& multm3(Mat3& m);
	Vec2& mults(float s);
	Vec2& divs(float s);
	bool approxEql(Vec2& v, float eps=0.000001);
};

class Vec3{
public:
    float x, y, z; 

	Vec3();
	Vec3(float xNew, float yNew, float zNew);
	Vec3& set(float xNew, float yNew, float zNew); 
	Vec3& set(Vec3& v); 
	float len();
	float len2();
	float dot(Vec3& v);
	Vec3& add(Vec3& v);
	Vec3& sub(Vec3& v);
	Vec3& mult(Vec3& v);
	Vec3& div(Vec3& v);
	Vec3& norm();
	Vec3& neg();
	Vec3& multm(Mat3& m);
	Vec3& multm4(Mat4& m);
	Vec3& mults(float s);
	Vec3& divs(float s);
	Vec3& cross(Vec3& v);
	bool approxEql(Vec3& v, float eps=0.000001);
};

class Vec4{
public:
    float x, y, z, w; 

	Vec4();
	Vec4(float xNew, float yNew, float zNew, float wNew);
	Vec4& set(float xNew, float yNew, float zNew, float wNew); 
	Vec4& set(Vec4& v); 
	float len();
	float len2();
	float dot(Vec4& v);
	Vec4& add(Vec4& v);
	Vec4& sub(Vec4& v);
	Vec4& mult(Vec4& v);
	Vec4& div(Vec4& v);
	Vec4& norm();
	Vec4& neg();
	Vec4& multm(Mat4& m);
	Vec4& mults(float s);
	Vec4& divs(float s);
	bool approxEql(Vec4& v, float eps=0.000001);
};

class Mat2{
public:
    float x0, y0, x1, y1; 

	static Mat2 identity();
	Mat2();
	Mat2& setcol(int c, Vec2& v);
	Mat2& setcol0(Vec2& v);
	Mat2& setcol1(Vec2& v);
	Mat2& copy(Mat2& m);
	float det();
	Mat2& inv();
	Mat2& tran();
	Mat2& mult(Mat2& m);
	Mat2& mults(float s);
	bool approxEql(Mat2& v, float eps=0.000001);
};

class Mat3{
public:
    float x0, y0, z0, x1, y1, z1, x2, y2, z2; 

	static Mat3 identity();
	Mat3();
	Mat3& setcol(int c, Vec3& v);
	Mat3& setcol0(Vec3& v);
	Mat3& setcol1(Vec3& v);
	Mat3& setcol2(Vec3& v);
	Mat3& copy(Mat3& m);
	Mat3& copy(Mat4& m);
	float det();
	Mat3& inv();
	Mat3& tran();
	Mat3& mult(Mat3& m);
	Mat3& mults(float s);
	bool approxEql(Mat3& v, float eps=0.000001);
};

class Mat4{
public:
    float x0, y0, z0, w0,  x1, y1, z1, w1,  x2, y2, z2, w2,  x3, y3, z3, w3; 

	static Mat4 identity();
	static Mat4 perspective(float fov, float aspect, float near, float far, bool leftHanded=true);
	static Mat4 orthographic(float left, float right, float bottom, float top, float near, float far, bool leftHanded=true );
	Mat4();
	Mat4& setcol(int c, Vec4& v);
	Mat4& setcol0(Vec4& v);
	Mat4& setcol1(Vec4& v);
	Mat4& setcol2(Vec4& v);
	Mat4& setcol3(Vec4& v);
	Mat4& copy(const Mat4& m);
	Mat4& copy(const float* m);
	float det();
	Mat4& inv();
	Mat4& tran();
	Mat4& mult(const Mat4& m);
	Mat4& mults(float s);
	Mat4& setAsTransform(const Vec3& position, const Quat& rotation, const Vec3& scale);
	Quat getRotation();
	void decomposeTRS(Vec3& position, Quat& rotation, Vec3& scale);
	void changeHandTRS(); 
	Mat4& setAsPerspective(float fov, float aspect, float near, float far, bool leftHanded=true);
	bool approxEql(Mat4& v, float eps=0.000001);
};

class Quat{
public:
    float x, y, z, w;

	static Quat axisAngle(Vec3& axis, float angle);
	static Quat identity();
	static float normalizeAngle(float angle); 
	static Vec3 normalizeAngles(Vec3& angles); 
	Quat();
	Quat(float xNew, float yNew, float zNew, float wNew);
	Quat& setAxisAngle(Vec3& axis, float angle);
	Quat& setEuler(Vec3& v);
	Vec3 getEuler();
	Mat4 getRotationMatrix() const; 
	Quat& mult(Quat& q);
};


//
//Impl
//


float signf(float v){
	return v >= 0 ? 1.0f : -1.0f;
}

//Vec2

Vec2::Vec2(){
	x = 0; 
	y = 0;
}

Vec2::Vec2(float xNew, float yNew){
	x = xNew; 
	y = yNew;
}

Vec2& Vec2::set(float xNew, float yNew){
	x = xNew; 
	y = yNew;
	return *this;
}

Vec2& Vec2::set(Vec2& v){
	x = v.x; 
	y = v.y;
	return *this;
}

float Vec2::len(){ 
	return sqrt(x*x + y*y);
}

float Vec2::len2(){ 
	return x*x + y*y;
}

float Vec2::dot(Vec2& v){
	return x*v.x + y*v.y;
}

Vec2& Vec2::add(Vec2& v){ 
	x = x + v.x; 
	y = y + v.y;
	return *this;
}

Vec2& Vec2::sub(Vec2& v){ 
	x = x - v.x; 
	y = y - v.y;
	return *this;
}

Vec2& Vec2::mult(Vec2& v){ 
	x = x * v.x;
	y = y * v.y;
	return *this;
}

Vec2& Vec2::div(Vec2& v){ 
	x = x / v.x; 
	y = y / v.y;
	return *this;
}

Vec2& Vec2::norm(){ 
	float l = sqrt(x*x + y*y);
	x = x / l; 
	y = y / l;
	return *this;		
}

Vec2& Vec2::neg(){ 
	x = -x;
	y = -y; 
	return *this;
}

Vec2& Vec2::multm(Mat2& m){
	float xn = x * m.x0 + y * m.x1; 
	y = x * m.y0 + y * m.y1; 
	x = xn; 
	return *this;
}

Vec2& Vec2::multm3(Mat3& m){
	//assume w is 1
	float xn = x * m.x0 + y * m.x1 + m.x2; 
	y = x * m.y0 + y * m.y1 + m.y2; 
	x = xn;
	return *this;
}

Vec2& Vec2::mults(float s){
	x *= s; 
	y *= s;
	return *this;
}

Vec2& Vec2::divs(float s){
	x /= s; 
	y /= s;
	return *this;
}

bool Vec2::approxEql(Vec2& v, float eps){
	return x > v.x - eps && x < v.x + eps  &&
			y > v.y - eps && y < v.y + eps; 
}


//
//Vec3


Vec3::Vec3(){
	x = 0; 
	y = 0;
	z = 0;
}

Vec3::Vec3(float xNew, float yNew, float zNew){
	x = xNew; 
	y = yNew; 
	z = zNew;
}

Vec3& Vec3::set(float xNew, float yNew, float zNew){
	x = xNew; 
	y = yNew; 
	z = zNew;
	return *this;
} 

Vec3& Vec3::set(Vec3& v){
	x = v.x; 
	y = v.y; 
	z = v.z; 
	return *this;
}

float Vec3::len(){ 
	return sqrt(x*x + y*y + z*z);
}

float Vec3::len2(){ 
	return x*x + y*y + z*z;
}

float Vec3::dot(Vec3& v){
	return x*v.x + y*v.y + z*v.z;
}

Vec3& Vec3::add(Vec3& v){ 
	x = x + v.x; 
	y = y + v.y;
	z = z + v.z;
	return *this;
}

Vec3& Vec3::sub(Vec3& v){ 
	x = x - v.x; 
	y = y - v.y;
	z = z - v.z;
	return *this;
}

Vec3& Vec3::mult(Vec3& v){ 
	x = x * v.x;
	y = y * v.y;
	z = z * v.z;
	return *this;
}

Vec3& Vec3::div(Vec3& v){ 
	x = x / v.x; 
	y = y / v.y;
	z = z / v.z;
	return *this;
}

Vec3& Vec3::norm(){ 
	float l = sqrt(x*x + y*y + z*z);
	x = x / l; 
	y = y / l; 
	z = z / l;
	return *this;
}

Vec3& Vec3::neg(){ 
	x = -x;
	y = -y;
	z = -z;	
	return *this;
}

Vec3& Vec3::multm(Mat3& m){
	float xn = x * m.x0 + y * m.x1 + z * m.x2; 
	float yn = x * m.y0 + y * m.y1 + z * m.y2; 
	z = x * m.z0 + y * m.z1 + z * m.z2; 
	
	x = xn; 
	y = yn; 
	
	return *this;
}

Vec3& Vec3::multm4(Mat4& m){
	//assume w is 1
	float xn = x * m.x0 + y * m.x1 + z * m.x2 + m.x3; 
	float yn = x * m.y0 + y * m.y1 + z * m.y2 + m.y3;  
	z = x * m.z0 + y * m.z1 + z * m.z2 + m.z3; 
	
	x = xn; 
	y = yn;
	
	return *this;
}

Vec3& Vec3::mults(float s){
	x *= s; 
	y *= s;
	z *= s;
	return *this;
}

Vec3& Vec3::divs(float s){
	x /= s; 
	y /= s;
	z /= s;
	return *this;
}

Vec3& Vec3::cross(Vec3& v){
	float xn = y * v.z - z * v.y; 
	float yn = z * v.x - x * v.z; 
	z =   x * v.y - y * v.x; 
	
	x = xn;
	y = yn;
	
	return *this; 
}

bool Vec3::approxEql(Vec3& v, float eps){
	return x > v.x - eps && x < v.x + eps  &&
			y > v.y - eps && y < v.y + eps &&
			z > v.z - eps && z < v.z + eps; 
}


//
//Vec4

Vec4::Vec4(){
	x = 0;
	y = 0;
	z = 0; 
	w = 0; 
}

Vec4::Vec4(float xNew, float yNew, float zNew, float wNew){
	x = xNew; 
	y = yNew; 
	z = zNew;
	w = wNew;
}

Vec4& Vec4::set(float xNew, float yNew, float zNew, float wNew){
	x = xNew; 
	y = yNew; 
	z = zNew;
	w = wNew;
	return *this;
} 

Vec4& Vec4::set(Vec4& v){
	x = v.x; 
	y = v.y; 
	z = v.z;
	w = v.w; 
	return *this; 
}

float Vec4::len(){ 
	return sqrt(x*x + y*y + z*z + w*w);
}

float Vec4::len2(){ 
	return x*x + y*y + z*z + w*w;
}

float Vec4::dot(Vec4& v){
	return x*v.x + y*v.y + z*v.z + w*v.w;
}

Vec4& Vec4::add(Vec4& v){ 
	x = x + v.x; 
	y = y + v.y;
	z = z + v.z;
	w = w + v.w;
	return *this;
}

Vec4& Vec4::sub(Vec4& v){ 
	x = x - v.x; 
	y = y - v.y;
	z = z - v.z;
	w = w - v.w;
	return *this;
}

Vec4& Vec4::mult(Vec4& v){ 
	x = x * v.x;
	y = y * v.y;
	z = z * v.z;
	w = w * v.w;
	return *this;
}

Vec4& Vec4::div(Vec4& v){ 
	x = x / v.x; 
	y = y / v.y;
	z = z / v.z;
	w = w / v.w;
	return *this;
}

Vec4& Vec4::norm(){ 
	float l = sqrt(x*x + y*y + z*z + w*w);
	x = x / l; 
	y = y / l; 
	z = z / l;
	w = w / l;
	return *this;
}

Vec4& Vec4::neg(){ 
	x = -x;
	y = -y;
	z = -z;		
	w = -w;
	return *this;
}

Vec4& Vec4::multm(Mat4& m){
	float xn = x * m.x0 + y * m.x1 + z * m.x2 + w * m.x3; 
	float yn = x * m.y0 + y * m.y1 + z * m.y2 + w * m.y3;  
	float zn = x * m.z0 + y * m.z1 + z * m.z2 + w * m.z3; 
	w = x * m.w0 + y * m.w1 + z * m.w2 + w * m.w3; 
	
	x = xn; 
	y = yn; 
	z = zn; 
	
	return *this;
}

Vec4& Vec4::mults(float s){
	x *= s; 
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

Vec4& Vec4::divs(float s){
	x /= s; 
	y /= s;
	z /= s;
	w /= s; 
	return *this;
}

bool Vec4::approxEql(Vec4& v, float eps){
	return x > v.x - eps && x < v.x + eps  &&
			y > v.y - eps && y < v.y + eps &&
			z > v.z - eps && z < v.z + eps && 
			w > v.w - eps && w < v.w + eps; 
}


//
//Mat2

Mat2 Mat2::identity(){
	Mat2 newM;
	
	newM.x0 = 1; 
	newM.y0 = 0; 
	
	newM.x1 = 0; 
	newM.y1 = 1; 
	
	return newM; 
}

Mat2::Mat2(){
	x0 = 0;
	y0 = 0;
	
	x1 = 0;
	y1 = 0;
}

Mat2& Mat2::setcol(int c, Vec2& v){
	if(c == 0){
		x0 = v.x;
		y0 = v.y;
	}
	else{
		x1 = v.x;
		y1 = v.y;
	}
	return *this; 
}

Mat2& Mat2::setcol0(Vec2& v){
	x0 = v.x;
	y0 = v.y;
	return *this; 
}

Mat2& Mat2::setcol1(Vec2& v){
	x1 = v.x;
	y1 = v.y;
	return *this; 
}

Mat2& Mat2::copy(Mat2& m){
	x0 = m.x0; 
	y0 = m.y0; 
	
	x1 = m.x1; 
	y1 = m.y1; 
	
	return *this; 
}

float Mat2::det(){
	return x0 * y1 - x1 * y0; 
}

Mat2& Mat2::inv(){
	float invdet = 1.0 / (x0 * y1 - x1 * y0); 
	
	y0 = -y0 * invdet;
	x1 = -x1 * invdet; 
	float x0New = y1 * invdet;
	y1 = x0 * invdet;
	x0 = x0New;
	
	return *this; 
}

Mat2& Mat2::tran(){
	float oldy0 = y0; 
	y0 = x1;
	x1 = oldy0; 
	
	return *this; 
}

Mat2& Mat2::mult(Mat2& m){ 
	//multiplication ordering is m*this
	float x0New = m.x0 * x0 + m.x1 * y0; 
	float y0New = m.y0 * x0 + m.y1 * y0; 
	
	x0 = x0New; 
	y0 = y0New;
	
	float x1New = m.x0 * x1 + m.x1 * y1; 
	float y1New = m.y0 * x1 + m.y1 * y1;  
	
	x1 = x1New; 
	y1 = y1New; 
	
	return *this; 
}

Mat2& Mat2::mults(float s){
	x0 *= s; 
	y0 *= s; 
	
	x1 *= s; 
	y1 *= s;
	
	return *this;
}

bool Mat2::approxEql(Mat2& v, float eps){
	return x0 > v.x0 - eps && x0 < v.x0 + eps  &&
			y0 > v.y0 - eps && y0 < v.y0 + eps &&
			x1 > v.x1 - eps && x1 < v.x1 + eps && 
			y1 > v.y1 - eps && y1 < v.y1 + eps; 
}


//
//Mat3

Mat3 Mat3::identity(){
	Mat3 newM;
	
	newM.x0 = 1; 
	newM.y0 = 0; 
	newM.z0 = 0; 
	
	newM.x1 = 0; 
	newM.y1 = 1; 
	newM.z1 = 0;

	newM.x2 = 0; 
	newM.y2 = 0; 
	newM.z2 = 1;
	
	return newM;
}

Mat3::Mat3(){
	x0 = 0;
	y0 = 0;
	z0 = 0;
	
	x1 = 0;
	y1 = 0;
	z1 = 0;
	
	x2 = 0;
	y2 = 0;
	z2 = 0;
}

Mat3& Mat3::setcol(int c, Vec3& v){
	if(c == 0){
		x0 = v.x;
		y0 = v.y;
		z0 = v.z;
	}
	else if(c == 1){
		x1 = v.x;
		y1 = v.y;
		z1 = v.z;
	}
	else{
		x2 = v.x;
		y2 = v.y;
		z2 = v.z;
	}
	return *this; 
}

Mat3& Mat3::setcol0(Vec3& v){
	x0 = v.x;
	y0 = v.y;
	z0 = v.z;
	return *this; 
}

Mat3& Mat3::setcol1(Vec3& v){
	x1 = v.x;
	y1 = v.y;
	z1 = v.z;
	return *this; 
}

Mat3& Mat3::setcol2(Vec3& v){
	x2 = v.x;
	y2 = v.y;
	z2 = v.z;
	return *this; 
}

Mat3& Mat3::copy(Mat3& m){
	x0 = m.x0; 
	y0 = m.y0; 
	z0 = m.z0; 
	
	x1 = m.x1; 
	y1 = m.y1; 
	z1 = m.z1; 
	
	x2 = m.x2; 
	y2 = m.y2; 
	z2 = m.z2; 
	
	return *this; 
}

Mat3& Mat3::copy(Mat4& m){
	x0 = m.x0; 
	y0 = m.y0; 
	z0 = m.z0; 
	
	x1 = m.x1; 
	y1 = m.y1; 
	z1 = m.z1; 
	
	x2 = m.x2; 
	y2 = m.y2; 
	z2 = m.z2; 
	
	return *this; 
}

float Mat3::det(){
	float det0 = y1 * z2 - y2 * z1; 
	float det1 = y0 * z2 - y2 * z0; 
	float det2 = y0 * z1 - y1 * z0; 
	
	return x0 * det0 - x1 * det1 + x2 * det2;
}

Mat3& Mat3::inv(){
	/*
	
	//m[col, row]
	m[0][0] = x0; 
	m[0][1] = y0; 
	m[0][2] = z0; 
	
	m[1][0] = x1; 
	m[1][1] = y1; 
	m[1][2] = z1;
	
	m[2][0] = x2; 
	m[2][1] = y2; 
	m[2][2] = z2;
	*/
	
	float det0 = y1 * z2 - y2 * z1; 
	float det1 = y0 * z2 - y2 * z0; 
	float det2 = y0 * z1 - y1 * z0; 
	
	float det = x0 * det0 - x1 * det1 + x2 * det2;
	float invdet = 1/det;
	
	float newx0 = +(y1 * z2 - y2 * z1) * invdet;
	float newx1 = -(x1 * z2 - x2 * z1) * invdet;
	float newx2 = +(x1 * y2 - x2 * y1) * invdet;
	float newy0 = -(y0 * z2 - y2 * z0) * invdet;
	float newy1 = +(x0 * z2 - x2 * z0) * invdet;
	float newy2 = -(x0 * y2 - x2 * y0) * invdet;
	float newz0 = +(y0 * z1 - y1 * z0) * invdet;
	float newz1 = -(x0 * z1 - x1 * z0) * invdet;
	float newz2 = +(x0 * y1 - x1 * y0) * invdet;
	
	x0 = newx0; 
	y0 = newy0; 
	z0 = newz0; 
	
	x1 = newx1; 
	y1 = newy1; 
	z1 = newz1;
	
	x2 = newx2; 
	y2 = newy2; 
	z2 = newz2;
	
	return *this; 
}

Mat3& Mat3::tran(){
	//swap y0 and x1   x0 x1 x2
	//swap z0 and x2   y0 y1 y2
	//swap z1 and y2   z0 z1 z2
	
	float oldy0 = y0; 
	y0 = x1;
	x1 = oldy0;

	float oldz0 = z0; 
	z0 = x2;
	x2 = oldz0;
	
	float oldz1 = z1; 
	z1 = y2;
	y2 = oldz1;
	
	return *this; 
}

Mat3& Mat3::mult(Mat3& m){
	float x0New = m.x0 * x0 + m.x1 * y0 + m.x2 * z0; 
	float y0New = m.y0 * x0 + m.y1 * y0 + m.y2 * z0;
	float z0New = m.z0 * x0 + m.z1 * y0 + m.z2 * z0;		
	
	x0 = x0New; 
	y0 = y0New; 
	z0 = z0New;
	
	float x1New = m.x0 * x1 + m.x1 * y1 + m.x2 * z1; 
	float y1New = m.y0 * x1 + m.y1 * y1 + m.y2 * z1;
	float z1New = m.z0 * x1 + m.z1 * y1 + m.z2 * z1;
	
	x1 = x1New; 
	y1 = y1New; 
	z1 = z1New; 
	
	float x2New = m.x0 * x2 + m.x1 * y2 + m.x2 * z2; 
	float y2New = m.y0 * x2 + m.y1 * y2 + m.y2 * z2;
	float z2New = m.z0 * x2 + m.z1 * y2 + m.z2 * z2; 
	
	x2 = x2New; 
	y2 = y2New; 
	z2 = z2New;
	
	return *this;
}

Mat3& Mat3::mults(float s){
	x0 *= s; 
	y0 *= s; 
	z0 *= s; 
	
	x1 *= s; 
	y1 *= s; 
	z1 *= s; 
	
	x2 *= s; 
	y2 *= s; 
	z2 *= s;
	
	return *this;
}

bool Mat3::approxEql(Mat3& v, float eps){
	return x0 > v.x0 - eps && x0 < v.x0 + eps  &&
			y0 > v.y0 - eps && y0 < v.y0 + eps &&
			z0 > v.z0 - eps && z0 < v.z0 + eps &&
		
			x1 > v.x1 - eps && x1 < v.x1 + eps && 
			y1 > v.y1 - eps && y1 < v.y1 + eps &&
			z1 > v.z1 - eps && z1 < v.z1 + eps &&

			x2 > v.x2 - eps && x2 < v.x2 + eps && 
			y2 > v.y2 - eps && y2 < v.y2 + eps &&
			z2 > v.z2 - eps && z2 < v.z2 + eps; 
}


//
//Mat4

Mat4 Mat4::identity(){
	Mat4 newM;
	
	newM.x0 = 1; 
	newM.y0 = 0; 
	newM.z0 = 0;
	newM.w0 = 0;		
	
	newM.x1 = 0; 
	newM.y1 = 1; 
	newM.z1 = 0;
	newM.w1 = 0;

	newM.x2 = 0; 
	newM.y2 = 0; 
	newM.z2 = 1;
	newM.w2 = 0;
	
	newM.x3 = 0; 
	newM.y3 = 0; 
	newM.z3 = 0;
	newM.w3 = 1;
	
	return newM;
}

Mat4 Mat4::perspective(float fov, float aspect, float near, float far, bool leftHanded){
	//https://github.com/g-truc/glm/blob/b3f87720261d623986f164b2a7f6a0a938430271/glm/ext/matrix_clip_space.inl
	//perspectiveLH_NO     / RH  (clip space z= -1 to 1  not 0 to 1 like z0 version)
	//http://www.songho.ca/opengl/gl_projectionmatrix.html
	//m[col, row]
	//m[3][2] = z3
	//m[2][3] = w2
	
	//x0 x1 x2 x3
	//y0 y1 y2 y3
	//z0 z1 z2 z3
	//w0 w1 w2 w3
	
	Mat4 pm; 
	
	if(fov <= 0 || aspect == 0){
		return pm;
	}

	float invd = 1.0f / (far - near);
	float tanHalfFov = tan(0.5f * fov);

	pm.y0 = 0;
	pm.z0 = 0;
	pm.w0 = 0;
	
	pm.x1 = 0;
	pm.z1 = 0;
	pm.w1 = 0;
	
	pm.x2 = 0;
	pm.y2 = 0;
	
	pm.x3 = 0;
	pm.y3 = 0;
	pm.w3 = 0;
	
	pm.x0 = 1.0f / (aspect * tanHalfFov);
	pm.y1 = 1.0f / tanHalfFov;
	pm.z2 = (leftHanded ? 1:-1) * (far + near) * invd;
	pm.w2 = (leftHanded ? 1:-1);
	pm.z3 = -(2 * far * near) * invd;		
	
	return pm; 
}

Mat4 Mat4::orthographic(float left, float right, float bottom, float top, float near, float far, bool leftHanded ){
	Mat4 pm; 
	
	float raddl = right + left; 
	float taddb = top + bottom; 
	float faddn = far + near;
	float rsubl = right - left; 
	float tsubb = top - bottom; 
	float fsubn = far - near; 

	pm.x0 = 2.0f / rsubl;
	pm.y1 = 2.0f / tsubb;
	pm.z2 = (leftHanded ? 2:-2) / fsubn;
	pm.x3 = -raddl / rsubl;
	pm.y3 = -taddb / tsubb;
	pm.z3 = -faddn / fsubn;
	
	return pm; 
}

Mat4::Mat4(){
}

Mat4& Mat4::setcol(int c, Vec4& v){
	if(c == 0){
		x0 = v.x;
		y0 = v.y;
		z0 = v.z;
		w0 = v.w;
	}
	else if(c == 1){
		x1 = v.x;
		y1 = v.y;
		z1 = v.z;
		w1 = v.w;
	}
	else if(c == 2){
		x2 = v.x;
		y2 = v.y;
		z2 = v.z;
		w2 = v.w;
	}
	else{
		x3 = v.x;
		y3 = v.y;
		z3 = v.z;
		w3 = v.w;
	}
	return *this; 
}

Mat4& Mat4::setcol0(Vec4& v){
	x0 = v.x;
	y0 = v.y;
	z0 = v.z;
	w0 = v.w;
	return *this; 
}

Mat4& Mat4::setcol1(Vec4& v){
	x1 = v.x;
	y1 = v.y;
	z1 = v.z;
	w1 = v.w;
	return *this; 
}

Mat4& Mat4::setcol2(Vec4& v){
	x2 = v.x;
	y2 = v.y;
	z2 = v.z;
	w2 = v.w;
	return *this; 
}

Mat4& Mat4::setcol3(Vec4& v){
	x3 = v.x;
	y3 = v.y;
	z3 = v.z;
	w3 = v.w;
	return *this; 
}

Mat4& Mat4::copy(const Mat4& m){
	x0 = m.x0; 
	y0 = m.y0; 
	z0 = m.z0; 
	w0 = m.w0; 
	
	x1 = m.x1; 
	y1 = m.y1; 
	z1 = m.z1; 
	w1 = m.w1; 
	
	x2 = m.x2; 
	y2 = m.y2; 
	z2 = m.z2; 
	w2 = m.w2; 
	
	x3 = m.x3; 
	y3 = m.y3; 
	z3 = m.z3; 
	w3 = m.w3; 
	
	return *this; 
}

Mat4& Mat4::copy(const float* m){
	x0 = m[0]; 
	y0 = m[1]; 
	z0 = m[2]; 
	w0 = m[3]; 
	
	x1 = m[4]; 
	y1 = m[5]; 
	z1 = m[6]; 
	w1 = m[7]; 
	
	x2 = m[8]; 
	y2 = m[9]; 
	z2 = m[10]; 
	w2 = m[11]; 
	
	x3 = m[12]; 
	y3 = m[13]; 
	z3 = m[14]; 
	w3 = m[15]; 
	
	return *this; 
}

float Mat4::det(){
	/*
	
	//m[col, row]
	m[0][0] = x0; 
	m[0][1] = y0; 
	m[0][2] = z0; 
	m[0][3] = w0; 
	
	m[1][0] = x1; 
	m[1][1] = y1; 
	m[1][2] = z1;
	m[1][3] = w1;
	
	m[2][0] = x2; 
	m[2][1] = y2; 
	m[2][2] = z2;
	m[2][3] = w2;
	
	m[3][0] = x3; 
	m[3][1] = y3; 
	m[3][2] = z3;
	m[3][3] = w3;
	*/
	/* From glm ....
	T SubFactor00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	T SubFactor01 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	T SubFactor02 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	T SubFactor03 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	T SubFactor04 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	T SubFactor05 = m[2][0] * m[3][1] - m[3][0] * m[2][1];

	vec<4, T, Q> DetCof(
		+ (m[1][1] * SubFactor00 - m[1][2] * SubFactor01 + m[1][3] * SubFactor02),
		- (m[1][0] * SubFactor00 - m[1][2] * SubFactor03 + m[1][3] * SubFactor04),
		+ (m[1][0] * SubFactor01 - m[1][1] * SubFactor03 + m[1][3] * SubFactor05),
		- (m[1][0] * SubFactor02 - m[1][1] * SubFactor04 + m[1][2] * SubFactor05));

	return
		m[0][0] * DetCof[0] + m[0][1] * DetCof[1] +
		m[0][2] * DetCof[2] + m[0][3] * DetCof[3];*/
		
	float SubFactor00 = z2 * w3 - z3 * w2;
	float SubFactor01 = y2 * w3 - y3 * w2;
	float SubFactor02 = y2 * z3 - y3 * z2;
	float SubFactor03 = x2 * w3 - x3 * w2;
	float SubFactor04 = x2 * z3 - x3 * z2;
	float SubFactor05 = x2 * y3 - x3 * y2;

	float DetCof0 = +(y1 * SubFactor00 - z1 * SubFactor01 + w1 * SubFactor02); 
	float DetCof1 = -(x1 * SubFactor00 - z1 * SubFactor03 + w1 * SubFactor04);
	float DetCof2 = +(x1 * SubFactor01 - y1 * SubFactor03 + w1 * SubFactor05);
	float DetCof3 = -(x1 * SubFactor02 - y1 * SubFactor04 + z1 * SubFactor05);

	return x0 * DetCof0 + y0 * DetCof1   +   z0 * DetCof2 + w0 * DetCof3;
}

Mat4& Mat4::inv(){
	//https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
	
	//m[col, row]
	/*
	m[0][0] = x0; 
	m[0][1] = y0; 
	m[0][2] = z0; 
	m[0][3] = w0; 
	
	m[1][0] = x1; 
	m[1][1] = y1; 
	m[1][2] = z1;
	m[1][3] = w1;
	
	m[2][0] = x2; 
	m[2][1] = y2; 
	m[2][2] = z2;
	m[2][3] = w2;
	
	m[3][0] = x3; 
	m[3][1] = y3; 
	m[3][2] = z3;
	m[3][3] = w3;
	*/
	
	float A2323 = z2 * w3 - w2 * z3 ;
	float A1323 = y2 * w3 - w2 * y3 ;
	float A1223 = y2 * z3 - z2 * y3 ;
	float A0323 = x2 * w3 - w2 * x3 ;
	float A0223 = x2 * z3 - z2 * x3 ;
	float A0123 = x2 * y3 - y2 * x3 ;
	float A2313 = z1 * w3 - w1 * z3 ;
	float A1313 = y1 * w3 - w1 * y3 ;
	float A1213 = y1 * z3 - z1 * y3 ;
	float A2312 = z1 * w2 - w1 * z2 ;
	float A1312 = y1 * w2 - w1 * y2 ;
	float A1212 = y1 * z2 - z1 * y2 ;
	float A0313 = x1 * w3 - w1 * x3 ;
	float A0213 = x1 * z3 - z1 * x3 ;
	float A0312 = x1 * w2 - w1 * x2 ;
	float A0212 = x1 * z2 - z1 * x2 ;
	float A0113 = x1 * y3 - y1 * x3 ;
	float A0112 = x1 * y2 - y1 * x2 ;

	float det = x0 * ( y1 * A2323 - z1 * A1323 + w1 * A1223 ) 
		-y0 * ( x1 * A2323 - z1 * A0323 + w1 * A0223 ) 
		+z0 * ( x1 * A1323 - y1 * A0323 + w1 * A0123 ) 
		-w0 * ( x1 * A1223 - y1 * A0223 + z1 * A0123 ) ;
	det = 1 / det;

	float m00 = det *  ( y1 * A2323 - z1 * A1323 + w1 * A1223 );
	float m01 = det * -( y0 * A2323 - z0 * A1323 + w0 * A1223 );
	float m02 = det *  ( y0 * A2313 - z0 * A1313 + w0 * A1213 );
	float m03 = det * -( y0 * A2312 - z0 * A1312 + w0 * A1212 );
	float m10 = det * -( x1 * A2323 - z1 * A0323 + w1 * A0223 );
	float m11 = det *  ( x0 * A2323 - z0 * A0323 + w0 * A0223 );
	float m12 = det * -( x0 * A2313 - z0 * A0313 + w0 * A0213 );
	float m13 = det *  ( x0 * A2312 - z0 * A0312 + w0 * A0212 );
	float m20 = det *  ( x1 * A1323 - y1 * A0323 + w1 * A0123 );
	float m21 = det * -( x0 * A1323 - y0 * A0323 + w0 * A0123 );
	float m22 = det *  ( x0 * A1313 - y0 * A0313 + w0 * A0113 );
	float m23 = det * -( x0 * A1312 - y0 * A0312 + w0 * A0112 );
	float m30 = det * -( x1 * A1223 - y1 * A0223 + z1 * A0123 );
	float m31 = det *  ( x0 * A1223 - y0 * A0223 + z0 * A0123 );
	float m32 = det * -( x0 * A1213 - y0 * A0213 + z0 * A0113 );
	float m33 = det *  ( x0 * A1212 - y0 * A0212 + z0 * A0112 );
	
	x0 = m00;
	y0 = m01;
	z0 = m02;
	w0 = m03;
	
	x1 = m10;
	y1 = m11;
	z1 = m12;
	w1 = m13;
	
	x2 = m20;
	y2 = m21;
	z2 = m22;
	w2 = m23;
	
	x3 = m30;
	y3 = m31;
	z3 = m32;
	w3 = m33;
	
	return *this; 
}

Mat4& Mat4::tran(){
	//x0 x1 x2 x3
	//y0 y1 y2 y3
	//z0 z1 z2 z3
	//w0 w1 w2 w3
	
	//swap y0 and x1
	//swap z0 and x2
	//swap z1 and y2
	//swap w0 and x3
	//swap w1 and y3
	//swap w2 and z3
	
	float temp = y0; 
	y0 = x1;
	x1 = temp;

	temp = z0; 
	z0 = x2;
	x2 = temp;
	
	temp = z1; 
	z1 = y2;
	y2 = temp;
	
	temp = w0; 
	w0 = x3;
	x3 = temp;
	
	temp = w1; 
	w1 = y3;
	y3 = temp;
	
	temp = w2; 
	w2 = z3;
	z3 = temp;
	
	return *this; 
}

Mat4& Mat4::mult(const Mat4& m){
	float x0New = m.x0 * x0 + m.x1 * y0 + m.x2 * z0 + m.x3 * w0; 
	float y0New = m.y0 * x0 + m.y1 * y0 + m.y2 * z0 + m.y3 * w0;
	float z0New = m.z0 * x0 + m.z1 * y0 + m.z2 * z0 + m.z3 * w0;	
	float w0New = m.w0 * x0 + m.w1 * y0 + m.w2 * z0 + m.w3 * w0;	

	x0 = x0New; 
	y0 = y0New; 
	z0 = z0New; 
	w0 = w0New; 		
	
	float x1New = m.x0 * x1 + m.x1 * y1 + m.x2 * z1 + m.x3 * w1; 
	float y1New = m.y0 * x1 + m.y1 * y1 + m.y2 * z1 + m.y3 * w1;
	float z1New = m.z0 * x1 + m.z1 * y1 + m.z2 * z1 + m.z3 * w1;
	float w1New = m.w0 * x1 + m.w1 * y1 + m.w2 * z1 + m.w3 * w1;
	
	x1 = x1New; 
	y1 = y1New; 
	z1 = z1New; 
	w1 = w1New;
	
	float x2New = m.x0 * x2 + m.x1 * y2 + m.x2 * z2 + m.x3 * w2; 
	float y2New = m.y0 * x2 + m.y1 * y2 + m.y2 * z2 + m.y3 * w2;
	float z2New = m.z0 * x2 + m.z1 * y2 + m.z2 * z2 + m.z3 * w2;
	float w2New = m.w0 * x2 + m.w1 * y2 + m.w2 * z2 + m.w3 * w2;
	
	x2 = x2New; 
	y2 = y2New; 
	z2 = z2New;
	w2 = w2New; 
	
	float x3New = m.x0 * x3 + m.x1 * y3 + m.x2 * z3 + m.x3 * w3; 
	float y3New = m.y0 * x3 + m.y1 * y3 + m.y2 * z3 + m.y3 * w3;
	float z3New = m.z0 * x3 + m.z1 * y3 + m.z2 * z3 + m.z3 * w3;
	float w3New = m.w0 * x3 + m.w1 * y3 + m.w2 * z3 + m.w3 * w3;
	
	x3 = x3New; 
	y3 = y3New; 
	z3 = z3New;
	w3 = w3New; 
	
	return *this;
}

Mat4& Mat4::mults(float s){
	x0 *= s; 
	y0 *= s; 
	z0 *= s; 
	w0 *= s; 
	
	x1 *= s; 
	y1 *= s; 
	z1 *= s; 
	w1 *= s; 
	
	x2 *= s; 
	y2 *= s; 
	z2 *= s;
	w2 *= s; 
	
	x3 *= s; 
	y3 *= s; 
	z3 *= s;
	w3 *= s; 
	
	return *this;
}

Mat4& Mat4::setAsTransform(const Vec3& position, const Quat& rotation, const Vec3& scale){

	//Set the rotation Part of the TRS matrix 
	Mat4 R = rotation.getRotationMatrix(); 
	copy(R); 

	//Multiply by the scale matrix
	float sx = scale.x; 
	float sy = scale.y; 
	float sz = scale.z; 

	x0 *= sx; 
	y0 *= sy; 
	z0 *= sz; 

	x1 *= sx; 
	y1 *= sy; 
	z1 *= sz;

	x2 *= sx; 
	y2 *= sy; 
	z2 *= sz;

	//Add in the translation
	x3 = position.x;
	y3 = position.y;
	z3 = position.z;

	return *this;
}

Quat Mat4::getRotation(){
	//https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
	//https://answers.unity.com/questions/11363/converting-matrix4x4-to-quaternion-vector3.html

	//m[0,0] -> x0; 
	//m[1,1] -> y1;
	//m[2,2] -> z1;
	//[row, col] -> x/y/z 0/1/2
	
	//[2,1]-> z1
	//[0,2]-> x2
	//[1,0]-> y0
	
	//[1,2]-> y2
	//[2,0]-> z0
	//[0,1]-> x1

	Quat q;
	
    q.w = sqrtf( fmax( 0, 1 + x0 + y1 + z2 ) ) / 2; 
    q.x = sqrtf( fmax( 0, 1 + x0 - y1 - z2 ) ) / 2; 
    q.y = sqrtf( fmax( 0, 1 - x0 + y1 - z2 ) ) / 2; 
    q.z = sqrtf( fmax( 0, 1 - x0 - y1 + z2 ) ) / 2; 

    q.x *= signf( q.x * ( z1 - y2 ) );
    q.y *= signf( q.y * ( x2 - z0 ) );
    q.z *= signf( q.z * ( y0 - x1 ) );

	return q; 
}

void Mat4::decomposeTRS(Vec3& position, Quat& rotation, Vec3& scale){
	//https://github.com/gltf-rs/gltf/issues/21
	//https://github.com/google/vector_math.dart/blob/master/lib/src/vector_math/matrix4.dart#L1766-L1804

	//position
	position.x = x3; 
	position.y = y3; 
	position.z = z3; 

	//scale 
	float sx = sqrt(x0*x0 + y0*y0 + z0*z0); 
	float sy = sqrt(x1*x1 + y1*y1 + z1*z1); 
	float sz = sqrt(x2*x2 + y2*y2 + z2*z2); 
    
    if (det() < 0) {
      sx = -sx;
    }

	//rotation
	Mat3 r;
	r.x0 = x0 / sx; 
	r.y0 = y0 / sx; 
	r.z0 = z0 / sx; 
	r.x1 = x1 / sy; 
	r.y1 = y1 / sy; 
	r.z1 = z1 / sy; 
	r.x2 = x2 / sz; 
	r.y2 = y2 / sz; 
	r.z2 = z2 / sz; 

	rotation.w = sqrtf( fmax( 0, 1 + r.x0 + r.y1 + r.z2 ) ) / 2; 
    rotation.x = sqrtf( fmax( 0, 1 + r.x0 - r.y1 - r.z2 ) ) / 2; 
    rotation.y = sqrtf( fmax( 0, 1 - r.x0 + r.y1 - r.z2 ) ) / 2; 
    rotation.z = sqrtf( fmax( 0, 1 - r.x0 - r.y1 + r.z2 ) ) / 2; 

    rotation.x *= signf( rotation.x * ( r.z1 - r.y2 ) );
    rotation.y *= signf( rotation.y * ( r.x2 - r.z0 ) );
    rotation.z *= signf( rotation.z * ( r.y0 - r.x1 ) );
}

void Mat4::changeHandTRS(){
	//https://answers.unity.com/questions/1729262/changing-a-transform-matrix-from-right-handed-to-l.html
	z0 = -z0; 
	z1 = -z1; 
	z3 = -z3;

	x2 = -x2; 
	y2 = -y2;
}

Mat4& Mat4::setAsPerspective(float fov, float aspect, float near, float far, bool leftHanded){
	//https://github.com/g-truc/glm/blob/b3f87720261d623986f164b2a7f6a0a938430271/glm/ext/matrix_clip_space.inl
	//perspectiveLH_NO     / RH  (clip space z= -1 to 1  not 0 to 1 like z0 version)
	//http://www.songho.ca/opengl/gl_projectionmatrix.html
	
	if(fov <= 0 || aspect == 0){
		return *this;
	}

	float invd = 1 / (far - near);
	float tanHalfFov = tan(0.5 * fov);
	
	x0 = 1 / (aspect * tanHalfFov);
	y1 = 1 / tanHalfFov;
	z2 = (leftHanded ? 1:-1) * (far + near) * invd;
	w2 = (leftHanded ? 1:-1);
	z3 = -(2 * far * near) * invd;		

	y0 = 0;
	z0 = 0;
	w0 = 0;
	
	x1 = 0;
	z1 = 0;
	w1 = 0;
	
	x2 = 0;
	y2 = 0;
	
	x3 = 0;
	y3 = 0;
	w3 = 0;
	
	return *this; 
}

bool Mat4::approxEql(Mat4& v, float eps){
	return x0 > v.x0 - eps && x0 < v.x0 + eps  &&
			y0 > v.y0 - eps && y0 < v.y0 + eps &&
			z0 > v.z0 - eps && z0 < v.z0 + eps &&
			w0 > v.w0 - eps && w0 < v.w0 + eps &&
		
			x1 > v.x1 - eps && x1 < v.x1 + eps && 
			y1 > v.y1 - eps && y1 < v.y1 + eps &&
			z1 > v.z1 - eps && z1 < v.z1 + eps &&
			w1 > v.w1 - eps && w1 < v.w1 + eps &&

			x2 > v.x2 - eps && x2 < v.x2 + eps && 
			y2 > v.y2 - eps && y2 < v.y2 + eps &&
			z2 > v.z2 - eps && z2 < v.z2 + eps && 
			w2 > v.w2 - eps && w2 < v.w2 + eps && 

			x3 > v.x3 - eps && x3 < v.x3 + eps && 
			y3 > v.y3 - eps && y3 < v.y3 + eps &&
			z3 > v.z3 - eps && z3 < v.z3 + eps && 
			w3 > v.w3 - eps && w3 < v.w3 + eps 
		; 
}


//
//Quat

Quat Quat::axisAngle(Vec3& axis, float angle){
	//https://lucidar.me/en/quaternions/quaternions-rotations/
	float halfAngle = angle / 2.0f; 
	float sinHalfAngle = sin(halfAngle); 
	float x = axis.x * sinHalfAngle;
	float y = axis.y * sinHalfAngle;
	float z = axis.z * sinHalfAngle;
	float w = cos(halfAngle);
	
	return Quat(x, y, z, w);
}

Quat Quat::identity(){
	return Quat(0, 0, 0, 1);
}

float Quat::normalizeAngle(float angle){
	while (angle > TWO_PI)
		angle -= TWO_PI;
	while (angle < 0)
		angle += TWO_PI;
	return angle;
}

Vec3 Quat::normalizeAngles(Vec3& angles){
	Vec3 v(normalizeAngle(angles.x), normalizeAngle(angles.y), normalizeAngle(angles.z)); 
	return v;  
}

Quat::Quat(){
	x = 0; 
	y = 0; 
	z = 0; 
	w = 1; 
}

Quat::Quat(float xNew, float yNew, float zNew, float wNew){
	x = xNew;
	y = yNew; 
	z = zNew; 
	w = wNew; 
}

Quat& Quat::setAxisAngle(Vec3& axis, float angle){
	float halfAngle = angle / 2; 
	float sinHalfAngle = sin(halfAngle); 
	x = axis.x * sinHalfAngle;
	y = axis.y * sinHalfAngle;
	z = axis.z * sinHalfAngle;
	w = cos(halfAngle);
	
	return *this; 
}

//https://gist.github.com/aeroson/043001ca12fe29ee911e
//https://stackoverflow.com/questions/12088610/conversion-between-euler-quaternion-like-in-unity3d-engine
Quat& Quat::setEuler(Vec3& v){
	float yaw = v.x;
	float pitch = v.y;
	float roll = v.z;

	float rollOver2 = roll * 0.5f;
	float sinRollOver2 = sinf(rollOver2);
	float cosRollOver2 = cosf(rollOver2);
	float pitchOver2 = pitch * 0.5f;
	float sinPitchOver2 = sinf(pitchOver2);
	float cosPitchOver2 = cosf(pitchOver2);
	float yawOver2 = yaw * 0.5f;
	float sinYawOver2 = sinf(yawOver2);
	float cosYawOver2 = cosf(yawOver2);
	/*
	x = cosYawOver2 * cosPitchOver2 * cosRollOver2 + sinYawOver2 * sinPitchOver2 * sinRollOver2;
	y = cosYawOver2 * cosPitchOver2 * sinRollOver2 - sinYawOver2 * sinPitchOver2 * cosRollOver2;
	z = cosYawOver2 * sinPitchOver2 * cosRollOver2 + sinYawOver2 * cosPitchOver2 * sinRollOver2;
	w = sinYawOver2 * cosPitchOver2 * cosRollOver2 - cosYawOver2 * sinPitchOver2 * sinRollOver2;
	*/
	//Corrected by person in comments of gist 
	x = sinYawOver2 * cosPitchOver2 * cosRollOver2 + cosYawOver2 * sinPitchOver2 * sinRollOver2; // confirmed (scc+css)
	y = cosYawOver2 * sinPitchOver2 * cosRollOver2 - sinYawOver2 * cosPitchOver2 * sinRollOver2; // confirmed (csc-scs)
	z = cosYawOver2 * cosPitchOver2 * sinRollOver2 - sinYawOver2 * sinPitchOver2 * cosRollOver2; // confirmed (ccs-ssc)
	w = cosYawOver2 * cosPitchOver2 * cosRollOver2 + sinYawOver2 * sinPitchOver2 * sinRollOver2; // confirmed (ccc+sss)

	return *this; 
}

//https://gist.github.com/aeroson/043001ca12fe29ee911e
//https://stackoverflow.com/questions/12088610/conversion-between-euler-quaternion-like-in-unity3d-engine
Vec3 Quat::getEuler(){
	Vec3 v(0, 0, 0); 
	float sqw = w * w;
	float sqx = x * x;
	float sqy = y * y;
	float sqz = z * z;
	float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	float test = x * w - y * z;

	if (test > 0.4995f * unit)
	{ // singularity at north pole
		v.y = 2.0f * atan2f(y, x);
		v.x = PI / 2;
		v.z = 0;
		return normalizeAngles(v);
	}
	if (test < -0.4995f * unit)
	{ // singularity at south pole
		v.y = -2.0f * atan2(y, x);
		v.x = -PI / 2;
		v.z = 0;
		return normalizeAngles(v);
	}
	Quat q(w, z, x, y);
	v.y = atan2f(2.0f * q.x * q.w + 2.0f * q.y * q.z, 1.0f - 2.0f * (q.z * q.z + q.w * q.w));     // Yaw
	v.x = asinf(2.0f * (q.x * q.z - q.w * q.y));                             // Pitch
	v.z = atan2f(2.0f * q.x * q.y + 2.0f * q.z * q.w, 1.0f - 2.0f * (q.y * q.y + q.z * q.z));      // Roll
	return normalizeAngles(v);
}

Mat4 Quat::getRotationMatrix() const {
	Mat4 m;

	float x2 = 2.0f * x * x; 
	float y2 = 2.0f * y * y;
	float z2 = 2.0f * z * z; 

	float xy = 2.0f * x * y; 
	float xz = 2.0f * x * z; 
	float yz = 2.0f * y * z; 
	float wx = 2.0f * w * x; 
	float wy = 2.0f * w * y; 
	float wz = 2.0f * w * z; 

	m.x0 = 1 - y2 - z2; 
	m.y0 = xy + wz; 
	m.z0 = xz - wy; 
	m.w0 = 0; 

	m.x1 = xy - wz;
	m.y1 = 1 - x2 - z2; 
	m.z1 = yz + wx; 
	m.w1 = 0; 

	m.x2 = xz + wy; 
	m.y2 = yz - wx;
	m.z2 = 1 - x2 - y2;
	m.w2 = 0; 
	
	m.x3 = 0; 
	m.y3 = 0; 
	m.z3 = 0; 
	m.w3 = 1; 

	return m; 
}

Quat& Quat::mult(Quat& q){
	//https://www.sciencedirect.com/topics/computer-science/quaternion-multiplication
	float crossx = y * q.z - z * q.y; 
	float crossy = z * q.x - x * q.z; 
	float crossz = x * q.y - y * q.x; 
	float dotxyz = x * q.x + y * q.y + z * q.z; 
	
	w = w * q.w - dotxyz; 
	x = crossx + w * q.x + q.w * x; 
	y = crossy + w * q.y + q.w * y; 
	z = crossz + w * q.z + q.w * z; 
	
	return *this; 
}
