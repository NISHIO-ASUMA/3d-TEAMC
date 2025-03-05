//==============================================================================================================
//
// マーク処理　[ mark.cpp ]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================
#include "mark.h"
#include "player.h"
#include "boss.h"

//**************************************************************************************************************
// マクロ定義
//**************************************************************************************************************
#define MARK_TEXTURE (10)						  // テクスチャの数
#define FILE_NAME ("data/MODEL/OBJ/yajirusi.x")   // 読み込むファイル名
#define NULLVECTOR3 (D3DXVECTOR3(0.0f,0.0f,0.0f)) // 初期化用マクロ変数

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
	ENEMY* pEnemy = GetEnemy();
	Player* pPlayer = GetPlayer();
	Boss* pBoss = Getboss();

	bool bFirst = true;
	int nIdxEnemy = 0;

	float fDistanceNow = 0.0f;
	float fDistanceStock = 0.0f;

	// 敵の最大数分求める
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		// 距離を求める
		float DisposX = pEnemy[nCnt].pos.x - pPlayer->pos.x;
		float DisposY = pEnemy[nCnt].pos.y - pPlayer->pos.y;
		float DisposZ = pEnemy[nCnt].pos.z - pPlayer->pos.z;

		// 距離を求める
		fDistanceNow = sqrtf((DisposX * DisposX) + (DisposY * DisposY) + (DisposZ * DisposZ));

		// 最初だけ通す
		if (bFirst == true)
		{
			fDistanceStock = fDistanceNow;
			bFirst = false;
			nIdxEnemy = nCnt;
		}
		else
		{
			// 今の距離がストックされた距離より小さかったら
			if (fDistanceNow < fDistanceStock)
			{
				fDistanceStock = fDistanceNow; // 距離を保存
				nIdxEnemy = nCnt; // 近い敵のインデックスを保存
			}
		}		
	}

	bool bFirstBoss = true;
	int nIdxBoss = 0;

	float BossfDistanceNow = 0.0f;
	float BossfDistanceStock = 0.0f;

	// ボスの最大数分求める
	for (int nCnt = 0; nCnt < MAX_BOSS; nCnt++)
	{
		// 距離を求める
		float DisposX = pBoss[nCnt].pos.x - pPlayer->pos.x;
		float DisposY = pBoss[nCnt].pos.y - pPlayer->pos.y;
		float DisposZ = pBoss[nCnt].pos.z - pPlayer->pos.z;

		// 距離を求める
		BossfDistanceNow = sqrtf((DisposX * DisposX) + (DisposY * DisposY) + (DisposZ * DisposZ));

		// 最初だけ通す
		if (bFirstBoss == true)
		{
			BossfDistanceStock = BossfDistanceNow;
			bFirstBoss = false;
			nIdxBoss = nCnt;
		}
		else
		{
			// 今の距離がストックされた距離より小さかったら
			if (BossfDistanceNow < BossfDistanceStock)
			{
				BossfDistanceStock = BossfDistanceNow; // 距離を保存
				nIdxBoss = nCnt; // 近い敵のインデックスを保存
			}
		}
	}

	// どっちかの距離が300以内だったら
	if (fDistanceStock >= 300.0f && BossfDistanceStock >= 300.0f)
	{
		// 表示する
		g_Mark.bUse = true;

		// 敵のほうが近かったら
		if (BossfDistanceStock > fDistanceStock)
		{
			// 敵までの角度を求める
			float fAngle = atan2f(pEnemy[nIdxEnemy].pos.x - pPlayer->pos.x, pEnemy[nIdxEnemy].pos.z - pPlayer->pos.z);

			g_Mark.rotDest.y = fAngle + D3DX_PI;
		}
		// ボスのほうが近かったら
		else if (BossfDistanceStock < fDistanceStock)
		{
			// ボスまでの角度を求める
			float fAngle = atan2f(pBoss[nIdxBoss].pos.x - pPlayer->pos.x, pBoss[nIdxBoss].pos.z - pPlayer->pos.z);

			g_Mark.rotDest.y = fAngle + D3DX_PI;
		}

		// 目的の角度までの距離を求める
		D3DXVECTOR3 diffRot = g_Mark.rotDest - g_Mark.rot;

		// 角度の正規化
		NormalizeRotation(&diffRot.x, &diffRot.y, &diffRot.z);

		// 目的の角度に近づける
		g_Mark.rot.y += (g_Mark.rotDest.y - g_Mark.rot.y) * 0.5f;
	}
	else
	{
		g_Mark.bUse = false;
	}
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
	D3DXMatrixTranslation(&mtxTrans, pPlayer->pos.x, pPlayer->pos.y + 100.0f, pPlayer->pos.z);
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
