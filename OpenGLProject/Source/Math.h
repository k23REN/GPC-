#pragma once

#include <memory.h>

#include <cassert>
#include <cmath>
#include <limits>
#include <xmmintrin.h>
class Vector3;
class Quaternion;
 void __vectorcall VecStore(Vector3* out, const __m128& in);
 void __vectorcall  QuatStore(Quaternion* out, const __m128& in);
#define M_PI 3.14159265358979
#define deg_to_rad(deg) (((deg) / 360) * 2 * M_PI)
#define rad_to_deg(rad) (((rad) / 2 / M_PI) * 360)
 namespace test{

   const float tanHalfFovy = tanf(deg_to_rad(60.0f) / 2.0f);

   float a_near = 0.01f;
   float a_far = 100.0f;
   float yScale = 1.0f / tanHalfFovy;
   float xScale = 1.0f / (tanHalfFovy * (640.0f / 480));
   float temp[16] = {xScale,
                     0.0f,
                     0.0f,
                     0.0f,
                     0.0f,
                     yScale,
                     0.0f,
                     0.0f,
                     0.0f,
                     0.0f,
                     -(a_far + a_near) / (a_far - a_near),
                     -1.0f,
                     0.0f,
                     0.0f,
                     -(2.0f * a_far * a_near) / (a_far - a_near),
                     0.0f};
 }

namespace Math {
const float Pi = 3.1415926535f;
const float TwoPi = Pi * 2.0f;
const float PiOver2 = Pi / 2.0f;
const float Infinity = std::numeric_limits<float>::infinity();
const float NegInfinity = -std::numeric_limits<float>::infinity();



inline float ToRadians(float degrees) { return degrees * Pi / 180.0f; }

inline float ToDegrees(float radians) { return radians * 180.0f / Pi; }

inline bool NearZero(float val, float epsilon = 0.001f) {
  if (fabs(val) <= epsilon) {
    return true;
  } else {
    return false;
  }
}

template <typename T>
T Max(const T& a, const T& b) {
  return (a < b ? b : a);
}

template <typename T>
T Min(const T& a, const T& b) {
  return (a < b ? a : b);
}

template <typename T>
T Clamp(const T& value, const T& lower, const T& upper) {
  return Min(upper, Max(lower, value));
}

inline float Abs(float value) { return fabs(value); }

inline float Cos(float angle) { return cosf(angle); }

inline float Sin(float angle) { return sinf(angle); }

inline float Tan(float angle) { return tanf(angle); }

inline float Acos(float value) { return acosf(value); }

inline float Atan2(float y, float x) { return atan2f(y, x); }

inline float Cot(float angle) { return 1.0f / Tan(angle); }

inline float Lerp(float a, float b, float f) { return a + f * (b - a); }

inline float Sqrt(float value) { return sqrtf(value); }

inline float Fmod(float numer, float denom) { return fmod(numer, denom); }

}  // namespace Math

// 2D Vector
class Vector2 {
 public:
  float x;
  float y;

  constexpr Vector2() noexcept : x(0.0f), y(0.0f) {}

  explicit constexpr Vector2(float inX, float inY) noexcept : x(inX), y(inY) {}

  // Set both components in one line
  void Set(float inX, float inY) {
    x = inX;
    y = inY;
  }

  // Vector addition (a + b)
  friend Vector2 operator+(const Vector2& a, const Vector2& b) noexcept {
    return Vector2(a.x + b.x, a.y + b.y);
  }

  // Vector subtraction (a - b)
  friend Vector2 operator-(const Vector2& a, const Vector2& b) noexcept {
    return Vector2(a.x - b.x, a.y - b.y);
  }

  // Component-wise multiplication
  // (a.x * b.x, ...)
  friend Vector2 operator*(const Vector2& a, const Vector2& b) noexcept {
    return Vector2(a.x * b.x, a.y * b.y);
  }

  // Scalar multiplication
  friend Vector2 operator*(const Vector2& vec, float scalar) noexcept {
    return Vector2(vec.x * scalar, vec.y * scalar);
  }

  // Scalar multiplication
  friend Vector2 operator*(float scalar, const Vector2& vec) noexcept {
    return Vector2(vec.x * scalar, vec.y * scalar);
  }

  friend Vector2 operator/(const Vector2& a, const Vector2& b) {
    return Vector2(a.x / b.x, a.y / b.y);
  }

  // Scalar multiplication
  friend Vector2 operator/(const Vector2& vec, float scalar) {
    assert(scalar != 0.0f);
    return Vector2(vec.x / scalar, vec.y / scalar);
  }

  // Scalar multiplication
  friend Vector2 operator/(float scalar, const Vector2& vec) {
    assert(scalar != 0.0f);
    return Vector2(vec.x / scalar, vec.y / scalar);
  }

  // Scalar *=
  Vector2& operator*=(float scalar) noexcept {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  // Vector +=
  Vector2& operator+=(const Vector2& right) noexcept {
    x += right.x;
    y += right.y;
    return *this;
  }

  // Vector -=
  Vector2& operator-=(const Vector2& right) noexcept {
    x -= right.x;
    y -= right.y;
    return *this;
  }

  // Length squared of vector
  float LengthSq() const { return (x * x + y * y); }

  // Length of vector
  float Length() const { return (Math::Sqrt(LengthSq())); }

  // Normalize this vector
  void Normalize() {
    float length = Length();
    x /= length;
    y /= length;
  }

  // Normalize the provided vector
  static Vector2 Normalize(const Vector2& vec) {
    Vector2 temp = vec;
    temp.Normalize();
    return temp;
  }

  // Dot product between two vectors (a dot b)
  static float Dot(const Vector2& a, const Vector2& b) {
    return (a.x * b.x + a.y * b.y);
  }

  // Lerp from A to B by f
  static Vector2 Lerp(const Vector2& a, const Vector2& b, float f) {
    return Vector2(a + f * (b - a));
  }

  // Reflect V about (normalized) N
  static Vector2 Reflect(const Vector2& v, const Vector2& n) {
    return v - 2.0f * Vector2::Dot(v, n) * n;
  }

  // Transform vector by matrix
  static Vector2 Transform(const Vector2& vec, const class Matrix3& mat,
                           float w = 1.0f);

  static const Vector2 Zero;
  static const Vector2 UnitX;
  static const Vector2 UnitY;
  static const Vector2 NegUnitX;
  static const Vector2 NegUnitY;
};

// 3D Vector
class Vector3 {
 public:
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;

  Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

  explicit Vector3(float inX, float inY, float inZ) : x(inX), y(inY), z(inZ) {}

   Vector3(const __m128& V)
  {
  	VecStore(this, V);
  }

  // Cast to a const float pointer
  const float* GetAsFloatPtr() const {
    return reinterpret_cast<const float*>(&x);
  }

  // Set all three components in one line
  void Set(float inX, float inY, float inZ) {
    x = inX;
    y = inY;
    z = inZ;
  }

  // Vector addition (a + b)
  friend Vector3 operator+(const Vector3& a, const Vector3& b) {
    return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
  }

  // Vector subtraction (a - b)
  friend Vector3 operator-(const Vector3& a, const Vector3& b) {
    return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
  }

  // Component-wise multiplication
  friend Vector3 operator*(const Vector3& left, const Vector3& right) {
    return Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
  }

  // Scalar multiplication
  friend Vector3 operator*(const Vector3& vec, float scalar) {
    return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
  }

  // Scalar multiplication
  friend Vector3 operator*(float scalar, const Vector3& vec) {
    return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
  }

  friend Vector3 operator/(const Vector3& a, const Vector3& b) {
    return Vector3(a.x / b.x, a.y / b.y, a.z / b.z);
  }

  // Scalar multiplication
  friend Vector3 operator/(const Vector3& vec, float scalar) {
    assert(scalar != 0.0f);
    return Vector3(vec.x / scalar, vec.y / scalar, vec.z / scalar);
  }

  // Scalar multiplication
  friend Vector3 operator/(float scalar, const Vector3& vec) {
    assert(scalar != 0.0f);
    return Vector3(vec.x / scalar, vec.y / scalar, vec.z / scalar);
  }

  // Scalar *=
  Vector3& operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }

  // Vector +=
  Vector3& operator+=(const Vector3& right) {
    x += right.x;
    y += right.y;
    z += right.z;
    return *this;
  }

  // Vector -=
  Vector3& operator-=(const Vector3& right) {
    x -= right.x;
    y -= right.y;
    z -= right.z;
    return *this;
  }

  // Length squared of vector
  float LengthSq() const { return (x * x + y * y + z * z); }

  // Length of vector
  float Length() const { return (Math::Sqrt(LengthSq())); }

  // Normalize this vector
  void Normalize() {
    float length = Length();
    x /= length;
    y /= length;
    z /= length;
  }

  // Normalize the provided vector
  static Vector3 Normalize(const Vector3& vec) {
    Vector3 temp = vec;
    temp.Normalize();
    return temp;
  }

  // Dot product between two vectors (a dot b)
  static float Dot(const Vector3& a, const Vector3& b) {
    return (a.x * b.x + a.y * b.y + a.z * b.z);
  }

  // Cross product between two vectors (a cross b)
  static Vector3 Cross(const Vector3& a, const Vector3& b) {
    Vector3 temp;
    temp.x = a.y * b.z - a.z * b.y;
    temp.y = a.z * b.x - a.x * b.z;
    temp.z = a.x * b.y - a.y * b.x;
    return temp;
  }

  // Lerp from A to B by f
  static Vector3 Lerp(const Vector3& a, const Vector3& b, float f) {
    return Vector3(a + f * (b - a));
  }

  // Reflect V about (normalized) N
  static Vector3 Reflect(const Vector3& v, const Vector3& n) {
    return v - 2.0f * Vector3::Dot(v, n) * n;
  }

  static Vector3 Transform(const Vector3& vec, const class Matrix4& mat,
                           float w = 1.0f);
  // This will transform the vector and renormalize the w component
  static Vector3 TransformWithPerspDiv(const Vector3& vec,
                                       const class Matrix4& mat,
                                       float w = 1.0f);

  // Transform a Vector3 by a quaternion
  static Vector3 Transform(const Vector3& v, const class Quaternion& q);

  static const Vector3 Zero;
  static const Vector3 UnitX;
  static const Vector3 UnitY;
  static const Vector3 UnitZ;
  static const Vector3 NegUnitX;
  static const Vector3 NegUnitY;
  static const Vector3 NegUnitZ;
  static const Vector3 Infinity;
  static const Vector3 NegInfinity;
};

// 3x3 Matrix
class Matrix3 {
 public:
  float mat[3][3];

  Matrix3() { *this = Matrix3::Identity; }

  explicit Matrix3(float inMat[3][3]) { memcpy(mat, inMat, 9 * sizeof(float)); }

  // Cast to a const float pointer
  const float* GetAsFloatPtr() const {
    return reinterpret_cast<const float*>(&mat[0][0]);
  }

  // Matrix multiplication
  friend Matrix3 operator*(const Matrix3& left, const Matrix3& right) {
    Matrix3 retVal;
    // row 0
    retVal.mat[0][0] = left.mat[0][0] * right.mat[0][0] +
                       left.mat[0][1] * right.mat[1][0] +
                       left.mat[0][2] * right.mat[2][0];

    retVal.mat[0][1] = left.mat[0][0] * right.mat[0][1] +
                       left.mat[0][1] * right.mat[1][1] +
                       left.mat[0][2] * right.mat[2][1];

    retVal.mat[0][2] = left.mat[0][0] * right.mat[0][2] +
                       left.mat[0][1] * right.mat[1][2] +
                       left.mat[0][2] * right.mat[2][2];

    // row 1
    retVal.mat[1][0] = left.mat[1][0] * right.mat[0][0] +
                       left.mat[1][1] * right.mat[1][0] +
                       left.mat[1][2] * right.mat[2][0];

    retVal.mat[1][1] = left.mat[1][0] * right.mat[0][1] +
                       left.mat[1][1] * right.mat[1][1] +
                       left.mat[1][2] * right.mat[2][1];

    retVal.mat[1][2] = left.mat[1][0] * right.mat[0][2] +
                       left.mat[1][1] * right.mat[1][2] +
                       left.mat[1][2] * right.mat[2][2];

    // row 2
    retVal.mat[2][0] = left.mat[2][0] * right.mat[0][0] +
                       left.mat[2][1] * right.mat[1][0] +
                       left.mat[2][2] * right.mat[2][0];

    retVal.mat[2][1] = left.mat[2][0] * right.mat[0][1] +
                       left.mat[2][1] * right.mat[1][1] +
                       left.mat[2][2] * right.mat[2][1];

    retVal.mat[2][2] = left.mat[2][0] * right.mat[0][2] +
                       left.mat[2][1] * right.mat[1][2] +
                       left.mat[2][2] * right.mat[2][2];

    return retVal;
  }

  Matrix3& operator*=(const Matrix3& right) {
    *this = *this * right;
    return *this;
  }

  // Create a scale matrix with x and y scales
  static Matrix3 CreateScale(float xScale, float yScale) {
    float temp[3][3] = {
        {xScale, 0.0f, 0.0f},
        {0.0f, yScale, 0.0f},
        {0.0f, 0.0f, 1.0f},
    };
    return Matrix3(temp);
  }

  static Matrix3 CreateScale(const Vector2& scaleVector) {
    return CreateScale(scaleVector.x, scaleVector.y);
  }

  // Create a scale matrix with a uniform factor
  static Matrix3 CreateScale(float scale) { return CreateScale(scale, scale); }

  // Create a rotation matrix about the Z axis
  // theta is in radians
  static Matrix3 CreateRotation(float theta) {
    float temp[3][3] = {
        {Math::Cos(theta), Math::Sin(theta), 0.0f},
        {-Math::Sin(theta), Math::Cos(theta), 0.0f},
        {0.0f, 0.0f, 1.0f},
    };
    return Matrix3(temp);
  }

  // Create a translation matrix (on the xy-plane)
  static Matrix3 CreateTranslation(const Vector2& trans) {
    float temp[3][3] = {
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {trans.x, trans.y, 1.0f},
    };
    return Matrix3(temp);
  }

  static const Matrix3 Identity;
};

// 4x4 Matrix
class Matrix4 {
 public:
  float mat[4][4];

  Matrix4() { *this = Matrix4::Identity; }

  explicit Matrix4(float inMat[4][4]) {
    memcpy(mat, inMat, 16 * sizeof(float));
  }

  // Cast to a const float pointer
  const float* GetAsFloatPtr() const {
    return reinterpret_cast<const float*>(&mat[0][0]);
  }

  // Matrix multiplication (a * b)
  friend Matrix4 operator*(const Matrix4& a, const Matrix4& b) {
    Matrix4 retVal;
    // row 0
    retVal.mat[0][0] = a.mat[0][0] * b.mat[0][0] + a.mat[0][1] * b.mat[1][0] +
                       a.mat[0][2] * b.mat[2][0] + a.mat[0][3] * b.mat[3][0];

    retVal.mat[0][1] = a.mat[0][0] * b.mat[0][1] + a.mat[0][1] * b.mat[1][1] +
                       a.mat[0][2] * b.mat[2][1] + a.mat[0][3] * b.mat[3][1];

    retVal.mat[0][2] = a.mat[0][0] * b.mat[0][2] + a.mat[0][1] * b.mat[1][2] +
                       a.mat[0][2] * b.mat[2][2] + a.mat[0][3] * b.mat[3][2];

    retVal.mat[0][3] = a.mat[0][0] * b.mat[0][3] + a.mat[0][1] * b.mat[1][3] +
                       a.mat[0][2] * b.mat[2][3] + a.mat[0][3] * b.mat[3][3];

    // row 1
    retVal.mat[1][0] = a.mat[1][0] * b.mat[0][0] + a.mat[1][1] * b.mat[1][0] +
                       a.mat[1][2] * b.mat[2][0] + a.mat[1][3] * b.mat[3][0];

    retVal.mat[1][1] = a.mat[1][0] * b.mat[0][1] + a.mat[1][1] * b.mat[1][1] +
                       a.mat[1][2] * b.mat[2][1] + a.mat[1][3] * b.mat[3][1];

    retVal.mat[1][2] = a.mat[1][0] * b.mat[0][2] + a.mat[1][1] * b.mat[1][2] +
                       a.mat[1][2] * b.mat[2][2] + a.mat[1][3] * b.mat[3][2];

    retVal.mat[1][3] = a.mat[1][0] * b.mat[0][3] + a.mat[1][1] * b.mat[1][3] +
                       a.mat[1][2] * b.mat[2][3] + a.mat[1][3] * b.mat[3][3];

    // row 2
    retVal.mat[2][0] = a.mat[2][0] * b.mat[0][0] + a.mat[2][1] * b.mat[1][0] +
                       a.mat[2][2] * b.mat[2][0] + a.mat[2][3] * b.mat[3][0];

    retVal.mat[2][1] = a.mat[2][0] * b.mat[0][1] + a.mat[2][1] * b.mat[1][1] +
                       a.mat[2][2] * b.mat[2][1] + a.mat[2][3] * b.mat[3][1];

    retVal.mat[2][2] = a.mat[2][0] * b.mat[0][2] + a.mat[2][1] * b.mat[1][2] +
                       a.mat[2][2] * b.mat[2][2] + a.mat[2][3] * b.mat[3][2];

    retVal.mat[2][3] = a.mat[2][0] * b.mat[0][3] + a.mat[2][1] * b.mat[1][3] +
                       a.mat[2][2] * b.mat[2][3] + a.mat[2][3] * b.mat[3][3];

    // row 3
    retVal.mat[3][0] = a.mat[3][0] * b.mat[0][0] + a.mat[3][1] * b.mat[1][0] +
                       a.mat[3][2] * b.mat[2][0] + a.mat[3][3] * b.mat[3][0];

    retVal.mat[3][1] = a.mat[3][0] * b.mat[0][1] + a.mat[3][1] * b.mat[1][1] +
                       a.mat[3][2] * b.mat[2][1] + a.mat[3][3] * b.mat[3][1];

    retVal.mat[3][2] = a.mat[3][0] * b.mat[0][2] + a.mat[3][1] * b.mat[1][2] +
                       a.mat[3][2] * b.mat[2][2] + a.mat[3][3] * b.mat[3][2];

    retVal.mat[3][3] = a.mat[3][0] * b.mat[0][3] + a.mat[3][1] * b.mat[1][3] +
                       a.mat[3][2] * b.mat[2][3] + a.mat[3][3] * b.mat[3][3];

    return retVal;
  }

  Matrix4& operator*=(const Matrix4& right) {
    *this = *this * right;
    return *this;
  }

  // Invert the matrix - super slow
  void Invert();

  // Get the translation component of the matrix
  Vector3 GetTranslation() const {
    return Vector3(mat[3][0], mat[3][1], mat[3][2]);
  }

  // Get the X axis of the matrix (forward)
  Vector3 GetXAxis() const {
    return Vector3::Normalize(Vector3(mat[0][0], mat[0][1], mat[0][2]));
  }

  // Get the Y axis of the matrix (left)
  Vector3 GetYAxis() const {
    return Vector3::Normalize(Vector3(mat[1][0], mat[1][1], mat[1][2]));
  }

  // Get the Z axis of the matrix (up)
  Vector3 GetZAxis() const {
    return Vector3::Normalize(Vector3(mat[2][0], mat[2][1], mat[2][2]));
  }

  // Extract the scale component from the matrix
  Vector3 GetScale() const {
    Vector3 retVal;
    retVal.x = Vector3(mat[0][0], mat[0][1], mat[0][2]).Length();
    retVal.y = Vector3(mat[1][0], mat[1][1], mat[1][2]).Length();
    retVal.z = Vector3(mat[2][0], mat[2][1], mat[2][2]).Length();
    return retVal;
  }

  // Create a scale matrix with x, y, and z scales
  static Matrix4 CreateScale(float xScale, float yScale, float zScale) {
    float temp[4][4] = {{xScale, 0.0f, 0.0f, 0.0f},
                        {0.0f, yScale, 0.0f, 0.0f},
                        {0.0f, 0.0f, zScale, 0.0f},
                        {0.0f, 0.0f, 0.0f, 1.0f}};
    return Matrix4(temp);
  }

  static Matrix4 CreateScale(const Vector3& scaleVector) {
    return CreateScale(scaleVector.x, scaleVector.y, scaleVector.z);
  }

  // Create a scale matrix with a uniform factor
  static Matrix4 CreateScale(float scale) {
    return CreateScale(scale, scale, scale);
  }

  // Rotation about x-axis
  static Matrix4 CreateRotationX(float theta) {
    float temp[4][4] = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, Math::Cos(theta), Math::Sin(theta), 0.0f},
        {0.0f, -Math::Sin(theta), Math::Cos(theta), 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    };
    return Matrix4(temp);
  }

  // Rotation about y-axis
  static Matrix4 CreateRotationY(float theta) {
    float temp[4][4] = {
        {Math::Cos(theta), 0.0f, -Math::Sin(theta), 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {Math::Sin(theta), 0.0f, Math::Cos(theta), 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    };
    return Matrix4(temp);
  }

  // Rotation about z-axis
  static Matrix4 CreateRotationZ(float theta) {
    float temp[4][4] = {
        {Math::Cos(theta), Math::Sin(theta), 0.0f, 0.0f},
        {-Math::Sin(theta), Math::Cos(theta), 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    };
    return Matrix4(temp);
  }

  // Create a rotation matrix from a quaternion
  static Matrix4 CreateFromQuaternion(const class Quaternion& q);

  static Matrix4 CreateTranslation(const Vector3& trans) {
    float temp[4][4] = {{1.0f, 0.0f, 0.0f, 0.0f},
                        {0.0f, 1.0f, 0.0f, 0.0f},
                        {0.0f, 0.0f, 1.0f, 0.0f},
                        {trans.x, trans.y, trans.z, 1.0f}};
    return Matrix4(temp);
  }

  static Matrix4 CreateLookAt(const Vector3& eye, const Vector3& target,
                              const Vector3& up) {
    Vector3 zaxis = Vector3::Normalize(target - eye);
    Vector3 xaxis = Vector3::Normalize(Vector3::Cross(up, zaxis));
    Vector3 yaxis = Vector3::Normalize(Vector3::Cross(zaxis, xaxis));
    Vector3 trans;
    trans.x = -Vector3::Dot(xaxis, eye);
    trans.y = -Vector3::Dot(yaxis, eye);
    trans.z = -Vector3::Dot(zaxis, eye);

    float temp[4][4] = {{xaxis.x, yaxis.x, zaxis.x, 0.0f},
                        {xaxis.y, yaxis.y, zaxis.y, 0.0f},
                        {xaxis.z, yaxis.z, zaxis.z, 0.0f},
                        {trans.x, trans.y, trans.z, 1.0f}};
    return Matrix4(temp);
  }

  static Matrix4 CreateOrtho(float a_width, float a_height, float a_near, float a_far) {
    float temp[4][4] = {{2.0f / a_width, 0.0f, 0.0f, 0.0f},
                        {0.0f, (2.0f / a_height), 0.0f, 0.0f},
                        {0.0f, 0.0f, 1.0f / (a_far - a_near), 0.0f},
                        {0.0f, 0.0f, a_near / (a_near - a_far), 1.0f},};
    return Matrix4(temp);
  }

  static Matrix4 CreatePerspectiveFOV(float a_fovY, float a_width, float a_height,
                                      float a_near, float a_far) {
    float yScale = Math::Cot(a_fovY / 2.0f);
    float xScale = yScale * a_height / a_width;
    float temp[4][4] = {{xScale, 0.0f, 0.0f, 0.0f},
                        {0.0f, yScale, 0.0f, 0.0f},
                        {0.0f, 0.0f, (a_far / (a_far - a_near)), 1.0f},
                        {0.0f, 0.0f, (-a_near * (a_far / (a_far - a_near))), 0.0f}};
    return Matrix4(temp);
  }

  // Create "Simple" View-Projection Matrix from Chapter 6
  static Matrix4 CreateSimpleViewProj(float width, float height) {
    float temp[4][4] = {{2.0f / width, 0.0f, 0.0f, 0.0f},
                        {0.0f, 2.0f / height, 0.0f, 0.0f},
                        {0.0f, 0.0f, 1.0f, 0.0f},
                        {0.0f, 0.0f, 1.0f, 1.0f}};
    return Matrix4(temp);
  }

  //static Matrix4 CreateFromTRS(const Vector3& translation,
  //                               const Quaternion& rotation,
  //                               const Vector3& scale) {
  //  return Matrix4::CreateTranslation(translation) *
  //          Matrix4::CreateFromQuaternion(rotation) * 
  //          Matrix4::CreateScale(scale);
  //}

	static Matrix4 CreateFromSRT(const Vector3& scale,
                                    const Quaternion& rotation,
                                    const Vector3& translation) {
    return
                  Matrix4::CreateFromQuaternion(rotation) *
                  Matrix4::CreateTranslation(translation);
  }

  static const Matrix4 Identity;
};

// (Unit) Quaternion
class Quaternion {
 public:
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;
  float w = 0.0f;

  Quaternion() { *this = Quaternion::Identity; }

  // XMVECTORから代入してきた時
   Quaternion(const __m128& V)
  {
  	QuatStore(this, V);
  }

  // This directly sets the quaternion components --
  // don't use for axis/angle
  explicit Quaternion(float inX, float inY, float inZ, float inW) {
    Set(inX, inY, inZ, inW);
  }

  // Construct the quaternion from an axis and angle
  // It is assumed that axis is already normalized,
  // and the angle is in radians
  explicit Quaternion(const Vector3& axis, float angle) {
    float scalar = Math::Sin(angle / 2.0f);
    x = axis.x * scalar;
    y = axis.y * scalar;
    z = axis.z * scalar;
    w = Math::Cos(angle / 2.0f);
  }

  // Directly set the internal components
  void Set(float inX, float inY, float inZ, float inW) {
    x = inX;
    y = inY;
    z = inZ;
    w = inW;
  }

  void Conjugate() {
    x *= -1.0f;
    y *= -1.0f;
    z *= -1.0f;
  }

  float LengthSq() const { return (x * x + y * y + z * z + w * w); }

  float Length() const { return Math::Sqrt(LengthSq()); }

  void Normalize() {
    float length = Length();
    x /= length;
    y /= length;
    z /= length;
    w /= length;
  }

  // Normalize the provided quaternion
  static Quaternion Normalize(const Quaternion& q) {
    Quaternion retVal = q;
    retVal.Normalize();
    return retVal;
  }

  // Linear interpolation
  static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float f) {
    Quaternion retVal;
    retVal.x = Math::Lerp(a.x, b.x, f);
    retVal.y = Math::Lerp(a.y, b.y, f);
    retVal.z = Math::Lerp(a.z, b.z, f);
    retVal.w = Math::Lerp(a.w, b.w, f);
    retVal.Normalize();
    return retVal;
  }

  static float Dot(const Quaternion& a, const Quaternion& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
  }

  // Spherical Linear Interpolation
  static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float f) {
    float rawCosm = Quaternion::Dot(a, b);

    float cosom = -rawCosm;
    if (rawCosm >= 0.0f) {
      cosom = rawCosm;
    }

    float scale0, scale1;

    if (cosom < 0.9999f) {
      const float omega = Math::Acos(cosom);
      const float invSin = 1.f / Math::Sin(omega);
      scale0 = Math::Sin((1.f - f) * omega) * invSin;
      scale1 = Math::Sin(f * omega) * invSin;
    } else {
      // Use linear interpolation if the quaternions
      // are collinear
      scale0 = 1.0f - f;
      scale1 = f;
    }

    if (rawCosm < 0.0f) {
      scale1 = -scale1;
    }

    Quaternion retVal;
    retVal.x = scale0 * a.x + scale1 * b.x;
    retVal.y = scale0 * a.y + scale1 * b.y;
    retVal.z = scale0 * a.z + scale1 * b.z;
    retVal.w = scale0 * a.w + scale1 * b.w;
    retVal.Normalize();
    return retVal;
  }

  // Concatenate
  // Rotate by q FOLLOWED BY p
  static Quaternion Concatenate(const Quaternion& q, const Quaternion& p) {
    Quaternion retVal;

    // Vector component is:
    // ps * qv + qs * pv + pv x qv
    Vector3 qv(q.x, q.y, q.z);
    Vector3 pv(p.x, p.y, p.z);
    Vector3 newVec = p.w * qv + q.w * pv + Vector3::Cross(pv, qv);
    retVal.x = newVec.x;
    retVal.y = newVec.y;
    retVal.z = newVec.z;

    // Scalar component is:
    // ps * qs - pv . qv
    retVal.w = p.w * q.w - Vector3::Dot(pv, qv);

    return retVal;
  }

  //回転行列をクォータニオンにする
  static const Quaternion MatrixToQuaternion(const Matrix4& a_mat) {
    Quaternion q;
    
    auto& mat = a_mat.mat;

    float s;
    float tr = mat[0][0] + mat[1][1] + mat[2][2] + 1.0f;
    if (tr >= 1.0f) {
      s = 0.5f / sqrt(tr);
      q.w = 0.25f / s;
      q.x = (mat[1][2] - mat[2][1]) * s;
      q.y = (mat[2][0] - mat[0][2]) * s;
      q.z = (mat[0][1] - mat[1][0]) * s;
      return q;
    } else {
      float max;
      if (mat[1][1] > mat[2][2]) {
        max = mat[1][1];
      } else {
        max = mat[2][2];
      }

      if (max < mat[0][0]) {
        s = sqrt(mat[0][0] - (mat[1][1] + mat[2][2]) + 1.0f);
        float x = s * 0.5f;
        s = 0.5f / s;
        q.x = x;
        q.y = (mat[0][1] + mat[1][0]) * s;
        q.z = (mat[2][0] + mat[0][2]) * s;
        q.w = (mat[1][2] - mat[2][1]) * s;
        return q;
      } else if (max == mat[1][1]) {
        s = sqrt(mat[1][1] - (mat[2][2] + mat[0][0]) + 1.0f);
        float y = s * 0.5f;
        s = 0.5f / s;
        q.x = (mat[0][1] + mat[1][0]) * s;
        q.y = y;
        q.z = (mat[1][2] + mat[2][1]) * s;
        q.w = (mat[2][0] - mat[0][2]) * s;
        return q;
      } else {
        s = sqrt(mat[2][2] - (mat[0][0] + mat[1][1]) + 1.0f);
        float z = s * 0.5f;
        s = 0.5f / s;
        q.x = (mat[2][0] + mat[0][2]) * s;
        q.y = (mat[1][2] + mat[2][1]) * s;
        q.z = z;
        q.w = (mat[0][1] - mat[1][0]) * s;
        return q;
      }
    }
  }

  static const Quaternion Identity;
};

namespace Color {
static const Vector3 Black(0.0f, 0.0f, 0.0f);
static const Vector3 White(1.0f, 1.0f, 1.0f);
static const Vector3 Red(1.0f, 0.0f, 0.0f);
static const Vector3 Green(0.0f, 1.0f, 0.0f);
static const Vector3 Blue(0.0f, 0.0f, 1.0f);
static const Vector3 Yellow(1.0f, 1.0f, 0.0f);
static const Vector3 LightYellow(1.0f, 1.0f, 0.88f);
static const Vector3 LightBlue(0.68f, 0.85f, 0.9f);
static const Vector3 LightPink(1.0f, 0.71f, 0.76f);
static const Vector3 LightGreen(0.56f, 0.93f, 0.56f);
}  // namespace Color

//#define CALL_SIMD
/**
* @brief 正の無限表現を返す
*/
 template<class T>
 constexpr T Infinity = std::numeric_limits<T>::infinity();

/**
* @brief シグナルを発生させないNaNを返す
*/
 template<class T>
 constexpr T QuietNaN = std::numeric_limits<T>::quiet_NaN();

 inline __m128 __vectorcall VectorSelect(__m128 v1, __m128 v2, __m128 control) {
   __m128 vTemp1 = _mm_andnot_ps(control, v1);
   __m128 vTemp2 = _mm_and_ps(v2, control);
   return _mm_or_ps(vTemp1, vTemp2);
 }

 inline __m128 __vectorcall Vec4Mul(__m128 left, __m128 right) {
   return _mm_mul_ps(left, right);
 }

 inline void __vectorcall QuatStore(Quaternion* out, const __m128& in) {
   _mm_storeu_ps(&out->x, in);
 }

 inline __m128 __vectorcall QuatConjugate(__m128 v) {
   static const __m128 NegativeOne3 = {-1.0f, -1.0f, -1.0f, 1.0f};
   return _mm_mul_ps(v, NegativeOne3);
 }

 inline __m128 __vectorcall QuatSet(const Quaternion& v) {
   return _mm_set_ps(v.w, v.z, v.y, v.x);
 }

 inline __m128 __vectorcall QuatSet(float x, float y, float z, float w) {
   return _mm_set_ps(w, z, y, x);
 }

 inline __m128 __vectorcall QuatSet(float value) { return _mm_set_ps1(value); }

 inline __m128 __vectorcall QuatAdd(__m128 left, __m128 right) {
   return _mm_add_ps(left, right);
 }

 inline __m128 __vectorcall QuatVecMul(__m128 left, __m128 right) {
   return _mm_mul_ps(left, right);
 }

 inline __m128 __vectorcall QuatMul(__m128 left, __m128 right) {
   // [ (Q2.w * Q1.x) + (Q2.x * Q1.w) + (Q2.y * Q1.z) - (Q2.z * Q1.y),
   //   (Q2.w * Q1.y) - (Q2.x * Q1.z) + (Q2.y * Q1.w) + (Q2.z * Q1.x),
   //   (Q2.w * Q1.z) + (Q2.x * Q1.y) - (Q2.y * Q1.x) + (Q2.z * Q1.w),
   //   (Q2.w * Q1.w) - (Q2.x * Q1.x) - (Q2.y * Q1.y) - (Q2.z * Q1.z) ]

   static const __m128 ControlWZYX = {1.0f, -1.0f, 1.0f, -1.0f};
   static const __m128 ControlZWXY = {1.0f, 1.0f, -1.0f, -1.0f};
   static const __m128 ControlYXWZ = {-1.0f, 1.0f, 1.0f, -1.0f};

   __m128 otherX = right;
   __m128 otherY = right;
   __m128 otherZ = right;
   __m128 result = right;
   // vResult = { vResult.mW, vResult.mW, vResult.mW, vResult.mW }.
   result = _mm_shuffle_ps(result, result, _MM_SHUFFLE(3, 3, 3, 3));
   // otherX = { otherX.x, otherX.x, otherX.x, otherX.x }.
   otherX = _mm_shuffle_ps(otherX, otherX, _MM_SHUFFLE(0, 0, 0, 0));
   // otherY = { otherY.y, otherY.y, otherY.y, otherY.y }.
   otherY = _mm_shuffle_ps(otherY, otherY, _MM_SHUFFLE(1, 1, 1, 1));
   // otherZ = { otherZ.z, otherZ.z, otherZ.z, otherZ.z }.
   otherZ = _mm_shuffle_ps(otherZ, otherZ, _MM_SHUFFLE(2, 2, 2, 2));

   // result * left.
   result = Vec4Mul(result, QuatSet(left));

   __m128 ownShuffle = QuatSet(left);

   // ownShuffle = { ownShuffle.mW, ownShuffle.z, ownShuffle.y, ownShuffle.x
   ownShuffle = _mm_shuffle_ps(ownShuffle, ownShuffle, _MM_SHUFFLE(0, 1, 2, 3));

   // otherX * ownShuffle(thisWZYX)
   otherX = QuatVecMul(otherX, ownShuffle);
   // ownShuffle = { ownShuffle.z, ownShuffle.mW, ownShuffle.x, ownShuffle.y
   ownShuffle = _mm_shuffle_ps(ownShuffle, ownShuffle, _MM_SHUFFLE(2, 3, 0, 1));

   // YとZの符号を反転
   otherX = QuatVecMul(otherX, ControlWZYX);

   // otherY * ownShuffle(thisZWXY)
   otherY = QuatVecMul(otherY, ownShuffle);
   // ownShuffle = { ownShuffle.y, ownShuffle.x, ownShuffle.mW, ownShuffle.z
   ownShuffle = _mm_shuffle_ps(ownShuffle, ownShuffle, _MM_SHUFFLE(0, 1, 2, 3));

   // ZとWの符号を反転
   otherY = QuatVecMul(otherY, ControlZWXY);

   // otherZ * ownShuffle(thisYXWZ)
   otherZ = QuatVecMul(otherZ, ownShuffle);
   result = QuatAdd(result, otherX);

   // XとWの符号を反転
   otherZ = QuatVecMul(otherZ, ControlYXWZ);
   // otherY + otherZ
   otherY = QuatAdd(otherY, otherZ);
   // result + otherY
   result = QuatAdd(result, otherY);

   return result;
 }

 inline __m128 __vectorcall Vec3Set(const Vector3& v) {
   return _mm_set_ps(1.0f, v.z, v.y, v.x);
 }
 inline __m128 __vectorcall Vec3Set(float x, float y, float z) {
   return _mm_set_ps(1.0f, z, y, x);
 }
 inline __m128 __vectorcall Vec3Set(float value) {
   return _mm_set_ps(1.0f, value, value, value);
 }
 inline void __vectorcall VecStore(Vector3* out, const __m128& in) {
   //( xmVec.y, xmVec.y, xmVec.y, xmVec.y).
   __m128 y = _mm_shuffle_ps(in, in, _MM_SHUFFLE(1, 1, 1, 1));
   __m128 z = _mm_shuffle_ps(in, in, _MM_SHUFFLE(2, 2, 2, 2));
   // xmVec.xをxへ代入.
   _mm_store_ss(&out->x, in);
   // xmVec.xをyへ代入.
   _mm_store_ss(&out->y, y);
   _mm_store_ss(&out->z, z);
 }
 inline __m128 __vectorcall MLoadVec3(const Vector3* in) {
   __m128 x = _mm_load_ss(&in->x);
   __m128 y = _mm_load_ss(&in->y);
   __m128 z = _mm_load_ss(&in->z);
   __m128 xy = _mm_unpacklo_ps(x, y);
   return _mm_movelh_ps(xy, z);
 }
 inline __m128 __vectorcall Vec3Add(__m128 left, __m128 right) {
   return _mm_add_ps(left, right);
 }
 inline __m128 __vectorcall Vec3Sub(__m128 left, __m128 right) {
   return _mm_sub_ps(left, right);
 }
 inline __m128 __vectorcall Vec3Mul(__m128 left, __m128 right) {
   return _mm_mul_ps(left, right);
 }
 inline __m128 __vectorcall Vec3Div(__m128 left, __m128 right) {
   return _mm_div_ps(left, right);
 }
 inline __m128 __vectorcall Vec3Length(__m128 v) {
   // xとyとzのドット積を求める.
   //*this * *this.
   __m128 lengthSq = _mm_mul_ps(v, v);

   // lengthSqからyとzの値を取得.
   // lengthSq{x,y,z,w} -> temp{z,y,z,y}.
   __m128 temp = _mm_shuffle_ps(lengthSq, lengthSq, _MM_SHUFFLE(1, 2, 1, 2));
   // lengthSqのx(下位32ビット) +
   // tempのz(下位32ビット),残り上位32ビットはlengthSqの値をコピー. x + z.
   lengthSq = _mm_add_ss(lengthSq, temp);
   // tempからyの値を取得.
   // temp{y,y,y,y}.
   temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(1, 1, 1, 1));
   // lengthSqのx(下位32ビット) +
   // tempのy(下位32ビット),残り上位32ビットはlengthSqの値をコピー. x + z + y.
   lengthSq = _mm_add_ss(lengthSq, temp);
   // LengthSqのx(下位32ビット)を取得.
   lengthSq = _mm_shuffle_ps(lengthSq, lengthSq, _MM_SHUFFLE(0, 0, 0, 0));
   // lengthSqの平方根を計算.
   lengthSq = _mm_sqrt_ps(lengthSq);

   return lengthSq;
 }
 inline __m128 __vectorcall Vec3Dot(__m128 v) {
   //*this * vec.
   __m128 dot = _mm_mul_ps(v, v);
   // x = dot.vector4_f32[1], y = dot.vector4_f32[2]
   __m128 temp = _mm_shuffle_ps(dot, dot, _MM_SHUFFLE(2, 1, 2, 1));
   // dot.m128_f32[0] = x + y.
   dot = _mm_add_ss(dot, temp);
   // x = dot.m128_f32[2].
   temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(1, 1, 1, 1));
   // dot.ma_mat[0][1]8_f32[0] = (x + y) + z.
   dot = _mm_add_ss(dot, temp);
   // dotの全要素にxを代入し、先頭のdot.m128_f32[0]を返す.
   return _mm_shuffle_ps(dot, dot, _MM_SHUFFLE(0, 0, 0, 0));
 }
 inline __m128 __vectorcall Vec3Dot(__m128 left, __m128 right) {
   //*this * vec.
   __m128 dot = _mm_mul_ps(left, right);
   // x = dot.vector4_f32[1], y = dot.vector4_f32[2]
   __m128 temp = _mm_shuffle_ps(dot, dot, _MM_SHUFFLE(2, 1, 2, 1));
   // dot.m128_f32[0] = x + y.
   dot = _mm_add_ss(dot, temp);
   // x = dot.m128_f32[2].
   temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(1, 1, 1, 1));
   // dot.ma_mat[0][1]8_f32[0] = (x + y) + z.
   dot = _mm_add_ss(dot, temp);
   // dotの全要素にxを代入し、先頭のdot.m128_f32[0]を返す.
   return _mm_shuffle_ps(dot, dot, _MM_SHUFFLE(0, 0, 0, 0));
 }
 inline __m128 __vectorcall Vec3Cross(__m128 left, __m128 right) {
   // [ V1.y*V2.z - V1.z*V2.y,
   //   V1.z*V2.x - V1.x*V2.z,
   //   V1.x*V2.y - V1.y*V2.x ]
   //最後にwに1を代入

   return _mm_and_ps(
       _mm_sub_ps(
           _mm_mul_ps(_mm_shuffle_ps(left, left, _MM_SHUFFLE(3, 0, 2, 1)),
                      _mm_shuffle_ps(right, right, _MM_SHUFFLE(3, 1, 0, 2))),
           _mm_mul_ps(_mm_shuffle_ps(left, left, _MM_SHUFFLE(3, 1, 0, 2)),
                      _mm_shuffle_ps(right, right, _MM_SHUFFLE(3, 0, 2, 1)))),
       {static_cast<float>(0xFFFFFFFF), static_cast<float>(0xFFFFFFFF),
        static_cast<float>(0xFFFFFFFF), static_cast<float>(0x00000000)});
 }
 inline __m128 __vectorcall Vec3Normalize(__m128 v) {
   // x,y,zそれぞれのドット積を求める.
   __m128 lengthSq = _mm_mul_ps(v, v);
   // x = lengthSq.vector4_f32[1], y = lengthSq.vector4_f32[2]
   __m128 temp = _mm_shuffle_ps(lengthSq, lengthSq, _MM_SHUFFLE(2, 1, 2, 1));
   // lengthSq.x + temp.x
   lengthSq = _mm_add_ss(lengthSq, temp);
   // temp = { temp.y, temp.y, temp.y, temp.y }
   temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(1, 1, 1, 1));

   // lengthSq.x + temp.x
   lengthSq = _mm_add_ss(lengthSq, temp);
   // lengthSq = {lengthSq.x, lengthSq.x, lengthSq.x, lengthSq.x }
   lengthSq = _mm_shuffle_ps(lengthSq, lengthSq, _MM_SHUFFLE(0, 0, 0, 0));
   //平方根.
   __m128 result = _mm_sqrt_ps(lengthSq);
   // 4つの単精度浮動小数点値を0クリアしたものをzeroMaskに代入.
   __m128 zeroMask = _mm_setzero_ps();

   zeroMask = _mm_cmpneq_ps(zeroMask, result);

   lengthSq = _mm_cmpneq_ps(lengthSq, {Infinity<float>, Infinity<float>,
                                       Infinity<float>, Infinity<float>});

   result = _mm_div_ps(v, result);

   result = _mm_and_ps(result, zeroMask);

   __m128 temp1 = _mm_andnot_ps(lengthSq, {QuietNaN<float>, QuietNaN<float>,
                                           QuietNaN<float>, QuietNaN<float>});
   __m128 temp2 = _mm_and_ps(result, lengthSq);

   result = _mm_or_ps(temp1, temp2);
   return result;
 }

 inline __m128 __vectorcall Vec3Rotate(__m128 v, __m128 quat) {
   __m128 A = VectorSelect(
       {static_cast<float>(0xFFFFFFFF), static_cast<float>(0xFFFFFFFF),
        static_cast<float>(0xFFFFFFFF), static_cast<float>(0x00000000)},
       v,
       {static_cast<float>(0xFFFFFFFF), static_cast<float>(0xFFFFFFFF),
        static_cast<float>(0xFFFFFFFF), static_cast<float>(0x00000000)});
   __m128 Q = QuatConjugate(quat);
   __m128 Result = QuatMul(Q, A);
   return QuatMul(Result, quat);
 }