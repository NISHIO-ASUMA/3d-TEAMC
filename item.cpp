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

//**************************************************************************************************************
//マクロ定義
//**************************************************************************************************************
#define MAX_WORD (256)
#define MAX_DURABILITY (100) // 耐久力

//**************************************************************************************************************
//プロトタイプ宣言
//**************************************************************************************************************
void LoadItemModel(void); // アイテムのロード処理
void CraftItem(void);
void CraftMixItem(int nCntItem,int MixItem,int motionchange);
void EnableCraftIcon(int nCntItem, int Item1, int Item2, int MixItem);
void LoadDurability(void); // アイテムの耐久力のロード処理
void PickUpItemAnimation(int nCntItem); // アイテムを拾える時の演出

//**************************************************************************************************************
//グローバル変数宣言
//**************************************************************************************************************
Item g_Item[MAX_ITEM];			// 構造体変数
int g_ItemTypeMax;				// 種類数
TEXTURE_INFO g_TexItem[ITEMTYPE_MAX];	// テクスチャ関係
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
		g_Item[nCntItem].nLife = 120;						   // 体力
		g_Item[nCntItem].state = ITEMSTATE_NORMAL;			   // 状態
		g_Item[nCntItem].fRadius = 100.0f;					   // 半径
		g_Item[nCntItem].nLife = 180;						   // 体力
		g_Item[nCntItem].durability = 0;		   // 耐久力
		g_Item[nCntItem].EnableCraft = false;				   // クラフトできるか否か
		g_Item[nCntItem].grabity = 0.0f;				   // クラフトできるか否か
		g_Item[nCntItem].nEasingCnt = 0;
		g_Item[nCntItem].nImpactCount = 110;
		g_Item[nCntItem].Maxdurability = 0;


		for (int nCntNum = 0; nCntNum < ITEMTYPE_MAX; nCntNum++)
		{
			g_Item[nCntItem].bMixItem[nCntNum] = false;				   // クラフト後のアイテム表示用フラグ
		}
	}

	LoadDurability();
	LoadItemModel(); // アイテムのロード処理

	bFIrstCraftItem = false;

	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		g_TexItem[nCntNum].nType = nCntNum;			            // 番号
		g_TexItem[nCntNum].nElement = ITEMELEMENT_STANDARD;     // 初期化
		g_TexItem[nCntNum].Maxdurability = 0;     // 初期化

		ElementChange(nCntNum);
		g_TexItem[nCntNum].Maxdurability = g_TexItem[nCntNum].durability;

		D3DXMATERIAL* pMat; // マテリアルへのポインタ

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

	Player* pPlayer = GetPlayer();
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
		// プレイヤーのアイテムを刀にする
		if (bFIrstCraftItem == false && g_Item[nCntItem].nType == ITEMTYPE_KATANA)
		{
			// アイテム変更
			Itemchange(nCntItem,g_Item[nCntItem].nType);

			// モーションの変更
			MotionChange(MOTION_KATANA, 0);

			// アイテムのインデックスを保存
			pPlayer->ItemIdx = nCntItem;
			g_Item[nCntItem].state = ITEMSTATE_HOLD;

			// ステータス変更
			StatusChange(3.1f, D3DXVECTOR3(0.0f, g_Item[nCntItem].Size.y, 0.0f), 100);

			// 最初に切り替えた
			bFIrstCraftItem = true;
		}

		// 投げられたアイテムにエフェクト
		if (g_Item[nCntItem].state == ITEMSTATE_THROW)
		{
			// アイテムの属性を反映し
			int nType = g_Item[nCntItem].nType;
			g_Item[nCntItem].nElement = g_TexItem[nType].nElement;
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
			else if(g_Item[nCntItem].nElement == ITEMELEMENT_BLOOD)// 出血属性
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

		// 前回の位置を代入
		g_Item[nCntItem].posOld = g_Item[nCntItem].pos;

		// 位置の更新
		g_Item[nCntItem].pos += g_Item[nCntItem].move;

		if (CollisionBlockItem(&g_Item[nCntItem].pos, &g_Item[nCntItem].posOld, &g_Item[nCntItem].move, &g_Item[nCntItem].Size))
		{
			g_Item[nCntItem].bUse = false;
			SetExplosion(g_Item[nCntItem].pos, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 60, 30.0f, 30.0f, EXPLOSION_HIT);
		}

		if (g_Item[nCntItem].state == ITEMSTATE_NORMAL || g_Item[nCntItem].state == ITEMSTATE_RELEASE)
		{
			// アイテムを拾える範囲に入った
			if (CollisionItem(nCntItem, 20.0f, 20.0f) == true)
			{
				// 拾えるアイテムの更新
				PickUpItemAnimation(nCntItem);
			}
			else
			{
				g_Item[nCntItem].nEasingCnt = 0;
				g_Item[nCntItem].pos.y += SetSmoothAprroach(0.0f, g_Item[nCntItem].pos.y, 0.1f);
				g_Item[nCntItem].rot.x += SetSmoothAprroach(0.0f, g_Item[nCntItem].rot.x, 0.1f);;
				g_Item[nCntItem].rot.y += SetSmoothAprroach(0.0f, g_Item[nCntItem].rot.y, 0.1f);;
			}
		}

		//// 角度の正規化X
		//if (g_Item[nCntItem].rot.x > D3DX_PI)
		//{
		//	g_Item[nCntItem].rot.x += -D3DX_PI * 2.0f;
		//}
		//else if (g_Item[nCntItem].rot.x < D3DX_PI)
		//{
		//	g_Item[nCntItem].rot.x += D3DX_PI * 2.0f;
		//}

		//// 角度の正規化Y
		//if (g_Item[nCntItem].rot.y > D3DX_PI)
		//{
		//	g_Item[nCntItem].rot.y += -D3DX_PI * 2.0f;
		//}
		//else if (g_Item[nCntItem].rot.y < D3DX_PI)
		//{
		//	g_Item[nCntItem].rot.y += D3DX_PI * 2.0f;
		//}

		// プレイヤーがクラフト状態だったら
		if (pPlayer->bCraft == true)
		{
			CraftItem();
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

			for (int nCntMat = 0; nCntMat < (int)g_Item[nCntItem].ItemTex[nCntNum].g_dwNumMatModel; nCntMat++)
			{
				// マテリアルのデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_Item[nCntItem].ItemTex[nCntNum].g_pBuffMatModel->GetBufferPointer();

				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_Item[nCntItem].ItemTex[nCntNum].g_apTextureModel[nCntMat]);

				// ブロック(パーツ)の描画
				g_Item[nCntItem].ItemTex[nCntNum].g_pMeshModel->DrawSubset(nCntMat);
			}
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
	pPlayer->nElement = g_TexItem[nType].nElement; // アイテムの属性情報を代入

	// 同じインデックスじゃないかつ耐久値が減っていないなら
	if (nIdx != pPlayer->ItemIdx && g_Item[nIdx].durability == g_Item[nIdx].Maxdurability)
	{
		// 耐久力を代入
		g_Item[nIdx].durability = g_TexItem[nType].durability;
		g_Item[nIdx].Maxdurability = g_TexItem[nType].Maxdurability;
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
					&g_TexItem[nType].g_pBuffMatModel,
					NULL,
					&g_TexItem[nType].g_dwNumMatModel,
					&g_TexItem[nType].g_pMeshModel);

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
	if (g_TexItem[nTypeItem].nType == ITEMTYPE_BAT) // バットの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_GOLF) // ゴルフクラブの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_HUNMER) // ハンマーの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if(g_TexItem[nTypeItem].nType == ITEMTYPE_STONE) // 石の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_WOOD) // 角材の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_STONEBAT) // 石付きバットの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_BLOOD;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_LIGHT) // 蛍光灯の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_SPARK;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_LIGHTWOOD) // 電撃剣の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_SPARK;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_HARISEN) // ハリセンの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_ICEBLOCK) // 氷塊の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_FREEZE;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_ICEBLOCKSOWRD) // 凍結剣の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_FREEZE;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_IRON) // 鉄パイプの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_BLOOD;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_IRONBAT) // 金属バットの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_BLOOD;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_SURFBOARD) // サーフボードの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_AQUA;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_TORCH) // 松明の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_FIRE;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_TORCHSWORD) // 猛火剣の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_FIRE;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_BAR) // バールの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_BLOOD;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_HEADSTATUE) // 頭像の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_HEADSTATUTORSO) // マネキンの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_MEGAPHONE) // メガホンの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_RUBBERCUP) // すっぽんの属性(仮)
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_TELEPHONEPOLE) // 電柱の属性(仮)
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_BLOOD;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_TORSO) // トルソーの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_FLUORESCENTLIGHTMEGAPHONE) // 拡声器の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_SPARK;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_BONESPEAR) // 骨槍の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_DARK;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_FISH) // 魚の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_AQUA;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_HEX) // 呪物の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_DARK;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_HEXMANDOLIN) // ハープの属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_DARK;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_SURFBOARDFISH) // 鮫浮き輪の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_AQUA;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_TUTORIAL) // 看板の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_BLOOD;
	}
	else if (g_TexItem[nTypeItem].nType == ITEMTYPE_KATANA) // 初期刀の属性
	{
		g_TexItem[nTypeItem].nElement = ITEMELEMENT_STANDARD;
	}
}
//==============================================================================================================
// アイテムの取得
//==============================================================================================================
TEXTURE_INFO* GetItemOrigin(void)
{
	return &g_TexItem[0];
}
//==============================================================================================================
// アイテムのクラフト
//==============================================================================================================
void CraftItem(void)
{
	Player* pPlayer = GetPlayer();
	Craftui* pMix = GetMixUI();

	// キーを押したら
	for (int nCnt = 0; nCnt < MAX_ITEM; nCnt++)
	{
		if (OnMouseTriggerDown(LEFT_MOUSE) || JoypadTrigger(JOYKEY_A))
		{
			// ストーンバットの材料がそろった
			if (((g_Item[nCnt].nType == ITEMTYPE_STONE && g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_BAT || (g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_STONE && g_Item[nCnt].nType == ITEMTYPE_BAT))
				&& g_Item[nCnt].state == ITEMSTATE_STOCK && g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD))
			{
				// クラフト後のアイテムの処理
				CraftMixItem(nCnt, ITEMTYPE_STONEBAT, MOTION_DBHAND);

				// ステータスの変更
				StatusChange(3.1f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 150);

				g_Item[pPlayer->StockItemIdx].state = ITEMSTATE_NORMAL;
			}
			// 氷の剣の材料がそろった
			if (((g_Item[nCnt].nType == ITEMTYPE_ICEBLOCK && g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_KATANA || (g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_ICEBLOCK && g_Item[nCnt].nType == ITEMTYPE_KATANA))
				&& g_Item[nCnt].state == ITEMSTATE_STOCK && g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD))
			{
				CraftMixItem(nCnt, ITEMTYPE_ICEBLOCKSOWRD, MOTION_KATANA);

				// ステータスの変更
				StatusChange(3.1f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 150);

				g_Item[pPlayer->StockItemIdx].state = ITEMSTATE_NORMAL;
			}
			// 炎の剣の材料がそろった
			if (((g_Item[nCnt].nType == ITEMTYPE_TORCH && g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_KATANA || (g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_TORCH && g_Item[nCnt].nType == ITEMTYPE_KATANA))
				&& g_Item[nCnt].state == ITEMSTATE_STOCK && g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD))
			{
				CraftMixItem(nCnt, ITEMTYPE_TORCHSWORD, MOTION_KATANA);

				// ステータスの変更
				StatusChange(3.1f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 150);

				g_Item[pPlayer->StockItemIdx].state = ITEMSTATE_NORMAL;
			}
			// 雷の剣の材料がそろった
			if (((g_Item[nCnt].nType == ITEMTYPE_LIGHT && g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_WOOD || (g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_LIGHT && g_Item[nCnt].nType == ITEMTYPE_WOOD))
				&& g_Item[nCnt].state == ITEMSTATE_STOCK && g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD))
			{
				CraftMixItem(nCnt, ITEMTYPE_LIGHTWOOD, MOTION_DBHAND);

				// ステータスの変更
				StatusChange(3.1f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 150);

				g_Item[pPlayer->StockItemIdx].state = ITEMSTATE_NORMAL;
			}
			// 金属バットの材料がそろった
			if (((g_Item[nCnt].nType == ITEMTYPE_IRON && g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_BAT || (g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_IRON && g_Item[nCnt].nType == ITEMTYPE_BAT))
				&& g_Item[nCnt].state == ITEMSTATE_STOCK && g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD))
			{
				CraftMixItem(nCnt, ITEMTYPE_IRONBAT, MOTION_DBHAND);

				// ステータスの変更
				StatusChange(3.1f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 150);

				g_Item[pPlayer->StockItemIdx].state = ITEMSTATE_NORMAL;
			}
			// 全身マネキンの材料がそろった
			if (((g_Item[nCnt].nType == ITEMTYPE_HEADSTATUE && g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_TORSO || (g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_HEADSTATUE && g_Item[nCnt].nType == ITEMTYPE_TORSO))
				&& g_Item[nCnt].state == ITEMSTATE_STOCK && g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD))
			{
				CraftMixItem(nCnt, ITEMTYPE_HEADSTATUTORSO, MOTION_BIGWEPON);

				// ステータスの変更
				StatusChange(2.9f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 150);

				g_Item[pPlayer->StockItemIdx].state = ITEMSTATE_NORMAL;
			}
			// 鮫浮き輪の材料がそろった
			if (((g_Item[nCnt].nType == ITEMTYPE_FISH && g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_SURFBOARD || (g_Item[pPlayer->ItemIdx].nType == ITEMTYPE_FISH && g_Item[nCnt].nType == ITEMTYPE_SURFBOARD))
				&& g_Item[nCnt].state == ITEMSTATE_STOCK && g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD))
			{
				CraftMixItem(nCnt, ITEMTYPE_SURFBOARDFISH, MOTION_BIGWEPON);

				// ステータスの変更
				StatusChange(2.9f, D3DXVECTOR3(0.0f, 75.0f, 0.0f), 150);

				g_Item[pPlayer->StockItemIdx].state = ITEMSTATE_NORMAL;
			}
		}

		// クラフトアイコンを表示するかしないか
		EnableCraftIcon(nCnt, ITEMTYPE_STONE, ITEMTYPE_BAT, ITEMTYPE_STONEBAT);
		EnableCraftIcon(nCnt, ITEMTYPE_ICEBLOCK, ITEMTYPE_KATANA, ITEMTYPE_ICEBLOCKSOWRD);
		EnableCraftIcon(nCnt, ITEMTYPE_TORCH, ITEMTYPE_KATANA, ITEMTYPE_TORCHSWORD);
		EnableCraftIcon(nCnt, ITEMTYPE_WOOD, ITEMTYPE_LIGHT, ITEMTYPE_LIGHTWOOD);
		EnableCraftIcon(nCnt, ITEMTYPE_BAT, ITEMTYPE_IRON, ITEMTYPE_IRONBAT);
		EnableCraftIcon(nCnt, ITEMTYPE_HEADSTATUE, ITEMTYPE_TORSO, ITEMTYPE_HEADSTATUTORSO);
		EnableCraftIcon(nCnt, ITEMTYPE_FISH, ITEMTYPE_SURFBOARD, ITEMTYPE_SURFBOARDFISH);
	}
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
		2.0f, 2, 40, 20, 5.0f, 2.0f, false,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y + 30.0f, g_Item[nCntItem].pos.z),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(3.14f, 3.14f, 3.14f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
		3.0f, 2, 40, 20, 3.0f, 2.0f, false,
		D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y + 30.0f, g_Item[nCntItem].pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f), 3.0f, 2, 30, 10, 10.0f, 40.0f, true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
	SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y + 30.0f, g_Item[nCntItem].pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), 3.0f, 2, 30, 10, 10.0f, 40.0f, true, D3DXVECTOR3(0.0f, 4.0f, 0.0f));
	
	// SEを出す
	PlaySound(SOUND_LABEL_CRAFT);

	// 持っているアイテムを変更
	Itemchange(nCntItem,MixItem);

	// クラフトに使ったアイテムを消す
	g_Item[nCntItem].bUse = false;

	// モーションの変更
	MotionChange(motionchange, 0);

	// 種類を代入
	int nType = g_Item[pPlayer->ItemIdx].nType;

	// アイテムの見た目を変える
	g_Item[pPlayer->ItemIdx].ItemTex[nType] = g_TexItem[MixItem];
	g_Item[pPlayer->ItemIdx].durability = g_TexItem[MixItem].durability;
	g_Item[pPlayer->ItemIdx].Maxdurability = g_TexItem[MixItem].durability;

	// 手に持ってるアイテムの種類を石バットにする
	g_Item[pPlayer->ItemIdx].nType = MixItem;
}
//==============================================================================================================
// クラフト先のアイテムのアイコンの表示
//==============================================================================================================
void EnableCraftIcon(int nCntItem, int Item1, int Item2, int MixItem)
{
	Player* pPlayer = GetPlayer();

	// 石バットの素材が範囲内にある時
	if (((g_Item[pPlayer->StockItemIdx].nType == Item1 && g_Item[pPlayer->ItemIdx].nType == Item2) || (g_Item[pPlayer->ItemIdx].nType == Item1 && g_Item[pPlayer->StockItemIdx].nType == Item2)) &&
		g_Item[pPlayer->StockItemIdx].state == ITEMSTATE_STOCK && g_Item[pPlayer->ItemIdx].state == ITEMSTATE_HOLD)
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
// アイテムの耐久力のロード処理
//==============================================================================================================
void LoadDurability(void)
{
	FILE* pFile; // ファイルのポインタ

	// ファイルを開く
	pFile = fopen("data\\item_Info.txt", "r");

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
			int nData = fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "ITEMTYPE") == 0)
			{
				fscanf(pFile, "%s", &skip[0]);
				fscanf(pFile, "%d", &nType);
			}

			if (strcmp(&aString[0], "DURABILITY") == 0)
			{
				fscanf(pFile, "%s", &skip[0]);
				fscanf(pFile, "%d", &g_TexItem[nType].durability);			
			}

			if (strcmp(&aString[0], "END_ITEMSET") == 0)
			{
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
	// インクリメント
	g_Item[nCntItem].nEasingCnt++;
	
	float time = SetEase(g_Item[nCntItem].nEasingCnt,120);

	// 目的の値に近づける
	g_Item[nCntItem].pos.y += SetSmoothAprroach(20.0f, g_Item[nCntItem].pos.y, EaseInOutCubic(time));

	// 目的の値に近づける
	g_Item[nCntItem].rot.x += SetSmoothAprroach(D3DX_PI * 0.15f, g_Item[nCntItem].rot.x, EaseInOutCubic(time));

	// 目的の値に近づける
	g_Item[nCntItem].rot.y += 0.01f;

<<<<<<< HEAD
	// TOOD : 長尾ここにパーティクルを出す処理を頼む
	//SetParticle();
=======
	// 長尾ここに頼む
	if (g_Item[nCntItem].nType == ITEMTYPE_ONIGIRI)
	{
		SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y + g_Item[nCntItem].Size.y / 1.5f, g_Item[nCntItem].pos.z),
			D3DXVECTOR3(1.57f, g_Item[nCntItem].rot.y, 1.57f),
			D3DXVECTOR3(0.2f, 3.14f, 0.2f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f),
			2.0f, 2, 20, 4, 4.0f, 40.0f,
			false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	else
	{
		SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y + g_Item[nCntItem].Size.y / 1.5f, g_Item[nCntItem].pos.z),
			D3DXVECTOR3(1.57f, g_Item[nCntItem].rot.y, 1.57f),
			D3DXVECTOR3(0.2f, 3.14f, 0.2f),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
			2.0f, 2, 20, 4, 4.0f, 40.0f,
			false, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

>>>>>>> 40f9d302e427779171d3abc1ee1be9a0483912d2
	if (g_Item[nCntItem].nImpactCount >= 120)
	{
		SetImpact(D3DXVECTOR3(g_Item[nCntItem].pos.x,0.0f, g_Item[nCntItem].pos.z),COLOR_GOLD,20,10.0f,7.0f,1.0f,60,IMPACTTYPE_NORMAL,0);
		SetImpact(D3DXVECTOR3(g_Item[nCntItem].pos.x, 0.0f, g_Item[nCntItem].pos.z), COLOR_GOLD, 20, 1.0f, 0.0f, 1.0f, 60, IMPACTTYPE_NORMAL, 0);
		g_Item[nCntItem].nImpactCount = 0;
	}
	else
	{
		// 120になるまでインクリメント
		g_Item[nCntItem].nImpactCount++;
	}
}
