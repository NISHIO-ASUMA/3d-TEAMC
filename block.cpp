//============================
//
// ブロック[block.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
// インクルードファイル
//****************************
#include "block.h"
#include "input.h"
#include "mouse.h"
#include "camera.h"

//****************************
// マクロ定義
//****************************
#define MAX_WORD (256)	  // 最大の文字数
#define HALF_VALUE (0.6f) // 割る数

//****************************
// プロトタイプ宣言
//****************************
void LoadBlockModel(void); // モデル読み込み処理
void SetMtx(int nCntBlock); // ワールドマトリックスの設定(中心pos)
void PushPlayer(int nCntBlock); // OBBの押し出し

//****************************
// グローバル変数宣言
//****************************
BLOCK g_Block[MAX_BLOCK];		// 構造体変数
BLOCK g_TexBlock[BLOCKTYPE_MAX];// ブロックのテクスチャ情報
int nCounterStateBlock;			// 状態管理カウンター
int g_NumBlock;					// ブロックの配置した数
int g_BlockTypeMax;				// 種類数

//=============================
// ブロックの初期化処理
//=============================
void InitBlock(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_Block[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 座標
		g_Block[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 移動量
		g_Block[nCntBlock].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 角度
		g_Block[nCntBlock].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f); // 拡大率
		g_Block[nCntBlock].nType = BLOCKTYPE_APARTMENT;			 // 種類
		g_Block[nCntBlock].bUse = false;						 // 未使用状態
		g_Block[nCntBlock].nLife = 20;							 // 体力
		g_Block[nCntBlock].state = BLOCKSTATE_NORMAL;			 // 状態
		g_Block[nCntBlock].fRadius = 100.0f;					 // 半径
	}

	LoadBlockModel(); // ブロックのロード

	//グローバル変数の初期化
	g_NumBlock = 0;		   // ブロックの数
	nCounterStateBlock = 0;// 状態カウンター

	for (int nCntNum = 0; nCntNum < g_BlockTypeMax; nCntNum++)
	{
		D3DXMATERIAL* pMat; // マテリアルへのポインタ

		// マテリアルのデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_TexBlock[nCntNum].BlockTex[nCntNum].g_pBuffMatBlock->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_TexBlock[nCntNum].BlockTex[nCntNum].g_dwNumMatBlock; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_TexBlock[nCntNum].BlockTex[nCntNum].g_apTextureBlock[nCntMat]);
			}
		}
	}

	// 頂点座標の取得
	int nNumVtx;	// 頂点数
	DWORD sizeFVF;  // 頂点フォーマットのサイズ
	BYTE* pVtxBuff; // 頂点バッファへのポインタ
	DWORD normal;

	for (int nCntNum = 0; nCntNum < g_BlockTypeMax; nCntNum++)
	{
		// 頂点数の取得
		nNumVtx = g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock->GetNumVertices();

		// 頂点フォーマットのサイズ取得
		sizeFVF = D3DXGetFVFVertexSize(g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock->GetFVF());
		
		// 頂点バッファのロック
		g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			// 頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			// 頂点座標を比較してブロックの最小値,最大値を取得
			if (vtx.x < g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.x)
			{
				g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.x = vtx.x;
			}
			else if (vtx.y < g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.y)
			{
				g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.y = vtx.y;
			}
			else if (vtx.z < g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.z)
			{
				g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.z = vtx.z;
			}
			else if (vtx.x > g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.x)
			{
				g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.x = vtx.x;
			}
			else if (vtx.y > g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.y)
			{
				g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.y = vtx.y;
			}
			else if (vtx.z > g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.z)
			{
				g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.z = vtx.z;
			}

			// 頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;
		}

		// サイズに代入
		g_TexBlock[nCntNum].Size.x = g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.x - g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.x;
		g_TexBlock[nCntNum].Size.y = g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.y - g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.y;
		g_TexBlock[nCntNum].Size.z = g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.z - g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.z;

		// 頂点バッファのアンロック
		g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock->UnlockVertexBuffer();
	}
}
//=============================
// ブロックの終了処理
//=============================
void UninitBlock(void)
{
	for (int nCntNum = 0; nCntNum < g_BlockTypeMax; nCntNum++)
	{
		// テクスチャの破棄
		for (int nCntTex = 0; nCntTex < (int)g_TexBlock[nCntNum].BlockTex[nCntNum].g_dwNumMatBlock; nCntTex++)
		{
			if (g_TexBlock[nCntNum].BlockTex[nCntNum].g_apTextureBlock[nCntTex] != NULL)
			{
				g_TexBlock[nCntNum].BlockTex[nCntNum].g_apTextureBlock[nCntTex]->Release();
				g_TexBlock[nCntNum].BlockTex[nCntNum].g_apTextureBlock[nCntTex] = NULL;
			}
		}

		// メッシュの破棄
		if (g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock != NULL)
		{
			g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock->Release();
			g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock = NULL;
		}

		// マテリアルの破棄
		if (g_TexBlock[nCntNum].BlockTex[nCntNum].g_pBuffMatBlock != NULL)
		{
			g_TexBlock[nCntNum].BlockTex[nCntNum].g_pBuffMatBlock->Release();
			g_TexBlock[nCntNum].BlockTex[nCntNum].g_pBuffMatBlock = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		for (int nCntNum = 0; nCntNum < g_BlockTypeMax; nCntNum++)
		{
			// テクスチャの破棄
			for (int nCntTex = 0; nCntTex < (int)g_Block[nCnt].BlockTex[nCntNum].g_dwNumMatBlock; nCntTex++)
			{
				if (g_Block[nCnt].BlockTex[nCntNum].g_apTextureBlock[nCntTex] != NULL)
				{
					g_Block[nCnt].BlockTex[nCntNum].g_apTextureBlock[nCntTex] = NULL;
				}
			}

			// メッシュの破棄
			if (g_Block[nCnt].BlockTex[nCntNum].g_pMeshBlock != NULL)
			{
				g_Block[nCnt].BlockTex[nCntNum].g_pMeshBlock = NULL;
			}

			// マテリアルの破棄
			if (g_Block[nCnt].BlockTex[nCntNum].g_pBuffMatBlock != NULL)
			{
				g_Block[nCnt].BlockTex[nCntNum].g_pBuffMatBlock = NULL;
			}
		}
	}

}
//=============================
// ブロックの更新処理
//=============================
void UpdateBlock(void)
{
	Player* pPlayer = GetPlayer(); // プレイヤーの取得

	// 全部ブロック分回す
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		// ブロックが使われていない
		if (!g_Block[nCntBlock].bUse)
		{
			// 処理を読み飛ばす
			continue;
		}

		CreateObb(nCntBlock);

		// OBBの判定(未完成)
		if (collisionObb(nCntBlock))
		{
			PushPlayer(nCntBlock);
		}	
	}
}
//=============================
// ブロックの描画処理
//=============================
void DrawBlock(void)
{
	// デバイスのポインタを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 現在のモードを取得
	MODE mode = GetMode();

	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans, mtxScal,mtxParent;

	D3DMATERIAL9 matDef; // 現在のマテリアル保存用

	D3DXMATERIAL* pMat;  // マテリアルデータへのポインタ

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_Block[nCntBlock].bUse)
		{//未使用だったら
			//読み飛ばしてカウントを進める
			continue;
		}

		int nType = g_Block[nCntBlock].nType;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Block[nCntBlock].mtxWorldBlock);

		// 大きさを反映
		D3DXMatrixScaling(&mtxScal, g_Block[nCntBlock].Scal.y, g_Block[nCntBlock].Scal.x, g_Block[nCntBlock].Scal.z);
		D3DXMatrixMultiply(&g_Block[nCntBlock].mtxWorldBlock, &g_Block[nCntBlock].mtxWorldBlock, &mtxScal);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Block[nCntBlock].rot.y, g_Block[nCntBlock].rot.x, g_Block[nCntBlock].rot.z);
		D3DXMatrixMultiply(&g_Block[nCntBlock].mtxWorldBlock, &g_Block[nCntBlock].mtxWorldBlock, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Block[nCntBlock].pos.x, g_Block[nCntBlock].pos.y, g_Block[nCntBlock].pos.z);
		D3DXMatrixMultiply(&g_Block[nCntBlock].mtxWorldBlock, &g_Block[nCntBlock].mtxWorldBlock, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Block[nCntBlock].mtxWorldBlock);
		
		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		for (int nCntMat = 0; nCntMat < (int)g_Block[nCntBlock].BlockTex[nType].g_dwNumMatBlock; nCntMat++)
		{
			// マテリアルのデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_Block[nCntBlock].BlockTex[nType].g_pBuffMatBlock->GetBufferPointer();

			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_Block[nCntBlock].BlockTex[nType].g_apTextureBlock[nCntMat]);

			// ブロック(パーツ)の描画
			g_Block[nCntBlock].BlockTex[nType].g_pMeshBlock->DrawSubset(nCntMat);
		}
		SetMtx(nCntBlock);
	}
	
}
//=======================
// ブロックの設定処理
//=======================
void SetBlock(D3DXVECTOR3 pos, int nType, D3DXVECTOR3 Scal)
{
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_Block[nCntBlock].bUse)
		{//未使用状態だったら
			g_Block[nCntBlock] = g_TexBlock[nType]; // テクスチャタイプ

			g_Block[nCntBlock].pos = pos;	  // 座標
			g_Block[nCntBlock].Scal = Scal;	  // 拡大率
			g_Block[nCntBlock].nType = nType; // 種類
			g_Block[nCntBlock].bUse = true;   // 使用状態

			g_NumBlock++; // インクリメント
			break;
		}
	}
}
//=======================
// ブロックの判定処理
//=======================
bool CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize)
{
	// 着地判定
	bool bLanding = false;

	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_Block[nCntBlock].bUse || g_Block[nCntBlock].nType == BLOCKTYPE_MANHOLE)
		{//未使用だったら
			//読み飛ばしてカウントを進める
			continue;
		}

		if (pPosOld->y <= g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y
			&& pPosOld->y + pSize->y >= g_Block[nCntBlock].pos.y)
		{
			// 左右のめり込み判定
			if (pPos->z - pSize->z * HALF_VALUE < g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z
				&& pPos->z + pSize->z * HALF_VALUE > g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z)
			{
				// xが左から右にめり込んだ	
				if (pPosOld->x + pSize->x * HALF_VALUE < g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.y
					&& pPos->x + pSize->x * HALF_VALUE > g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.y)
				{
					pPos->x = g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x - pSize->x * HALF_VALUE - 0.1f;
				}
				// xが右から左にめり込んだ	
				else if (pPosOld->x - pSize->x * HALF_VALUE > g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x
					&& pPos->x - pSize->x * HALF_VALUE < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x)
				{
					pPos->x = g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x + pSize->x * HALF_VALUE + 0.1f;
				}
			}

			// 前と後ろの判定
			if (pPos->x - pSize->x * HALF_VALUE < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x
				&& pPos->x + pSize->x * HALF_VALUE > g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x)
			{
				// zが前方からめり込んだ
				if (pPosOld->z + pSize->z * HALF_VALUE < g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z
					&& pPos->z + pSize->z * HALF_VALUE > g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z)
				{
					pPos->z = g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z - pSize->z * HALF_VALUE - 0.1f;
				}
				// zが後方からめり込んだ
				else if (pPosOld->z - pSize->z * HALF_VALUE > g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z
					&& pPos->z - pSize->z * HALF_VALUE < g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z)
				{
					pPos->z = g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z + pSize->z * HALF_VALUE + HALF_VALUE;
				}
			}
		}

		if (pPos->x - pSize->x * HALF_VALUE <= g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x
			&& pPos->x + pSize->x * HALF_VALUE >= g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x)
		{
			if (pPos->z - pSize->z * HALF_VALUE <= g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z
				&& pPos->z + pSize->z * HALF_VALUE >= g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z)
			{
				// 上から下
				if (pPosOld->y >= g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y
					&& pPos->y < g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y)
				{
 					bLanding = true;
					pPos->y = g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y - pSize->y;
					pMove->y = 0.0f;
				}
				// 下から上
				else if (pPosOld->y + pSize->y * HALF_VALUE <= g_Block[nCntBlock].pos.y - g_Block[nCntBlock].Size.y * HALF_VALUE * g_Block[nCntBlock].Scal.y
					&& pPos->y + pSize->y * HALF_VALUE > g_Block[nCntBlock].pos.y - g_Block[nCntBlock].Size.y * HALF_VALUE * g_Block[nCntBlock].Scal.y)
				{
					pPos->y = pPosOld->y;
					pMove->y = 0.0f;
				}

			}
		}		
	}

	return bLanding; // 着地判定を返す
}
//=================================
// ブロックとアイテムの当たり判定
//=================================
bool CollisionBlockItem(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize)
{
	// 当たったかどうか
	bool bHit = false;

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_Block[nCntBlock].bUse)
		{// 未使用だったら
			// 読み飛ばしてカウントを進める
			continue;
		}

		if (pPosOld->y <= g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y
			&& pPosOld->y + pSize->y >= g_Block[nCntBlock].pos.y)
		{
			// 左右のめり込み判定
			if (pPos->z - pSize->z * HALF_VALUE < g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z
				&& pPos->z + pSize->z * HALF_VALUE > g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z)
			{
				// xが左から右にめり込んだ	
				if (pPosOld->x + pSize->x * HALF_VALUE < g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.y
					&& pPos->x + pSize->x * HALF_VALUE > g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.y)
				{
					bHit = true;
					pPos->x = g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x - pSize->x * HALF_VALUE - 0.1f;
				}
				// xが右から左にめり込んだ	
				else if (pPosOld->x - pSize->x * HALF_VALUE > g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x
					&& pPos->x - pSize->x * HALF_VALUE < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x)
				{
					bHit = true;
					pPos->x = g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x + pSize->x * HALF_VALUE + 0.1f;
				}
			}

			// 前と後ろの判定
			if (pPos->x - pSize->x * HALF_VALUE < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x
				&& pPos->x + pSize->x * HALF_VALUE > g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x)
			{
				// zが前方からめり込んだ
				if (pPosOld->z + pSize->z * HALF_VALUE < g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z
					&& pPos->z + pSize->z * HALF_VALUE > g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z)
				{
					bHit = true;
					pPos->z = g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z - pSize->z * HALF_VALUE - 0.1f;
				}
				// zが後方からめり込んだ
				else if (pPosOld->z - pSize->z * HALF_VALUE > g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z
					&& pPos->z - pSize->z * HALF_VALUE < g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z)
				{
					bHit = true;
					pPos->z = g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z + pSize->z * HALF_VALUE + HALF_VALUE;
				}
			}
		}

		if (pPos->x - pSize->x * HALF_VALUE <= g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x
			&& pPos->x + pSize->x * HALF_VALUE >= g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * HALF_VALUE * g_Block[nCntBlock].Scal.x)
		{
			if (pPos->z - pSize->z * HALF_VALUE <= g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z
				&& pPos->z + pSize->z * HALF_VALUE >= g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * HALF_VALUE * g_Block[nCntBlock].Scal.z)
			{
				// 上から下
				if (pPosOld->y >= g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y
					&& pPos->y < g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y)
				{
					bHit = true;
					pPos->y = g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y - pSize->y;
					pMove->y = 0.0f;
				}
				// 下から上
				else if (pPosOld->y + pSize->y * HALF_VALUE <= g_Block[nCntBlock].pos.y - g_Block[nCntBlock].Size.y * HALF_VALUE * g_Block[nCntBlock].Scal.y
					&& pPos->y + pSize->y * HALF_VALUE > g_Block[nCntBlock].pos.y - g_Block[nCntBlock].Size.y * HALF_VALUE * g_Block[nCntBlock].Scal.y)
				{
					bHit = true;
					pPos->y = pPosOld->y;
					pMove->y = 0.0f;
				}

			}
		}
	}

	// 判定を返す
	return bHit;
}
//=======================
// ブロックの取得処理
//=======================
BLOCK* GetBlock()
{
	return &g_Block[0];
}
//=======================
// ブロックの数取得処理
//=======================
int NumBlock(void)
{
	return g_NumBlock;
}
//=============================
// ステージのロード処理
//=============================
void LoadTitleState(void)
{
	FILE* pFile; // ファイルポインタを宣言

	char Skip[3]; // イコールを読み飛ばす用

	// ローカル変数宣言--------------------
	D3DXVECTOR3 pos(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Scal(0.0f, 0.0f, 0.0f);
	int nType = 0;
	int nIdx = 0;

	//-------------------------------------

	// ファイルを開く
	pFile = fopen("data\\save.txt", "r");

	if (pFile != NULL)
	{
		while (1)
		{
			char aString[MAX_WORD];

			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "BLOCKSET") == 0)
			{// BLOCKSETを読み取った
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "BLOCKTYPE") == 0)
					{// BLOCKTYPEを読み取った
						fscanf(pFile, "%s", &Skip[0]);
						fscanf(pFile, "%d", &nType);
					}
					else if (strcmp(aString, "POS") == 0)
					{// POSを読み取った
						fscanf(pFile, "%s", &Skip[0]);
						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);
					}
					else if (strcmp(aString, "SIZE") == 0)
					{// SIZEを読みとった
						fscanf(pFile, "%s", &Skip[0]);
						fscanf(pFile, "%f", &Scal.x);
						fscanf(pFile, "%f", &Scal.y);
						fscanf(pFile, "%f", &Scal.z);
					}
					else if (strcmp(aString, "END_BLOCKSET") == 0)
					{// END_BLOCKSETを読み取った
						// ブロックにセットする
						SetBlock(pos, nType, Scal);
						nIdx++;
						break;
					}
				}
			}

		    if (strcmp(aString, "END_SCRIPT") == 0)
			{// END_SCRIPTを読み取った
				break;
			}
		}
	}
	else
	{//開けなかったとき	
		//メッセージボックス
		MessageBox(NULL, "ファイルが開けません。", "エラー(Edit.cpp)", MB_OK);
		return;
	}

	// ファイルを閉じる
	fclose(pFile);
}
//=============================
// ブロックのモデルのロード処理
//=============================
void LoadBlockModel(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// ファイルポインタを宣言
	FILE* pFile; 

	// ローカル変数宣言-----
	char skip[5];
	int nType = 0;
	//----------------------

	// ファイルを開く
	pFile = fopen("data\\MODEL_TXT\\BLOCK.txt", "r");

	if (pFile != NULL)
	{
		char aString[MAX_WORD];

		while (1)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "MAX_TYPE") == 0)
			{// MAX_TYPEを読み取った
				fscanf(pFile, "%s", &skip[0]);
				fscanf(pFile, "%d", &g_BlockTypeMax);
			}
			else if (strcmp(aString, "MODEL_FILENAME") == 0)
			{// MODEL_FILENAMEを読み取った
				fscanf(pFile, "%s", &skip[0]);

				fscanf(pFile, "%s", &aString[0]);

				// ファイル名を保存
				const char* MODEL_FILENAME = {};

				// 読み取ったパスを代入
				MODEL_FILENAME = aString;

				//Xファイルの読み込み
				D3DXLoadMeshFromX(MODEL_FILENAME,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_TexBlock[nType].BlockTex[nType].g_pBuffMatBlock,
					NULL,
					&g_TexBlock[nType].BlockTex[nType].g_dwNumMatBlock,
					&g_TexBlock[nType].BlockTex[nType].g_pMeshBlock);

				nType++;
			}
			else if (strcmp(aString, "END_SCRIPT") == 0)
			{// END_SCRIPTを読み取った
				break;
			}
		}
	}
	else
	{
		//メッセージボックス
		MessageBox(NULL, "ファイルが開けません。", "エラー(Block.cpp)", MB_OK);
		return;
	}

	// ファイルを閉じる
	fclose(pFile);
}
//=====================================
// チュートリアルのモデルのロード処理
//=====================================
void tutoload(void)
{
	// ファイルポインタを宣言
	FILE* pFile; 

	// ローカル変数宣言---------------------
	char Skip[3];
	D3DXVECTOR3 pos(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Scal(0.0f, 0.0f, 0.0f);
	int nType = 0;
	int nIdx = 0;
	//--------------------------------------

	// ファイルを開く
	pFile = fopen("data\\savetuto.txt", "r");

	if (pFile != NULL)
	{
		while (1)
		{
			// 文字列
			char aString[MAX_WORD];

			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "BLOCKSET") == 0)
			{// BLOCKSETを読み取った
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "BLOCKTYPE") == 0)
					{// BLOCKTYPEを読み取った
						fscanf(pFile, "%s", &Skip[0]);
						fscanf(pFile, "%d", &nType);
					}
					else if (strcmp(aString, "POS") == 0)
					{// POSを読み取った
						fscanf(pFile, "%s", &Skip[0]);
						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);
					}
					else if (strcmp(aString, "SIZE") == 0)
					{// SIZEを読み取った
						fscanf(pFile, "%s", &Skip[0]);
						fscanf(pFile, "%f", &Scal.x);
						fscanf(pFile, "%f", &Scal.y);
						fscanf(pFile, "%f", &Scal.z);
					}
					else if (strcmp(aString, "END_BLOCKSET") == 0)
					{// END_BLOCKSETを読み取った
						SetBlock(pos, nType, Scal);
						nIdx++;
						break;
					}
				}
			}

			if (strcmp(aString, "END_SCRIPT") == 0)
			{// END_SCRIPTを読み取った
				break;
			}
		}
	}
	else
	{//開けなかったとき	
		//メッセージボックス
		MessageBox(NULL, "ファイルが開けません。", "エラー(Edit.cpp)", MB_OK);
		return;
	}

	// ファイルを閉じる
	fclose(pFile);

}
//=======================
//ブロックOBBの作成
//=======================
void CreateObb(int nCnt)
{
	int nType = g_Block[nCnt].nType; // 種類を代入

	D3DXMATRIX mtxRot; // 計算用マトリックス

	// 位置を代入
	D3DXVECTOR3 pos(g_Block[nCnt].Obb.ObbMtx._41, g_Block[nCnt].Obb.ObbMtx._42, g_Block[nCnt].Obb.ObbMtx._43);

	// 位置
	g_Block[nCnt].Obb.CenterPos = pos;

	// 回転行列の設定
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Block[nCnt].rot.y, g_Block[nCnt].rot.x, g_Block[nCnt].rot.z);

	// 回転行列
	g_Block[nCnt].Obb.VecRot[0] = D3DXVECTOR3(mtxRot._11, mtxRot._12, mtxRot._13); // 回転行列X
	g_Block[nCnt].Obb.VecRot[1] = D3DXVECTOR3(mtxRot._21, mtxRot._22, mtxRot._23); // 回転行列Y
	g_Block[nCnt].Obb.VecRot[2] = D3DXVECTOR3(mtxRot._31, mtxRot._32, mtxRot._33); // 回転行列Z

	// 長さ取得
	g_Block[nCnt].Obb.Length[0] = fabsf((g_Block[nCnt].BlockTex[nType].vtxMax.x * g_Block[nCnt].Scal.x) - (g_Block[nCnt].BlockTex[nType].vtxMin.x * g_Block[nCnt].Scal.x)) * 0.7f; // 長さX
	g_Block[nCnt].Obb.Length[1] = fabsf((g_Block[nCnt].BlockTex[nType].vtxMax.y * g_Block[nCnt].Scal.y) - (g_Block[nCnt].BlockTex[nType].vtxMin.y * g_Block[nCnt].Scal.y)) * 0.7f; // 長さY
	g_Block[nCnt].Obb.Length[2] = fabsf((g_Block[nCnt].BlockTex[nType].vtxMax.z * g_Block[nCnt].Scal.z) - (g_Block[nCnt].BlockTex[nType].vtxMin.z * g_Block[nCnt].Scal.z)) * 0.6f; // 長さZ
}
//=======================
// OBBの判定
//=======================
bool collisionObb(int nCnt)
{
	Player* pPlayer = GetPlayer();
	D3DXMATRIX mtxRot; // 計算用マトリックス

	float PlayerLength[3];

	// OBBの回転
	D3DXVECTOR3 NAe1 = g_Block[nCnt].Obb.VecRot[0], Ae1 = NAe1 * g_Block[nCnt].Obb.Length[0];
	D3DXVECTOR3 NAe2 = g_Block[nCnt].Obb.VecRot[1], Ae2 = NAe2 * g_Block[nCnt].Obb.Length[1];
	D3DXVECTOR3 NAe3 = g_Block[nCnt].Obb.VecRot[2], Ae3 = NAe3 * g_Block[nCnt].Obb.Length[2];

	// 回転行列
	D3DXVECTOR3 Nbe1(0.0f,0.0f,0.0f);
	D3DXVECTOR3 Nbe2(0.0f,0.0f,0.0f);
	D3DXVECTOR3 Nbe3(0.0f,0.0f,0.0f);

	D3DXVECTOR3 Max(pPlayer->Motion.aModel[2].vtxMax.x, pPlayer->Motion.aModel[2].vtxMax.y, pPlayer->Motion.aModel[2].vtxMax.z);
	D3DXVECTOR3 Min(pPlayer->Motion.aModel[2].vtxMin.x, pPlayer->Motion.aModel[2].vtxMin.y, pPlayer->Motion.aModel[2].vtxMin.z);

	// Player
	PlayerLength[0] = fabsf(Max.x - Min.x);
	PlayerLength[1] = fabsf(Max.y - Min.y);
	PlayerLength[2] = fabsf(Max.z - Min.z);

	// Player
	D3DXVECTOR3 NBe1 = Nbe1 * PlayerLength[0];
	D3DXVECTOR3 NBe2 = Nbe2 * PlayerLength[1];
	D3DXVECTOR3 NBe3 = Nbe3 * PlayerLength[2];

	//モデル情報の代入
	D3DXVECTOR3 Model(pPlayer->Motion.aModel[0].mtxWorld._41, pPlayer->Motion.aModel[0].mtxWorld._42, pPlayer->Motion.aModel[0].mtxWorld._43);

	// 中心からプレイヤーの位置を求める
	D3DXVECTOR3 Interval = Model - g_Block[nCnt].Obb.CenterPos;

	// 分離軸を求める
	float VecL = fabsf(D3DXVec3Dot(&Interval, &NAe1));
	float rA = D3DXVec3Length(&Ae1);
	float rB = LenSegOnSeparateAxis(&NAe1, &NBe1, &NBe2, &NBe3);

	// 触れていない
	if (VecL > rA + rB)
	{
		return false;
	}

	// 分離軸 : Ae2
	rA = D3DXVec3Length(&Ae2);
	rB = LenSegOnSeparateAxis(&NAe2, &NBe1, &NBe2, &NBe3);
	VecL = fabs(D3DXVec3Dot(&Interval, &NAe2));
	if (VecL > rA + rB)
		return false;

	// 分離軸 : Ae3
	rA = D3DXVec3Length(&Ae3);
	rB = LenSegOnSeparateAxis(&NAe3, &NBe1, &NBe2, &NBe3);
	VecL = fabs(D3DXVec3Dot(&Interval, &NAe3));
	if (VecL > rA + rB)
		return false;

	// 分離軸 : Be1
	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&NBe1);
	VecL = fabs(D3DXVec3Dot(&Interval, &NBe1));
	if (VecL > rA + rB)
		return false;

	// 分離軸 : Be2
	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&NBe2);
	VecL = fabs(D3DXVec3Dot(&Interval, &NBe2));
	if (VecL > rA + rB)
		return false;

	// 分離軸 : Be3
	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&NBe3);
	VecL = fabs(D3DXVec3Dot(&Interval, &NBe3));
	if (VecL > rA + rB)
		return false;

	// 分離軸 : C11
	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross, &NAe1, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3, 0);
	rB = LenSegOnSeparateAxis(&Cross, &NBe2, &NBe3, 0);
	VecL = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (VecL > rA + rB)
		return false;

	// 分離軸 : C12
	D3DXVec3Cross(&Cross, &NAe1, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3, 0);
	rB = LenSegOnSeparateAxis(&Cross, &NBe1, &NBe3, 0);
	VecL = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (VecL > rA + rB)
		return false;

	// 分離軸 : C13
	D3DXVec3Cross(&Cross, &NAe1, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3, 0);
	rB = LenSegOnSeparateAxis(&Cross, &NBe1, &NBe2, 0);
	VecL = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (VecL > rA + rB)
		return false;

	// 分離軸 : C21
	D3DXVec3Cross(&Cross, &NAe2, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3, 0);
	rB = LenSegOnSeparateAxis(&Cross, &NBe2, &NBe3, 0);
	VecL = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (VecL > rA + rB)
		return false;

	// 分離軸 : C22
	D3DXVec3Cross(&Cross, &NAe2, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3, 0);
	rB = LenSegOnSeparateAxis(&Cross, &NBe1, &NBe3, 0);
	VecL = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (VecL > rA + rB)
		return false;

	// 分離軸 : C23
	D3DXVec3Cross(&Cross, &NAe2, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3, 0);
	rB = LenSegOnSeparateAxis(&Cross, &NBe1, &NBe2, 0);
	VecL = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (VecL > rA + rB)
		return false;

	// 分離軸 : C31
	D3DXVec3Cross(&Cross, &NAe3, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2, 0);
	rB = LenSegOnSeparateAxis(&Cross, &NBe2, &NBe3, 0);
	VecL = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (VecL > rA + rB)
		return false;

	// 分離軸 : C32
	D3DXVec3Cross(&Cross, &NAe3, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2, 0);
	rB = LenSegOnSeparateAxis(&Cross, &NBe1, &NBe3, 0);
	VecL = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (VecL > rA + rB)
		return false;

	// 分離軸 : C33
	D3DXVec3Cross(&Cross, &NAe3, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2, 0);
	rB = LenSegOnSeparateAxis(&Cross, &NBe1, &NBe2, 0);
	VecL = fabs(D3DXVec3Dot(&Interval, &Cross));
	if (VecL > rA + rB)
		return false;

	return true; // 当たっている
}
//=============================
// 超平面の計算?
//=============================
float LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3)
{
	// 3つの内積の絶対値の和で投影線分長を計算
	// 分離軸Sepは標準化されていること
	float r1 = fabs(D3DXVec3Dot(Sep, e1));
	float r2 = fabs(D3DXVec3Dot(Sep, e2));
	float r3 = e3 ? (fabs(D3DXVec3Dot(Sep, e3))) : 0;
	return r1 + r2 + r3;
}
//=======================
//ブロックのマトリックス
//=======================
void SetMtx(int nCntBlock)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal, mtxParent;
	Player* pPlayer = GetPlayer();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Block[nCntBlock].Obb.ObbMtx);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Block[nCntBlock].rot.y, g_Block[nCntBlock].rot.x, g_Block[nCntBlock].rot.z);
	D3DXMatrixMultiply(&g_Block[nCntBlock].Obb.ObbMtx, &g_Block[nCntBlock].Obb.ObbMtx, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, 0.0f, g_Block[nCntBlock].Size.y * 0.5f, 0.0f);
	D3DXMatrixMultiply(&g_Block[nCntBlock].Obb.ObbMtx, &g_Block[nCntBlock].Obb.ObbMtx, &mtxTrans);


	mtxParent = g_Block[nCntBlock].mtxWorldBlock;

	//算出した[パーツのワールドマトリックス]と[親のマトリックス]をかけあわせる
	D3DXMatrixMultiply(&g_Block[nCntBlock].Obb.ObbMtx,
		&g_Block[nCntBlock].Obb.ObbMtx,
		&mtxParent);//自分自分親

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Block[nCntBlock].Obb.ObbMtx);
}
//=========================
// OBBの押し出し処理
//=========================
void PushPlayer(int nCntBlock)
{
	Player* pPlayer = GetPlayer();

	int nType = g_Block[nCntBlock].nType;

	D3DXVec3Normalize(&g_Block[nCntBlock].Obb.VecRot[0], &g_Block[nCntBlock].Obb.VecRot[0]);
	D3DXVec3Normalize(&g_Block[nCntBlock].Obb.VecRot[1], &g_Block[nCntBlock].Obb.VecRot[1]);
	D3DXVec3Normalize(&g_Block[nCntBlock].Obb.VecRot[2], &g_Block[nCntBlock].Obb.VecRot[2]);

	D3DXVECTOR3 VecMoveF = pPlayer->pos - pPlayer->posOld; // 進行ベクトル
	float VecA = D3DXVec3Dot(&VecMoveF, &g_Block[nCntBlock].Obb.VecRot[0]);

	D3DXVECTOR3 WallMove = VecMoveF - VecA * g_Block[nCntBlock].Obb.VecRot[0];
	D3DXVec3Normalize(&WallMove, &WallMove);

	
	pPlayer->move.x += WallMove.x * pPlayer->speed;
	pPlayer->move.z += WallMove.z * pPlayer->speed;

}
