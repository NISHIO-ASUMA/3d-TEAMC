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

//**************************************************************************************************************
//マクロ定義
//**************************************************************************************************************
#define MAX_WORD (256)
#define MAX_DURABILITY (100) // 耐久力

//**************************************************************************************************************
//プロトタイプ宣言
//**************************************************************************************************************
void LoadItemModel(void); // アイテムのロード処理
void CraftItem(int nCntItem);
void CraftMixItem(int nCntItem,int MixItem,int motionchange);
void EnableCraftIcon(int nCntItem, int Item1, int Item2, int MixItem);
bool CraftItemController(int nCntItem, int Item1, int HoldItem);

//**************************************************************************************************************
//グローバル変数宣言
//**************************************************************************************************************
Item g_Item[MAX_ITEM];			// 構造体変数
int g_ItemTypeMax;				// 種類数
Item g_TexItem[ITEMTYPE_MAX];	// テクスチャ関係
bool bFIrstCraftItem = false;

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
		g_Item[nCntItem].nLife = 20;						   // 体力
		g_Item[nCntItem].state = ITEMSTATE_NORMAL;			   // 状態
		g_Item[nCntItem].fRadius = 100.0f;					   // 半径
		g_Item[nCntItem].nLife = 180;						   // 体力
		g_Item[nCntItem].durability = MAX_DURABILITY;		   // 耐久力
		g_Item[nCntItem].EnableCraft = false;				   // クラフトできるか否か

		for (int nCntNum = 0; nCntNum < ITEMTYPE_MAX; nCntNum++)
		{
			g_Item[nCntItem].bMixItem[nCntNum] = false;				   // クラフト後のアイテム表示用フラグ
		}
	}

	LoadItemModel(); // アイテムのロード処理
	bFIrstCraftItem = false;

	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		//g_TexItem[nCntNum].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//g_TexItem[nCntNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//g_TexItem[nCntNum].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_TexItem[nCntNum].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);// 拡大率
		g_TexItem[nCntNum].nLife = 120;							// 体力
		g_TexItem[nCntNum].durability = MAX_DURABILITY;			// 耐久力
		g_TexItem[nCntNum].state = ITEMSTATE_NORMAL;			// 状態
		g_TexItem[nCntNum].nType = nCntNum;			            // 番号
		g_TexItem[nCntNum].nElement = ITEMELEMENT_STANDARD;     // 初期化
		ElementChange(nCntNum);
		g_TexItem[nCntNum].bMixItem[nCntNum] = false;           // クラフト後のアイテム表示用フラグ

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

	Player* pPlayer = GetPlayer();
}
//===============================================================================================================
//ブロックの終了処理
//===============================================================================================================
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
//===============================================================================================================
//ブロックの更新処理
//===============================================================================================================
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

		// プレイヤーのアイテムを刀にする
		if (bFIrstCraftItem == false && g_Item[nCntItem].nType == ITEMTYPE_KATANA)
		{
			// アイテム変更
			Itemchange(g_Item[nCntItem].nType);

			// モーションの変更
			MotionChange(MOTION_KATANA, 0);

			// アイテムのインデックスを保存
			pPlayer->ItemIdx = nCntItem;

			// ステータス変更
			StatusChange(3.1f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 50);

			// 最初に切り替えた
			bFIrstCraftItem = true;
		}

		// 投げられたアイテムにエフェクト
		if (g_Item[nCntItem].state == ITEMSTATE_THROW)
		{
			int nType = g_Item[nCntItem].nType;
			g_Item[nCntItem].nElement = g_TexItem[nType].nElement;
			if (g_Item[nCntItem].nElement == ITEMELEMENT_STANDARD)
			{
				SetParticle(g_Item[nCntItem].pos,
					g_Item[nCntItem].rot,
					D3DXVECTOR3(1.0f, 1.0f, 1.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f),
					2.0f, 2, 20, 7, 3.0f, 10.0f,
					false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
			else if(g_Item[nCntItem].nElement == ITEMELEMENT_BLOOD)
			{
				SetParticle(g_Item[nCntItem].pos,
					g_Item[nCntItem].rot,
					D3DXVECTOR3(1.0f, 1.0f, 1.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
					3.0f, 2, 20, 7, 3.0f, 10.0f,
					false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
			else if (g_Item[nCntItem].nElement == ITEMELEMENT_FIRE)
			{
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
			else if (g_Item[nCntItem].nElement == ITEMELEMENT_FREEZE)
			{
				SetParticle(g_Item[nCntItem].pos,
					g_Item[nCntItem].rot,
					D3DXVECTOR3(1.0f, 1.0f, 1.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					2.0f, 2, 50, 7, 0.5f, 10.0f,
					false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
			else if (g_Item[nCntItem].nElement == ITEMELEMENT_SPARK)
			{
				SetParticle(g_Item[nCntItem].pos,
					g_Item[nCntItem].rot,
					D3DXVECTOR3(2.0f, 2.0f, 2.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
					1.0f, 2, 10, 20, 9.0f, 10.0f,
					false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
			else if (g_Item[nCntItem].nElement == ITEMELEMENT_AQUA)
			{
				SetParticle(g_Item[nCntItem].pos,
					g_Item[nCntItem].rot,
					D3DXVECTOR3(1.0f, 1.0f, 1.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f),
					5.0f, 2, 20, 7, 3.0f, 20.0f,
					true, D3DXVECTOR3(0.0f, -4.0f, 0.0f));
			}
			else if (g_Item[nCntItem].nElement == ITEMELEMENT_DARK)
			{
				SetParticle(g_Item[nCntItem].pos,
					g_Item[nCntItem].rot,
					D3DXVECTOR3(1.0f, 1.0f, 1.0f),
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
					5.0f, 2, 45, 7, 3.0f, 7.0f,
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

		// プレイヤーがクラフト状態だったら
		if (pPlayer->bCraft)
		{
			CraftItem(nCntItem);
		}

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
//=========================================================================================================
//ブロックの設定処理
//=========================================================================================================
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
void Itemchange(int nType)
{
	// プレイヤーの取得
	Player* pPlayer = GetPlayer();

	pPlayer->Motion.aModel[15].dwNumMat = g_TexItem[nType].ItemTex[nType].g_dwNumMatItem; // アイテムのマテリアルの情報を代入
	pPlayer->Motion.aModel[15].pBuffMat = g_TexItem[nType].ItemTex[nType].g_pBuffMatItem; // アイテムのバッファの情報を代入
	pPlayer->Motion.aModel[15].pMesh = g_TexItem[nType].ItemTex[nType].g_pMeshItem;       // アイテムのメッシュの情報を代入
	pPlayer->nElement = g_TexItem[nType].nElement; // アイテムの属性情報を代入

	//if (g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD)
	//{
	//	g_Item[pPlayer->ItemIdx].nType = nType;
	//}
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

//==============================================================================================================
// アイテムの属性変更処理
//==============================================================================================================
void ElementChange(int nTypeItem)
{
	// 属性を武器ごとに判別する
	if (g_TexItem[nTypeItem].nType == 0) // バットの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == 1) // ゴルフクラブの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == 2) // ハンマーの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if(g_TexItem[nTypeItem].nType == 3) // 石の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == 4) // 角材の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == 5) // 石付きバットの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_BLOOD;
	}
	else if (g_TexItem[nTypeItem].nType == 6) // 蛍光灯の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_SPARK;
	}
	else if (g_TexItem[nTypeItem].nType == 7) // 電撃剣の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_SPARK;
	}
	else if (g_TexItem[nTypeItem].nType == 8) // ハリセンの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == 9) // 氷塊の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_FREEZE;
	}
	else if (g_TexItem[nTypeItem].nType == 10) // 凍結剣の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_FREEZE;
	}
	else if (g_TexItem[nTypeItem].nType == 11) // 鉄パイプの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_BLOOD;
	}
	else if (g_TexItem[nTypeItem].nType == 12) // 金属バットの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_BLOOD;
	}
	else if (g_TexItem[nTypeItem].nType == 13) // サーフボードの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_AQUA;
	}
	else if (g_TexItem[nTypeItem].nType == 14) // 松明の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_FIRE;
	}
	else if (g_TexItem[nTypeItem].nType == 15) // 猛火剣の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_FIRE;
	}
	else if (g_TexItem[nTypeItem].nType == 16) // バールの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_BLOOD;
	}
	else if (g_TexItem[nTypeItem].nType == 17) // 頭像の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == 18) // マネキンの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == 19) // メガホンの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == 20) // すっぽんの属性(仮)
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == 21) // 電柱の属性(仮)
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_BLOOD;
	}
	else if (g_TexItem[nTypeItem].nType == 22) // トルソーの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == 23) // 拡声器の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_SPARK;
	}
	else if (g_TexItem[nTypeItem].nType == 24) // 骨槍の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_DARK;
	}
	else if (g_TexItem[nTypeItem].nType == 25) // 魚の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_AQUA;
	}
	else if (g_TexItem[nTypeItem].nType == 26) // 呪物の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_DARK;
	}
	else if (g_TexItem[nTypeItem].nType == 27) // ハープの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_DARK;
	}
	else if (g_TexItem[nTypeItem].nType == 28) // 鮫浮き輪の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_AQUA;
	}
	else if (g_TexItem[nTypeItem].nType == 29) // 看板の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_BLOOD;
	}
	else if (g_TexItem[nTypeItem].nType == 30) // 初期刀の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
}
//==============================================================================================================
// アイテムのクラフト
//==============================================================================================================
void CraftItemRange(BLOCK* pBlock)
{
	Player* pPlayer = GetPlayer();

	for (int nCnt = 0; nCnt < MAX_ITEM; nCnt++)
	{
		if (!pBlock->bUse && pBlock->nType != BLOCKTYPE_WORKBENCH)
		{
			continue;
		}

		//// クラフト範囲内に入った
		//if (sphererange(&pBlock->pos, &g_Item[nCnt].pos, 100.0f, 50.0f))
		//{
		//	// クラフト可能なアイテム
		//	g_Item[nCnt].EnableCraft = true;
		//}
		//else
		//{
		//	// クラフトできないアイテム
		//	g_Item[nCnt].EnableCraft = false;
		//}
	}
}
//==============================================================================================================
// アイテムのクラフト
//==============================================================================================================
void CraftItem(int nCntItem)
{
	Player* pPlayer = GetPlayer();
	Craftui* pMix = GetMixUI();

	// キーを押したら
	if (OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_A))
	{
		if (CraftItemController(nCntItem,WEPONTYPE_STONE,WEPONTYPE_BAT))
		{				// アイテムをクラフトした時の処理
			CraftMixItem(nCntItem, WEPONTYPE_STONEBAT, MOTION_DBHAND);

			// ステータスの変更
			StatusChange(3.1f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 90);
		}

		// 氷の剣の材料がそろった
		if ((g_Item[nCntItem].nType == ITEMTYPE_ICEBLOCK && g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_KATANA &&
			g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD) ||
			(g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_ICEBLOCK && g_Item[nCntItem].nType == ITEMTYPE_KATANA) &&
			g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD)
		{
			// アイテムをクラフトした時の処理
			CraftMixItem(nCntItem, ITEMTYPE_ICEBLOCKSOWRD, MOTION_KATANA);

			// ステータスの変更
			StatusChange(3.1f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 90);
		}
	}

	// クラフトアイコンを表示するかしないか
	EnableCraftIcon(nCntItem, WEPONTYPE_STONE, WEPONTYPE_BAT, WEPONTYPE_STONEBAT);
	EnableCraftIcon(nCntItem, ITEMTYPE_ICEBLOCK, ITEMTYPE_KATANA, ITEMTYPE_ICEBLOCKSOWRD);
}
//==============================================================================================================
// クラフト先のアイテム
//==============================================================================================================
void CraftMixItem(int nCntItem, int MixItem, int motionchange)
{
	Player* pPlayer = GetPlayer();

	// 火花っぽいエフェクト
	SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y + 30.0f, g_Item[nCntItem].pos.z),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(3.14f, 3.14f, 3.14f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f),
		2.0f, 2, 40, 40, 5.0f, 2.0f, false,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y + 30.0f, g_Item[nCntItem].pos.z),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(3.14f, 3.14f, 3.14f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
		3.0f, 2, 40, 40, 3.0f, 2.0f, false,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y + 30.0f, g_Item[nCntItem].pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), 3.0f, 2, 30, 10, 10.0f, 40.0f, true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
	SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y + 30.0f, g_Item[nCntItem].pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 3.0f, 2, 30, 10, 10.0f, 40.0f, true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
	
	// SEを出す
	PlaySound(SOUND_LABEL_CRAFT);

	// 持っているアイテムを変更
	Itemchange(MixItem);

	// モーションの変更
	MotionChange(motionchange, 0);

	// クラフトに使ったアイテムを消す
	g_Item[nCntItem].bUse = false;

	// 手に持ってるアイテムの種類を石破っと
	g_Item[pPlayer->ItemIdx].nType = MixItem;
}
//==============================================================================================================
// クラフト先のアイテムのアイコンの表示
//==============================================================================================================
void EnableCraftIcon(int nCntItem, int Item1, int Item2, int MixItem)
{
	Player* pPlayer = GetPlayer();

	// 石バットの素材が範囲内にある時
	if ((g_Item[nCntItem].nType == Item1 && g_Item[pPlayer->ItemIdx].nType == Item2 &&
		g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD) ||
		(g_Item[pPlayer->ItemIdx].nType == Item1 && g_Item[nCntItem].nType == Item2) &&
		g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD)
	{
		// アイコンを表示する
		g_Item[nCntItem].bMixItem[MixItem] = true;
	}
	else
	{
		// アイコンを消す
		g_Item[nCntItem].bMixItem[MixItem] = false;
	}
}
//==============================================================================================================
// アイテムの材料がそろった
//==============================================================================================================
bool CraftItemController(int nCntItem, int Item1, int HoldItem)
{
	static bool bCraft = false;

	Player* pPlayer = GetPlayer();

	//// アイテムをクラフトした時の処理
	//CraftMixItem(nCntItem, WEPONTYPE_STONEBAT, MOTION_DBHAND);

	//// ステータスの変更
	//StatusChange(3.1f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 90);

	// ストーンバットの材料がそろった
	if ((g_Item[nCntItem].nType == Item1 &&
		g_Item[pPlayer->ItemIdx].nType == HoldItem &&
		g_Item[nCntItem].state == ITEMSTATE_STOCK && 
		g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD))
	{
		bCraft = true;
	}
	else
	{
		bCraft = false;
	}

	return bCraft;
}
