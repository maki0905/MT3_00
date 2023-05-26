#pragma once
#pragma once
#include <cmath>
#include "Vector3.h"
#include "Matrix4x4.h"

// �x�N�g���̉��@
Vector3 Add(Vector3& v1, Vector3& v2);
// �x�N�g���̌��@
Vector3 Subtract(Vector3& v1, Vector3 v2);
// �X�J���[�{
Vector3 Multiply(const float& k, Vector3 v1);
// ����
float Dot(Vector3& v1, Vector3& v2);
// ����(�m����)
float Length(Vector3& v1);
// ���K��
Vector3 Normalize(Vector3& v1);

// �x�N�g���ϊ�
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);


// �s��̉��@
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
// �s��̌��@
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
// �s��̐�
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// �t�s��
Matrix4x4 Inverse(const Matrix4x4& m);
// �]�u�s��
Matrix4x4 Transpose(const Matrix4x4& m);
// �P�ʍs��̍쐬
Matrix4x4 MakeIdentity4x4();

// ���s�ړ��s��
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
// �g��k���s��
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// ���W�ϊ�
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

// X����]�s��
Matrix4x4 MakeRotateXMatrix(float radian);
// Y����]�s��
Matrix4x4 MakeRotateYMatrix(float radian);
// Z����]�s��
Matrix4x4 MakeRotateZMatrix(float radian);

// �A�t�B���ϊ��s��
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

// �P�ʍs��̍쐬
Matrix4x4 MakeIdentity4x4();

// �������ˍs��
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
// ���ˉe�s��
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
// �r���[�|�[�g�ύX�s��
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);