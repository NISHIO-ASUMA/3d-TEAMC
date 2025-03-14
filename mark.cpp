//==============================================================================================================
//
// マーク処理　[ mark.cpp ]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================
#include "mark.h"
#include "player.h"
#include "boss.h"
#include "math.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define MARK_TEXTURE (10)						  // テクスチャの数
#define FILE_NAME ("data/MODEL/OBJ/yajirusi.x")   // 読み込むファイル名

//**************************************************************************************************************
// グローバル変数
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMark[MARK_TEXTURE] = {};	//プレイヤーのテクスチャへのポインタ
LPD3DXMESH g_pMeshMark;							//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatMark;						//マテリアルへのポインタ
DWORD g_dwNumMatMark;								//マテリアルの数
Mark g_Mark;

//=========================================================================================================
// 矢印の初期化処理
//=========================================================================================================
void InitMark(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	g_Mark.pos = NULLVECTOR3;
	g_Mark.rot = NULLVECTOR3;
	g_Mark.move = NULLVECTOR3;
	g_Mark.offpos = NULLVECTOR3;
	g_Mark.offrot = NULLVECTOR3;
	g_Mark.bUse = false;

	//Xファイルの読み込み
	D3DXLoadMeshFromX(FILE_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatMark,
		NULL,
		&g_dwNumMatMark,
		&g_pMeshMark);

	D3DXMATERIAL* pMat;//マテリアルへのポインタ

	//マテリアルのデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatMark->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatMark; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//このファイル名を使用してテクスチャを読み込む
				//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureMark[nCntMat]);
		}
	}

}
//=========================================================================================================
// 矢印の終了処理
//=========================================================================================================
void UninitMark(void)
{
	for (int nCnt = 0; nCnt < (int)g_dwNumMatMark; nCnt++)
	{
		if (g_pTextureMark[nCnt] != NULL)
		{
			g_pTextureMark[nCnt]->Release();
			g_pTextureMark[nCnt] = NULL;
		}
	}
		//メッシュの破棄
	if (g_pMeshMark != NULL)
	{
		g_pMeshMark->Release();
		g_pMeshMark = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatMark != NULL)
	{
		g_pBuffMatMark->Release();
		g_pBuffMatMark = NULL;
	}
}
//=========================================================================================================
// 矢印の更新処理
//=========================================================================================================
void UpdateMark(void)
{
	// 使用状態じゃない時
	if (g_Mark.bUse == false)
	{
		// 関数を抜ける
		return;
	}

	// 矢印の角度の正規化
	if (g_Mark.rotDest.y - g_Mark.rot.y >= D3DX_PI)
	{
		g_Mark.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_Mark.rotDest.y - g_Mark.rot.y <= -D3DX_PI)
	{
		g_Mark.rot.y -= D3DX_PI * 2.0f;
	}

	g_Mark.rot.y += SetSmoothAprroach(g_Mark.rotDest.y,g_Mark.rot.y,0.1f);
}
//=========================================================================================================
// 矢印の描画処理
//=========================================================================================================
void DrawMark(void)
{
	Player* pPlayer = GetPlayer();

	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Mark.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Mark.rot.y, g_Mark.rot.x, g_Mark.rot.z);
	D3DXMatrixMultiply(&g_Mark.mtxWorld, &g_Mark.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, pPlayer->pos.x, pPlayer->pos.y + 150.0f, pPlayer->pos.z);
	D3DXMatrixMultiply(&g_Mark.mtxWorld, &g_Mark.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Mark.mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルのデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatMark->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatMark; nCntMat++)
	{
		if (g_Mark.bUse == true)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMark[nCntMat]);

			//モデル(パーツ)の描画
			g_pMeshMark->DrawSubset(nCntMat);
		}
	}

	//保存していたマテリアルを元に戻す
	pDevice->SetMaterial(&matDef);

}
//=========================================================================================================
// 矢印の設定処理
//=========================================================================================================
void SetMark(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (g_Mark.bUse == false)
	{
		g_Mark.pos = pos;
		g_Mark.rot = rot;
		g_Mark.bUse = true;
	}
}
//=========================================================================================================
// 矢印の取得
//=========================================================================================================
Mark* GetMark(void)
{
	return &g_Mark;
}
