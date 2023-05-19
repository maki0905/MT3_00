#include <Novice.h>
#include "Sphere.h"
#include <imgui.h>
#include <cstdint>
#include "MathFunction.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <imgui.h>


const char kWindowTitle[] = "LE1A_16_マキユキノリ_タイトル";

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* name);
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewporMatrix);
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	static const int kWindowWidth = 1280;
	static const int kWindowHeight = 720;


	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// Sphere
	Sphere sphere{
		{0.0f, 0.0f, 3.5f},
		1.0f
	};

	// カメラの位置と角度
	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };

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
		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("GameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("SphereCenter", &sphere.center.x, 0.01f);
		ImGui::DragFloat("SpherRadius", &sphere.radius, 0.01f);
		ImGui::End();
		
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float (kWindowHeight),0.0f, 1.0f);
		
		

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		DrawGrid(viewProjectionMatrix, viewportMatrix);
		DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, 0x000000AA);
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

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
	const float kGridHalfwidth = 2.0f; // Gridの半分の幅
	const uint32_t kSubdivision = 10; // 分割数
	const float kGridEvery = (kGridHalfwidth * 2.0f) / float(kSubdivision); // 1つ分の長さ
	// 奥から手前への線を順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		// ワールド座標系上の始点と終点を求める
		Vector3 start = { -(kGridEvery * float(kSubdivision / uint32_t(kGridHalfwidth))), 0.0f, kGridEvery * float(float(kSubdivision) / kGridHalfwidth - float(zIndex))};
		Vector3 end = { (kGridEvery * float(kSubdivision / uint32_t(kGridHalfwidth))), 0.0f, kGridEvery * float(float(kSubdivision) / kGridHalfwidth - float(zIndex)) };
		// スクリーン座標系まで変換をかける
		Vector3 ndcVertex = Transform(start, viewProjectionMatrix);
		start = Transform(ndcVertex, viewportMatrix);
		ndcVertex = Transform(end, viewProjectionMatrix);
		end = Transform(ndcVertex, viewportMatrix);
		// 変換した座標を使って表示、色は薄い灰色(0xAAAAAAFF)、原点は黒ぐらいが良いが、なんでも良い
		if (zIndex == (kSubdivision / int(kGridHalfwidth))) {
			Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0x000000FF);
		}
		else {
			Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0xAAAAAAFF);
		}
		
	}
	// 左から右も同じように順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		// ワールド座標系上の始点と終点を求める
		Vector3 start = { kGridEvery * float(float(zIndex) - float(kSubdivision) / kGridHalfwidth), 0.0f, -(kGridEvery * float(kSubdivision / uint32_t(kGridHalfwidth))) };
		Vector3 end = { kGridEvery * float(float(zIndex) - float(kSubdivision) / kGridHalfwidth), 0.0f, (kGridEvery * float(kSubdivision / uint32_t(kGridHalfwidth))) };
		// スクリーン座標系まで変換をかける
		Vector3 ndcVertex = Transform(start, viewProjectionMatrix);
		start = Transform(ndcVertex, viewportMatrix);
		ndcVertex = Transform(end, viewProjectionMatrix);
		end = Transform(ndcVertex, viewportMatrix);
		// 変換した座標を使って表示、色は薄い灰色(0xAAAAAAFF)、原点は黒ぐらいが良いが、なんでも良い
		if (zIndex == (kSubdivision / int(kGridHalfwidth))) {
			Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0x000000FF);
		}
		else {
			Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), 0xAAAAAAFF);
		}
	}
}


void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	const uint32_t kSubdivision = 15; // 分割数
	const float kLonEvery = float(M_PI / float(kSubdivision)); // 経度分割1つ分の角度
	const float kLatEvery = float(2.0f * M_PI / float(kSubdivision)); // 緯度分割1つ分の角度
	Vector3 center = sphere.center;
	float radius = sphere.radius;
	// 緯度の方向に分割 -π/2 ~ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = float(-M_PI / 2.0f) + float(kLatEvery * float(latIndex)); // 現在の緯度
		// 経度の方向に分割 0 ~ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = float(float(lonIndex) * kLonEvery); // 現在の経度
			// world座標系でのa,b,cを求める
			Vector3 a, b, c;
			a = Multiply( radius, { std::cos(lat) * std::cos(lon), std::sin(lat), std::cos(lat) * std::sin(lon)});
			b = Multiply( radius, { std::cos(lat + kLatEvery) * std::cos(lon), std::sin(lat + kLatEvery), std::cos(lat + kLatEvery) * std::sin(lon)});
			c = Multiply( radius, { std::cos(lat) * std::cos(lon + kLonEvery), std::sin(lat), std::cos(lat) * std::sin(lon + kLonEvery) });
			// a, b, c, をScreen座標系まで変換
			Vector3 ndcVertex = Transform(Add(a,center), viewProjectionMatrix);
			a = Transform(ndcVertex, viewportMatrix);
			ndcVertex = Transform(Add(b, center), viewProjectionMatrix);
			b = Transform(ndcVertex, viewportMatrix);
			ndcVertex = Transform(Add(c, center), viewProjectionMatrix);
			c = Transform(ndcVertex, viewportMatrix);
			// ab, acで線を引く
			Novice::DrawLine(int(a.x), int(a.y), int(b.x), int(b.y), color);
			Novice::DrawLine(int(a.x), int(a.y), int(c.x), int(c.y), color);
		}

	}



}
