//==============================================================================================================
//
// math [math.cpp]
// Author: YOSHIDA YUTO
//
//==============================================================================================================

//*******************************************************************************************************************
// インクルードファイル宣言
//*******************************************************************************************************************
#include "math.h"

//==============================================================================================================
// だんだん目標の値に近づける関数
//==============================================================================================================
float SetSmoothAprroach(float fDestValue, float Value, float coefficient)
{
	// だんだん目標の値に近づける
	float OutValue = (fDestValue - Value) * coefficient;
	return OutValue;
}
//===============================================================================================================
// 円の判定
//===============================================================================================================
bool sphererange(D3DXVECTOR3* pPos1, D3DXVECTOR3* pPos2, float radius1, float radius2) // 円の当たり判定
{
	bool bRange = false;

	D3DXVECTOR3 DiffPos; // 計算用

	// 距離XYZを求める
	DiffPos.x = pPos1->x - pPos2->x;
	DiffPos.y = pPos1->y - pPos2->y;
	DiffPos.z = pPos1->z - pPos2->z;

	// 距離を求める
	float fDistance = (DiffPos.x * DiffPos.x) + (DiffPos.y * DiffPos.y) + (DiffPos.z * DiffPos.z);

	// 半径を求める
	float fRadius = radius1 + radius2;

	// 半径を求める
	fRadius = fRadius * fRadius;

	if (fDistance <= fRadius)
	{
		bRange = true;
	}
	return bRange;
}
//=====================================================================================================
// ワールドマトリックスをD3DXVECTOR3に変換する
//=====================================================================================================
D3DXVECTOR3 SetMtxConversion(D3DXMATRIX mtxworld)
{
	// XYZに変換
	D3DXVECTOR3 OutPut(mtxworld._41, mtxworld._42, mtxworld._43);

	// 変換した値を返す
	return OutPut;
}
////=====================================================================================================
//// エフェクトのテクスチャアニメーション
////=====================================================================================================
//void SetTextureAnimation(int *pPosX,int *pPosY,int *pAnimSpeed,int *pCounterAnim,int *pPatternAnim, VERTEX_3D *pVtx, LPDIRECT3DVERTEXBUFFER9 *pVtxBuff)
//{
//	VERTEX_3D* pVtx;
//
//	//頂点バッファをロック
//	g_pVtxBuffEffectX->Lock(0, 0, (void**)&pVtx, 0);
//
//	// アニメーションカウンターを加算
//	g_EffectX[nCnt].g_nCounterAnim++;
//
//	// テクスチャ座標を計算
//	float Uv = 1.0f / XtexPos;
//	float Hv = 1.0f / YtexPos;
//
//	// アニメーションのスピード
//	if (g_EffectX[nCnt].g_nCounterAnim >= Animspeed)
//	{
//		g_EffectX[nCnt].g_nCounterAnim = NULL;//カウンターを初期値に戻す
//
//		g_EffectX[nCnt].g_nPatternAnim++;//パターンナンバーを更新
//
//		pVtx += 4 * nCnt;
//
//		// テクスチャ座標の更新
//		pVtx[0].tex = D3DXVECTOR2(g_EffectX[nCnt].g_nPatternAnim * Uv, (g_EffectX[nCnt].g_nPatternAnim / XtexPos) * Hv);
//		pVtx[1].tex = D3DXVECTOR2(Uv + g_EffectX[nCnt].g_nPatternAnim * Uv, (g_EffectX[nCnt].g_nPatternAnim / XtexPos) * Hv);
//		pVtx[2].tex = D3DXVECTOR2(g_EffectX[nCnt].g_nPatternAnim * Uv, (g_EffectX[nCnt].g_nPatternAnim / XtexPos) * Hv + Hv);
//		pVtx[3].tex = D3DXVECTOR2(Uv + g_EffectX[nCnt].g_nPatternAnim * Uv, (g_EffectX[nCnt].g_nPatternAnim / XtexPos) * Hv + Hv);
//	}
//
//	// パターンが最大になったら
//	if (g_EffectX[nCnt].g_nPatternAnim > XtexPos)
//	{
//		g_EffectX[nCnt].g_nPatternAnim = 0;//パターンナンバーを初期値に戻す
//	}
//
//	// 頂点バッファのアンロック
//	g_pVtxBuffEffectX->Unlock();
//}
