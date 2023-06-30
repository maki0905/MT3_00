#include <Novice.h>
#include "Sphere.h"
#include <imgui.h>
#include <cstdint>
#include "MathFunction.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <imgui.h>
#include "Collision.h"
#include "Plane.h"
#include "Triangle.h"
#include "AABB.h"


const char kWindowTitle[] = "LE1A_16_マキユキノリ_タイトル";

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* name);
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label);

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewporMatrix);
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	static const int kWindowWidth = 1280;
	static const int kWindowHeight = 720;


	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 viewProjectionMatrix;
	Matrix4x4 viewportMatrix;

	// デバッグカメラ
	bool isDebugCameraActive = false;
	Matrix4x4 debugCamera = MakeIdentity4x4();
	Vector3 debugCameraTranslate{ 0.0f, 0.0f, 0.0f };
	Vector3 debugCameraRotate{ 0.0f, 0.0f, 0.0f };

	

	Sphere sphere[2];
	sphere[0] = { {0.0f, 0.0f, 0.0f}, 0.5f };
	
	//Plane plane = { {0.0f, 1.0f, 0.0f}, 0.5f };

	Segment segment = { 
		.origin{0.7f, 0.3f, 0.0f}, 
		.diff{2.0f, -0.5f, 0.5f} 
	};

	Triangle triangle = { 
		{ 
			{0.0f, 1.0f, 0.0f}, 
		    {1.0f, 0.0f, 0.0f}, 
		     {-1.0f, 0.0f, 0.0f}
		} 
	};

	AABB aabb1{
		.min{-0.5f, -0.5f, -0.5f},
		.max{0.5f, 0.5f, 0.5f}
	};
	/*AABB aabb2{
		.min{0.2f, 0.2f, 0.2f},
		.max{1.0f, 1.0f, 1.0f}
	};*/


	// カメラの位置と角度
	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };
	Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);

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
		
		
		
		if (keys[DIK_Q] != 0 && preKeys[DIK_Q] == 0) {
			if (isDebugCameraActive == false) {
				isDebugCameraActive = true;
				debugCamera = cameraMatrix;
				debugCameraRotate = cameraRotate;
				debugCameraTranslate = cameraTranslate;
			}
			else {
				isDebugCameraActive = false;
			}
		}
		if (isDebugCameraActive == true) {
			debugCamera = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, debugCameraRotate, debugCameraTranslate);
			viewMatrix = Inverse(debugCamera);
			projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
			viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
			viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
			ImGui::Begin("DebugCamera");
			ImGui::DragFloat3("Rotate", &debugCameraRotate.x, 0.01f);
			ImGui::DragFloat3("Translate", &debugCameraTranslate.x, 0.01f);
			ImGui::End();
		}
		else {
			viewMatrix = Inverse(cameraMatrix);
			projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
			viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
			viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		}
		
		
		ImGui::Begin("Window");
		//ImGui::DragFloat3("SpherCenter1", &sphere[0].center.x, 0.01f);
		//ImGui::DragFloat("SpherRadius1", &sphere[0].radius, 0.01f);
		//ImGui::DragFloat3("Plane.Normal", &plane.normal.x, 0.01f);
		//plane.normal = Normalize(plane.normal);
		//ImGui::DragFloat("Plane.distance", &plane.distance, 0.01f);
		ImGui::DragFloat3("Line.origin", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("Line.diff", &segment.diff.x, 0.01f);
		/*ImGui::DragFloat3("Triangle.vertices[0]", &triangle.vertices[0].x, 0.01f);
		ImGui::DragFloat3("Triangle.vertices[1]", &triangle.vertices[1].x, 0.01f);
		ImGui::DragFloat3("Triangle.vertices[2]", &triangle.vertices[2].x, 0.01f);*/
		ImGui::DragFloat3("aabb1.min", &aabb1.min.x, 0.01f);
		ImGui::DragFloat3("aabb1.max", &aabb1.max.x, 0.01f);
		aabb1.min.x = (std::min)(aabb1.min.x, aabb1.max.x);
		aabb1.max.x = (std::max)(aabb1.min.x, aabb1.max.x);
		aabb1.min.y = (std::min)(aabb1.min.y, aabb1.max.y);
		aabb1.max.y = (std::max)(aabb1.min.y, aabb1.max.y);
		aabb1.min.z = (std::min)(aabb1.min.z, aabb1.max.z);
		aabb1.max.z = (std::max)(aabb1.min.z, aabb1.max.z);
		/*ImGui::DragFloat3("aabb2.min", &aabb2.min.x, 0.01f);
		ImGui::DragFloat3("aabb2.max", &aabb2.max.x, 0.01f);
		aabb2.min.x = (std::min)(aabb2.min.x, aabb2.max.x);
		aabb2.max.x = (std::max)(aabb2.min.x, aabb2.max.x);
		aabb2.min.y = (std::min)(aabb2.min.y, aabb2.max.y);
		aabb2.max.y = (std::max)(aabb2.min.y, aabb2.max.y);
		aabb2.min.z = (std::min)(aabb2.min.z, aabb2.max.z);
		aabb2.max.z = (std::max)(aabb2.min.z, aabb2.max.z);*/
		ImGui::End();
		
		
		
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		DrawGrid(viewProjectionMatrix, viewportMatrix);
		/*if (IsCollision(sphere[0], plane)) {
			DrawSphere(sphere[0], viewProjectionMatrix, viewportMatrix, RED);
		}
		else {
			DrawSphere(sphere[0], viewProjectionMatrix, viewportMatrix, WHITE);
		}*/
		/*if (IsCollision(segment, plane)) {
			
			Novice::DrawLine(int(screenPoint[0].x), int(screenPoint[0].y), int(screenPoint[1].x), int(screenPoint[1].y), RED);
		}
		else {
			Novice::DrawLine(int(screenPoint[0].x), int(screenPoint[0].y), int(screenPoint[1].x), int(screenPoint[1].y), WHITE);
		}
		DrawPlane(plane, viewProjectionMatrix, viewportMatrix, WHITE);*/
		/*if (IsCollision(triangle, segment)) {
			DrawSegment(segment, viewProjectionMatrix, viewportMatrix, RED);
		}
		else {
			DrawSegment(segment, viewProjectionMatrix, viewportMatrix, WHITE);
		}
		
		DrawTriangle(triangle, viewProjectionMatrix, viewportMatrix, WHITE);*/

		/*if (IsCollision(aabb1, aabb2)) {
			DrawAABB(aabb1, viewProjectionMatrix, viewportMatrix, RED);
		}
		else {
			DrawAABB(aabb1, viewProjectionMatrix, viewportMatrix, WHITE);
		}
		DrawAABB(aabb2, viewProjectionMatrix, viewportMatrix, WHITE);*/
		DrawSegment(segment, viewProjectionMatrix, viewportMatrix, WHITE);
		if (IsCollision(aabb1,segment)) {
			DrawAABB(aabb1, viewProjectionMatrix, viewportMatrix, RED);
		}
		else {
			DrawAABB(aabb1, viewProjectionMatrix, viewportMatrix, WHITE);
		}
		/*if (IsCollision(sphere[0], aabb1)) {
			DrawAABB(aabb1, viewProjectionMatrix, viewportMatrix, RED);
		}
		else {
			DrawAABB(aabb1, viewProjectionMatrix, viewportMatrix, WHITE);
		}*/

		//DrawSphere(sphere[0], viewProjectionMatrix, viewportMatrix, WHITE);
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

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 center = Multiply(plane.distance, plane.normal);
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z };

	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}

	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[0].x), int(points[0].y), color);
}

