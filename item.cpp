//============================
//
// アイテム[item.cpp]
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
#include"player.h"

//****************************
//マクロ定義
//****************************
#define MOUSE_SIZE (50.0f)
#define MAX_WORD (256)

//****************************
//プロトタイプ宣言
//****************************
void SetMtxItem(int nCnt);
void LoadItemModel(void); // アイテムのロード処理

//****************************
//グローバル変数宣言
//****************************
Item g_Item[MAX_ITEM];
int g_ItemTypeMax;
Item g_TexItem[ITEMTYPE_MAX];

//=============================
//ブロックの初期化処理
//=============================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_Item[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //座標
		g_Item[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//移動量
		g_Item[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //角度
		g_Item[nCntItem].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);//拡大率
		g_Item[nCntItem].nType = ITEMTYPE_BAT;				  //種類
		g_Item[nCntItem].bUse = false;						  //未使用判定
		g_Item[nCntItem].nLife = 20;						  //体力
		g_Item[nCntItem].state = ITEMSTATE_NORMAL;			  //状態
		g_Item[nCntItem].fRadius = 100.0f;					  //半径
		g_Item[nCntItem].nLife = 180;						  //体力
	}

	LoadItemModel(); // アイテムのロード処理

	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		//g_TexItem[nCntNum].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//g_TexItem[nCntNum].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//g_TexItem[nCntNum].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_TexItem[nCntNum].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);//拡大率
		g_TexItem[nCntNum].nLife = 120;							//体力

		D3DXMATERIAL* pMat;//マテリアルへのポインタ

		//マテリアルのデータへのポインタを取得
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

	//頂点座標の取得
	int nNumVtx;//頂点数
	DWORD sizeFVF;//頂点フォーマットのサイズ
	BYTE* pVtxBuff;//頂点バッファへのポインタ

	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		//頂点数の取得
		nNumVtx = g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem->GetNumVertices();

		//頂点フォーマットのサイズ取得
		sizeFVF = D3DXGetFVFVertexSize(g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem->GetFVF());

		//頂点バッファのロック
		g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//頂点座標を比較してブロックの最小値,最大値を取得
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

			//頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;

			//サイズを代入
			g_TexItem[nCntNum].Size.x = g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.x - g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.x;
			g_TexItem[nCntNum].Size.y = g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.y - g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.y;
			g_TexItem[nCntNum].Size.z = g_TexItem[nCntNum].ItemTex[nCntNum].vtxMax.z - g_TexItem[nCntNum].ItemTex[nCntNum].vtxMin.z;
		}


		//頂点バッファのアンロック
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
		//テクスチャの破棄
		for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
		{
			if (g_TexItem[nCntNum].ItemTex[nCntNum].g_apTextureItem[nCntTex] != NULL)
			{
				g_TexItem[nCntNum].ItemTex[nCntNum].g_apTextureItem[nCntTex]->Release();
				g_TexItem[nCntNum].ItemTex[nCntNum].g_apTextureItem[nCntTex] = NULL;
			}
		}

		//メッシュの破棄
		if (g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem != NULL)
		{
			g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem->Release();
			g_TexItem[nCntNum].ItemTex[nCntNum].g_pMeshItem = NULL;
		}

		//マテリアル
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
	Player* pPlayer = GetPlayer();//プレイヤー取得

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (!g_Item[nCntItem].bUse)
		{
			//使用中じゃなかったら
			continue;
		}

		if (g_Item[nCntItem].state == ITEMSTATE_HOLD && KeyboardTrigger(DIK_RETURN))
		{
			//プレイヤーの向いている方向に飛ばす
			g_Item[nCntItem].move.x = sinf(pPlayer->rot.y + D3DX_PI) * 10.0f;
			g_Item[nCntItem].move.y = 0.0f;
			g_Item[nCntItem].move.z = cosf(pPlayer->rot.y + D3DX_PI) * 10.0f;

			//発射地点を親の位置にする
			g_Item[nCntItem].pos.x = pPlayer->Motion.aModel[PARENT].mtxWorld._41;
			g_Item[nCntItem].pos.y = pPlayer->Motion.aModel[PARENT].mtxWorld._42;
			g_Item[nCntItem].pos.z = pPlayer->Motion.aModel[PARENT].mtxWorld._43;

			//ブロックの投げられている状態にする
			g_Item[nCntItem].state = ITEMSTATE_THROW;

			//プレイヤーを何も持っていない状態にする
			pPlayer->HandState = PLAYERHOLD_NO;
		}
		if (g_Item[nCntItem].state == ITEMSTATE_HOLD && KeyboardTrigger(DIK_O))
		{
			g_Item[nCntItem].state = ITEMSTATE_NORMAL;
			pPlayer->HandState = PLAYERHOLD_NO;

			//発射地点を親の位置にする
			g_Item[nCntItem].pos.x = pPlayer->pos.x;
			g_Item[nCntItem].pos.y = 0.0f;
			g_Item[nCntItem].pos.z = pPlayer->pos.z;

		}

		if (g_Item[nCntItem].state == ITEMSTATE_THROW)
		{
			//体力を減らす
			g_Item[nCntItem].nLife--;

			if (g_Item[nCntItem].nLife <= 0)
			{//体力が0以下なら
				//未使用判定
				g_Item[nCntItem].bUse = false;
			}
		}

		//位置の更新
		g_Item[nCntItem].pos += g_Item[nCntItem].move;

		CollisionItem(nCntItem,// アイテムのインデックスを渡す
			20.0f, // アイテムの半径
			20.0f); // プレイヤーの半径
	}

}
//=============================
//ブロックの描画処理
//=============================
void DrawItem(void)
{
	MODE mode = GetMode();//現在のモードを取得

	Player* pPlayer = GetPlayer();//プレイヤーの取得

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal, mtxParent;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	for (int nCntNum = 0; nCntNum < g_ItemTypeMax; nCntNum++)
	{
		for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
		{
			if (!g_Item[nCntItem].bUse)
			{//未使用状態なら
				//下の処理を通さずカウントを進める
				continue;
			}

			if (g_Item[nCntItem].state == ITEMSTATE_HOLD)
			{
				//取得処理
				SetMtxItem(nCntItem);
			}
			else
			{
				//ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_Item[nCntItem].mtxWorldItem);

				//大きさを反映
				D3DXMatrixScaling(&mtxScal, g_Item[nCntItem].Scal.y, g_Item[nCntItem].Scal.x, g_Item[nCntItem].Scal.z);
				D3DXMatrixMultiply(&g_Item[nCntItem].mtxWorldItem, &g_Item[nCntItem].mtxWorldItem, &mtxScal);

				//向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Item[nCntItem].rot.y, g_Item[nCntItem].rot.x, g_Item[nCntItem].rot.z);
				D3DXMatrixMultiply(&g_Item[nCntItem].mtxWorldItem, &g_Item[nCntItem].mtxWorldItem, &mtxRot);

				//位置を反映
				D3DXMatrixTranslation(&mtxTrans, g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y, g_Item[nCntItem].pos.z);
				D3DXMatrixMultiply(&g_Item[nCntItem].mtxWorldItem, &g_Item[nCntItem].mtxWorldItem, &mtxTrans);

				//ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_Item[nCntItem].mtxWorldItem);
			}

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			for (int nCntMat = 0; nCntMat < (int)g_Item[nCntItem].ItemTex[nCntNum].g_dwNumMatItem; nCntMat++)
			{
				//マテリアルのデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_Item[nCntItem].ItemTex[nCntNum].g_pBuffMatItem->GetBufferPointer();

				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_Item[nCntItem].ItemTex[nCntNum].g_apTextureItem[nCntMat]);

				//ブロック(パーツ)の描画
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
		{//未使用状態なら
			g_Item[nCntItem] = g_TexItem[nType]; // 必要な情報を代入

			g_Item[nCntItem].pos = pos;	   //座標
			g_Item[nCntItem].nType = nType;//種類
			g_Item[nCntItem].Scal = Scal;  //拡大率
			g_Item[nCntItem].bUse = true;  //使用判定

			break;
		}
	}
}
//=======================
//ブロックの取得処理
//=======================
void Itemchange(int nType)
{
	Player* pPlayer = GetPlayer();

	pPlayer->Motion.aModel[15].dwNumMat = g_TexItem[nType].ItemTex[nType].g_dwNumMatItem;
	pPlayer->Motion.aModel[15].pBuffMat = g_TexItem[nType].ItemTex[nType].g_pBuffMatItem;
	pPlayer->Motion.aModel[15].pMesh = g_TexItem[nType].ItemTex[nType].g_pMeshItem;
}
//=======================
//アイテムの取得
//=======================
Item* GetItem(void)
{
	return &g_Item[0];
}
////=======================
////ブロックの取得処理
////=======================
//ITEM* GetItem()
//{
//	return &g_Item[0];
//}
//int NumItem(void)
//{
//	return g_NumItem;
//}
//=======================
//ブロックの親処理
//=======================
void SetMtxItem(int nCnt)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal, mtxParent;

	Player* pPlayer = GetPlayer();//プレイヤーの取得

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Item[nCnt].mtxWorldItem);

	//大きさを反映
	D3DXMatrixScaling(&mtxScal, g_Item[nCnt].Scal.y, g_Item[nCnt].Scal.x, g_Item[nCnt].Scal.z);
	D3DXMatrixMultiply(&g_Item[nCnt].mtxWorldItem, &g_Item[nCnt].mtxWorldItem, &mtxScal);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Item[nCnt].rot.y, g_Item[nCnt].rot.x, g_Item[nCnt].rot.z);
	D3DXMatrixMultiply(&g_Item[nCnt].mtxWorldItem, &g_Item[nCnt].mtxWorldItem, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Item[nCnt].pos.x, g_Item[nCnt].pos.y, g_Item[nCnt].pos.z);
	D3DXMatrixMultiply(&g_Item[nCnt].mtxWorldItem, &g_Item[nCnt].mtxWorldItem, &mtxTrans);

	g_Item[nCnt].mtxParent = pPlayer->Motion.aModel[PARENT].mtxWorld;

	//算出した[パーツのワールドマトリックス]と[親のマトリックス]をかけあわせる
	D3DXMatrixMultiply(&g_Item[nCnt].mtxWorldItem,
		&g_Item[nCnt].mtxWorldItem,
		&g_Item[nCnt].mtxParent);//自分自分親

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Item[nCnt].mtxWorldItem);
}
//============================
// アイテムのロード処理
//============================
void LoadItemModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	FILE* pFile; // ファイルのポインタ

	char skip[5];
	int nType = 0;

	pFile = fopen("data\\MODEL_TXT\\ITEM.txt", "r");

	if (pFile != NULL)
	{
		char aString[MAX_WORD];

		while (1)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "MAX_TYPE") == 0)
			{
				fscanf(pFile, "%s", &skip[0]);
				fscanf(pFile, "%d", &g_ItemTypeMax);
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
					&g_TexItem[nType].ItemTex[nType].g_pBuffMatItem,
					NULL,
					&g_TexItem[nType].ItemTex[nType].g_dwNumMatItem,
					&g_TexItem[nType].ItemTex[nType].g_pMeshItem);

				nType++;
			}
			else if (strcmp(aString, "END_SCRIPT") == 0)
			{
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
	fclose(pFile);
}
