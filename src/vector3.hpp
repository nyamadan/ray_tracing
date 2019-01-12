#pragma once
#include <math.h>
#include <stdlib.h>
#include <iostream>

class Vector3 {
   public:
    static inline Vector3 normalize(const Vector3& v);
    static inline float dot(const Vector3& v1, const Vector3& v2);
    static inline Vector3 cross(Vector3& v1, const Vector3& v2);

    Vector3() {}
    Vector3(float e0, float e1, float e2) {
        e[0] = e0;
        e[1] = e1;
        e[2] = e2;
    }

    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }
    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }

    inline const Vector3& operator+() const { return *this; }
    inline const Vector3 operator-() const {
        return Vector3(-e[0], -e[1], -e[2]);
    }
    inline float operator[](int i) const { return e[i]; }
    inline float& operator[](int i) { return e[i]; }

    inline Vector3& operator+=(const Vector3& v2);
    inline Vector3& operator-=(const Vector3& v2);
    inline Vector3& operator*=(const Vector3& v2);
    inline Vector3& operator/=(const Vector3& v2);
    inline Vector3& operator*=(const float t);
    inline Vector3& operator/=(const float t);

    inline float length() const {
        return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    }

    inline float lengthSq() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    inline void normalize();

    float e[3];
};

inline std::istream& operator>>(std::istream& is, Vector3& t) {
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const Vector3& t) {
    os << t.e[0] << " " << t.e[1] << " " << t.e[2];
    return os;
}

inline void Vector3::normalize() {
    float k = 1.0f / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
}

inline Vector3 operator+(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline Vector3 operator-(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline Vector3 operator*(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline Vector3 operator/(const Vector3& v1, const Vector3& v2) {
    return Vector3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline Vector3 operator*(const Vector3& v, float t) {
    return Vector3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vector3 operator*(float t, const Vector3& v) {
    return Vector3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vector3 operator/(const Vector3& v, float t) {
    return Vector3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline Vector3 operator/(float t, const Vector3& v) {
    return Vector3(t / v.e[0], t / v.e[1], t / v.e[2]);
}

inline float Vector3::dot(const Vector3& v1, const Vector3& v2) {
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline Vector3 Vector3::cross(Vector3& v1, const Vector3& v2) {
    return Vector3(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
                   v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
                   v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}

inline Vector3& Vector3::operator+=(const Vector3& v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];

    return *this;
}

inline Vector3& Vector3::operator-=(const Vector3& v) {
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];

    return *this;
}

inline Vector3& Vector3::operator*=(const Vector3& v) {
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];

    return *this;
}

inline Vector3& Vector3::operator/=(const Vector3& v) {
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];

    return *this;
}

inline Vector3& Vector3::operator*=(const float t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;

    return *this;
}

inline Vector3& Vector3::operator/=(const float t) {
    float k = 1.0f / t;
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;

    return *this;
}

inline Vector3 Vector3::normalize(const Vector3& v) { return v / v.length(); }
