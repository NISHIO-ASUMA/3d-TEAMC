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
#define MAX_WORD (256)

//****************************
//プロトタイプ宣言
//****************************
void LoadBlockModel(void);

//****************************
//グローバル変数宣言
//****************************
BLOCK g_Block[MAX_BLOCK];
BLOCK g_TexBlock[BLOCKTYPE_MAX];
int nCounterStateBlock;
int g_NumBlock;
int g_BlockTypeMax;

//=============================
//ブロックの初期化処理
//=============================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_Block[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //座標
		g_Block[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
		g_Block[nCntBlock].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //角度
		g_Block[nCntBlock].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);//拡大率
		g_Block[nCntBlock].nType = BLOCKTYPE_APARTMENT;				//種類
		g_Block[nCntBlock].bUse = false;						//未使用状態
		g_Block[nCntBlock].nLife = 20;							//体力
		g_Block[nCntBlock].state = BLOCKSTATE_NORMAL;			//状態
		g_Block[nCntBlock].fRadius = 100.0f;					//半径
	}

	LoadBlockModel(); // ブロックのロード

	//グローバル変数の初期化
	g_NumBlock = 0;		   //ブロックの数
	nCounterStateBlock = 0;//状態カウンター

	for (int nCntNum = 0; nCntNum < g_BlockTypeMax; nCntNum++)
	{
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

	for (int nCntNum = 0; nCntNum < g_BlockTypeMax; nCntNum++)
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

			//サイズに代入
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
	for (int nCntNum = 0; nCntNum < g_BlockTypeMax; nCntNum++)
	{
		//テクスチャの破棄
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

		//マテリアルの破棄
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

}
//=============================
//ブロックの描画処理
//=============================
void DrawBlock(void)
{
	MODE mode = GetMode();//現在のモードを取得

	Player* pPlayer = GetPlayer();//プレイヤー取得

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタを取得

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal,mtxParent;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	for (int nCntNum = 0; nCntNum < g_BlockTypeMax; nCntNum++)
	{
		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
		{
			if (!g_Block[nCntBlock].bUse)
			{//未使用だったら
				//読み飛ばしてカウントを進める
				continue;
			}

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
		{//未使用状態だったら
			g_Block[nCntBlock] = g_TexBlock[nType];//テクスチャタイプ

			g_Block[nCntBlock].pos = pos;	 //座標
			g_Block[nCntBlock].Scal = Scal;	 //拡大率
			g_Block[nCntBlock].nType = nType;//種類
			g_Block[nCntBlock].bUse = true;  //使用状態

			g_NumBlock++;//インクリメント
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
		{//未使用だったら
			//読み飛ばしてカウントを進める
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
				}
				//xが右から左にめり込んだ	
				else if (pPosOld->x - pSize->x * 0.5f > g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.x
					&& pPos->x - pSize->x * 0.5f < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.x)
				{
					pPos->x = g_Block[nCntBlock].pos.x + g_Block[nCntBlock].Size.x * 0.5f * g_Block[nCntBlock].Scal.x + pSize->x * 0.5f + 0.1f;
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
				}
				//zが後方からめり込んだ
				else if (pPosOld->z - pSize->z * 0.5f > g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z
					&& pPos->z - pSize->z * 0.5f < g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z)
				{
					pPos->z = g_Block[nCntBlock].pos.z + g_Block[nCntBlock].Size.z * 0.5f * g_Block[nCntBlock].Scal.z + pSize->z * 0.5f + 0.5f;
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

	return bLanding;//着地判定を返す
}
//=======================
//ブロックの取得処理
//=======================
BLOCK* GetBlock()
{
	return &g_Block[0];
}
//=======================
//ブロックの数取得処理
//=======================
int NumBlock(void)
{
	return g_NumBlock;
}
//=============================
//ブロックのモデルのロード処理
//=============================
void LoadBlockModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	FILE* pFile; // ファイルのポインタ

	char skip[5];
	int nType = 0;

	pFile = fopen("data\\MODEL_TXT\\BLOCK.txt", "r");

	if (pFile != NULL)
	{
		char aString[MAX_WORD];

		while (1)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "MAX_TYPE") == 0)
			{
				fscanf(pFile, "%s", &skip[0]);
				fscanf(pFile, "%d", &g_BlockTypeMax);
			}
			else if (strcmp(aString, "MODEL_FILENAME") == 0)
			{
				fscanf(pFile, "%s", &skip[0]);

				fscanf(pFile, "%s", &aString[0]);

				const char* MODEL_FILENAME = {};

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
			{
				fclose(pFile);
				break;
			}
		}
	}
}
