//============================
//
// アイテム [item.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//インクルードファイル
//****************************
#include "input.h"
#include "mouse.h"
#include "camera.h"
#include "item.h"
#include "block.h"
#include "player.h"
#include "explosion.h"
#include "HPGauge.h"
#include "Shadow.h"
#include "Particle.h"

//****************************
//マクロ定義
//****************************
#define MAX_WORD (256)
#define MAX_DURABILITY (100) // 耐久力

//****************************
//プロトタイプ宣言
//****************************
void LoadItemModel(void); // アイテムのロード処理

//****************************
//グローバル変数宣言
//****************************
Item g_Item[MAX_ITEM];			// 構造体変数
int g_ItemTypeMax;				// 種類数
Item g_TexItem[ITEMTYPE_MAX];	// テクスチャ関係

//=============================
//ブロックの初期化処理
//=============================
void InitItem(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_Item[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 座標
		g_Item[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 移動量
		g_Item[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 角度
		g_Item[nCntItem].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f); // 拡大率
		g_Item[nCntItem].nType = ITEMTYPE_BAT;				   // 種類
		g_Item[nCntItem].bUse = false;						   // 未使用判定
		g_Item[nCntItem].nLife = 20;						   // 体力
		g_Item[nCntItem].state = ITEMSTATE_NORMAL;			   // 状態
		g_Item[nCntItem].fRadius = 100.0f;					   // 半径
		g_Item[nCntItem].nLife = 180;						   // 体力
		g_Item[nCntItem].durability = MAX_DURABILITY;		   // 耐久力
	}

	LoadItemModel(); // アイテムのロード処理

	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		//g_TexItem[nCntNum].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//g_TexItem[nCntNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//g_TexItem[nCntNum].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_TexItem[nCntNum].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);// 拡大率
		g_TexItem[nCntNum].nLife = 120;							// 体力
		g_TexItem[nCntNum].durability = MAX_DURABILITY;			// 耐久力
		g_TexItem[nCntNum].state = ITEMSTATE_NORMAL;			// 状態

		D3DXMATERIAL* pMat; // マテリアルへのポインタ

		// マテリアルのデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_TexItem[nCntNum].ItemTex[nCntNum].g_pBuffMatItem->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_TexItem[nCntNum].ItemTex[nCntNum].g_dwNumMatItem; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//このファイル名を使用してテクスチャを読み込む
				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_TexItem[nCntNum].ItemTex[nCntNum].g_apTextureItem[nCntMat]);
			}
		}
	}

	// 頂点座標の取得
	int nNumVtx;	// 頂点数
	DWORD sizeFVF;  // 頂点フォーマットのサイズ
	BYTE* pVtxBuff; // 頂点バッファへのポインタ

	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		// 頂点数の取得
		nNumVtx = g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem->GetNumVertices();

		// 頂点フォーマットのサイズ取得
		sizeFVF = D3DXGetFVFVertexSize(g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem->GetFVF());

		// 頂点バッファのロック
		g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			// 頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			// 頂点座標を比較してブロックの最小値,最大値を取得
			if (vtx.x < g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.x)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.x = vtx.x;
			}
			else if (vtx.y < g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.y)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.y = vtx.y;
			}
			else if (vtx.z < g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.z)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.z = vtx.z;
			}
			else if (vtx.x > g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.x)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.x = vtx.x;
			}
			else if (vtx.y > g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.y)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.y = vtx.y;
			}
			else if (vtx.z > g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.z)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.z = vtx.z;
			}

			// 頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;

			// サイズを代入
			g_TexItem[nCntNum].Size.x = g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.x - g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.x;
			g_TexItem[nCntNum].Size.y = g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.y - g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.y;
			g_TexItem[nCntNum].Size.z = g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.z - g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.z;
		}

		// 頂点バッファのアンロック
		g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem->UnlockVertexBuffer();
	}
}
//=============================
//ブロックの終了処理
//=============================
void UninitItem(void)
{
	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		// テクスチャの破棄
		for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
		{
			if (g_TexItem[nCntNum].ItemTex[nCntNum].g_apTextureItem[nCntTex] != NULL)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].g_apTextureItem[nCntTex]->Release();
				g_TexItem[nCntNum].ItemTex[nCntNum].g_apTextureItem[nCntTex] = NULL;
			}
		}

		// メッシュの破棄
		if (g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem != NULL)
		{
			g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem->Release();
			g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem = NULL;
		}

		// マテリアルの破棄
		if (g_TexItem[nCntNum].ItemTex[nCntNum].g_pBuffMatItem != NULL)
		{
			g_TexItem[nCntNum].ItemTex[nCntNum].g_pBuffMatItem->Release();
			g_TexItem[nCntNum].ItemTex[nCntNum].g_pBuffMatItem = NULL;
		}
	}
}
//=============================
//ブロックの更新処理
//=============================
void UpdateItem(void)
{
	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		// プレイヤーがものを持っているかつ攻撃モーションのキーが3になったら
		if (!pPlayer->AttackSp&&pPlayer->HandState == PLAYERHOLD_HOLD && pPlayer->Motion.nKey == 3 && pPlayer->Motion.motionType == MOTIONTYPE_ACTION)
		{
			ThrowItem();
			
		}
		
		if (!g_Item[nCntItem].bUse)
		{//使用中じゃなかったら
			// 処理を読み飛ばす
			continue;
		}

		if (g_Item[nCntItem].state == ITEMSTATE_THROW)
		{
			if (g_Item[nCntItem].nType == ITEMTYPE_HEX)
			{
				SetParticle(g_Item[nCntItem].pos,
					g_Item[nCntItem].rot,
					D3DXVECTOR3(1.0f, 1.0f, 1.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.5f, 0.0f, 0.5f, 1.0f),
					2.0f, 2, 20, 7, 3.0f, 10.0f,
					false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				SetParticle(g_Item[nCntItem].pos,
					g_Item[nCntItem].rot,
					D3DXVECTOR3(1.0f, 1.0f, 1.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
					2.0f, 2, 20, 3, 3.0f, 10.0f,
					false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
			else
			{
				SetParticle(g_Item[nCntItem].pos,
					g_Item[nCntItem].rot,
					D3DXVECTOR3(1.0f, 1.0f, 1.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f),
					2.0f, 2, 20, 7, 3.0f, 10.0f,
					false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
		}

		// 前回の位置を代入
		g_Item[nCntItem].posOld = g_Item[nCntItem].pos;

		// 位置の更新
		g_Item[nCntItem].pos += g_Item[nCntItem].move;

		if (CollisionBlockItem(&g_Item[nCntItem].pos, &g_Item[nCntItem].posOld, &g_Item[nCntItem].move, &g_Item[nCntItem].Size))
		{
			g_Item[nCntItem].bUse = false;
			SetExplosion(g_Item[nCntItem].pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 60, 30.0f, 30.0f, EXPLOSION_HIT);
		}

		CollisionItem(nCntItem,// アイテムのインデックスを渡す
			20.0f,  // アイテムの半径
			20.0f); // プレイヤーの半径

		// 状態を投げるにする
		if (g_Item[nCntItem].state == ITEMSTATE_THROW)
		{
			g_Item[nCntItem].nLife--; // デクリメント

			if (g_Item[nCntItem].nLife <= 0)
			{
				g_Item[nCntItem].bUse = false; // 消す
			}
		}

		if (g_Item[nCntItem].durability <= 0)
		{
			g_Item[nCntItem].bUse = false; // 消す
		}
	}

}
//=============================
//ブロックの描画処理
//=============================
void DrawItem(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 現在のモードを取得
	MODE mode = GetMode();

	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	// 計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal, mtxParent;

	D3DMATERIAL9 matDef; // 現在のマテリアル保存用

	D3DXMATERIAL* pMat; // マテリアルデータへのポインタ

	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
		{
			if (!g_Item[nCntItem].bUse)
			{//未使用状態なら
				//下の処理を通さずカウントを進める
				continue;
			}

			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Item[nCntItem].mtxWorldItem);

			// 大きさを反映
			D3DXMatrixScaling(&mtxScal, g_Item[nCntItem].Scal.y, g_Item[nCntItem].Scal.x, g_Item[nCntItem].Scal.z);
			D3DXMatrixMultiply(&g_Item[nCntItem].mtxWorldItem, &g_Item[nCntItem].mtxWorldItem, &mtxScal);

			// 向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Item[nCntItem].rot.y, g_Item[nCntItem].rot.x, g_Item[nCntItem].rot.z);
			D3DXMatrixMultiply(&g_Item[nCntItem].mtxWorldItem, &g_Item[nCntItem].mtxWorldItem, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y, g_Item[nCntItem].pos.z);
			D3DXMatrixMultiply(&g_Item[nCntItem].mtxWorldItem, &g_Item[nCntItem].mtxWorldItem, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Item[nCntItem].mtxWorldItem);
			
			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			for (int nCntMat = 0; nCntMat < (int)g_Item[nCntItem].ItemTex[nCntNum].g_dwNumMatItem; nCntMat++)
			{
				// マテリアルのデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_Item[nCntItem].ItemTex[nCntNum].g_pBuffMatItem->GetBufferPointer();

				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_Item[nCntItem].ItemTex[nCntNum].g_apTextureItem[nCntMat]);

				// ブロック(パーツ)の描画
				g_Item[nCntItem].ItemTex[nCntNum].g_pMeshItem->DrawSubset(nCntMat);
			}
		}
	}
}
//=======================
//ブロックの設定処理
//=======================
void SetItem(D3DXVECTOR3 pos, int nType,D3DXVECTOR3 Scal)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (!g_Item[nCntItem].bUse)
		{// 未使用状態なら

			g_Item[nCntItem] = g_TexItem[nType]; // 必要な情報を代入

			g_Item[nCntItem].pos = pos;			 // 座標
			g_Item[nCntItem].nType = nType;		 // 種類
			g_Item[nCntItem].Scal = Scal;		 // 拡大率
			g_Item[nCntItem].bUse = true;		 // 使用判定

			break;
		}
	}
}
//=======================
//アイテムの当たり判定
//=======================
bool HitThrowItem(D3DXVECTOR3* pPos, float ItemRadius, float EnemyRadius)
{
	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	bool bHit = false;  // 当たったかどうか

	D3DXVECTOR3 DisPos; // 距離計算用変数

	for (int nCnt = 0; nCnt < MAX_ITEM; nCnt++)
	{
		if (g_Item[nCnt].state == ITEMSTATE_THROW && g_Item[nCnt].bUse)
		{// ITEMSTATE_THROW　かつ bUseがtrue

			// 座標を設定
			DisPos.x = pPos->x - g_Item[nCnt].pos.x;
			DisPos.y = pPos->y - g_Item[nCnt].pos.y;
			DisPos.z = pPos->z - g_Item[nCnt].pos.z;

			// 距離を計算
			float fDistance = (DisPos.x * DisPos.x) + (DisPos.y * DisPos.y) + (DisPos.z * DisPos.z);

			// 半径を計算
			float fRadius = ItemRadius + EnemyRadius;

			fRadius = fRadius * fRadius;

			if (fDistance <= fRadius)
			{// fDistanceがfRadiusより小さい

				bHit = true;	// 判定を有効か

				if (!pPlayer->FeverMode)
				{
					g_Item[nCnt].durability--; // 耐久力をへらす
				}
				break;
			}
		}
	}
	// 判定を返す
	return bHit;
}
//=========================
//アイテムのテクスチャ取得
//=========================
void Itemchange(int nType)
{
	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	pPlayer->Motion.aModel[15].dwNumMat = g_TexItem[nType].ItemTex[nType].g_dwNumMatItem; // アイテムのマテリアルの情報を代入
	pPlayer->Motion.aModel[15].pBuffMat = g_TexItem[nType].ItemTex[nType].g_pBuffMatItem; // アイテムのバッファの情報を代入
	pPlayer->Motion.aModel[15].pMesh = g_TexItem[nType].ItemTex[nType].g_pMeshItem;       // アイテムのメッシュの情報を代入

	//if (g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD)
	//{
	//	g_Item[pPlayer->ItemIdx].nType = nType;
	//}
}
//=======================
//アイテムの取得
//=======================
Item* GetItem(void)
{
	return &g_Item[0];
}
//============================
// アイテムのロード処理
//============================
void LoadItemModel(void)
{
	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ファイルポインタを宣言
	FILE* pFile;

	// ローカル変数宣言-----------
	char skip[5];
	int nType = 0;
	//----------------------------

	// ファイルを開く
	pFile = fopen("data\\MODEL_TXT\\ITEM.txt", "r");

	if (pFile != NULL)
	{
		char aString[MAX_WORD];

		while (1)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "MAX_TYPE") == 0)
			{// MAX_TYPEを読み取った
				fscanf(pFile, "%s", &skip[0]);
				fscanf(pFile, "%d", &g_ItemTypeMax);
			}
			else if (strcmp(aString, "MODEL_FILENAME") == 0)
			{// MODEL_FILENAMEを読み取った
				fscanf(pFile, "%s", &skip[0]);

				fscanf(pFile, "%s", &aString[0]);

				const char* MODEL_FILENAME = {};

				MODEL_FILENAME = aString;

				//Xファイルの読み込み
				D3DXLoadMeshFromX(MODEL_FILENAME,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_TexItem[nType].ItemTex[nType].g_pBuffMatItem,
					NULL,
					&g_TexItem[nType].ItemTex[nType].g_dwNumMatItem,
					&g_TexItem[nType].ItemTex[nType].g_pMeshItem);

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
		MessageBox(NULL, "ファイルが開けません。", "エラー(item.cpp)", MB_OK);
		return;
	}
	// ファイルを閉じる
	fclose(pFile);
}
