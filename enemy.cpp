//============================
//
// 敵[enemy.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//インクルードファイル
//****************************
#include "enemy.h"
#include "player.h"
#include <stdio.h>
#include "Score.h"
#include "input.h"
#include "damagepop.h"
#include "block.h"
#include "item.h"

//****************************
//マクロ定義
//****************************
#define ENEMY_WORD (128) // 敵の最大数
#define SWORD_MTX (4) // 剣のマトリックスの数
#define TYPEONE_MOVE (2.5f) //敵0の移動量
#define TYPETWO_MOVE (1.5f) //敵1の移動量
#define TYPETHREE_MOVE (1.0f) //敵2の移動量
#define MAX_TEXENEMY (128) //テクスチャの最大数

//****************************
//グローバル変数宣言
//****************************
ENEMY g_Enemy[MAX_ENEMY];
ENEMY g_LoadEnemy[ENEMYTYPE_MAX];
int g_nNumEnemy;//敵の総数カウント

//****************************
//プロトタイプ宣言
//****************************
void LoadEnemy(int nType);    //読み込み処理

//=============================
//ブロックの初期化処理
//=============================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタを取得

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_Enemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//座標
		g_Enemy[nCntEnemy].AttackEnemy = D3DXVECTOR3(5.0f, 10.0f, 5.0f);//
		g_Enemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
		g_Enemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//角度
		//g_Enemy[nCntEnemy].nType = ENEMYTYPE_ONE;
		g_Enemy[nCntEnemy].bUse = false;								//未使用状態
		g_Enemy[nCntEnemy].Motion.bLoopMotion = true;					//ループするか否か
		g_Enemy[nCntEnemy].nLife = 10;									//体力
		g_Enemy[nCntEnemy].state = ENEMYSTATE_NORMAL;					//状態
	}

	//グローバル変数の初期化
	g_nNumEnemy = 0;

	//敵の読み込み
	for (int nCntEnemyType= 0; nCntEnemyType < ENEMYTYPE_MAX; nCntEnemyType++)
	{
		//読み込み
		LoadEnemy(0);

		//g_LoadEnemy[nCntEnemyType].
		//g_LoadEnemy[nCntEnemyType].nLife = 20;
		//g_LoadEnemy[nCntEnemyType].nType = ENEMYTYPE_ONE;
		g_LoadEnemy[nCntEnemyType].Motion.motionType = MOTIONTYPE_NEUTRAL;//モーションの種類
		g_LoadEnemy[nCntEnemyType].Motion.nKey = 0;						  //キー数
		g_LoadEnemy[nCntEnemyType].g_bDamage = true;					  //ダメージか否か
		g_LoadEnemy[nCntEnemyType].Motion.bLoopMotion = true;			  //ループか否か
		g_LoadEnemy[nCntEnemyType].Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  //サイズ
		g_LoadEnemy[nCntEnemyType].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	  //座標

		for (int nCntModel = 0; nCntModel < g_LoadEnemy[nCntEnemyType].Motion.nNumModel; nCntModel++)
		{
			D3DXMATERIAL* pMat;//マテリアルへのポインタ

			//マテリアルのデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pBuffMatEnemy->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].dwNumMatEnemy; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//このファイル名を使用してテクスチャを読み込む
					//テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pTextureEnemy[nCntMat]);
				}
			}

			//g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].Diffuse = pMat->MatD3D.Diffuse;

			//g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].FirstDiffuse = g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].Diffuse;

		}
	}

	//頂点座標の取得
	int nNumVtx;//頂点数
	DWORD sizeFVF;//頂点フォーマットのサイズ
	BYTE* pVtxBuff;//頂点バッファへのポインタ

	for (int nCntEnemyType = 0; nCntEnemyType < ENEMYTYPE_MAX; nCntEnemyType++)
	{
		for (int nCntModel = 0; nCntModel < g_LoadEnemy[nCntEnemyType].Motion.nNumModel; nCntModel++)
		{
			//頂点数の取得
			nNumVtx = g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pMeshEnemy->GetNumVertices();

			//頂点フォーマットのサイズ取得
			sizeFVF = D3DXGetFVFVertexSize(g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pMeshEnemy->GetFVF());

			//頂点バッファのロック
			g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pMeshEnemy->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				//頂点座標の代入
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

				//頂点座標を比較してブロックの最小値,最大値を取得
				if (vtx.x < g_LoadEnemy[nCntEnemyType].vtxMin.x)
				{
					g_LoadEnemy[nCntEnemyType].vtxMin.x = vtx.x;
				}
				else if (vtx.y < g_LoadEnemy[nCntEnemyType].vtxMin.y)
				{
					g_LoadEnemy[nCntEnemyType].vtxMin.y = vtx.y;
				}
				else if (vtx.z < g_LoadEnemy[nCntEnemyType].vtxMin.z)
				{
					g_LoadEnemy[nCntEnemyType].vtxMin.z = vtx.z;
				}
				else if (vtx.x > g_LoadEnemy[nCntEnemyType].vtxMax.x)
				{
					g_LoadEnemy[nCntEnemyType].vtxMax.x = vtx.x;
				}
				else if (vtx.y > g_LoadEnemy[nCntEnemyType].vtxMax.y)
				{
					g_LoadEnemy[nCntEnemyType].vtxMax.y = vtx.y;
				}
				else if (vtx.z > g_LoadEnemy[nCntEnemyType].vtxMax.z)
				{
					g_LoadEnemy[nCntEnemyType].vtxMax.z = vtx.z;
				}

				//頂点フォーマットのサイズ分ポインタを進める
				pVtxBuff += sizeFVF;

			}
			//頂点バッファのアンロック
			g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pMeshEnemy->UnlockVertexBuffer();		
		}

		//サイズに代入
		g_LoadEnemy[nCntEnemyType].Size.x = g_LoadEnemy[nCntEnemyType].vtxMax.x - g_LoadEnemy[nCntEnemyType].vtxMin.x;
		g_LoadEnemy[nCntEnemyType].Size.y = g_LoadEnemy[nCntEnemyType].vtxMax.y - g_LoadEnemy[nCntEnemyType].vtxMin.y;
		g_LoadEnemy[nCntEnemyType].Size.z = g_LoadEnemy[nCntEnemyType].vtxMax.z - g_LoadEnemy[nCntEnemyType].vtxMin.z;

	}
}
//=============================
//敵の終了処理
//=============================
void UninitEnemy(void)
{
		for (int nCntEnemyType = 0; nCntEnemyType < ENEMYTYPE_MAX; nCntEnemyType++)
		{
			for (int nCntModel = 0; nCntModel < g_LoadEnemy[nCntEnemyType].Motion.nNumModel; nCntModel++)
			{
				//テクスチャの破棄
				for (int TexCnt = 0; TexCnt < MAX_TEXENEMY; TexCnt++)
				{
					if (g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pTextureEnemy[TexCnt] != NULL)
					{
						g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pTextureEnemy[TexCnt]->Release();
						g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pTextureEnemy[TexCnt] = NULL;
					}
				}

				//メッシュの破棄
				if (g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pMeshEnemy != NULL)
				{
					g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pMeshEnemy->Release();
					g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pMeshEnemy = NULL;
				}

				//マテリアルの破棄
				if (g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pBuffMatEnemy != NULL)
				{
					g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pBuffMatEnemy->Release();
					g_LoadEnemy[nCntEnemyType].EnemyModel[nCntModel].pBuffMatEnemy = NULL;
				}
			}
		}
}
//=============================
//敵の更新処理
//=============================
void UpdateEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (!g_Enemy[nCntEnemy].bUse)
		{//未使用状態だったら
			//とばしてカウントを進める
			continue;
		}

		if(CollisionEnemy(&g_Enemy[nCntEnemy].pos, // 敵の位置
			30.0f, // ブロックの半径
			30.0f)) // 敵の半径 
		{
			HitEnemy(nCntEnemy, 1);
		}

		//モーションの更新
		UpdateMotion(&g_Enemy[nCntEnemy].Motion);
	}
}
//=============================
//敵の描画処理
//=============================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタを取得

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	int nNumEnemy = 0;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_Enemy[nCntEnemy].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Enemy[nCntEnemy].mtxWorldEnemy);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Enemy[nCntEnemy].rot.y, g_Enemy[nCntEnemy].rot.x, g_Enemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorldEnemy, &g_Enemy[nCntEnemy].mtxWorldEnemy, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Enemy[nCntEnemy].pos.x, g_Enemy[nCntEnemy].pos.y, g_Enemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_Enemy[nCntEnemy].mtxWorldEnemy, &g_Enemy[nCntEnemy].mtxWorldEnemy, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Enemy[nCntEnemy].mtxWorldEnemy);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//全モデルパーツの描画
			for (int nCntModel = 0; nCntModel < g_Enemy[nCntEnemy].Motion.nNumModel; nCntModel++)
			{
				D3DXMATRIX mtxRotModel, mtxTransform;//計算用
				D3DXMATRIX mtxParent;//親のマトリックス

				//ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld);

				//向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].rot.y, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].rot.x, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].rot.z);
				D3DXMatrixMultiply(&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld, &g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld, &mtxRotModel);

				//位置を反映
				D3DXMatrixTranslation(&mtxTransform, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pos.x, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pos.y, g_Enemy[nCntEnemy].Motion.aModel[nCntModel].pos.z);
				D3DXMatrixMultiply(&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld, &g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld, &mtxTransform);

				//パーツの[親のマトリックス]を設定
				if (g_Enemy[nCntEnemy].Motion.aModel[nCntModel].nIdxModelParent != -1)
				{
					//親モデルがある場合
					mtxParent = g_Enemy[nCntEnemy].Motion.aModel[g_Enemy[nCntEnemy].Motion.aModel[nCntModel].nIdxModelParent].mtxWorld;
				}
				else
				{//親モデルがない場合
					mtxParent = g_Enemy[nCntEnemy].mtxWorldEnemy;
				}

				//算出した[パーツのワールドマトリックス]と[親のマトリックス]をかけあわせる
				D3DXMatrixMultiply(&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld,
					&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld,
					&mtxParent);//自分自分親

				//パーツのワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD,
					&g_Enemy[nCntEnemy].Motion.aModel[nCntModel].mtxWorld);

				//マテリアルのデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_Enemy[nCntEnemy].EnemyModel[nNumEnemy].pBuffMatEnemy->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_Enemy[nCntEnemy].EnemyModel[nNumEnemy].dwNumMatEnemy; nCntMat++)
				{					
					D3DXMATERIAL color;

					switch (g_Enemy[nCntEnemy].nType)
					{
					case ENEMYTYPE_ONE:
						if (g_Enemy[nCntEnemy].state != ENEMYSTATE_DAMAGE)
						{
							//マテリアルの設定
							pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
						}
						else if (g_Enemy[nCntEnemy].state == ENEMYSTATE_DAMAGE)
						{
							color = pMat[nCntMat];

							color.MatD3D.Diffuse.r = 1.0f;
							color.MatD3D.Diffuse.g = 0.0f;
							color.MatD3D.Diffuse.b = 0.0f;
							color.MatD3D.Diffuse.a = 1.0f;

							pDevice->SetMaterial(&color.MatD3D);
						}
						break;
					default:
						break;
					}

					//テクスチャの設定
					pDevice->SetTexture(0, g_Enemy[nCntEnemy].EnemyModel[nNumEnemy].pTextureEnemy[nCntMat]);

					//ブロック(パーツ)の描画
					g_Enemy[nCntEnemy].EnemyModel[nNumEnemy].pMeshEnemy->DrawSubset(nCntMat);
				}
				//インクリメント
				nNumEnemy++;
			}
		}
		//初期化
		nNumEnemy = 0;
	}
}
//=======================
//敵の取得処理
//=======================
ENEMY* GetEnemy(void)
{
	return &g_Enemy[0];
}
//=======================
//敵のヒット処理
//=======================
void HitEnemy(int nCnt,int nDamage)
{
	g_Enemy[nCnt].nLife -= nDamage;

	if (g_Enemy[nCnt].nLife <=0)
	{//体力が0以下なら
		g_Enemy[nCnt].state = ENEMYSTATE_DEATH;//敵の状態を死亡状態にする

		g_Enemy[nCnt].bUse = false;			   //未使用判定

		g_nNumEnemy--;						   //デクリメント
	}
	else
	{
		g_Enemy[nCnt].state = ENEMYSTATE_DAMAGE;//敵の状態をダメージにする

		g_Enemy[nCnt].g_bDamage = false;//ダメージを通らなくする

		g_Enemy[nCnt].nCounterState = 30;//ダメージ状態からノーマルに戻るまでの時間
	}
}
//=======================
//敵の設定処理
//=======================
void SetEnemy(D3DXVECTOR3 pos, ENEMYTYPE nType,int nLife,D3DXVECTOR3 move)
{
	MODE mode = GetMode();//現在のモードの取得

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (!g_Enemy[nCntEnemy].bUse)
		{
			g_Enemy[nCntEnemy] = g_LoadEnemy[nType]; // 情報を代入

			g_Enemy[nCntEnemy].pos = pos;	 //座標
			g_Enemy[nCntEnemy].move = move;  //移動量
			g_Enemy[nCntEnemy].nType = nType;//種類
			g_Enemy[nCntEnemy].nLife = nLife;//体力
			g_Enemy[nCntEnemy].bUse = true;  //使用状態

			g_nNumEnemy++;//インクリメント
			break;
		}
	}
}
//=======================
//敵の総数取得処理
//=======================
int GetNumEnemy(void)
{
	return g_nNumEnemy;
}
//=============================
//敵のロード処理
//=============================
void LoadEnemy(int nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスポインタ

	FILE* pFile;//ファイルポインタ
	char aStr[ENEMY_WORD];//文字型の配列

	//ローカル変数
	int nNumModel = 0;
	int nCnt = 0;
	char gomi[3];
	int nNumParts = 0;
	int nIdx = 0;
	int Parent = 0;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int nNumKey = 0;
	int nCntPartsPos = 0;
	int nCntPartsRot = 0;
	int nCntMotion = 0;
	int nCntNum = 0;
	int EnenKey = 0;

	//種類ごとにファイルを読み込む
	switch (nType)
	{
	case 0:
		pFile = fopen(ENEMYFILE001, "r");
		break;
	case 1:
		pFile = fopen(ENEMYFILE002, "r");
		break;
	case 2:
		pFile = fopen(ENEMYFILE003, "r");
		break;
	default:
		pFile = NULL;
		break;
	}

	if (pFile != NULL)
	{//ファイルが開けたら
		while (1)
		{
			fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(aStr, "SCRIPT") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aStr[0]);

					if (strcmp(aStr, "NUM_MODEL") == 0)
					{
						fscanf(pFile, "%s", &gomi[0]);
						fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.nNumModel);

					}
					else if (strcmp(aStr, "MODEL_FILENAME") == 0)
					{
						fscanf(pFile, "%s", &gomi[0]);

						fscanf(pFile, "%s", &aStr[0]);

						const char* MODEL_FILE = {};

						MODEL_FILE = aStr;

						//Xファイルの読み込み
						D3DXLoadMeshFromX(MODEL_FILE,
							D3DXMESH_SYSTEMMEM,
							pDevice,
							NULL,
							&g_LoadEnemy[nType].EnemyModel[nCntNum].pBuffMatEnemy,
							NULL,
							&g_LoadEnemy[nType].EnemyModel[nCntNum].dwNumMatEnemy,
							&g_LoadEnemy[nType].EnemyModel[nCntNum].pMeshEnemy);

						if (nCntNum < g_LoadEnemy[nType].Motion.nNumModel)
						{
							nCntNum++;
						}

						if (nCntNum >= g_LoadEnemy[nType].Motion.nNumModel)
						{
							nCntNum = 0;
						}

					}
					else if (strcmp(aStr, "CHARACTERSET") == 0)
					{
						while (1)
						{
							fscanf(pFile, "%s", &aStr[0]);

							if (strcmp(aStr, "NUM_PARTS") == 0)
							{
								fscanf(pFile, "%s", &gomi[0]);

								fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.nNumModel);

								break;
							}
						}
					}
					else if (strcmp(aStr, "PARTSSET") == 0)
					{
						while (1)
						{
							fscanf(pFile, "%s", &aStr[0]);

							if (strcmp(aStr, "INDEX") == 0)
							{
								fscanf(pFile, "%s", &gomi[0]);

								fscanf(pFile, "%d", &nIdx);

							}
							else if (strcmp(aStr, "PARENT") == 0)
							{
								fscanf(pFile, "%s", &gomi[0]);
								fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.aModel[nIdx].nIdxModelParent);

							}
							else if (strcmp(aStr, "POS") == 0)
							{
								fscanf(pFile, "%s", &gomi[0]);

								fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offpos.x);
								fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offpos.y);
								fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offpos.z);
							}
							else if (strcmp(aStr, "ROT") == 0)
							{
								fscanf(pFile, "%s", &gomi[0]);

								fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offrot.x);
								fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offrot.y);
								fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aModel[nIdx].offrot.z);
							}
							else if (strcmp(aStr, "END_PARTSSET") == 0)
							{
								break;
							}
						}
					}
					else if (strcmp(aStr, "MOTIONSET") == 0)
					{
						char aStr4[ENEMY_WORD];

						while (1)
						{
							fscanf(pFile, "%s", &aStr4[0]);

							if (strcmp(aStr4, "NUM_KEY") == 0)
							{
								fscanf(pFile, "%s", &gomi[0]);

								fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].nNumkey);
							}
							else if (strcmp(aStr4, "LOOP") == 0)
							{
								fscanf(pFile, "%s", &gomi[0]);

								fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].bLoop);
							}

							char aStr5[ENEMY_WORD];

							if (strcmp(aStr4, "KEYSET") == 0)
							{
								while (1)
								{
									fscanf(pFile, "%s", &aStr5[0]);

									if (strcmp(aStr5, "FRAME") == 0)
									{
										fscanf(pFile, "%s", &gomi[0]);

										fscanf(pFile, "%d", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[EnenKey].nFrame);
									}
									else if (strcmp(aStr5, "KEY") == 0)
									{
										char aStr6[ENEMY_WORD];

										while (1)
										{
											fscanf(pFile, "%s", &aStr6[0]);

											if (strcmp(aStr6, "POS") == 0)
											{
												fscanf(pFile, "%s", &gomi[0]);

												fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[EnenKey].aKey[nCntPartsPos].fPosX);
												fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[EnenKey].aKey[nCntPartsPos].fPosY);
												fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[EnenKey].aKey[nCntPartsPos].fPosZ);

												nCntPartsPos++;
											}
											else if (strcmp(aStr6, "ROT") == 0)
											{
												fscanf(pFile, "%s", &gomi[0]);

												fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[EnenKey].aKey[nCntPartsRot].fRotX);
												fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[EnenKey].aKey[nCntPartsRot].fRotY);
												fscanf(pFile, "%f", &g_LoadEnemy[nType].Motion.aMotionInfo[nCntMotion].aKeyInfo[EnenKey].aKey[nCntPartsRot].fRotZ);
												nCntPartsRot++;
											}
											else if (strcmp(aStr6, "END_KEY") == 0)
											{
												break;
											}
										}
									}
									else if (strcmp(aStr5, "END_KEYSET") == 0)
									{
										EnenKey++;
										nCntPartsPos = 0;
										nCntPartsRot = 0;
										break;
									}
								}
							}

							else if (strcmp(aStr4, "END_MOTIONSET") == 0)
							{
								EnenKey = 0;
								nCntMotion++;
								break;
							}
						}
					}

					else if (strcmp(aStr, "END_MODELSET") == 0)
					{
						break;
					}
				}
			}

			if (strcmp(aStr, "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}
	else
	{
		return;
	}
	fclose(pFile);

}
