#include "DebugDraw.h"
#include "DxLib.h"

std::list<MyLibrary::DebugDraw::CapsuleInfo> MyLibrary::DebugDraw::m_capsuleInfo;
std::list<MyLibrary::DebugDraw::SphereInfo> MyLibrary::DebugDraw::m_sphereInfo;
std::list<MyLibrary::DebugDraw::RectInfo> MyLibrary::DebugDraw::m_rectInfo;

/// <summary>
/// 描画情報の削除
/// </summary>
void MyLibrary::DebugDraw::Clear()
{
	//すべての描画情報リストの中身を全削除
	m_capsuleInfo.clear();
	m_sphereInfo.clear();
	m_rectInfo.clear();
}

/// <summary>
/// 描画
/// </summary>
void MyLibrary::DebugDraw::Draw()
{
#if _DEBUG

	//カプセルの描画情報リストにある情報分描画する
	for (auto& capsule : m_capsuleInfo)
	{
		capsule.vec = capsule.vec.GetNormalized() * capsule.len * 0.5f;

		DrawCapsule3D(VAdd(capsule.center.ConversionToVECTOR(), capsule.vec.ConversionToVECTOR()), VSub(capsule.center.ConversionToVECTOR(), capsule.vec.ConversionToVECTOR()),
			capsule.radius, 16, capsule.color, capsule.color, false);
	}

	//球体の描画情報リストにある情報分描画する
	for (auto& sphere : m_sphereInfo)
	{
		DrawSphere3D(sphere.center.ConversionToVECTOR(), sphere.radius, 16, sphere.color, sphere.color, false);
	}

	//矩形の描画情報リストにある情報分描画する
	for (auto& rect : m_rectInfo)
	{
	}

#endif
}

/// <summary>
/// カプセル型の描画リストに追加する
/// </summary>
/// <param name="center">中心座標</param>
/// <param name="vec">ベクター</param>
/// <param name="radius">半径</param>
/// <param name="len">長さ</param>
/// <param name="color">色</param>
void MyLibrary::DebugDraw::AddDrawCapsule(const LibVec3& center, const LibVec3& vec, const float& radius, const float& len, const unsigned int& color)
{
	//カプセル型の描画リストに追加する
	CapsuleInfo addInfo;
	addInfo.center = center;
	addInfo.vec = vec;
	addInfo.radius = radius;
	addInfo.len = len;
	addInfo.color = color;
	m_capsuleInfo.emplace_back(addInfo);
}

/// <summary>
/// 球体の描画リストに追加する
/// </summary>
/// <param name="center">中心座標</param>
/// <param name="radius">半径</param>
/// <param name="color">色</param>
void MyLibrary::DebugDraw::AddDrawSphere(const LibVec3& center, const float& radius, const unsigned int& color)
{
	//球体の描画リストに追加する
	SphereInfo addInfo;
	addInfo.center = center;
	addInfo.radius = radius;
	addInfo.color = color;
	m_sphereInfo.emplace_back(addInfo);
}

/// <summary>
/// 矩形の描画リストに追加する
/// </summary>
/// <param name="center">中心座標</param>
/// <param name="size">サイズ</param>
/// <param name="color">色</param>
void MyLibrary::DebugDraw::AddDrawRect(const LibVec3& center, const LibVec3::Size& size, const unsigned int& color)
{
	RectInfo addInfo;
	addInfo.center = center;
	addInfo.size = size;
	addInfo.color = color;
	m_rectInfo.emplace_back(addInfo);
}
