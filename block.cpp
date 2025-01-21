//============================
//
// ブロック[block.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//インクルードファイル
//****************************
#include "block.h"
#include "input.h"
#include "mouse.h"
#include "camera.h"

//****************************
//マクロ定義
//****************************
#define MOUSE_SIZE (50.0f)

//****************************
//プロトタイプ宣言
//****************************
void SetMtxBlock(int nCnt);

//****************************
//グローバル変数宣言
//****************************
BLOCK g_Block[MAX_BLOCK];
BLOCK g_TexBlock[BLOCKTYPE_MAX];
int nCounterStateBlock;
int g_NumBlock;

//=============================
//ブロックの初期化処理
//=============================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_Block[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Block[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Block[nCntBlock].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Block[nCntBlock].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Block[nCntBlock].nType = BLOCKTYPE_ONE;
		g_Block[nCntBlock].bUse = false;
		g_Block[nCntBlock].nLife = 20;
		g_Block[nCntBlock].state = BLOCKSTATE_NORMAL;
		g_Block[nCntBlock].fRadius = 100.0f;
		g_Block[nCntBlock].nLife = 180;
	}
	g_NumBlock = 0;
	nCounterStateBlock = 0;

	//g_Block.vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//g_Block.vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int nCntNum = 0; nCntNum < BLOCKTYPE_MAX; nCntNum++)
	{
		//Xファイルの読み込み
		D3DXLoadMeshFromX(MODELTYPE[nCntNum],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_TexBlock[nCntNum].BlockTex[nCntNum].g_pBuffMatBlock,
			NULL,
			&g_TexBlock[nCntNum].BlockTex[nCntNum].g_dwNumMatBlock,
			&g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock);
	}

	for (int nCntNum = 0; nCntNum < BLOCKTYPE_MAX; nCntNum++)
	{
		//g_TexBlock[nCntNum].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//g_TexBlock[nCntNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//g_TexBlock[nCntNum].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_TexBlock[nCntNum].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_TexBlock[nCntNum].bUse = true;
		g_TexBlock[nCntNum].nLife = 120;

		D3DXMATERIAL* pMat;//マテリアルへのポインタ

		//マテリアルのデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_TexBlock[nCntNum].BlockTex[nCntNum].g_pBuffMatBlock->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_TexBlock[nCntNum].BlockTex[nCntNum].g_dwNumMatBlock; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//このファイル名を使用してテクスチャを読み込む
					//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_TexBlock[nCntNum].BlockTex[nCntNum].g_apTextureBlock[nCntMat]);
			}
		}
	}

	//頂点座標の取得
	int nNumVtx;//頂点数
	DWORD sizeFVF;//頂点フォーマットのサイズ
	BYTE* pVtxBuff;//頂点バッファへのポインタ

	for (int nCntNum = 0; nCntNum < BLOCKTYPE_MAX; nCntNum++)
	{
		//頂点数の取得
		nNumVtx = g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock->GetNumVertices();

		//頂点フォーマットのサイズ取得
		sizeFVF = D3DXGetFVFVertexSize(g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock->GetFVF());

		//頂点バッファのロック
		g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//頂点座標を比較してブロックの最小値,最大値を取得
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

			//頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;

			g_TexBlock[nCntNum].Size.x = g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.x - g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.x;
			g_TexBlock[nCntNum].Size.y = g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.y - g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.y;
			g_TexBlock[nCntNum].Size.z = g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMax.z - g_TexBlock[nCntNum].BlockTex[nCntNum].vtxMin.z;
		}


		//頂点バッファのアンロック
		g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock->UnlockVertexBuffer();
	}
}
//=============================
//ブロックの終了処理
//=============================
void UninitBlock(void)
{
	for (int nCntNum = 0; nCntNum < BLOCKTYPE_MAX; nCntNum++)
	{
		for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
		{
			if (g_TexBlock[nCntNum].BlockTex[nCntNum].g_apTextureBlock[nCntTex] != NULL)
			{
				g_TexBlock[nCntNum].BlockTex[nCntNum].g_apTextureBlock[nCntTex]->Release();
				g_TexBlock[nCntNum].BlockTex[nCntNum].g_apTextureBlock[nCntTex] = NULL;
			}
		}

		//メッシュの破棄
		if (g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock != NULL)
		{
			g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock->Release();
			g_TexBlock[nCntNum].BlockTex[nCntNum].g_pMeshBlock = NULL;
		}

		//マテリアル
		if (g_TexBlock[nCntNum].BlockTex[nCntNum].g_pBuffMatBlock != NULL)
		{
			g_TexBlock[nCntNum].BlockTex[nCntNum].g_pBuffMatBlock->Release();
			g_TexBlock[nCntNum].BlockTex[nCntNum].g_pBuffMatBlock = NULL;
		}
	}
}
//=============================
//ブロックの更新処理
//=============================
void UpdateBlock(void)
{
	Player* pPlayer = GetPlayer();

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_Block[nCntBlock].bUse)
		{
			//使用中じゃなかったら
			continue;
		}

		if (g_Block[nCntBlock].state == BLOCKSTATE_HOLD && KeyboardTrigger(DIK_RETURN))
		{
			//プレイヤーの向いている方向に飛ばす
			g_Block[nCntBlock].move.x = sinf(pPlayer->rot.y + D3DX_PI) * 10.0f;
			g_Block[nCntBlock].move.y = 0.0f;
			g_Block[nCntBlock].move.z = cosf(pPlayer->rot.y + D3DX_PI) * 10.0f;

			//発射地点を親の位置にする
			g_Block[nCntBlock].pos.x = pPlayer->Motion.aModel[INDXPARENT].mtxWorld._41;
			g_Block[nCntBlock].pos.y = pPlayer->Motion.aModel[INDXPARENT].mtxWorld._42;
			g_Block[nCntBlock].pos.z = pPlayer->Motion.aModel[INDXPARENT].mtxWorld._43;

			//ブロックの投げられている状態にする
			g_Block[nCntBlock].state = BLOCKSTATE_THROW;

			//プレイヤーを何も持っていない状態にする
			pPlayer->HandState = PLAYERHOLD_NO;
		}
		if (g_Block[nCntBlock].state == BLOCKSTATE_HOLD && KeyboardTrigger(DIK_O))
		{
			g_Block[nCntBlock].state = BLOCKSTATE_NORMAL;
			pPlayer->HandState = PLAYERHOLD_NO;

			//発射地点を親の位置にする
			g_Block[nCntBlock].pos.x = pPlayer->pos.x;
			g_Block[nCntBlock].pos.y = 0.0f;
			g_Block[nCntBlock].pos.z = pPlayer->pos.z;

		}

		if (g_Block[nCntBlock].state == BLOCKSTATE_THROW)
		{
			g_Block[nCntBlock].nLife--;

			if (g_Block[nCntBlock].nLife <= 0)
			{
				g_Block[nCntBlock].bUse = false;
			}
		}
		//位置の更新
		g_Block[nCntBlock].pos += g_Block[nCntBlock].move;
	}

}
//=============================
//ブロックの描画処理
//=============================
void DrawBlock(void)
{
	MODE mode = GetMode();
	Player* pPlayer = GetPlayer();

	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal,mtxParent;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	for (int nCntNum = 0; nCntNum < BLOCKTYPE_MAX; nCntNum++)
	{
		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
		{
			if (!g_Block[nCntBlock].bUse)
			{
				continue;
			}

			if (g_Block[nCntBlock].state == BLOCKSTATE_HOLD)
			{
				SetMtxBlock(nCntBlock);
			}
			else
			{
				//ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_Block[nCntBlock].mtxWorldBlock);

				//大きさを反映
				D3DXMatrixScaling(&mtxScal, g_Block[nCntBlock].Scal.y, g_Block[nCntBlock].Scal.x, g_Block[nCntBlock].Scal.z);
				D3DXMatrixMultiply(&g_Block[nCntBlock].mtxWorldBlock, &g_Block[nCntBlock].mtxWorldBlock, &mtxScal);

				//向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Block[nCntBlock].rot.y, g_Block[nCntBlock].rot.x, g_Block[nCntBlock].rot.z);
				D3DXMatrixMultiply(&g_Block[nCntBlock].mtxWorldBlock, &g_Block[nCntBlock].mtxWorldBlock, &mtxRot);

				//位置を反映
				D3DXMatrixTranslation(&mtxTrans, g_Block[nCntBlock].pos.x, g_Block[nCntBlock].pos.y, g_Block[nCntBlock].pos.z);
				D3DXMatrixMultiply(&g_Block[nCntBlock].mtxWorldBlock, &g_Block[nCntBlock].mtxWorldBlock, &mtxTrans);

				//ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_Block[nCntBlock].mtxWorldBlock);
			}

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			for (int nCntMat = 0; nCntMat < (int)g_Block[nCntBlock].BlockTex[nCntNum].g_dwNumMatBlock; nCntMat++)
			{
				D3DXMATERIAL color;

				//マテリアルのデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_Block[nCntBlock].BlockTex[nCntNum].g_pBuffMatBlock->GetBufferPointer();

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_Block[nCntBlock].BlockTex[nCntNum].g_apTextureBlock[nCntMat]);

				//ブロック(パーツ)の描画
				g_Block[nCntBlock].BlockTex[nCntNum].g_pMeshBlock->DrawSubset(nCntMat);
			}
		}
	}
}
//=======================
//ブロックの設定処理
//=======================
void SetBlock(D3DXVECTOR3 pos, int nType, D3DXVECTOR3 Scal)
{
	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_Block[nCntBlock].bUse)
		{
			g_Block[nCntBlock] = g_TexBlock[nType];

			g_Block[nCntBlock].pos = pos;
			g_Block[nCntBlock].Scal = Scal;
			g_Block[nCntBlock].nType = nType;
			g_Block[nCntBlock].bUse = true;

			g_NumBlock++;
			break;
		}
	}
}
//=======================
//ブロックの判定処理
//=======================
bool CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pSize)
{
	bool bLanding = false;
	Player* pPlayer = GetPlayer();

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_Block[nCntBlock].bUse)
		{
			continue;
		}

		if (pPosOld->y <= g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y
			&& pPosOld->y + pSize->y >= g_Block[nCntBlock].pos.y)
		{
			//左右のめり込み判定
			if (pPos->z - pSize->z * 0.5f < g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z
				&& pPos->z + pSize->z * 0.5f > g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z)
			{
				//xが左から右にめり込んだ	
				if (pPosOld->x + pSize->x * 0.5f < g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.y
					&& pPos->x + pSize->x * 0.5f > g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.y)
				{
					pPos->x = g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.x - pSize->x * 0.5f - 0.1f;

					//岩を持つ処理
					if (KeyboardTrigger(DIK_L)&& pPlayer->HandState != PLAYERHOLD_HOLD)
					{
						g_Block[nCntBlock].state = BLOCKSTATE_HOLD;
						g_Block[nCntBlock].pos = pPlayer->Motion.aModel[INDXPARENT].pos;
						SetMtxBlock(nCntBlock);
						pPlayer->HandState = PLAYERHOLD_HOLD;
					}
				}
				//xが右から左にめり込んだ	
				else if (pPosOld->x - pSize->x * 0.5f > g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.x
					&& pPos->x - pSize->x * 0.5f < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.x)
				{
					pPos->x = g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.x + pSize->x * 0.5f + 0.1f;
					//岩を持つ処理
					if (KeyboardTrigger(DIK_L) && pPlayer->HandState != PLAYERHOLD_HOLD)
					{
						g_Block[nCntBlock].state = BLOCKSTATE_HOLD;
						g_Block[nCntBlock].pos = pPlayer->Motion.aModel[INDXPARENT].pos;
						SetMtxBlock(nCntBlock);
						pPlayer->HandState = PLAYERHOLD_HOLD;
					}
				}
			}

			//前と後ろの判定
			if (pPos->x - pSize->x * 0.5f < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.x
				&& pPos->x + pSize->x * 0.5f > g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.x)
			{
				//zが前方からめり込んだ
				if (pPosOld->z + pSize->z * 0.5f < g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z
					&& pPos->z + pSize->z * 0.5f > g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z)
				{
					pPos->z = g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z - pSize->z * 0.5f - 0.1f;

					//岩を持つ処理
					if (KeyboardTrigger(DIK_L) && pPlayer->HandState != PLAYERHOLD_HOLD)
					{
						g_Block[nCntBlock].state = BLOCKSTATE_HOLD;
						g_Block[nCntBlock].pos = pPlayer->Motion.aModel[INDXPARENT].pos;
						SetMtxBlock(nCntBlock);
						pPlayer->HandState = PLAYERHOLD_HOLD;
					}
				}
				//zが後方からめり込んだ
				else if (pPosOld->z - pSize->z * 0.5f > g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z
					&& pPos->z - pSize->z * 0.5f < g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z)
				{
					pPos->z = g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z + pSize->z * 0.5f + 0.5f;

					//岩を持つ処理
					if (KeyboardTrigger(DIK_L) && pPlayer->HandState != PLAYERHOLD_HOLD)
					{
						g_Block[nCntBlock].state = BLOCKSTATE_HOLD;
						g_Block[nCntBlock].pos = pPlayer->Motion.aModel[INDXPARENT].pos;
						SetMtxBlock(nCntBlock);
						pPlayer->HandState = PLAYERHOLD_HOLD;
					}
				}
			}
		}

		if (pPos->x - pSize->x * 0.5f <= g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.x
			&& pPos->x + pSize->x * 0.5f >= g_Block[nCntBlock].pos.x - g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.x)
		{
			if (pPos->z - pSize->z * 0.5f <= g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z
				&& pPos->z + pSize->z * 0.5f >= g_Block[nCntBlock].pos.z - g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z)
			{
				//上から下
				if (pPosOld->y >= g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y
					&& pPos->y < g_Block[nCntBlock].pos.y + g_Block[nCntBlock].Size.y * g_Block[nCntBlock].Scal.y)
				{
 					bLanding = true;
					pPos->y = pPosOld->y;
					pMove->y = 0.0f;
				}
				//下から上
				else if (pPosOld->y + pSize->y * 0.5f <= g_Block[nCntBlock].pos.y - g_Block[nCntBlock].Size.y * 0.5f * g_Block[nCntBlock].Scal.y
					&& pPos->y + pSize->y * 0.5f > g_Block[nCntBlock].pos.y - g_Block[nCntBlock].Size.y * 0.5f * g_Block[nCntBlock].Scal.y)
				{
					pPos->y = pPosOld->y;
					pMove->y = 0.0f;
				}

			}
		}		
	}

	return bLanding;
}
//=======================
//ブロックと敵の判定
//=======================
bool CollisionEnemy(D3DXVECTOR3* pPos, float BlockRadius, float EnemyRadius)
{
	bool bHit = false; // 判定を返す変数

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		float fDistanceX = g_Block[nCnt].pos.x - pPos->x; // 距離Xを算出
		float fDistanceY = g_Block[nCnt].pos.y - pPos->y; // 距離Yを算出
		float fDistanceZ = g_Block[nCnt].pos.z - pPos->z; // 距離Zを算出

		float fDistance = (fDistanceX * fDistanceX) + (fDistanceY * fDistanceY) + (fDistanceZ * fDistanceZ);

		float Radius = BlockRadius + EnemyRadius;

		Radius = Radius * Radius;

		if (fDistance <= Radius && g_Block[nCnt].state == BLOCKSTATE_THROW)
		{
			bHit = true;
			g_Block[nCnt].bUse = false;
		}
	}

	return bHit;
}
//=======================
//ブロックの取得処理
//=======================
BLOCK* GetBlock()
{
	return &g_Block[0];
}
int NumBlock(void)
{
	return g_NumBlock;
}
//=======================
//ブロックの親処理
//=======================
void SetMtxBlock(int nCnt)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal, mtxParent;
	Player* pPlayer = GetPlayer();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Block[nCnt].mtxWorldBlock);

	//大きさを反映
	D3DXMatrixScaling(&mtxScal, g_Block[nCnt].Scal.y, g_Block[nCnt].Scal.x, g_Block[nCnt].Scal.z);
	D3DXMatrixMultiply(&g_Block[nCnt].mtxWorldBlock, &g_Block[nCnt].mtxWorldBlock, &mtxScal);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Block[nCnt].rot.y, g_Block[nCnt].rot.x, g_Block[nCnt].rot.z);
	D3DXMatrixMultiply(&g_Block[nCnt].mtxWorldBlock, &g_Block[nCnt].mtxWorldBlock, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Block[nCnt].pos.x, g_Block[nCnt].pos.y, g_Block[nCnt].pos.z);
	D3DXMatrixMultiply(&g_Block[nCnt].mtxWorldBlock, &g_Block[nCnt].mtxWorldBlock, &mtxTrans);

	g_Block[nCnt].mtxParent = pPlayer->Motion.aModel[INDXPARENT].mtxWorld;

	//算出した[パーツのワールドマトリックス]と[親のマトリックス]をかけあわせる
	D3DXMatrixMultiply(&g_Block[nCnt].mtxWorldBlock,
		&g_Block[nCnt].mtxWorldBlock,
		&g_Block[nCnt].mtxParent);//自分自分親

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Block[nCnt].mtxWorldBlock);
}
