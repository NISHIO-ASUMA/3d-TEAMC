//==============================================================================================================
//
// バリア [barrier.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//**************************************************************************************************************
// インクルードファイル
//**************************************************************************************************************
#include "barrier.h"
#include "mark.h"
#include "effect2.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define HALF_VALUE (0.5f) // 半分にする

//**************************************************************************************************************
// プロトタイプ宣言
//**************************************************************************************************************
bool isCollisionZ(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier);		// Zの範囲
bool isCollisionRightX(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier);	// Xの面に左から右に当たった
bool isCollisionLeftX(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier);	// Xの面に右から左に当たった
bool isCollisionX(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier);		// Xの範囲
bool isCollisionFrontZ(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier);	// 前面に当たった
bool isCollisionBehindZ(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier);  // 背面に当たった

//**************************************************************************************************************
// グローバル変数
//**************************************************************************************************************
Barrier g_Barrier[MAX_BARRIER]; // バリアの情報

//==============================================================================================================
// バリアの初期化処理
//==============================================================================================================
void InitBarrier(void)
{
	// バリアの最大数分回す
	for (int nCnt = 0; nCnt < MAX_BARRIER; nCnt++)
	{
		g_Barrier[nCnt].pos = NULLVECTOR3;		// 位置
		g_Barrier[nCnt].vtxMax = NULLVECTOR3;	// 最大の頂点
		g_Barrier[nCnt].vtxMin = NULLVECTOR3;	// 最小の頂点
		g_Barrier[nCnt].Size = NULLVECTOR3;		// 大きさ
		g_Barrier[nCnt].bUse = false;			// 使用状態
	}
}
//==============================================================================================================
// バリアの設定処理
//==============================================================================================================
void SetBarrier(D3DXVECTOR3 pos, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	// バリアの最大数分回す
	for (int nCnt = 0; nCnt < MAX_BARRIER; nCnt++)
	{
		// 未使用だったら
		if (g_Barrier[nCnt].bUse == false)
		{
			g_Barrier[nCnt].pos = pos;			// 位置
			g_Barrier[nCnt].vtxMax = vtxMax;	// 最大の頂点
			g_Barrier[nCnt].vtxMin = vtxMin;	// 最小の頂点

			g_Barrier[nCnt].Size = vtxMax - vtxMin; // 大きさを求める
			g_Barrier[nCnt].bUse = true;			// 使用状態にする
			break;
		}
	}
}
//==============================================================================================================
// バリアの判定処理
//==============================================================================================================
void CollisionBarrier(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize)
{
	// バリアの最大数分回す
	for (int nCnt = 0; nCnt < MAX_BARRIER; nCnt++)
	{
		// 未使用だったら
		if (g_Barrier[nCnt].bUse == false) continue;

		// Zの範囲に入ってたら
		if (isCollisionZ(pPos, pPosOld, pMove, pSize, nCnt) == true)
		{
			// 左から右の面に当たった
			isCollisionRightX(pPos, pPosOld, pMove, pSize, nCnt);

			// 右から左の面に当たった
			isCollisionLeftX(pPos, pPosOld, pMove, pSize, nCnt);
		}

		// Xの範囲に入った
		if (isCollisionX(pPos, pPosOld, pMove, pSize, nCnt) == true)
		{
			// 前からめり込んだ
			isCollisionFrontZ(pPos, pPosOld, pMove, pSize, nCnt);

			// 後ろからめり込んだ
			isCollisionBehindZ(pPos, pPosOld, pMove, pSize, nCnt); 
		}

		float MaxX = g_Barrier[nCnt].pos.x + g_Barrier[nCnt].Size.x * 0.5f;
		float MaxY = g_Barrier[nCnt].pos.y + g_Barrier[nCnt].Size.y * 0.5f;
		float MaxZ = g_Barrier[nCnt].pos.z + g_Barrier[nCnt].Size.z * 0.5f;

		float MinX = g_Barrier[nCnt].pos.x - g_Barrier[nCnt].Size.x * 0.5f;
		float MinY = g_Barrier[nCnt].pos.y - g_Barrier[nCnt].Size.y * 0.5f;
		float MinZ = g_Barrier[nCnt].pos.z - g_Barrier[nCnt].Size.z * 0.5f;

		SetEffectX(D3DXVECTOR3(MaxX, MaxY, MaxZ),D3DXVECTOR3(0.0f,0.0f,0.0f),COLOR_RED,10,100.0f,0.0f,0);
		SetEffectX(D3DXVECTOR3(MinX, MinY, MinZ), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLOR_RED, 10, 100.0f, 0.0f, 0);
		//SetEffectX(D3DXVECTOR3(0.0f, 0.0f, MaxZ), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLOR_RED, 10, 10.0f, 0.0f, 0);
		//SetEffectX(D3DXVECTOR3(MinX, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLOR_RED, 10, 10.0f, 0.0f, 0);
		//SetEffectX(D3DXVECTOR3(0.0f, MinY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLOR_RED, 10, 10.0f, 0.0f, 0);
		//SetEffectX(D3DXVECTOR3(0.0f, 0.0f, MinZ), D3DXVECTOR3(0.0f, 0.0f, 0.0f), COLOR_RED, 10, 10.0f, 0.0f, 0);
	}
}
//==============================================================================================================
// Zの範囲
//==============================================================================================================
bool isCollisionZ(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize,int nCntBarrier)
{
	// バリアの中心から最小までの範囲
	const float BarrierZMin = g_Barrier[nCntBarrier].pos.z - g_Barrier[nCntBarrier].Size.z * HALF_VALUE;

	// バリアの中心から最大までの範囲
	const float BarrierZMax = g_Barrier[nCntBarrier].pos.z + g_Barrier[nCntBarrier].Size.z * HALF_VALUE;

	// ブロックに当たる対象の位置から最小まで
	const float pPosZMin = pPos->z - pSize->z * HALF_VALUE;

	// ブロックに当たる対象の位置から最大まで
	const float pPosZMax = pPos->z + pSize->z * HALF_VALUE;
	
	// Zの範囲に入っている
	const bool InRangeZ = (pPosZMin < BarrierZMax) && (pPosZMax > BarrierZMin);

	// Zの範囲に入っている
	if (InRangeZ == true) return true;
		
	// 当たっていない
	return false;
}
//==============================================================================================================
// Xの面に左から右に当たった
//==============================================================================================================
bool isCollisionRightX(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier)
{
	// 当たる対象の前のフレームの位置
	const float pPosOldMaxX = pPosOld->x + pSize->x * HALF_VALUE;

	// 当たる対象の位置
	const float pPosMaxX = pPos->x + pSize->x * HALF_VALUE;

	// バリアの最小の範囲
	const float BarrierMinX = g_Barrier[nCntBarrier].pos.x - g_Barrier[nCntBarrier].Size.x * HALF_VALUE;

	// 当たっているなら
	const bool isCollision = (pPosOldMaxX < BarrierMinX) && (pPosMaxX > BarrierMinX);

	// xが左から右にめり込んだ	
	if (isCollision == true)
	{
		// 押し戻す
		pPos->x = g_Barrier[nCntBarrier].pos.x - g_Barrier[nCntBarrier].Size.x * HALF_VALUE - pSize->x * HALF_VALUE - 0.1f;
		return true;
	}
	return false;
}
//==============================================================================================================
// Xの面に右から左に当たった
//==============================================================================================================
bool isCollisionLeftX(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier)
{
	// 当たる対象の前のフレームの位置
	const float pPosOldMinX = pPosOld->x - pSize->x * HALF_VALUE;

	// 当たる対象の位置
	const float pPosMinX = pPos->x - pSize->x * HALF_VALUE;

	// バリアの最小の範囲
	const float BarrierMaxX = g_Barrier[nCntBarrier].pos.x + g_Barrier[nCntBarrier].Size.x * HALF_VALUE;

	// 当たっているなら
	const bool isCollision = (pPosOldMinX > BarrierMaxX) && (pPosMinX < BarrierMaxX);

	// xが右から左にめり込んだ	
	if (isCollision == true)
	{
		// 押し戻す
		pPos->x = g_Barrier[nCntBarrier].pos.x + g_Barrier[nCntBarrier].Size.x * HALF_VALUE + pSize->x * HALF_VALUE + 0.1f;
		return true;
	}
	return false;
}
//==============================================================================================================
// Xの範囲
//==============================================================================================================
bool isCollisionX(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier)
{
	// バリアの中心から最小までの範囲
	const float BarrierXMin = g_Barrier[nCntBarrier].pos.x - g_Barrier[nCntBarrier].Size.x * HALF_VALUE;

	// バリアの中心から最大までの範囲
	const float BarrierXMax = g_Barrier[nCntBarrier].pos.x + g_Barrier[nCntBarrier].Size.x * HALF_VALUE;

	// ブロックに当たる対象の位置から最小まで
	const float pPosXMin = pPos->x - pSize->x * HALF_VALUE;

	// ブロックに当たる対象の位置から最大まで
	const float pPosXMax = pPos->x + pSize->x * HALF_VALUE;

	// Zの範囲に入っている
	const bool InRangeX = (pPosXMin < BarrierXMax) && (pPosXMax > BarrierXMin);

	// Xの範囲に入った
	if (InRangeX == true)
	{
		// 範囲に入っている
		return true;
	}

	// 範囲外
	return false;
}
//==============================================================================================================
// 前面に当たった
//==============================================================================================================
bool isCollisionFrontZ(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier)
{
	// 当たる対象の前のフレームの位置
	const float pPosOldMaxZ = pPosOld->z + pSize->z * HALF_VALUE;

	// 当たる対象の位置
	const float pPosMaxZ = pPos->z + pSize->z * HALF_VALUE;

	// バリアの最小の範囲
	const float BarrierMinZ = g_Barrier[nCntBarrier].pos.z - g_Barrier[nCntBarrier].Size.z * HALF_VALUE;

	// 当たっているなら
	const bool isCollisionZ = (pPosOldMaxZ < BarrierMinZ) && (pPosMaxZ > BarrierMinZ);

	// 当たっている
	if(isCollisionZ == true)
	{
		// 押し戻す
		pPos->z = g_Barrier[nCntBarrier].pos.z - g_Barrier[nCntBarrier].Size.z * HALF_VALUE - pSize->z * HALF_VALUE - 0.1f;
		return true;
	}
	return false;
}
//==============================================================================================================
// 背面に当たった
//==============================================================================================================
bool isCollisionBehindZ(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize, int nCntBarrier)
{
	// 当たる対象の前のフレームの位置
	const float pPosOldMinZ = pPosOld->z - pSize->z * HALF_VALUE;

	// 当たる対象の位置
	const float pPosMinZ = pPos->z - pSize->z * HALF_VALUE;

	// バリアの最小の範囲
	const float BarrierMaxZ = g_Barrier[nCntBarrier].pos.z + g_Barrier[nCntBarrier].Size.z * HALF_VALUE;

	// 当たっているなら
	const bool isCollisionZ = (pPosOldMinZ > BarrierMaxZ) && (pPosMinZ < BarrierMaxZ);

	// 当たっている
	if (isCollisionZ == true)
	{
		// 押し戻す
		pPos->z = g_Barrier[nCntBarrier].pos.z + g_Barrier[nCntBarrier].Size.z * HALF_VALUE + pSize->z * HALF_VALUE + HALF_VALUE;
		return true;
	}
	return false;
}
