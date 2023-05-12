#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Vector2.h"
#include <cmath>



// ベクトルの加法
Vector2 Add(Vector2& v1, Vector2& v2);
Vector3 Add(Vector3& v1, Vector3& v2);
// ベクトルの減法
Vector3 Subtract(Vector3& v1, Vector3 v2);
// スカラー倍
Vector2 Multiply(const float& k, Vector2 v1);
Vector3 Multiply(const float& k, Vector3 v1);
// 内積
float Dot(Vector3& v1, Vector3& v2);
// 長さ(ノルム)
float Length(Vector3& v1);
// 正規化
Vector3 Normalize(Vector3& v1);

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// 座標変換
// Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

// アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);
