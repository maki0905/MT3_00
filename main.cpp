#include <Novice.h>
#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>
#include <assert.h>

const char kWindowTitle[] = "LE1A_16_マキユキノリ_タイトル";

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* name);
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);

//クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

// 1. 透視投射行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
// 2. 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
// 3. ビューポート変更行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

// ベクトルの加法
Vector3 Add(Vector3& v1, Vector3& v2);

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// 座標変換
 Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);
// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

// アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	Vector3 v1{ 1.2f, -3.9f, 2.5f };
	Vector3 v2{ 2.8f, 0.4f, -1.3f };

	Vector3 ratate = { 0.0f, 0.0f, 0.0f };
	Vector3 translate = { 0.0f, 0.0f, 0.0f };
	Vector3 cameraPosition = { 0.0f, 0.0f, -10.0f };

	Vector3 kLocalVertices[3];
	kLocalVertices[0] = {0.0f, 1.0f, 0.0f};
	kLocalVertices[1] = {-1.0f, 0.0f, 0.0f};
	kLocalVertices[2] = {1.0f, 0.0f, 0.0f};


	const float kRotSpeed = 0.1f;
	

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		//WSキーで前後に、ADキーで左右に三角形を動かす。Y軸回転をさせる。という処理をここに書く。translateとrotaeの値を変更すれば良い。
		Vector3 move = { 0.0f, 0.0f, 0.0f };
		if (keys[DIK_W] != 0) {
			move.z += 0.2f;
		}
		if (keys[DIK_S] != 0) {
			move.z -= 0.2f;
		}
		if (keys[DIK_A] != 0) {
			move.x -= 0.2f;
		}
		if (keys[DIK_D] != 0) {
			move.x += 0.2f;
		}
		translate = Add(translate, move);

		ratate.y += kRotSpeed;

		//各種行列の計算
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, ratate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, cameraPosition);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		Vector3 screenVertices[3];
		for (uint32_t i = 0; i < 3; ++i) {
			Vector3 ndcVertex = Transform(kLocalVertices[i], worldViewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}
		Vector3 cross = Cross(v1, v2);


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		VectorScreenPrintf(0, 0, cross, "Cross");
		Novice::DrawTriangle(
			int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y),
			int(screenVertices[2].x), int(screenVertices[2].y), RED, kFillModeSolid
		);
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* name) {
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x, y, "%s", name);
			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight + 20, "%6.02f", matrix.m[row][column]);
		}
	}
}

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = (v1.y * v2.z) - (v1.z * v2.y);
	result.y = (v1.z * v2.x) - (v1.x * v2.z);
	result.z = (v1.x * v2.y) - (v1.y * v2.x);

	return result;
}

// 1. 透視投射行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			result.m[line][column] = 0;
		}
	}

	result.m[0][0] = (1 / aspectRatio) * (1 / std::tan(fovY / 2.0f));
	result.m[1][1] = (1 / std::tan(fovY / 2.0f));
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1;
	result.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
	return result;
}
// 2. 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip){
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			result.m[line][column] = 0;
		}
	}

	result.m[0][0] = 2 / (right - left);
	result.m[1][1] = 2 / (top - bottom);
	result.m[2][2] = 1 / (farClip - nearClip);
	result.m[3][3] = 1;
	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farClip);

	return result;
}
// 3. ビューポート変更行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth){
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			result.m[line][column] = 0;
		}
	}

	result.m[0][0] = width / 2.0f;
	result.m[1][1] = -height / 2.0f;
	result.m[2][2] = maxDepth - minDepth;
	result.m[3][3] = 1;
	result.m[3][0] = left + (width / 2.0f);
	result.m[3][1] = top + (height / 2.0f);
	result.m[3][2] = minDepth;

	return result;
}

// ベクトルの加法
Vector3 Add(Vector3& v1, Vector3& v2) {
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 result;
	float A =
		m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] +
		m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] +
		m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] -

		m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
		m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] -
		m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] -

		m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] -
		m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
		m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] +

		m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] +
		m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] +
		m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +

		m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] +
		m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] +
		m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] -

		m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
		m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] -
		m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] -

		m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] -
		m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
		m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] +

		m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] +
		m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] +
		m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	result.m[0][0] =
		(1 / A) * (
			m.m[1][1] * m.m[2][2] * m.m[3][3] +
			m.m[1][2] * m.m[2][3] * m.m[3][1] +
			m.m[1][3] * m.m[2][1] * m.m[3][2] -
			m.m[1][3] * m.m[2][2] * m.m[3][1] -
			m.m[1][2] * m.m[2][1] * m.m[3][3] -
			m.m[1][1] * m.m[2][3] * m.m[3][2]);
	result.m[0][1] =
		(1 / A) * (
			-m.m[0][1] * m.m[2][2] * m.m[3][3] -
			m.m[0][2] * m.m[2][3] * m.m[3][1] -
			m.m[0][3] * m.m[2][1] * m.m[3][2] +
			m.m[0][3] * m.m[2][2] * m.m[3][1] +
			m.m[0][2] * m.m[2][1] * m.m[3][3] +
			m.m[0][1] * m.m[2][3] * m.m[3][2]);
	result.m[0][2] =
		(1 / A) * (
			m.m[0][1] * m.m[1][2] * m.m[3][3] +
			m.m[0][2] * m.m[1][3] * m.m[3][1] +
			m.m[0][3] * m.m[1][1] * m.m[3][2] -
			m.m[0][3] * m.m[1][2] * m.m[3][1] -
			m.m[0][2] * m.m[1][1] * m.m[3][3] -
			m.m[0][1] * m.m[1][3] * m.m[3][2]);
	result.m[0][3] =
		(1 / A) * (
			-m.m[0][1] * m.m[1][2] * m.m[2][3] -
			m.m[0][2] * m.m[1][3] * m.m[2][1] -
			m.m[0][3] * m.m[1][1] * m.m[2][2] +
			m.m[0][3] * m.m[1][2] * m.m[2][1] +
			m.m[0][2] * m.m[1][1] * m.m[2][3] +
			m.m[0][1] * m.m[1][3] * m.m[2][2]);
	result.m[1][0] =
		(1 / A) * (
			-m.m[1][0] * m.m[2][2] * m.m[3][3] -
			m.m[1][2] * m.m[2][3] * m.m[3][0] -
			m.m[1][3] * m.m[2][0] * m.m[3][2] +
			m.m[1][3] * m.m[2][2] * m.m[3][0] +
			m.m[1][2] * m.m[2][0] * m.m[3][3] +
			m.m[1][0] * m.m[2][3] * m.m[3][2]);
	result.m[1][1] =
		(1 / A) * (
			m.m[0][0] * m.m[2][2] * m.m[3][3] +
			m.m[0][2] * m.m[2][3] * m.m[3][0] +
			m.m[0][3] * m.m[2][0] * m.m[3][2] -
			m.m[0][3] * m.m[2][2] * m.m[3][0] -
			m.m[0][2] * m.m[2][0] * m.m[3][3] -
			m.m[0][0] * m.m[2][3] * m.m[3][2]);
	result.m[1][2] =
		(1 / A) * (
			-m.m[0][0] * m.m[1][2] * m.m[3][3] -
			m.m[0][2] * m.m[1][3] * m.m[3][0] -
			m.m[0][3] * m.m[1][0] * m.m[3][2] +
			m.m[0][3] * m.m[1][2] * m.m[3][0] +
			m.m[0][2] * m.m[1][0] * m.m[3][3] +
			m.m[0][0] * m.m[1][3] * m.m[3][2]);
	result.m[1][3] =
		(1 / A) * (
			m.m[0][0] * m.m[1][2] * m.m[2][3] +
			m.m[0][2] * m.m[1][3] * m.m[2][0] +
			m.m[0][3] * m.m[1][0] * m.m[2][2] -
			m.m[0][3] * m.m[1][2] * m.m[2][0] -
			m.m[0][2] * m.m[1][0] * m.m[2][3] -
			m.m[0][0] * m.m[1][3] * m.m[2][2]);
	result.m[2][0] =
		(1 / A) * (
			m.m[1][0] * m.m[2][1] * m.m[3][3] +
			m.m[1][1] * m.m[2][3] * m.m[3][0] +
			m.m[1][3] * m.m[2][0] * m.m[3][1] -
			m.m[1][3] * m.m[2][1] * m.m[3][0] -
			m.m[1][1] * m.m[2][0] * m.m[3][3] -
			m.m[1][0] * m.m[2][3] * m.m[3][1]);
	result.m[2][1] =
		(1 / A) * (
			-m.m[0][0] * m.m[2][1] * m.m[3][3] -
			m.m[0][1] * m.m[2][3] * m.m[3][0] -
			m.m[0][3] * m.m[2][0] * m.m[3][1] +
			m.m[0][3] * m.m[2][1] * m.m[3][0] +
			m.m[0][1] * m.m[2][0] * m.m[3][3] +
			m.m[0][0] * m.m[2][3] * m.m[3][1]);
	result.m[2][2] =
		(1 / A) * (
			m.m[0][0] * m.m[1][1] * m.m[3][3] +
			m.m[0][1] * m.m[1][3] * m.m[3][0] +
			m.m[0][3] * m.m[1][0] * m.m[3][1] -
			m.m[0][3] * m.m[1][1] * m.m[3][0] -
			m.m[0][1] * m.m[1][0] * m.m[3][3] -
			m.m[0][0] * m.m[1][3] * m.m[3][1]);
	result.m[2][3] =
		(1 / A) * (
			-m.m[0][0] * m.m[1][1] * m.m[2][3] -
			m.m[0][1] * m.m[1][3] * m.m[2][0] -
			m.m[0][3] * m.m[1][0] * m.m[2][1] +
			m.m[0][3] * m.m[1][1] * m.m[2][0] +
			m.m[0][1] * m.m[1][0] * m.m[2][3] +
			m.m[0][0] * m.m[1][3] * m.m[2][1]);
	result.m[3][0] =
		(1 / A) * (
			-m.m[1][0] * m.m[2][1] * m.m[3][2] -
			m.m[1][1] * m.m[2][2] * m.m[3][0] -
			m.m[1][2] * m.m[2][0] * m.m[3][1] +
			m.m[1][2] * m.m[2][1] * m.m[3][0] +
			m.m[1][1] * m.m[2][0] * m.m[3][2] +
			m.m[1][0] * m.m[2][2] * m.m[3][1]);
	result.m[3][1] =
		(1 / A) * (
			m.m[0][0] * m.m[2][1] * m.m[3][2] +
			m.m[0][1] * m.m[2][2] * m.m[3][0] +
			m.m[0][2] * m.m[2][0] * m.m[3][1] -
			m.m[0][2] * m.m[2][1] * m.m[3][0] -
			m.m[0][1] * m.m[2][0] * m.m[3][2] -
			m.m[0][0] * m.m[2][2] * m.m[3][1]);
	result.m[3][2] =
		(1 / A) * (
			-m.m[0][0] * m.m[1][1] * m.m[3][2] -
			m.m[0][1] * m.m[1][2] * m.m[3][0] -
			m.m[0][2] * m.m[1][0] * m.m[3][1] +
			m.m[0][2] * m.m[1][1] * m.m[3][0] +
			m.m[0][1] * m.m[1][0] * m.m[3][2] +
			m.m[0][0] * m.m[1][2] * m.m[3][1]);
	result.m[3][3] =
		(1 / A) * (
			m.m[0][0] * m.m[1][1] * m.m[2][2] +
			m.m[0][1] * m.m[1][2] * m.m[2][0] +
			m.m[0][2] * m.m[1][0] * m.m[2][1] -
			m.m[0][2] * m.m[1][1] * m.m[2][0] -
			m.m[0][1] * m.m[1][0] * m.m[2][2] -
			m.m[0][0] * m.m[1][2] * m.m[2][1]);

	return result;
}

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int j = 0; j < 4; j++) {
			result.m[line][j] = 0;
		}
	}
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			result.m[line][column] =
				m1.m[line][0] * m2.m[0][column] + m1.m[line][1] * m2.m[1][column] +
				m1.m[line][2] * m2.m[2][column] + m1.m[line][3] * m2.m[3][column];
		}
	}
	return result;
}

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			if (line == column) {
				result.m[line][column] = 1;
			}
			else {
				result.m[line][column] = 0;
			}
		}
	}

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	return result;
}

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			if (line == column) {
				result.m[line][column] = 1;
			}
			else {
				result.m[line][column] = 0;
			}
		}
	}

	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;

	return result;
}

// 座標変換
 Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) { 
	 Vector3 result;
	 result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	 result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	 result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	 float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	 assert(w != 0.0f);
	 result.x /= w;
	 result.y /= w;
	 result.z /= w;
	 return result;
 }

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			if (line == column) {
				result.m[line][column] = 1;
			}
			else {
				result.m[line][column] = 0;
			}
		}
	}
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[2][1] = -std::sin(radian);
	result.m[2][2] = std::cos(radian);
	return result;
}

// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			if (line == column) {
				result.m[line][column] = 1;
			}
			else {
				result.m[line][column] = 0;
			}
		}
	}
	result.m[0][0] = std::cos(radian);
	result.m[0][2] = -std::sin(radian);
	result.m[2][0] = std::sin(radian);
	result.m[2][2] = std::cos(radian);
	return result;
}

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			if (line == column) {
				result.m[line][column] = 1;
			}
			else {
				result.m[line][column] = 0;
			}
		}
	}
	result.m[0][0] = std::cos(radian);
	result.m[0][1] = std::sin(radian);
	result.m[1][0] = -std::sin(radian);
	result.m[1][1] = std::cos(radian);
	return result;
}

// アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate) {
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			result.m[line][column] = 0;
		}
	}
	// スケーリング行列の作成
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

	// X軸周り回転行列の作成
	Matrix4x4 rotateX = MakeRotateXMatrix(rot.x);
	// Y軸周り回転行列の作成
	Matrix4x4 rotateY = MakeRotateYMatrix(rot.y);
	// Z軸周り回転行列の作成
	Matrix4x4 rotateZ = MakeRotateZMatrix(rot.z);
	// 回転行列の合成(Z回転 * X回転 * Y回転)
	Matrix4x4 rotateMatrix = Multiply(Multiply(rotateZ, rotateX), rotateY);

	// 平行移動行列の作成
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	result = Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);
	return result;
}