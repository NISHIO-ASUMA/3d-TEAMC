//==============================================================================================================
//
// アイテム [item.cpp]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
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
#include "icon.h"
#include "craftui.h"
#include "sound.h"
#include "Effect.h"
#include "billboard.h"
#include "math.h"
#include "easing.h"
#include "meshimpact.h"
#include <cassert>
#include "minimap.h"

//**************************************************************************************************************
//マクロ定義
//**************************************************************************************************************
#define MAX_WORD (256)
#define MAX_DURABILITY (100) // 耐久力
#define DESTANCE (25.0f) // 距離

//**************************************************************************************************************
//プロトタイプ宣言
//**************************************************************************************************************
void LoadItemModel(void); // アイテムのロード処理
void CraftItem(int nCnt);
void LoadItemInfo(void); // アイテムの情報のロード処理
void PickUpItemAnimation(int nCntItem); // アイテムを拾える時の演出
bool CheckMixItemMat(int pCraftMat, int pStockMat,int HoldIdx,int StockIdx);		// アイテムがクラフトできるかどうか
void UpdateCraftItemParam(int nCnt);                                                // クラフトアイテムのパラメータ設定
void SetUpFirstWepon(int nCnt);														// 最初のアイテムの設定
void UpdateTrackingItem(int nCnt);													// 追従するアイテムの更新処理
void CheckPlayerItem(int nCntItem, Player* pPlayer);								// プレイヤーが持っているアイテムを調べる
void DestroyItem(int nCntItem, Player* pPlayer);									// 耐久力がなくなったアイテムの破壊処理
void UpdateThrowItem(int nCntItem, Player* pPlayer);								// プレイヤーがアイテムを投げる処理
bool isThrowItem(int nCntItem, Player* pPlayer);									// アイテムを投げられるかどうか
void ItemUpdateState(int nCntItem);													// アイテムの状態遷移
void ThrowItemEffect(int nCntItem);													// アイテムを投げた時のエフェクト
void UpdateHitBlockItem(int nCntItem);												// アイテムがブロックに当たった処理
void UpdateNearItem(int nCntItem);													// 近くのアイテムの処理
void UpdateThrowItemLife(int nCntItem);												// 投げたアイテムの寿命管理処理

//**************************************************************************************************************
//グローバル変数宣言
//**************************************************************************************************************
Item g_Item[MAX_ITEM];					// 構造体変数
int g_ItemTypeMax;						// 種類数
MODEL_INFO g_TexItem[ITEMTYPE_MAX];		// テクスチャ関係
ITEM_INFO g_aItemInfo[ITEMTYPE_MAX];	// アイテムの情報
bool bFIrstCraftItem = false;
bool bCraftAnim = false;

//===============================================================================================================
//ブロックの初期化処理
//===============================================================================================================
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
		g_Item[nCntItem].nElement = ITEMELEMENT_STANDARD;	   // 種類
		g_Item[nCntItem].bUse = false;						   // 未使用判定
		g_Item[nCntItem].nLife = 120;						   // 体力
		g_Item[nCntItem].state = ITEMSTATE_NORMAL;			   // 状態
		g_Item[nCntItem].fRadius = 100.0f;					   // 半径
		g_Item[nCntItem].durability = 0;					   // 耐久力
		g_Item[nCntItem].EnableCraft = false;				   // クラフトできるか否か
		g_Item[nCntItem].grabity = 0.0f;					   // クラフトできるか否か
		g_Item[nCntItem].nEasingCnt = 0;					   // イージングのカウント
		g_Item[nCntItem].nImpactCount = 110;				   // 衝撃波のカウント
		g_Item[nCntItem].Itemtag[0] = {};					   // タグ
		g_Item[nCntItem].Power = 0;							   // 攻撃力
		g_Item[nCntItem].bTracking = false;					   // プレイヤーに追従するかどうか
	}

	LoadItemInfo();  // アイテムの情報

	LoadItemModel(); // アイテムのロード処理

	bCraftAnim = false;
	bFIrstCraftItem = false;

	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		g_aItemInfo[nCntNum].nType = nCntNum;			            // 番号
		g_aItemInfo[nCntNum].Maxdurability = g_aItemInfo[nCntNum].durability;

		D3DXMATERIAL* pMat; // マテリアルへのポインタ

		if (g_TexItem[nCntNum].g_pBuffMatModel == NULL) continue;
			
		// マテリアルのデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_TexItem[nCntNum].g_pBuffMatModel->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_TexItem[nCntNum].g_dwNumMatModel; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//このファイル名を使用してテクスチャを読み込む
				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_TexItem[nCntNum].g_apTextureModel[nCntMat]);
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
		nNumVtx = g_TexItem[nCntNum].g_pMeshModel->GetNumVertices();

		// 頂点フォーマットのサイズ取得
		sizeFVF = D3DXGetFVFVertexSize(g_TexItem[nCntNum].g_pMeshModel->GetFVF());

		// 頂点バッファのロック
		g_TexItem[nCntNum].g_pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			// 頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			// 頂点座標を比較してブロックの最小値,最大値を取得
			if (vtx.x < g_TexItem[nCntNum].vtxMin.x)
			{
				g_TexItem[nCntNum].vtxMin.x = vtx.x;
			}
			else if (vtx.y < g_TexItem[nCntNum].vtxMin.y)
			{
				g_TexItem[nCntNum].vtxMin.y = vtx.y;
			}
			else if (vtx.z < g_TexItem[nCntNum].vtxMin.z)
			{
				g_TexItem[nCntNum].vtxMin.z = vtx.z;
			}
			else if (vtx.x > g_TexItem[nCntNum].vtxMax.x)
			{
				g_TexItem[nCntNum].vtxMax.x = vtx.x;
			}
			else if (vtx.y > g_TexItem[nCntNum].vtxMax.y)
			{
				g_TexItem[nCntNum].vtxMax.y = vtx.y;
			}
			else if (vtx.z > g_TexItem[nCntNum].vtxMax.z)
			{
				g_TexItem[nCntNum].vtxMax.z = vtx.z;
			}

			// 頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;

			// サイズを代入
			g_TexItem[nCntNum].Size.x = g_TexItem[nCntNum].vtxMax.x - g_TexItem[nCntNum].vtxMin.x;
			g_TexItem[nCntNum].Size.y = g_TexItem[nCntNum].vtxMax.y - g_TexItem[nCntNum].vtxMin.y;
			g_TexItem[nCntNum].Size.z = g_TexItem[nCntNum].vtxMax.z - g_TexItem[nCntNum].vtxMin.z;
		}

		// 頂点バッファのアンロック
		g_TexItem[nCntNum].g_pMeshModel->UnlockVertexBuffer();
	}
}
//===============================================================================================================
//ブロックの終了処理
//===============================================================================================================
void UninitItem(void)
{
	// 音楽を停止
	StopSound();

	// 種類分回す
	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		// テクスチャの破棄
		for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
		{
			if (g_TexItem[nCntNum].g_apTextureModel[nCntTex] != NULL)
			{
				g_TexItem[nCntNum].g_apTextureModel[nCntTex]->Release();
				g_TexItem[nCntNum].g_apTextureModel[nCntTex] = NULL;
			}
		}

		// メッシュの破棄
		if (g_TexItem[nCntNum].g_pMeshModel != NULL)
		{
			g_TexItem[nCntNum].g_pMeshModel->Release();
			g_TexItem[nCntNum].g_pMeshModel = NULL;
		}

		// マテリアルの破棄
		if (g_TexItem[nCntNum].g_pBuffMatModel != NULL)
		{
			g_TexItem[nCntNum].g_pBuffMatModel->Release();
			g_TexItem[nCntNum].g_pBuffMatModel = NULL;
		}
	}

#ifdef _DEBUG

	// 種類分回す
	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		// テクスチャの破棄
		for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
		{
			// テクスチャがNULLか確認
			assert(g_TexItem[nCntNum].g_apTextureModel[nCntTex] == NULL && "Item.cpp、originテクスチャの破棄ができてません");
		}

		// メッシュがNULLか確認
		assert(g_TexItem[nCntNum].g_pMeshModel == NULL && "Item.cpp、originメッシュの破棄ができてません");

		// バッファがNULLか確認
		assert(g_TexItem[nCntNum].g_pBuffMatModel == NULL && "Item.cpp、originバッファの破棄ができてません");
	}
#endif

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		int nType = g_Item[nCntItem].nType;

		// テクスチャの破棄
		for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
		{
			if (g_Item[nCntItem].ItemTex[nType].g_apTextureModel[nCntTex] != NULL)
			{
				g_Item[nCntItem].ItemTex[nType].g_apTextureModel[nCntTex] = NULL;
			}
		}

		// メッシュの破棄
		if (g_Item[nCntItem].ItemTex[nType].g_pMeshModel != NULL)
		{
			g_Item[nCntItem].ItemTex[nType].g_pMeshModel = NULL;
		}

		// マテリアルの破棄
		if (g_Item[nCntItem].ItemTex[nType].g_pBuffMatModel != NULL)
		{
			g_Item[nCntItem].ItemTex[nType].g_pBuffMatModel = NULL;
		}
	}

#ifdef _DEBUG

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		int nType = g_Item[nCntItem].nType;

		// テクスチャの破棄
		for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
		{
			// テクスチャがNULLか確認
			assert(g_Item[nCntItem].ItemTex[nType].g_apTextureModel[nCntTex] == NULL && "Item.cpp、テクスチャの破棄ができてません");
		}

		// メッシュがNULLか確認
		assert(g_Item[nCntItem].ItemTex[nType].g_pMeshModel == NULL && "Item.cpp、メッシュの破棄ができてません");

		// バッファがNULLか確認
		assert(g_Item[nCntItem].ItemTex[nType].g_pBuffMatModel == NULL && "Item.cpp、バッファの破棄ができてません");
	}

#endif
}
//===============================================================================================================
//ブロックの更新処理
//===============================================================================================================
void UpdateItem(void)
{
	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		// プレイヤーの持っているアイテムを調べる
		CheckPlayerItem(nCntItem, pPlayer);

		// アイテムの破壊処理
		DestroyItem(nCntItem, pPlayer);
		
		// アイテムを投げる処理
		UpdateThrowItem(nCntItem, pPlayer);

		//使用中じゃなかったら処理を読み飛ばす
		if (g_Item[nCntItem].bUse == false) continue;

		// 最初のアイテムの設定
		SetUpFirstWepon(nCntItem);

		// アイテムの状態遷移処理
		ItemUpdateState(nCntItem);

		// 投げアイテムのエフェクト処理
		ThrowItemEffect(nCntItem);

		// 前回の位置を代入
		g_Item[nCntItem].posOld = g_Item[nCntItem].pos;

		// 位置の更新
		g_Item[nCntItem].pos += g_Item[nCntItem].move;

		// アイテムがブロックに当たった処理
		UpdateHitBlockItem(nCntItem);

		// アイテムが近くにある処理
		UpdateNearItem(nCntItem);

		// 追従するアイテムの更新処理
		UpdateTrackingItem(nCntItem);

		// プレイヤーがクラフト状態だったら
		if (pPlayer->bCraft == true) CraftItem(nCntItem);

		// 投げたアイテムの寿命管理処理
		UpdateThrowItemLife(nCntItem);
	}

}
//===============================================================================================================
//ブロックの描画処理
//===============================================================================================================
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

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (!g_Item[nCntItem].bUse)
		{//未使用状態なら
			//下の処理を通さずカウントを進める
			continue;
		}

		int nType = g_Item[nCntItem].nType;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Item[nCntItem].mtxWorldItem);

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

		for (int nCntMat = 0; nCntMat < (int)g_Item[nCntItem].ItemTex[nType].g_dwNumMatModel; nCntMat++)
		{
			// マテリアルのデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_Item[nCntItem].ItemTex[nType].g_pBuffMatModel->GetBufferPointer();

			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_Item[nCntItem].ItemTex[nType].g_apTextureModel[nCntMat]);

			// ブロック(パーツ)の描画
			g_Item[nCntItem].ItemTex[nType].g_pMeshModel->DrawSubset(nCntMat);
		}
	}
	
}
//=========================================================================================================
//ブロックの設定処理
//=========================================================================================================
void SetItem(D3DXVECTOR3 pos, int nType)
{
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (!g_Item[nCntItem].bUse)
		{// 未使用状態なら

			g_Item[nCntItem].ItemTex[nType] = g_TexItem[nType]; // 必要な情報を代入
			g_Item[nCntItem].Power = g_aItemInfo[nType].Power;  // 攻撃力
			g_Item[nCntItem].Maxdurability = g_aItemInfo[nType].Maxdurability;
			g_Item[nCntItem].durability = g_aItemInfo[nType].durability;
			g_Item[nCntItem].nElement = g_aItemInfo[nType].nElement;

			g_Item[nCntItem].pos = pos;			 // 座標
			g_Item[nCntItem].nType = nType;		 // 種類
			g_Item[nCntItem].bUse = true;		 // 使用判定

			g_Item[nCntItem].nIdxBillboardCount = SetBillboard(D3DXVECTOR3(pos.x,pos.y + 100.0f,pos.z), BILLBOARDTYPE_SECOND, 40.0f, 20.0f, BILLBOARDSTATE_NOSET);
			break;
		}
	}
}
//=========================================================================================================
//アイテムの当たり判定
//=========================================================================================================
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
//===========================================================================================================
//アイテムのテクスチャ取得
//===========================================================================================================
void Itemchange(int nIdx, int nType)
{
	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	pPlayer->Motion.aModel[15].dwNumMat = g_TexItem[nType].g_dwNumMatModel; // アイテムのマテリアルの情報を代入
	pPlayer->Motion.aModel[15].pBuffMat = g_TexItem[nType].g_pBuffMatModel; // アイテムのバッファの情報を代入
	pPlayer->Motion.aModel[15].pMesh = g_TexItem[nType].g_pMeshModel;       // アイテムのメッシュの情報を代入
	pPlayer->nElement = g_aItemInfo[nType].nElement; // アイテムの属性情報を代入

	// 同じインデックスじゃないかつ耐久値が減っていないなら
	if (nIdx != pPlayer->ItemIdx && g_Item[nIdx].durability == g_Item[nIdx].Maxdurability)
	{
		// 耐久力を代入
		g_Item[nIdx].durability = g_aItemInfo[nType].durability;
		g_Item[nIdx].Maxdurability = g_aItemInfo[nType].Maxdurability;


		strcpy(&g_Item[nIdx].Itemtag[0], &g_aItemInfo[nType].Itemtag[0]);
	}

	// 大きさを代入
	g_Item[nIdx].Size = g_TexItem[nType].Size;
}
//=========================================================================================================
//アイテムの取得
//=========================================================================================================
Item* GetItem(void)
{
	return &g_Item[0];
}
//==============================================================================================================
// アイテムのロード処理
//==============================================================================================================
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
			int nData = fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "MAX_TYPE") == 0)
			{// MAX_TYPEを読み取った
				nData = fscanf(pFile, "%s", &skip[0]);
				nData = fscanf(pFile, "%d", &g_ItemTypeMax);
			}
			else if (strcmp(aString, "MODEL_FILENAME") == 0)
			{// MODEL_FILENAMEを読み取った
				nData = fscanf(pFile, "%s", &skip[0]);

				nData = fscanf(pFile, "%s", &aString[0]);

				const char* MODEL_FILENAME = {};

				MODEL_FILENAME = aString;

				//Xファイルの読み込み
				D3DXLoadMeshFromX(MODEL_FILENAME,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_TexItem[nType].g_pBuffMatModel,
					NULL,
					&g_TexItem[nType].g_dwNumMatModel,
					&g_TexItem[nType].g_pMeshModel);

				nType++;
			}

			// EOFを読み取ったら
			if (nData == EOF)
			{
				// while文を抜ける
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
//==============================================================================================================
// アイテムの取得
//==============================================================================================================
MODEL_INFO* GetItemOrigin(void)
{
	return &g_TexItem[0];
}
//==============================================================================================================
// アイテムの情報の取得
//==============================================================================================================
ITEM_INFO* GetItemInfo(void)
{
	return &g_aItemInfo[0];
}
//==============================================================================================================
// アイテムのクラフト
//==============================================================================================================
void CraftItem(int nCnt)
{
	// クラフトのパラメーターの設定処理
	UpdateCraftItemParam(nCnt);
}
//==============================================================================================================
// クラフト先のアイテム
//==============================================================================================================
void CraftMixItem(int HoldIdx,int StockIdx)
{
	Player* pPlayer = GetPlayer();

	FILE* pFile; // ファイルのポインタ

	// ファイルを開く
	pFile = fopen("data\\ITEM\\craftrecipe.txt", "r");

	int nMixWepon = -1;
	int nCraftmat0 = -2;
	int nCraftmat1 = -3;

	char skip[10] = {};

	// ファイルが読み込めたら
	if (pFile != NULL)
	{
		// 文字を読み取る
		char aString[MAX_WORD] = {};

		while (1)
		{
			// 文字を読み取る
			int nData = fscanf(pFile, "%s", &aString[0]);

			// 合成先のアイテム
			if (strcmp(&aString[0], "MIXWEPON") == 0)
			{
				// [=]を飛ばす
				nData = fscanf(pFile, "%s", &skip[0]);

				// 合成先のアイテムを読み取る
				nData = fscanf(pFile, "%d", &nMixWepon);
			}

			// クラフトの素材1
			if (strcmp(&aString[0], "CRAFT_MATERIAL_ONE") == 0)
			{
				// [=]を飛ばす
				nData = fscanf(pFile, "%s", &skip[0]);

				// クラフトの素材を読み取る
				nData = fscanf(pFile, "%d", &nCraftmat0);
			}

			// クラフトの素材2
			if (strcmp(&aString[0], "CRAFT_MATERIAL_TWO") == 0)
			{
				// [=]を飛ばす
				nData = fscanf(pFile, "%s", &skip[0]);

				// クラフトの素材を読み取る
				nData = fscanf(pFile, "%d", &nCraftmat1);
			}

			// アイテムをストックしているか判定
			const bool ItemStock = g_Item[StockIdx].state == ITEMSTATE_STOCK;

			// アイテムを持っているか判定
			const bool ItemHold = g_Item[HoldIdx].state == ITEMSTATE_HOLD;

			// レシピと一致しているか判定
			const bool bRecipeCheck = (nCraftmat0 == g_Item[HoldIdx].nType && nCraftmat1 == g_Item[StockIdx].nType) || (nCraftmat1 == g_Item[HoldIdx].nType && nCraftmat0 == g_Item[StockIdx].nType);

			// クラフトできるか判定
			const bool isCraft = ItemStock && ItemHold && bRecipeCheck;

			// 手に持っているアイテムとストックしているアイテムがレシピと一致していたら
			if (isCraft == true)
			{
				break;
			}

			// EOFを読み取ったら
			if (nData == EOF)
			{
				// ファイルを閉じる
				fclose(pFile);

				return;
			}
		}
	}
	else
	{
		// エラーメッセージ
		MessageBox(NULL, "ファイルが開けません", "クラフトレシピ", MB_OK);
		return;
	}

	// ファイルを閉じる
	fclose(pFile);

	// 火花っぽいエフェクト
	SetParticle(D3DXVECTOR3(g_Item[HoldIdx].pos.x, g_Item[HoldIdx].pos.y + 30.0f, g_Item[HoldIdx].pos.z),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(3.14f, 3.14f, 3.14f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f),
		2.0f, 2, 40, 20, 5.0f, 2.0f, false,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	SetParticle(D3DXVECTOR3(g_Item[HoldIdx].pos.x, g_Item[HoldIdx].pos.y + 30.0f, g_Item[HoldIdx].pos.z),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(3.14f, 3.14f, 3.14f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
		3.0f, 2, 40, 20, 3.0f, 2.0f, false,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	SetParticle(D3DXVECTOR3(g_Item[HoldIdx].pos.x, g_Item[HoldIdx].pos.y + 30.0f, g_Item[HoldIdx].pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), 3.0f, 2, 30, 10, 10.0f, 40.0f, true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
	SetParticle(D3DXVECTOR3(g_Item[HoldIdx].pos.x, g_Item[HoldIdx].pos.y + 30.0f, g_Item[HoldIdx].pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 3.0f, 2, 30, 10, 10.0f, 40.0f, true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
	
	// SEを出す
	PlaySound(SOUND_LABEL_CRAFT);

	g_Item[pPlayer->ItemIdx].Power = g_aItemInfo[nMixWepon].Power;

	// 持っているアイテムを変更
	Itemchange(HoldIdx, nMixWepon);

	// クラフトに使ったアイテムを消す
	g_Item[HoldIdx].bUse = false;

	// 種類を代入
	int nType = g_Item[HoldIdx].nType;

	// アイテムの見た目を変える
	g_Item[HoldIdx].ItemTex[nType] = g_TexItem[nMixWepon];

	g_Item[HoldIdx].durability = g_aItemInfo[nMixWepon].durability;
	g_Item[HoldIdx].Maxdurability = g_aItemInfo[nMixWepon].Maxdurability;

	// 手に持ってるアイテムの種類を石バットにする
	g_Item[HoldIdx].nType = nMixWepon;

	// 属性を代入
	g_Item[HoldIdx].nElement = g_aItemInfo[nMixWepon].nElement;

	// 攻撃力を代入
	g_Item[HoldIdx].Power = g_aItemInfo[nMixWepon].Power;

	g_Item[HoldIdx].state = ITEMSTATE_HOLD;

	g_Item[StockIdx].state = ITEMSTATE_NORMAL;

	// 文字をコピー
	strcpy(&g_Item[HoldIdx].Itemtag[0], &g_aItemInfo[nMixWepon].Itemtag[0]);

	pPlayer->StockItemIdx = pPlayer->ItemIdx;

	// ステータスの変更
	LoadItemChange(nMixWepon, g_Item[HoldIdx].Size.y);
}
//==============================================================================================================
// アイテムの出現
//==============================================================================================================
void SpawonItem(D3DXVECTOR3 pos, int nType)
{
	Player* pPlayer = GetPlayer();

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[nCntItem].bUse == false && pPlayer->ItemIdx != nCntItem && pPlayer->StockItemIdx != nCntItem)
		{// 未使用状態なら

			g_Item[nCntItem].ItemTex[nType] = g_TexItem[nType]; // 必要な情報を代入
			g_Item[nCntItem].Power = g_aItemInfo[nType].Power;
			g_Item[nCntItem].Maxdurability = g_aItemInfo[nType].Maxdurability;
			g_Item[nCntItem].durability = g_aItemInfo[nType].durability;
			g_Item[nCntItem].nElement = g_aItemInfo[nType].nElement;

			g_Item[nCntItem].pos = pos;			 // 座標
			g_Item[nCntItem].nType = nType;		 // 種類
			g_Item[nCntItem].bUse = true;		 // 使用判定

			g_Item[nCntItem].nIdxBillboardCount = SetBillboard(D3DXVECTOR3(pos.x, pos.y + 100.0f, pos.z), BILLBOARDTYPE_SECOND, 40.0f, 20.0f, BILLBOARDSTATE_NOSET);
			break;
		}
	}
}
//==============================================================================================================
// 最初にクラフトさせるアイテムの設定処理
//==============================================================================================================
void SetFirstCraftItem(D3DXVECTOR3 pos, int nType)
{
	Player* pPlayer = GetPlayer();

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[nCntItem].bUse == false && pPlayer->ItemIdx != nCntItem && pPlayer->StockItemIdx != nCntItem)
		{// 未使用状態なら

			g_Item[nCntItem].ItemTex[nType] = g_TexItem[nType]; // 必要な情報を代入
			g_Item[nCntItem].Power = g_aItemInfo[nType].Power;
			g_Item[nCntItem].Maxdurability = g_aItemInfo[nType].Maxdurability;
			g_Item[nCntItem].durability = g_aItemInfo[nType].durability;
			g_Item[nCntItem].nElement = g_aItemInfo[nType].nElement;
			g_Item[nCntItem].bTracking = true;

			g_Item[nCntItem].pos = pos;			 // 座標
			g_Item[nCntItem].nType = nType;		 // 種類
			g_Item[nCntItem].bUse = true;		 // 使用判定

			break;
		}
	}
}
//==============================================================================================================
// クラフト先のアイテムのアイコンの表示
//==============================================================================================================
void EnableCraftIcon(int Item1, int Item2)
{
	Player* pPlayer = GetPlayer();

	FILE* pFile; // ファイルのポインタ

	// ファイルを開く
	pFile = fopen("data\\ITEM\\craftrecipe.txt", "r");

	int nMixWepon = -1;
	int nCraftmat0 = -2;
	int nCraftmat1 = -3;

	char skip[10] = {};

	// ファイルが読み込めたら
	if (pFile != NULL)
	{
		// 文字を読み取る
		char aString[MAX_WORD] = {};

		while (1)
		{
			// 文字を読み取る
			int nData = fscanf(pFile, "%s", &aString[0]);

			// 合成先のアイテム
			if (strcmp(&aString[0], "MIXWEPON") == 0)
			{
				// [=]を飛ばす
				nData = fscanf(pFile, "%s", &skip[0]);

				// 合成先のアイテムを読み取る
				nData = fscanf(pFile, "%d", &nMixWepon);
			}

			// クラフトの素材1
			if (strcmp(&aString[0], "CRAFT_MATERIAL_ONE") == 0)
			{
				// [=]を飛ばす
				nData = fscanf(pFile, "%s", &skip[0]);

				// クラフトの素材を読み取る
				nData = fscanf(pFile, "%d", &nCraftmat0);
			}

			// クラフトの素材2
			if (strcmp(&aString[0], "CRAFT_MATERIAL_TWO") == 0)
			{
				// [=]を飛ばす
				nData = fscanf(pFile, "%s", &skip[0]);

				// クラフトの素材を読み取る
				nData = fscanf(pFile, "%d", &nCraftmat1);
			}

			// 手に持っているアイテムとストックしているアイテムがレシピと一致していたら
			if ((nCraftmat0 == Item1 && nCraftmat1 == Item2) || (nCraftmat1 == Item1 && nCraftmat0 == Item2))
			{
				// 合成先アイテムを表示
				SetMixUI(UIPOTISION, nMixWepon, 80.0f, 80.0f, 0);

				// ファイルを閉じる
				fclose(pFile);

				// while文を抜ける
				return;
			}

			if (strcmp(&aString[0], "END_CRAFTSET") == 0)
			{
				// 確認し終わったら初期化
				nMixWepon = -1;

				// 確認し終わったら初期化
				nCraftmat0 = -2;

				// 確認し終わったら初期化
				nCraftmat1 = -3;
			}

			// EOFを読み取ったら
			if (nData == EOF)
			{
				// ファイルを閉じる
				fclose(pFile);
				
				// レシピと一致するものが無かったら強制的に関数を抜ける
				return;
			}
		}
	}
	else
	{
		// エラーメッセージ
		MessageBox(NULL, "ファイルが開けません", "クラフトレシピ", MB_OK);
		return;
	}
}
//==============================================================================================================
// アイテムの耐久力のロード処理
//==============================================================================================================
void LoadItemInfo(void)
{
	FILE* pFile; // ファイルのポインタ

	// ファイルを開く
	pFile = fopen("data\\ITEM\\item_Info.txt", "r");

	// 文字列格納変数
	char aString[MAX_WORD] = {};

	// [=]読み飛ばしよう変数
	char skip[3] = {};

	// 種類
	int nType = 0;

	// ファイルが開けたら
	if (pFile != NULL)
	{
		while (1)
		{
			// 文字を読み取る
			int nData = fscanf(pFile, "%s", &aString[0]);

			// ITEMTYPEを読み取ったら
			if (strcmp(&aString[0], "ITEMTYPE") == 0)
			{
				// [=]を飛ばす
				nData = fscanf(pFile, "%s", &skip[0]);

				// 種類を代入
				nData = fscanf(pFile, "%d", &nType);
			}

			// DURABILITYを読み取ったら
			if (strcmp(&aString[0], "DURABILITY") == 0)
			{
				// [=]を飛ばす
				nData = fscanf(pFile, "%s", &skip[0]);

				// 耐久力を代入
				nData = fscanf(pFile, "%d", &g_aItemInfo[nType].durability);
			}

			// TAGを読み取ったら
			if (strcmp(&aString[0], "TAG") == 0)
			{
				// [=]を飛ばす
				nData = fscanf(pFile, "%s", &skip[0]);

				// アイテムのタグを代入
				nData = fscanf(pFile, "%s", &g_aItemInfo[nType].Itemtag[0]);
			}

			// ELEMENTを読み取ったら
			if (strcmp(&aString[0], "ELEMENT") == 0)
			{
				// [=]を飛ばす
				nData = fscanf(pFile, "%s", &skip[0]);

				// 属性を代入
				nData = fscanf(pFile, "%d", &g_aItemInfo[nType].nElement);
			}

			// POWERを読み取ったら
			if (strcmp(&aString[0], "POWER") == 0)
			{
				// [=]を飛ばす
				nData = fscanf(pFile, "%s", &skip[0]);

				// 攻撃力を代入
				nData = fscanf(pFile, "%d", &g_aItemInfo[nType].Power);
			}

			// END_ITEMSETを読み取ったら
			if (strcmp(&aString[0], "END_ITEMSET") == 0)
			{
				// つぎの種類の情報へ
				nType++;
			}

			// ファイルの最後まで行ったら
			if (nData == EOF)
			{
				// While文を抜ける
				break;
			}
		}
	}
	else
	{// 開けなかったとき
		// メッセージボックスを表示
		MessageBox(NULL, "ファイルが開けません", "LoadDuability", MB_OK);
		return;
	}

	// ファイルを閉じる
	fclose(pFile);
}
//==============================================================================================================
// アイテムを拾える時の演出
//==============================================================================================================
void PickUpItemAnimation(int nCntItem)
{
	// プレイヤー追従してたら
	if (g_Item[nCntItem].bTracking == true) return;

	// インクリメント
	g_Item[nCntItem].nEasingCnt++;
	
	float time = SetEase(g_Item[nCntItem].nEasingCnt,120);

	// 目的の値に近づける
	g_Item[nCntItem].pos.y += SetSmoothAprroach(20.0f, g_Item[nCntItem].pos.y, EaseInOutCubic(time));

	// 目的の値に近づける
	g_Item[nCntItem].rot.x += SetSmoothAprroach(D3DX_PI * 0.15f, g_Item[nCntItem].rot.x, EaseInOutCubic(time));

	// 目的の値に近づける
	g_Item[nCntItem].rot.y += 0.01f;

	// アイテムの周りにパーティクル出現
	if (g_Item[nCntItem].nType == ITEMTYPE_ONIGIRI)
	{
		SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y + g_Item[nCntItem].Size.y / 1.5f, g_Item[nCntItem].pos.z),
			D3DXVECTOR3(1.57f, g_Item[nCntItem].rot.y, 1.57f),
			D3DXVECTOR3(0.2f, 3.14f, 0.2f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f),
			2.0f, 1, 20, 1, 3.0f, 12.0f,
			false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	else
	{
		SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y + g_Item[nCntItem].Size.y / 1.5f, g_Item[nCntItem].pos.z),
			D3DXVECTOR3(1.57f, g_Item[nCntItem].rot.y, 1.57f),
			D3DXVECTOR3(0.2f, 3.14f, 0.2f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
			2.0f, 1, 20, 1, 3.0f, 12.0f,
			false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	if (g_Item[nCntItem].nImpactCount >= 60)
	{
		if (g_Item[nCntItem].nType == ITEMTYPE_ONIGIRI)
		{
			SetImpact(D3DXVECTOR3(g_Item[nCntItem].pos.x, 0.0f, g_Item[nCntItem].pos.z), COLOR_GREEN, 20, 10.0f, 7.0f, 1.0f, 30, IMPACTTYPE_NORMAL, 0);
			SetImpact(D3DXVECTOR3(g_Item[nCntItem].pos.x, 0.0f, g_Item[nCntItem].pos.z), COLOR_GREEN, 20, 1.0f, 0.0f, 1.0f, 30, IMPACTTYPE_NORMAL, 0);
			g_Item[nCntItem].nImpactCount = 0;
		}
		else
		{
			SetImpact(D3DXVECTOR3(g_Item[nCntItem].pos.x, 0.0f, g_Item[nCntItem].pos.z), COLOR_GOLD, 20, 10.0f, 7.0f, 1.0f, 30, IMPACTTYPE_NORMAL, 0);
			SetImpact(D3DXVECTOR3(g_Item[nCntItem].pos.x, 0.0f, g_Item[nCntItem].pos.z), COLOR_GOLD, 20, 1.0f, 0.0f, 1.0f, 30, IMPACTTYPE_NORMAL, 0);
			g_Item[nCntItem].nImpactCount = 0;
		}
	}
	else
	{
		// 120になるまでインクリメント
		g_Item[nCntItem].nImpactCount++;
	}
}
//==============================================================================================================
// アイテムがクラフトできるかどうか
//==============================================================================================================
bool CheckMixItemMat(int pCraftMat, int pStockMat, int HoldIdx, int StockIdx)
{
	Player* pPlayer = GetPlayer();

	FILE* pFile; // ファイルのポインタ

	// ファイルを開く
	pFile = fopen("data\\ITEM\\craftrecipe.txt", "r");

	int nMixWepon = -1;
	int nCraftmat0 = -2;
	int nCraftmat1 = -3;

	char skip[10] = {};

	// ファイルが読み込めたら
	if (pFile != NULL)
	{
		// 文字を読み取る
		char aString[MAX_WORD] = {};

		while (1)
		{
			// 文字を読み取る
			int nData = fscanf(pFile, "%s", &aString[0]);

			// 合成先のアイテム
			if (strcmp(&aString[0], "MIXWEPON") == 0)
			{
				// [=]を飛ばす
				nData = fscanf(pFile, "%s", &skip[0]);

				// 合成先のアイテムを読み取る
				nData = fscanf(pFile, "%d", &nMixWepon);
			}

			// クラフトの素材1
			if (strcmp(&aString[0], "CRAFT_MATERIAL_ONE") == 0)
			{
				// [=]を飛ばす
				nData = fscanf(pFile, "%s", &skip[0]);

				// クラフトの素材を読み取る
				nData = fscanf(pFile, "%d", &nCraftmat0);
			}

			// クラフトの素材2
			if (strcmp(&aString[0], "CRAFT_MATERIAL_TWO") == 0)
			{
				// [=]を飛ばす
				nData = fscanf(pFile, "%s", &skip[0]);

				// クラフトの素材を読み取る
				nData = fscanf(pFile, "%d", &nCraftmat1);
			}

			// アイテムをストックしているか判定
			const bool ItemStock = g_Item[StockIdx].state == ITEMSTATE_STOCK;

			// アイテムを持っているか判定
			const bool ItemHold = g_Item[HoldIdx].state == ITEMSTATE_HOLD;

			// レシピと一致しているか判定
			const bool bRecipeCheck = (nCraftmat0 == pCraftMat && nCraftmat1 == pStockMat) || (nCraftmat1 == pCraftMat && nCraftmat0 == pStockMat);

			// クラフトできるか判定
			const bool isCraft = ItemStock && ItemHold && bRecipeCheck;

			// 手に持っているアイテムとストックしているアイテムがレシピと一致していたら
			if (isCraft == true)
			{
				// ファイルを閉じる
				fclose(pFile);

				// while文を抜ける
				return true;
			}

			if (strcmp(&aString[0], "END_CRAFTSET") == 0)
			{
				// 確認し終わったら初期化
				nMixWepon = -1;

				// 確認し終わったら初期化
				nCraftmat0 = -2;

				// 確認し終わったら初期化
				nCraftmat1 = -3;
			}

			// EOFを読み取ったら
			if (nData == EOF)
			{
				// ファイルを閉じる
				fclose(pFile);

				// レシピと一致するものが無かったら強制的に関数を抜ける
				return false;
			}
		}
	}
	else
	{
		// エラーメッセージ
		MessageBox(NULL, "ファイルが開けません", "クラフトレシピ", MB_OK);
		return false;
	}
}
//==============================================================================================================
// クラフトアイテムのパラメータ設定
//==============================================================================================================
void UpdateCraftItemParam(int nCnt)
{
	Player* pPlayer = GetPlayer();

	// キーを押したら
	if ((OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_A)) && GetIconAnim() == false)
	{
		// プレイヤーが持っているアイテムがレシピと一致するか確認
		const bool CheckMatItem = CheckMixItemMat(g_Item[pPlayer->ItemIdx].nType, g_Item[pPlayer->StockItemIdx].nType, pPlayer->ItemIdx, pPlayer->StockItemIdx) == true;

		// クラフトの素材が揃っていたら
		if (CheckMatItem == true)
		{
			// ミニマップのアイテムのアイコンのリセット
			ResetItemMinimap();

			// クラフトアイコンのアニメーション処理
			EnableCraftIconAnim(true);

			// プレイヤーが最初のクラフトタイムでクラフトを実行したら
			if (pPlayer->bFirstCraft == false && GetFirstCraftTIme() == true)
			{
				// クラフトした
				pPlayer->bFirstCraft = true;

				// クラフトモードを解除
				EnableFirstCraftTime(false);
			}
		}
	}
}
//==============================================================================================================
// 最初のアイテムの設定
//==============================================================================================================
void SetUpFirstWepon(int nCnt)
{
	MODE mode = GetMode();
	Player* pPlayer = GetPlayer();

	// チュートリアルじゃなかったら
	if (mode != MODE_TUTORIAL) return;

	// もう変えた
	if (bFIrstCraftItem == true) return;

	// プレイヤーのアイテムを刀にする
	if (bFIrstCraftItem == false && g_Item[nCnt].nType == ITEMTYPE_KATANA)
	{
		// アイテム変更
		Itemchange(nCnt, g_Item[nCnt].nType);

		// モーションの変更
		MotionChange(MOTION_KATANA, 0);

		// アイテムのインデックスを保存
		pPlayer->ItemIdx = nCnt;
		pPlayer->StockItemIdx = nCnt;

		g_Item[nCnt].state = ITEMSTATE_HOLD;

		// ステータス変更
		StatusChange(3.1f, D3DXVECTOR3(0.0f, g_Item[nCnt].Size.y, 0.0f), 100);

		// 最初に切り替えた
		bFIrstCraftItem = true;
	}
}
//==============================================================================================================
// 追従するアイテムの更新処理
//==============================================================================================================
void UpdateTrackingItem(int nCnt)
{
	// 追従しないなら関数を抜ける
	if (g_Item[nCnt].bTracking == false) return;

	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	// 角度
	static float fAngle = 0.0f;

	// 角度を加算
	fAngle += 0.05f;

	// 角度を正規化
	if (fAngle < -D3DX_PI)
	{
		fAngle += D3DX_PI * 2.0f;
	}
	else if (fAngle > D3DX_PI)
	{
		fAngle += -D3DX_PI * 2.0f;
	}

	// アイテムの種類がバットだったら
	if (g_Item[nCnt].nType == ITEMTYPE_TORCH)
	{
		// 位置を設定
		g_Item[nCnt].pos.x = pPlayer->pos.x + sinf(fAngle) * DESTANCE;
		g_Item[nCnt].pos.y = pPlayer->pos.y + DESTANCE;
		g_Item[nCnt].pos.z = pPlayer->pos.z + cosf(fAngle) * DESTANCE;
	}
	// アイテムの種類が石だったら
	else if (g_Item[nCnt].nType == ITEMTYPE_KATANA)
	{
		// 位置を設定
		g_Item[nCnt].pos.x = pPlayer->pos.x - sinf(fAngle) * DESTANCE;
		g_Item[nCnt].pos.y = pPlayer->pos.y + DESTANCE;
		g_Item[nCnt].pos.z = pPlayer->pos.z - cosf(fAngle) * DESTANCE;
	}
}
//==============================================================================================================
// プレイヤーが持っているアイテムを調べる
//==============================================================================================================
void CheckPlayerItem(int nCntItem, Player* pPlayer)
{
	// 最初の変更
	static bool bFIrstChenge = true;

	// 最初のアイテムだったら
	const int nfirstItem = 0;

	if (nCntItem == nfirstItem)
	{
		bFIrstChenge = true;
	}

	// ホールドのアイテムがあったら
	if (g_Item[nCntItem].state == ITEMSTATE_HOLD)
	{
		// 持っているアイテムの種類を代入
		pPlayer->HoldItemType = g_Item[nCntItem].nType;
		bFIrstChenge = false;
	}
	// ストックのアイテムがあったら
	else if (g_Item[nCntItem].state == ITEMSTATE_STOCK)
	{
		// ストックしているアイテムの種類を代入
		pPlayer->StockItemType = g_Item[nCntItem].nType;
		bFIrstChenge = false;
	}

	// 最初の変更が終わってない
	if (bFIrstChenge == true)
	{
		// 存在しない種類を代入
		pPlayer->StockItemType = ITEMTYPE_NONEXISTENT;

		// 存在しない種類を代入
		pPlayer->HoldItemType = ITEMTYPE_NONEXISTENT;
	}
}
//==============================================================================================================
// 耐久力がなくなったアイテムの破壊処理
//==============================================================================================================
void DestroyItem(int nCntItem, Player* pPlayer)
{
	// 耐久力が0になったら
	if (g_Item[nCntItem].durability <= 0 && g_Item[nCntItem].state == ITEMSTATE_HOLD)
	{
		int nType = g_Item[nCntItem].nType;

		// 音楽再生
		PlaySound(SPUND_LABEL_WEPONBREAK);

		// アイテムを壊す
		pPlayer->Itembreak[nCntItem] = true;
		g_Item[nCntItem].bUse = false; // 消す
	}
}
//==============================================================================================================
// プレイヤーがアイテムを投げる処理
//==============================================================================================================
void UpdateThrowItem(int nCntItem, Player* pPlayer)
{
	// アイテムが投げられるなら
	if (isThrowItem(nCntItem,pPlayer))
	{
		ThrowItem();
	}
}
//==============================================================================================================
// アイテムを投げられるかどうか
//==============================================================================================================
bool isThrowItem(int nCntItem, Player* pPlayer)
{
	// プレイヤーがスペシャル攻撃をしていたら
	if (pPlayer->AttackSp == true) return false;

	// プレイヤーがアイテムを持っていなかったら
	if (pPlayer->HandState != PLAYERHOLD_HOLD) return false;

	// モーションのキーが3じゃなかったら
	if (pPlayer->Motion.nKey != 3) return false;

	// モーションの種類が攻撃じゃなかったら
	if (pPlayer->Motion.motionType != MOTIONTYPE_ACTION) return false;

	// アイテムを投げられる
	return true;
}
//==============================================================================================================
// アイテムの状態遷移
//==============================================================================================================
void ItemUpdateState(int nCntItem)
{
	// 状態の遷移
	switch (g_Item[nCntItem].state)
	{
	case ITEMSTATE_NORMAL:
		break;
	case ITEMSTATE_HOLD:
		break;
	case ITEMSTATE_THROW:
		break;
	case ITEMSTATE_STOCK:
		break;
	case ITEMSTATE_RELEASE:
		g_Item[nCntItem].nCounterState--;
		if (g_Item[nCntItem].nCounterState <= 0)
		{
			g_Item[nCntItem].state = ITEMSTATE_NORMAL;
		}
		break;
	default:
		break;
	}
}
//==============================================================================================================
// アイテムを投げた時のエフェクト
//==============================================================================================================
void ThrowItemEffect(int nCntItem)
{
	// 投げられたアイテムにエフェクト
	if (g_Item[nCntItem].state == ITEMSTATE_THROW)
	{
		// アイテムの属性を反映し
		int nType = g_Item[nCntItem].nType;
		g_Item[nCntItem].nElement = g_aItemInfo[nType].nElement;
		if (g_Item[nCntItem].nElement == ITEMELEMENT_STANDARD)// 無属性なら
		{
			// エフェクトを出す
			SetEffect(g_Item[nCntItem].pos, g_Item[nCntItem].rot, 20, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 0.0f, g_Item[nCntItem].Size.y);

			// パーティクルを出す、以下全て同じ
			SetParticle(g_Item[nCntItem].pos,
				g_Item[nCntItem].rot,
				D3DXVECTOR3(1.0f, 1.0f, 1.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f),
				2.0f, 2, 20, 7, 3.0f, 10.0f,
				false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
		else if (g_Item[nCntItem].nElement == ITEMELEMENT_BLOOD)// 出血属性
		{
			// エフェクトを出す
			SetEffect(g_Item[nCntItem].pos, g_Item[nCntItem].rot, 20, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, g_Item[nCntItem].Size.y);
			SetParticle(g_Item[nCntItem].pos,
				g_Item[nCntItem].rot,
				D3DXVECTOR3(1.0f, 1.0f, 1.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
				3.0f, 2, 20, 7, 3.0f, 10.0f,
				false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
		else if (g_Item[nCntItem].nElement == ITEMELEMENT_FIRE)// 炎属性
		{
			// エフェクトを出す
			SetEffect(g_Item[nCntItem].pos, g_Item[nCntItem].rot, 20, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), 0.0f, g_Item[nCntItem].Size.y);

			SetParticle(g_Item[nCntItem].pos,
				g_Item[nCntItem].rot,
				D3DXVECTOR3(1.0f, 1.0f, 1.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f),
				4.0f, 2, 20, 7, 3.0f, 20.0f,
				true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));

			SetParticle(g_Item[nCntItem].pos,
				g_Item[nCntItem].rot,
				D3DXVECTOR3(1.0f, 1.0f, 1.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
				4.0f, 2, 20, 7, 3.0f, 20.0f,
				true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
		}
		else if (g_Item[nCntItem].nElement == ITEMELEMENT_FREEZE)// 氷属性
		{
			// エフェクトを出す
			SetEffect(g_Item[nCntItem].pos, g_Item[nCntItem].rot, 20, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, g_Item[nCntItem].Size.y);
			SetParticle(g_Item[nCntItem].pos,
				g_Item[nCntItem].rot,
				D3DXVECTOR3(1.0f, 1.0f, 1.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				2.0f, 2, 50, 7, 0.5f, 10.0f,
				false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
		else if (g_Item[nCntItem].nElement == ITEMELEMENT_SPARK)// 雷撃属性
		{
			// エフェクトを出す
			SetEffect(g_Item[nCntItem].pos, g_Item[nCntItem].rot, 20, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 0.0f, g_Item[nCntItem].Size.y);
			SetParticle(g_Item[nCntItem].pos,
				g_Item[nCntItem].rot,
				D3DXVECTOR3(2.0f, 2.0f, 2.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
				1.0f, 2, 10, 20, 9.0f, 10.0f,
				false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
		else if (g_Item[nCntItem].nElement == ITEMELEMENT_AQUA)// 水属性
		{
			// エフェクトを出す
			SetEffect(g_Item[nCntItem].pos, g_Item[nCntItem].rot, 20, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 0.0f, g_Item[nCntItem].Size.y);
			SetParticle(g_Item[nCntItem].pos,
				g_Item[nCntItem].rot,
				D3DXVECTOR3(1.0f, 1.0f, 1.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f),
				5.0f, 2, 20, 7, 3.0f, 20.0f,
				true, D3DXVECTOR3(0.0f, -4.0f, 0.0f));
		}
		else if (g_Item[nCntItem].nElement == ITEMELEMENT_DARK)// 闇属性
		{
			// エフェクトを出す
			SetEffect(g_Item[nCntItem].pos, g_Item[nCntItem].rot, 20, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 0.0f, g_Item[nCntItem].Size.y);
			SetParticle(g_Item[nCntItem].pos,
				g_Item[nCntItem].rot,
				D3DXVECTOR3(1.0f, 1.0f, 1.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
				5.0f, 2, 35, 7, 3.0f, 7.0f,
				false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}
}
//==============================================================================================================
// アイテムがブロックに当たった処理
//==============================================================================================================
void UpdateHitBlockItem(int nCntItem)
{
	if (CollisionBlockItem(&g_Item[nCntItem].pos, &g_Item[nCntItem].posOld, &g_Item[nCntItem].move, &g_Item[nCntItem].Size))
	{
		g_Item[nCntItem].bUse = false;
		SetExplosion(g_Item[nCntItem].pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 60, 30.0f, 30.0f, EXPLOSION_HIT);
	}
}
//==============================================================================================================
// 近くのアイテムの処理
//==============================================================================================================
void UpdateNearItem(int nCntItem)
{
	// アイテムの状態がノーマルとリリース以外だったら
	if (g_Item[nCntItem].state != ITEMSTATE_NORMAL && g_Item[nCntItem].state != ITEMSTATE_RELEASE) return;

	// アイテムを拾える範囲に入った
	if (CollisionItem(nCntItem, 30.0f, 30.0f) == true)
	{
		// 拾えるアイテムの更新
		PickUpItemAnimation(nCntItem);
	}
	else
	{
		// イージングのカウントをリセット
		g_Item[nCntItem].nEasingCnt = 0;
		g_Item[nCntItem].pos.y += SetSmoothAprroach(0.0f, g_Item[nCntItem].pos.y, 0.1f);
		g_Item[nCntItem].rot.x += SetSmoothAprroach(0.0f, g_Item[nCntItem].rot.x, 0.1f);;
		g_Item[nCntItem].rot.y += SetSmoothAprroach(0.0f, g_Item[nCntItem].rot.y, 0.1f);;
	}
}
//==============================================================================================================
// 投げたアイテムの寿命管理処理
//==============================================================================================================
void UpdateThrowItemLife(int nCntItem)
{
	// アイテムの状態が投げられた状態じゃなかったら関数を抜ける
	if (g_Item[nCntItem].state != ITEMSTATE_THROW) return;

	// 寿命を減らす
	g_Item[nCntItem].nLife--; // デクリメント

	// アイテムの寿命が尽きたら消す
	if (g_Item[nCntItem].nLife <= 0) g_Item[nCntItem].bUse = false;
}
