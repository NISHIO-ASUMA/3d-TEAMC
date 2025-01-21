//============================
//
// edit [edit.cpp]
// Author:YOSHIDA YUUTO
//
//============================
 
//***********************
//インクルードファイル
//***********************
#include "edit.h"
#include "input.h"
#include "block.h"
#include "meshfield.h"
#include "camera.h"
#include "light.h"
#include <stdbool.h>
#include <stdio.h>
#include "fade.h"
#include "game.h"
#include "mouse.h"

//****************************
//マクロ定義
//****************************
#define OBJ_MOVE (10.0f) // オブジェクトの移動量
#define MAX_WORD (128) // 文字数

//****************************
//グローバル変数
//****************************
EDIT_INFO g_Edit[MAX_BLOCK];			// ブロックの情報
BLOCKTEX g_BlockTexInfo[BLOCKTYPE_MAX]; // ブロックのテクスチャ情報保存用変数
int g_EditCount,nCntobj;				// オブジェクトのカウント保存用変数
int g_LoadCnt;							// ロードするファイルを切り替える変数
int g_nNumBlock;						// 設置したオブジェクトの数

//===========================
//エディット画面の初期化処理
//===========================
void InitEdit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		g_Edit[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Edit[nCnt].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Edit[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Edit[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Edit[nCnt].bUse = false;
		g_Edit[nCnt].ObjCnt = 0;
		g_Edit[nCnt].nEditMode = EDITMODE_BLOCK;
		g_Edit[nCnt].nBlockTypeNumber = 0;
	}

	g_nNumBlock = 0;
	nCntobj = 0;

	for (int nCntNum = 0; nCntNum < BLOCKTYPE_MAX; nCntNum++)
	{
		//Xファイルの読み込み
		D3DXLoadMeshFromX(MODELTYPE[nCntNum],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_BlockTexInfo[nCntNum].g_pBuffMatBlock,
			NULL,
			&g_BlockTexInfo[nCntNum].g_dwNumMatBlock,
			&g_BlockTexInfo[nCntNum].g_pMeshBlock);

		D3DXMATERIAL* pMat;//マテリアルへのポインタ

		//マテリアルのデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_BlockTexInfo[nCntNum].g_pBuffMatBlock->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_BlockTexInfo[nCntNum].g_dwNumMatBlock; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//このファイル名を使用してテクスチャを読み込む
					//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_BlockTexInfo[nCntNum].g_apTextureBlock[nCntMat]);
			}
		}
	}

	g_Edit[0].bUse = true;						// 0番目を使用状態にする
	g_Edit[0].ModelTex[0] = g_BlockTexInfo[0];  // 種類0番目のテクスチャを代入
	g_EditCount = 0;							// オブジェクトのカウント初期化
	g_LoadCnt = 0;								// ロードのカウント初期化
	nCntobj = 0;								// オブジェクトのカウント初期化
}

//===========================
//エディット画面の終了処理
//===========================
void UninitEdit(void)
{
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		for (int nCntNum = 0; nCntNum < BLOCKTYPE_MAX; nCntNum++)
		{
			for (int TexCnt = 0; TexCnt < MAX_TEX; TexCnt++)
			{
				//テクスチャの破棄
				if (g_Edit[nCnt].ModelTex[nCntNum].g_apTextureBlock[TexCnt] != NULL)
				{
					g_Edit[nCnt].ModelTex[nCntNum].g_apTextureBlock[TexCnt]->Release();
					g_Edit[nCnt].ModelTex[nCntNum].g_apTextureBlock[TexCnt] = NULL;
				}
			}

			//マテリアルの破棄
			if (g_Edit[nCnt].ModelTex[nCntNum].g_pBuffMatBlock != NULL)
			{
				g_Edit[nCnt].ModelTex[nCntNum].g_pBuffMatBlock->Release();
				g_Edit[nCnt].ModelTex[nCntNum].g_pBuffMatBlock = NULL;
			}
			//メッシュの破棄
			if (g_Edit[nCnt].ModelTex[nCntNum].g_pMeshBlock != NULL)
			{
				g_Edit[nCnt].ModelTex[nCntNum].g_pMeshBlock->Release();
				g_Edit[nCnt].ModelTex[nCntNum].g_pMeshBlock = NULL;
			}
		}
	}
}

//===========================
//エディット画面の更新処理
//===========================
void UpdateEdit(void)
{
	Camera* pCamera = GetCamera();

	static int nLoad = 0; // リロード回数を制限する変数

	if (g_Edit[g_EditCount].bUse)
	{
		//移動量の減衰
		g_Edit[g_EditCount].move.x += (0.0f - g_Edit[g_EditCount].move.x) * 0.5f;
		g_Edit[g_EditCount].move.z += (0.0f - g_Edit[g_EditCount].move.z) * 0.5f;

		//位置の更新
		g_Edit[g_EditCount].pos += g_Edit[g_EditCount].move;

		//上移動
		if (GetKeyboardPress(DIK_W))
		{
			g_Edit[g_EditCount].move.x += sinf(pCamera->rot.y) * OBJ_MOVE;
			g_Edit[g_EditCount].move.z += cosf(pCamera->rot.y) * OBJ_MOVE;
		}
		//左移動
		if (GetKeyboardPress(DIK_A))
		{
			g_Edit[g_EditCount].move.x -= cosf(-pCamera->rot.y) * OBJ_MOVE;
			g_Edit[g_EditCount].move.z -= sinf(-pCamera->rot.y) * OBJ_MOVE;
		}
		//下移動
		if (GetKeyboardPress(DIK_S))
		{
			g_Edit[g_EditCount].move.x -= sinf(pCamera->rot.y) * OBJ_MOVE;
			g_Edit[g_EditCount].move.z -= cosf(pCamera->rot.y) * OBJ_MOVE;
		}
		//右移動
		if (GetKeyboardPress(DIK_D))
		{
			g_Edit[g_EditCount].move.x += cosf(-pCamera->rot.y) * OBJ_MOVE;
			g_Edit[g_EditCount].move.z += sinf(-pCamera->rot.y) * OBJ_MOVE;
		}

		//配置
		if (KeyboardTrigger(DIK_RETURN))
		{
			g_Edit[g_EditCount + 1].pos = g_Edit[g_EditCount].pos;
			g_Edit[g_EditCount + 1].bUse = true;
			g_Edit[g_EditCount + 1].ModelTex[0] = g_BlockTexInfo[0];
			g_nNumBlock++;
			g_EditCount++;
		}

		//ブロック消去
		if (KeyboardTrigger(DIK_BACKSPACE)&& g_EditCount > 0)
		{
			g_Edit[g_EditCount].bUse = false;
			g_EditCount--;
			g_nNumBlock--;
		}

		//種類変更
		if (KeyboardTrigger(DIK_F) && g_Edit[g_EditCount].nType < BLOCKTYPE_MAX - 1)
		{
			g_Edit[g_EditCount].nType++;
			g_Edit[g_EditCount].ModelTex[g_Edit[g_EditCount].nType] = g_BlockTexInfo[g_Edit[g_EditCount].nType];
		}
		else if (KeyboardTrigger(DIK_G) && g_Edit[g_EditCount].nType > 0)
		{
			g_Edit[g_EditCount].nType--;
			g_Edit[g_EditCount].ModelTex[g_Edit[g_EditCount].nType] = g_BlockTexInfo[g_Edit[g_EditCount].nType];
		}

		if (KeyboardTrigger(DIK_V))
		{
			g_Edit[g_EditCount].Scal *= 1.1f;
		}
		if (KeyboardTrigger(DIK_B))
		{
			g_Edit[g_EditCount].Scal *= 0.9f;
		}
	}

	BLOCK* pBlock = GetBlock();

	if (KeyboardTrigger(DIK_F7))
	{
		SaveEdit();
	}

	else if (KeyboardTrigger(DIK_F8) && nLoad == 0)
	{
		nLoad = 1;
		ReLoadEdit();
	}

	if (KeyboardTrigger(DIK_F2) && nLoad == 1)
	{
		nLoad = 0;
	}
}

//===========================
//エディット画面の描画処理
//===========================
void DrawEdit(void)
{
	MODE mode = GetMode();
	EDIT_INFO* pEdit = GetEdit();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();;

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		for (int nCntNum = 0; nCntNum < BLOCKTYPE_MAX; nCntNum++)
		{
			if (!g_Edit[nCntBlock].bUse)
			{
				continue;
			}

			int nType = g_Edit[nCntBlock].nType;

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Edit[nCntBlock].mtxWorld);

			//向きを反映
			D3DXMatrixScaling(&mtxScal, g_Edit[nCntBlock].Scal.y, g_Edit[nCntBlock].Scal.x, g_Edit[nCntBlock].Scal.z);
			D3DXMatrixMultiply(&g_Edit[nCntBlock].mtxWorld, &g_Edit[nCntBlock].mtxWorld, &mtxScal);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Edit[nCntBlock].rot.y, g_Edit[nCntBlock].rot.x, g_Edit[nCntBlock].rot.z);
			D3DXMatrixMultiply(&g_Edit[nCntBlock].mtxWorld, &g_Edit[nCntBlock].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Edit[nCntBlock].pos.x, g_Edit[nCntBlock].pos.y, g_Edit[nCntBlock].pos.z);
			D3DXMatrixMultiply(&g_Edit[nCntBlock].mtxWorld, &g_Edit[nCntBlock].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Edit[nCntBlock].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			for (int nCntMat = 0; nCntMat < (int)g_Edit[nCntBlock].ModelTex[nType].g_dwNumMatBlock; nCntMat++)
			{
				D3DXMATERIAL color;

				//マテリアルのデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_Edit[nCntBlock].ModelTex[nType].g_pBuffMatBlock->GetBufferPointer();

				//選択中のブロックだったら
				if (nCntBlock == g_EditCount)
				{
					color = pMat[nCntMat];

					color.MatD3D.Diffuse.r = 1.0f;
					color.MatD3D.Diffuse.g = 0.0f;
					color.MatD3D.Diffuse.b = 0.0f;
					color.MatD3D.Diffuse.a = 1.0f;

					//マテリアルの設定
					pDevice->SetMaterial(&color.MatD3D);
				}
				else
				{
					//マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				}

				//テクスチャの設定
				pDevice->SetTexture(0, g_Edit[nCntBlock].ModelTex[nType].g_apTextureBlock[nCntMat]);

				//ブロック(パーツ)の描画
				g_Edit[nCntBlock].ModelTex[nType].g_pMeshBlock->DrawSubset(nCntMat);
			}
			
		}
	}
	pDevice->SetMaterial(&matDef);
}

//===========================
//エディット画面のセーブ処理
//===========================
void SaveEdit(void)
{
	FILE* pFile;

	pFile = fopen("data\\saveEdit.txt", "w");

	if (pFile != NULL)
	{
		fwrite(&g_EditCount, sizeof(int), 0, pFile);

		fprintf(pFile,

			"+==========================================+\n"
			"+			   マップエディター             +\n"
			"+==========================================+\n\n");

		for (int nCnt = 0; nCnt < g_EditCount; nCnt++)
		{
			if (g_Edit[nCnt].bUse)
			{
				fprintf(pFile, "BLOCKSET\n");

				fprintf(pFile, "   BLOCKTYPE = %d\n", g_Edit[nCnt].nType);

				fprintf(pFile, "   POS = %.1f %.1f %.1f\n",g_Edit[nCnt].pos.x,g_Edit[nCnt].pos.y, g_Edit[nCnt].pos.z);

				fprintf(pFile, "   SIZE = %.1f %.1f %.1f\n", g_Edit[nCnt].Scal.x, g_Edit[nCnt].Scal.y, g_Edit[nCnt].Scal.z);

				fprintf(pFile, "END_BLOCKSET\n\n");
			}
		}

		fprintf(pFile, "BLOCK_COUNT = %d \n", g_EditCount);
		fprintf(pFile, "END_SCRIPT");
	}
	else
	{
		return;
	}
	fclose(pFile);
}

//===========================
//エディット画面のロード処理
//===========================
void LoadEdit(void)
{
	FILE* pFile;
	int nType;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;

	float fWidth;
	float fHeight;
	char skip[5];
	fWidth = 0.0f;
	fHeight = 0.0f;

	nType = 0;

	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Scal(0.0f, 0.0f, 0.0f);

	//switch (g_LoadCnt)
	//{
	//case 0:
	//	//ファイルを開く
	//	pFile = fopen("data\\save.txt,","r");
	//	break;
	//default:
	//	pFile = NULL;
	//	break;
	//}

	pFile = fopen("data\\saveEdit.txt", "r");

	if (pFile != NULL)
	{
		while (1)
		{
			char aString[MAX_WORD];

			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "BLOCKSET") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "BLOCKTYPE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &nType);
					}
					else if (strcmp(aString, "POS") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);
					}
					else if (strcmp(aString, "SIZE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &Scal.x);
						fscanf(pFile, "%f", &Scal.y);
						fscanf(pFile, "%f", &Scal.z);
					}
					else if (strcmp(aString, "END_BLOCKSET") == 0)
					{
						SetBlock(pos,nType, Scal);
						break;
					}
				}
			}

			if (strcmp(aString, "BLOCK_COUNT") == 0)
			{
				fscanf(pFile, "%s", &skip[0]);
				fscanf(pFile, "%d", &nCntobj);
			}
			else if (strcmp(aString, "END_SCRIPT") == 0)
			{
				break;
			}
		}
		/*g_Edit++;*/

	}
	else
	{//開けなかったとき	
		return;
	}
	fclose(pFile);
}
//===========================
//エディットのリロード
//===========================
void ReLoadEdit(void)
{
	FILE* pFile;
	int nType;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;

	int nIdx = 0;
	float fWidth;
	float fHeight;
	char skip[5];
	fWidth = 0.0f;
	fHeight = 0.0f;

	nType = 0;

	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Scal(0.0f, 0.0f, 0.0f);

	//switch (g_LoadCnt)
	//{
	//case 0:
	//	//ファイルを開く
	//	pFile = fopen("data\\save.txt,","r");
	//	break;
	//default:
	//	pFile = NULL;
	//	break;
	//}

	pFile = fopen("data\\saveEdit.txt", "r");

	if (pFile != NULL)
	{
		while (1)
		{
			char aString[MAX_WORD];

			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "BLOCKSET") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "BLOCKTYPE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &g_Edit[nIdx].nType);
					}
					else if (strcmp(aString, "POS") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &g_Edit[nIdx].pos.x);
						fscanf(pFile, "%f", &g_Edit[nIdx].pos.y);
						fscanf(pFile, "%f", &g_Edit[nIdx].pos.z);
					}
					else if (strcmp(aString, "SIZE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &g_Edit[nIdx].Scal.x);
						fscanf(pFile, "%f", &g_Edit[nIdx].Scal.y);
						fscanf(pFile, "%f", &g_Edit[nIdx].Scal.z);
					}
					else if (strcmp(aString, "END_BLOCKSET") == 0)
					{
						nIdx++;
						break;
					}
				}
			}

			if (strcmp(aString, "BLOCK_COUNT") == 0)
			{
				fscanf(pFile, "%s", &skip[0]);
				fscanf(pFile, "%d", &nCntobj);
			}
			else if (strcmp(aString, "END_SCRIPT") == 0)
			{
				break;
			}
		}
		/*g_Edit++;*/

	}
	else
	{//開けなかったとき	
		return;
	}
	fclose(pFile);

	for (int nCnt = 0; nCnt < nCntobj; nCnt++)
	{
		g_Edit[g_EditCount + 1].bUse = true;
		g_Edit[g_EditCount + 1].ModelTex[0] = g_BlockTexInfo[0];
		g_EditCount++;
		g_nNumBlock++;
	}

}

//===========================
//エディット習得
//===========================
EDIT_INFO* GetEdit(void)
{
	return &g_Edit[0];
}
//===========================
//オブジェクトの取得
//===========================
int GetNumobj(void)
{
	return g_nNumBlock;
}
