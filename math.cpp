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
//=====================================================================================================
// 目標までのベクトルを計算する
//=====================================================================================================
D3DXVECTOR3 SetSubtractVectors(D3DXVECTOR3 targetpos, D3DXVECTOR3 pos)
{
	D3DXVECTOR3 OutPut = targetpos - pos;

	return OutPut;
}
//=====================================================================================================
// エフェクトのテクスチャアニメーション
//=====================================================================================================
void SetTextureAnimation(int pPosX,int pPosY,int pAnimSpeed,int *pCounterAnim,int *pPatternAnim, VERTEX_2D *pVtx, LPDIRECT3DVERTEXBUFFER9 pVtxBuff,int nCnt)
{
	//頂点バッファをロック
	if (FAILED(pVtxBuff->Lock(0, 0, (void**)&pVtx, 0)))
	{
		// エラーが起きたら関数を抜ける
		return;
	}

	// アニメーションカウンターを加算
	(*pCounterAnim)++;

	// テクスチャ座標を計算
	float Uv = 1.0f / pPosX;
	float Hv = 1.0f / pPosY;

	// アニメーションのスピード
	if (*pCounterAnim >= pAnimSpeed)
	{
		*pCounterAnim = 0;//カウンターを初期値に戻す

		(*pPatternAnim)++;//パターンナンバーを更新

		pVtx += 4 * nCnt;

		// テクスチャ座標の更新
		pVtx[0].tex = D3DXVECTOR2(*pPatternAnim * Uv, (*pPatternAnim / pPosX) * Hv);
		pVtx[1].tex = D3DXVECTOR2(Uv + *pPatternAnim * Uv, (*pPatternAnim / pPosX) * Hv);
		pVtx[2].tex = D3DXVECTOR2(*pPatternAnim * Uv, (*pPatternAnim / pPosX) * Hv + Hv);
		pVtx[3].tex = D3DXVECTOR2(Uv + *pPatternAnim * Uv, (*pPatternAnim / pPosX) * Hv + Hv);

		// パターンが最大になったら
		if (*pPatternAnim > (pPosX * pPosY))
		{
			*pPatternAnim = 0;//パターンナンバーを初期値に戻す
		}
	}

	// 頂点バッファのアンロック
	pVtxBuff->Unlock();
}
