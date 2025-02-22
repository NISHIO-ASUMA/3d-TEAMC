//==============================================================================================================
//
// edit [edit.cpp]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//***********************
//インクルードファイル
//***********************
#include "edit.h"
#include "input.h"
#include "item.h"
#include "meshfield.h"
#include "camera.h"
#include "light.h"
#include <stdio.h>
#include "fade.h"
#include "game.h"
#include "mouse.h"
#include "edit2d.h"

//**************************************************************************************************************
//マクロ定義
//**************************************************************************************************************
#define OBJ_MOVE (10.0f) // オブジェクトの移動量
#define MAX_WORD (256)   // 文字数
#define MAX_OBJ (512)    // 最大オブジェクト数

//**************************************************************************************************************
//プロトタイプ宣言
//**************************************************************************************************************
void LoadEditObj(int category); // 編集モードで使うオブジェクトのロード処理

//**************************************************************************************************************
//グローバル変数
//**************************************************************************************************************
EDIT_INFO g_Edit[MAX_OBJ];			    // エディットの情報
EditTex g_BlockTexInfo[EDITMODE_MAX];	// エディットの情報保存用変数3d
LPDIRECT3DTEXTURE9 g_pTex;              // テクスチャ保存
int g_EditCount, nCntobj;               // オブジェクトのカウント、保存用
int g_nNumBlock;						// オブジェクトの数
bool EditMode2d;
int g_PressTime;

//=============================================================================================================
//エディット画面の初期化処理
//=============================================================================================================
void InitEdit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	InitEdit2d();

	//ブロックの最大数分初期化
	for (int nCnt = 0; nCnt < MAX_OBJ; nCnt++)
	{
		g_Edit[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
		g_Edit[nCnt].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// 大きさの初期化(最小1.0f)
		g_Edit[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量の初期化
		g_Edit[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 回転量の初期化
		g_Edit[nCnt].bUse = false;							// 未使用状態
		g_Edit[nCnt].ObjCnt = 0;							// オブジェの数の保存用変数の初期化
		g_Edit[nCnt].EditCategory = EDITMODE_BLOCK;			// 現在のオブジェクトのカテゴリーの初期化
		g_Edit[nCnt].fMove = 10.0f;							// オブジェクトの移動量
	}

	g_PressTime = 0;

	EditMode2d = false; // 2DOBJ編集モードか

	// アイテムとブロックのテクスチャ情報を取得
	BLOCK* BlockOrigin = GetBlockOrigin();
	Item* ItemOrigin = GetItemOrigin();

	// ブロックの種類
	for (int nCntBlock = 0; nCntBlock < BLOCKTYPE_MAX; nCntBlock++)
	{
		// テクスチャ情報を取得
		g_BlockTexInfo[0].pModel[nCntBlock] = BlockOrigin[nCntBlock].BlockTex[nCntBlock];
	}

	// アイテムの種類
	for (int nCntItem = 0; nCntItem < ITEMTYPE_MAX; nCntItem++)
	{
		// テクスチャ情報を取得
		g_BlockTexInfo[1].pModel[nCntItem] = ItemOrigin[nCntItem].ItemTex[nCntItem];
	}

	// ブロック最大数を代入
	g_BlockTexInfo[0].nNumModel = BLOCKTYPE_MAX;

	// アイテム最大数を代入
	g_BlockTexInfo[1].nNumModel = ITEMTYPE_MAX;

	g_nNumBlock = 0;												// オブジェクトの数の初期化
	g_Edit[0].bUse = true;											// 一つ目を使用状態にする
	g_Edit[0].Category[0].nNumModel = g_BlockTexInfo[0].nNumModel;  // 0番のオブジェクトに種類の数を代入(代入しないと描画されない)
	g_Edit[0].Category[0].pModel[0] = g_BlockTexInfo[0].pModel[0];  // 0番のカテゴリー0番目の0種類目の情報を代入
	g_EditCount = 0;												// オブジェクトのカウントの初期化
	nCntobj = 0;													// オブジェクトのカウント保存用変数の初期化
}

//=============================================================================================================
//エディット画面の終了処理
//=============================================================================================================
void UninitEdit(void)
{
	UninitEdit2d();

	// 全オブジェクト分回す
	for (int nCntEdit = 0; nCntEdit < MAX_OBJ; nCntEdit++)
	{
		//カテゴリー分回す
		for (int nCntNum = 0; nCntNum < EDITMODE_MAX; nCntNum++)
		{
			//各カテゴリーの種類数分回す
			for (int nCntModel = 0; nCntModel < g_Edit[nCntEdit].Category[nCntNum].nNumModel; nCntModel++)
			{
				//テクスチャ分回す
				for (int nCnt = 0; nCnt < (int)g_Edit[nCntEdit].Category[nCntNum].pModel[nCntModel].g_dwNumMatModel; nCnt++)
				{
					//テクスチャの破棄
					if (g_Edit[nCntEdit].Category[nCntNum].pModel[nCntModel].g_apTextureModel[nCnt] != NULL)
					{
						g_Edit[nCntEdit].Category[nCntNum].pModel[nCntModel].g_apTextureModel[nCnt] = NULL;
					}
				}
				//バッファの破棄
				if (g_Edit[nCntEdit].Category[nCntNum].pModel[nCntModel].g_pBuffMatModel != NULL)
				{
					g_Edit[nCntEdit].Category[nCntNum].pModel[nCntModel].g_pBuffMatModel = NULL;
				}

				//メッシュの破棄
				if (g_Edit[nCntEdit].Category[nCntNum].pModel[nCntModel].g_pMeshModel != NULL)
				{
					g_Edit[nCntEdit].Category[nCntNum].pModel[nCntModel].g_pMeshModel = NULL;
				}
			}
		}
	}
}

//=============================================================================================================
//エディット画面の更新処理
//=============================================================================================================
void UpdateEdit(void)
{
	SetCursorVisibility(true); // カーソルを表示

	static int nLoad = 0; // リロード回数制限用変数の初期化

	Camera* pCamera = GetCamera(); // カメラのポインタを取得

	//使用状態だったら
	if (g_Edit[g_EditCount].bUse && !EditMode2d)
	{
		//移動量の減衰
		g_Edit[g_EditCount].move.x += (0.0f - g_Edit[g_EditCount].move.x) * 0.5f;
		g_Edit[g_EditCount].move.z += (0.0f - g_Edit[g_EditCount].move.z) * 0.5f;

		//位置の更新
		g_Edit[g_EditCount].pos += g_Edit[g_EditCount].move;

		//上移動
		if (GetKeyboardPress(DIK_W))
		{
			g_Edit[g_EditCount].move.x += sinf(pCamera->rot.y) * g_Edit[g_EditCount].fMove;
			g_Edit[g_EditCount].move.z += cosf(pCamera->rot.y) * g_Edit[g_EditCount].fMove;
		}
		//左移動
		if (GetKeyboardPress(DIK_A))
		{
			g_Edit[g_EditCount].move.x -= cosf(-pCamera->rot.y) * g_Edit[g_EditCount].fMove;
			g_Edit[g_EditCount].move.z -= sinf(-pCamera->rot.y) * g_Edit[g_EditCount].fMove;
		}
		//下移動
		if (GetKeyboardPress(DIK_S))
		{
			g_Edit[g_EditCount].move.x -= sinf(pCamera->rot.y) * g_Edit[g_EditCount].fMove;
			g_Edit[g_EditCount].move.z -= cosf(pCamera->rot.y) * g_Edit[g_EditCount].fMove;
		}
		//右移動
		if (GetKeyboardPress(DIK_D))
		{
			g_Edit[g_EditCount].move.x += cosf(-pCamera->rot.y) * g_Edit[g_EditCount].fMove;
			g_Edit[g_EditCount].move.z += sinf(-pCamera->rot.y) * g_Edit[g_EditCount].fMove;
		}

		int category = g_Edit[g_EditCount].EditCategory; // タイプ保存用(Old)

		//配置
		if (KeyboardTrigger(DIK_RETURN))
		{
			g_Edit[g_EditCount + 1].pos = g_Edit[g_EditCount].pos;							// 次のオブジェクトに現在のオブジェクトの位置を代入
			g_Edit[g_EditCount + 1].bUse = true;
			

			// 次のオブジェクトを使用状態にする
			g_Edit[g_EditCount + 1].Scal = g_Edit[g_EditCount].Scal;						// 次のオブジェクトにスケールを代入する
			g_Edit[g_EditCount + 1].EditCategory = g_Edit[g_EditCount].EditCategory;        // 今のカテゴリーを代入
			g_Edit[g_EditCount + 1].nType = g_Edit[g_EditCount].nType;						// 今のタイプを代入
			g_Edit[g_EditCount + 1].rot = g_Edit[g_EditCount].rot;							// 今の角度を代入
			g_Edit[g_EditCount + 1].fMove = g_Edit[g_EditCount].fMove;							// 今の角度を代入

			int Category = g_Edit[g_EditCount].EditCategory; // 変数に代入
			int nType = g_Edit[g_EditCount].nType;           // 変数に代入

			g_Edit[g_EditCount + 1].Category[Category].pModel[nType] = g_Edit[g_EditCount].Category[Category].pModel[nType];
			g_Edit[g_EditCount + 1].Category[Category].nNumModel = g_BlockTexInfo[Category].nNumModel;

			g_nNumBlock++;																	// オブジェクト数 + 1
			g_EditCount++;																	// オブジェクトのカウント + 1
		}

		//ブロック消去
		if (KeyboardTrigger(DIK_BACKSPACE) && g_EditCount > 0)
		{
			g_Edit[g_EditCount].bUse = false; // 選択中のオブジェクトを消す
			g_EditCount--;                    // オブジェクトのカウントを減らす
			g_nNumBlock--;                    // オブジェクト数を減らす
		}

		//種類変更
		if (KeyboardTrigger(DIK_F) && g_Edit[g_EditCount].nType < g_Edit[g_EditCount].Category[category].nNumModel - 1)
		{
			g_Edit[g_EditCount].nType++; // 種類 + 1
			int nType = g_Edit[g_EditCount].nType;           // 変数に代入

			g_Edit[g_EditCount].Category[category].nNumModel = g_BlockTexInfo[category].nNumModel;	// 次のオブジェクトにカテゴリー0、の種類数を代入 
			g_Edit[g_EditCount].Category[category].pModel[nType] = g_BlockTexInfo[category].pModel[nType]; // 現在のオブジェクトの情報を代入
		}
		else if (KeyboardTrigger(DIK_G) && g_Edit[g_EditCount].nType > 0)
		{
			g_Edit[g_EditCount].nType--; // 種類 - 1
			int nType = g_Edit[g_EditCount].nType;           // 変数に代入
			g_Edit[g_EditCount].Category[category].nNumModel = g_BlockTexInfo[category].nNumModel;	// 次のオブジェクトにカテゴリー0、の種類数を代入 
			g_Edit[g_EditCount].Category[category].pModel[nType] = g_BlockTexInfo[category].pModel[nType]; // 現在のオブジェクトの情報を代入
		}

		if (KeyboardTrigger(DIK_V))
		{
			g_Edit[g_EditCount].Scal *= 1.1f; // 拡大
		}
		if (KeyboardTrigger(DIK_B))
		{
			g_Edit[g_EditCount].Scal *= 0.9f; // 縮小
		}

		// カテゴリー変更
		if (KeyboardTrigger(DIK_Y) && g_Edit[g_EditCount].EditCategory < EDITMODE_MAX - 1)
		{
			g_Edit[g_EditCount].EditCategory++;
			int Category = g_Edit[g_EditCount].EditCategory; // 変数に代入
			int nType = g_Edit[g_EditCount].nType;           // 変数に代入

			// カテゴリー + 1
			g_Edit[g_EditCount].Category[Category].nNumModel = g_BlockTexInfo[Category].nNumModel;		// 現在のカテゴリーの情報を代入

			// 次のカテゴリーの全種類数が現在のモデルのカテゴリー全種類数より少なかったら
			if (g_Edit[g_EditCount].Category[g_Edit[g_EditCount].EditCategory].nNumModel < g_Edit[g_EditCount].Category[category].nNumModel)
			{
				int nNumModel = g_Edit[g_EditCount].Category[Category].nNumModel - 1; // 代入
				g_Edit[g_EditCount].nType = nNumModel; //一番最後の種類にする
			}

			g_Edit[g_EditCount].Category[g_Edit[g_EditCount].EditCategory].pModel[g_Edit[g_EditCount].nType] = g_BlockTexInfo[g_Edit[g_EditCount].EditCategory].pModel[g_Edit[g_EditCount].nType];		// 現在のカテゴリーの情報を代入
		}
		else if (KeyboardTrigger(DIK_U) && g_Edit[g_EditCount].EditCategory > 0)
		{
			g_Edit[g_EditCount].EditCategory--;	// カテゴリー + 1

			int Category = g_Edit[g_EditCount].EditCategory; // 変数に代入

			g_Edit[g_EditCount].Category[Category].nNumModel = g_BlockTexInfo[Category].nNumModel;		// 現在のカテゴリーの情報を代入

			// 次のカテゴリーの全種類数が現在のモデルのカテゴリー全種類数より少なかったら
			if (g_Edit[g_EditCount].Category[g_Edit[g_EditCount].EditCategory].nNumModel < g_Edit[g_EditCount].Category[category].nNumModel)
			{
				int nNumModel = g_Edit[g_EditCount].Category[g_Edit[g_EditCount].EditCategory].nNumModel - 1; // 代入
				g_Edit[g_EditCount].nType = nNumModel; //一番最後の種類にする
			}

			g_Edit[g_EditCount].Category[g_Edit[g_EditCount].EditCategory].pModel[g_Edit[g_EditCount].nType] = g_BlockTexInfo[g_Edit[g_EditCount].EditCategory].pModel[g_Edit[g_EditCount].nType];		// 現在のカテゴリーの情報を代入
		}

		if (GetKeyboardPress(DIK_H))
		{
			g_Edit[g_EditCount].fMove += 0.1f; // 移動量変更
		}
		else if (GetKeyboardPress(DIK_J) && g_Edit[g_EditCount].fMove > 0.0f)
		{
			g_Edit[g_EditCount].fMove -= 0.1f; // 移動量変更
		}

		// yの移動
		if (GetKeyboardPress(DIK_UP))
		{
			g_Edit[g_EditCount].pos.y += g_Edit[g_EditCount].fMove;
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{
			g_Edit[g_EditCount].pos.y -= g_Edit[g_EditCount].fMove;
		}

		// カメラを選択中のオブジェクトの場所へ移動
		if (KeyboardTrigger(DIK_F6))
		{
			pCamera->posV.x = g_Edit[g_EditCount].pos.x;
			pCamera->posV.z = g_Edit[g_EditCount].pos.z;
		}

		// 向きの変更
		if (GetKeyboardPress(DIK_1))
		{
			g_Edit[g_EditCount].rot.x += D3DXToRadian(1);
		}
		else if (GetKeyboardPress(DIK_2))
		{
			g_Edit[g_EditCount].rot.y += D3DXToRadian(1);
		}
		else if (GetKeyboardPress(DIK_3))
		{
			g_Edit[g_EditCount].rot.z += D3DXToRadian(1);
		}
		else if (GetKeyboardPress(DIK_4))
		{
			g_Edit[g_EditCount].rot.x -= D3DXToRadian(1);
		}
		else if (GetKeyboardPress(DIK_5))
		{
			g_Edit[g_EditCount].rot.y -= D3DXToRadian(1);
		}
		else if (GetKeyboardPress(DIK_6))
		{
			g_Edit[g_EditCount].rot.z -= D3DXToRadian(1);
		}

		// 向きの正規化
		if (g_Edit[g_EditCount].rot.x < -D3DX_PI)
		{
			g_Edit[g_EditCount].rot.x += D3DX_PI * 2.0f;
		}
		if (g_Edit[g_EditCount].rot.y < -D3DX_PI)
		{
			g_Edit[g_EditCount].rot.y += D3DX_PI * 2.0f;
		}
		if (g_Edit[g_EditCount].rot.z < -D3DX_PI)
		{
			g_Edit[g_EditCount].rot.z += D3DX_PI * 2.0f;
		}
		// 向きの正規化
		if (g_Edit[g_EditCount].rot.x > D3DX_PI)
		{
			g_Edit[g_EditCount].rot.x += -D3DX_PI * 2.0f;
		}
		if (g_Edit[g_EditCount].rot.y > D3DX_PI)
		{
			g_Edit[g_EditCount].rot.y += -D3DX_PI * 2.0f;
		}
		if (g_Edit[g_EditCount].rot.z > D3DX_PI)
		{
			g_Edit[g_EditCount].rot.z += -D3DX_PI * 2.0f;
		}

		// 編集オブジェクトの選択
		if (GetKeyboardPress(DIK_O) && g_EditCount < g_nNumBlock)
		{
			g_EditCount++;
		}
		else if (GetKeyboardPress(DIK_L) && g_EditCount > 0)
		{
			g_EditCount--;
		}
		
		// 編集オブジェクトの選択
		if (KeyboardTrigger(DIK_I) && g_EditCount < g_nNumBlock)
		{
			g_EditCount++;
		}
		else if (KeyboardTrigger(DIK_K) && g_EditCount > 0)
		{
			g_EditCount--;
		}

	}
	else if (EditMode2d)
	{
		UpdateEdit2d(); // 2D編集モード
	}

	if (KeyboardTrigger(DIK_F9) && !EditMode2d)
	{
		EditMode2d = true;
	}
	else if (KeyboardTrigger(DIK_F9) && EditMode2d)
	{
		EditMode2d = false;
	}


	if (KeyboardTrigger(DIK_F7))
	{
		SaveEdit(); // 書き出し
		SaveEdit2d(); // 書き出し
	}

	else if (KeyboardTrigger(DIK_F8) && nLoad == 0)
	{
		nLoad = 1; // 回数制限
		ReLoadEdit(); // リロード
		ReLoadEdit2d(); // リロード2d
	}

	if (KeyboardTrigger(DIK_F2) && nLoad == 1)
	{
		nLoad = 0; // 回数制限解除
	}

}

//=============================================================================================================
//エディット画面の描画処理
//=============================================================================================================
void DrawEdit(void)
{
	MODE mode = GetMode();
	EDIT_INFO* pEdit = GetEdit();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	for (int nCntBlock = 0; nCntBlock < MAX_OBJ; nCntBlock++)
	{
		if (!g_Edit[nCntBlock].bUse)
		{//未使用だったら
			//処理を飛ばす
			continue;
		}

		int nType = g_Edit[nCntBlock].nType;
		int EditCategory = g_Edit[nCntBlock].EditCategory;

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Edit[nCntBlock].mtxWorld);

		//向きを反映
		D3DXMatrixScaling(&mtxScal, g_Edit[nCntBlock].Scal.x, g_Edit[nCntBlock].Scal.y, g_Edit[nCntBlock].Scal.z);
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

		for (int nCntMat = 0; nCntMat < (int)g_Edit[nCntBlock].Category[EditCategory].pModel[nType].g_dwNumMatModel; nCntMat++)
		{
			//マテリアルのデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_Edit[nCntBlock].Category[EditCategory].pModel[nType].g_pBuffMatModel->GetBufferPointer();

			//選択中のブロックだったら
			if (nCntBlock == g_EditCount)
			{
				D3DXMATERIAL color;

				color = pMat[nCntMat];

				color.MatD3D.Diffuse.r = 1.0f;
				color.MatD3D.Diffuse.g = 0.5f;
				color.MatD3D.Diffuse.b = 0.5f;
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
			pDevice->SetTexture(0, g_Edit[nCntBlock].Category[EditCategory].pModel[nType].g_apTextureModel[nCntMat]);

			//ブロック(パーツ)の描画
			g_Edit[nCntBlock].Category[EditCategory].pModel[nType].g_pMeshModel->DrawSubset(nCntMat);
		}
		pDevice->SetMaterial(&matDef);
	}

	DrawEdit2d(); // 2D編集モードの時
}

//=============================================================================================================
//エディット画面のセーブ処理
//=============================================================================================================
void SaveEdit(void)
{
	FILE* pFile;

	pFile = fopen("data\\save.txt", "w");

	if (pFile != NULL)
	{
		fwrite(&g_EditCount, sizeof(int), 0, pFile);

		fprintf(pFile,

			"+==========================================+\n"
			"+	  マップエディター 作成者 吉田祐人      +\n"
			"+==========================================+\n\n");

		for (int nCnt = 0; nCnt < g_EditCount; nCnt++)
		{
			if (g_Edit[nCnt].bUse && g_Edit[nCnt].EditCategory == EDITMODE_BLOCK)
			{
				fprintf(pFile, "BLOCKSET\n");

				fprintf(pFile, "EDITCATEGORY = %d			# [ カテゴリー%d ] \n", g_Edit[nCnt].EditCategory,g_Edit[nCnt].EditCategory);

				fprintf(pFile, "   BLOCKTYPE = %d			# [ ブロックの種類 ]\n", g_Edit[nCnt].nType);

				fprintf(pFile, "   POS = %.1f %.1f %.1f			# [ 位置 ]\n", g_Edit[nCnt].pos.x, g_Edit[nCnt].pos.y, g_Edit[nCnt].pos.z);

				fprintf(pFile, "   ROT = %.3f %.3f %.3f			# [ 向き ]\n", g_Edit[nCnt].rot.x, g_Edit[nCnt].rot.y, g_Edit[nCnt].rot.z);

				fprintf(pFile, "   SIZE = %.1f %.1f %.1f		# [ 大きさ ]\n", g_Edit[nCnt].Scal.x, g_Edit[nCnt].Scal.y, g_Edit[nCnt].Scal.z);

				fprintf(pFile, "END_BLOCKSET\n\n");
			}
			else if (g_Edit[nCnt].bUse && g_Edit[nCnt].EditCategory == EDITMODE_ITEM)
			{
				fprintf(pFile, "ITEMSET\n");

				fprintf(pFile, "EDITCATEGORY = %d			# [ カテゴリー%d ]\n", g_Edit[nCnt].EditCategory, g_Edit[nCnt].EditCategory);

				fprintf(pFile, "   ITEMTYPE = %d			# [ アイテムの種類 ]\n", g_Edit[nCnt].nType);

				fprintf(pFile, "   POS = %.1f %.1f %.1f			# [ 位置 ]\n", g_Edit[nCnt].pos.x, g_Edit[nCnt].pos.y, g_Edit[nCnt].pos.z);

				fprintf(pFile, "   SIZE = %.1f %.1f %.1f		# [ 大きさ ]\n", g_Edit[nCnt].Scal.x, g_Edit[nCnt].Scal.y, g_Edit[nCnt].Scal.z);

				fprintf(pFile, "END_ITEMSET\n\n");
			}
		}

		fprintf(pFile, "BLOCK_COUNT = %d    # [ ブロックの配置数 ]\n", g_EditCount);
		fprintf(pFile, "END_SCRIPT          # この行は消さないでください");
	}
	else
	{
		//メッセージボックス
		MessageBox(NULL, "ファイルが開けません。", "エラー(SaveEdit)", MB_OK);
		return;
	}
	fclose(pFile);
}

//=============================================================================================================
//エディット画面のロード処理
//=============================================================================================================
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
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
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

	pFile = fopen("data\\save.txt", "r");

	if (pFile != NULL)
	{
		while (1)
		{
			char aString[MAX_WORD];

			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "BLOCKSET") == 0 || strcmp(aString, "ITEMSET") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "BLOCKTYPE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &nType);
					}
					else if (strcmp(aString, "ITEMTYPE") == 0)
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
					else if (strcmp(aString, "ROT") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &rot.x);
						fscanf(pFile, "%f", &rot.y);
						fscanf(pFile, "%f", &rot.z);
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
						SetBlock(pos, rot,nType, Scal);
						break;
					}
					else if (strcmp(aString, "END_ITEMSET") == 0)
					{
						SetItem(pos, nType, Scal);
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
				//メッセージボックス
		MessageBox(NULL, "ファイルが開けません。", "エラー(LoadEdit)", MB_OK);
		return;
	}
	fclose(pFile);
}
//=============================================================================================================
//エディットのリロード
//=============================================================================================================
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

	pFile = fopen("data\\save.txt", "r");

	if (pFile != NULL)
	{
		while (1)
		{
			char aString[MAX_WORD];

			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "BLOCKSET") == 0 || strcmp(aString, "ITEMSET") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "BLOCKTYPE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &g_Edit[nIdx].nType);
					}
					else if (strcmp(aString, "ITEMTYPE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &g_Edit[nIdx].nType);
					}
					else if (strcmp(aString, "EDITCATEGORY") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &g_Edit[nIdx].EditCategory);
					}
					else if (strcmp(aString, "POS") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &g_Edit[nIdx].pos.x);
						fscanf(pFile, "%f", &g_Edit[nIdx].pos.y);
						fscanf(pFile, "%f", &g_Edit[nIdx].pos.z);
					}
					else if (strcmp(aString, "ROT") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &g_Edit[nIdx].rot.x);
						fscanf(pFile, "%f", &g_Edit[nIdx].rot.y);
						fscanf(pFile, "%f", &g_Edit[nIdx].rot.z);
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
					else if (strcmp(aString, "END_ITEMSET") == 0)
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
				//メッセージボックス
		MessageBox(NULL, "ファイルが開けません。", "エラー(Edit.cpp)", MB_OK);
		return;
	}
	fclose(pFile);

	for (int nCnt = 0; nCnt < nCntobj; nCnt++)
	{
		g_Edit[g_EditCount].bUse = true; // 置かれていたブロックを使用状態にする
		g_Edit[g_EditCount].Category[g_Edit[g_EditCount].EditCategory].pModel[g_Edit[g_EditCount].nType] = g_BlockTexInfo[g_Edit[g_EditCount].EditCategory].pModel[g_Edit[g_EditCount].nType];
		g_Edit[g_EditCount].Category[g_Edit[g_EditCount].EditCategory].nNumModel = g_BlockTexInfo[g_Edit[g_EditCount].EditCategory].nNumModel;

		g_EditCount++;
		g_nNumBlock++;	
	}
	// 使用状態にする
	g_Edit[g_EditCount].bUse = true;
	g_Edit[g_EditCount].Category[g_Edit[g_EditCount].EditCategory].pModel[g_Edit[g_EditCount].nType] = g_BlockTexInfo[g_Edit[g_EditCount].EditCategory].pModel[g_Edit[g_EditCount].nType];
	g_Edit[g_EditCount].Category[g_Edit[g_EditCount].EditCategory].nNumModel = g_BlockTexInfo[g_Edit[g_EditCount].EditCategory].nNumModel;
}

//=============================================================================================================
//エディット習得
//=============================================================================================================
EDIT_INFO* GetEdit(void)
{
	return &g_Edit[0];
}
//=============================================================================================================
//オブジェクトの取得
//=============================================================================================================
int GetNumobj(void)
{
	return g_EditCount;
}
//=============================================================================================================
//オブジェクトのロード
//=============================================================================================================
void LoadEditObj(int category)
{
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//FILE* pFile;
	//char Skip[5];
	//int nCntModel = 0;

	//switch (category)
	//{
	//case EDITMODE_BLOCK:
	//	pFile = fopen("data\\MODEL_TXT\\BLOCK.txt", "r");
	//	break;
	//case EDITMODE_ITEM:
	//	pFile = fopen("data\\MODEL_TXT\\ITEM.txt", "r");
	//	break;
	//default:
	//	pFile = NULL;
	//	break;
	//}

	//if (pFile != NULL)
	//{
	//	char aString[256];

	//	while (1)
	//	{
	//		fscanf(pFile, "%s", &aString[0]);

	//		if (strcmp(aString, "MAX_TYPE") == 0)
	//		{
	//			fscanf(pFile, "%s", &Skip[0]);
	//			fscanf(pFile, "%d", &g_BlockTexInfo[category].nNumModel);
	//		}
	//		else if (strcmp(aString, "MODEL_FILENAME") == 0)
	//		{
	//			fscanf(pFile, "%s", &Skip[0]);

	//			fscanf(pFile, "%s", &aString[0]);

	//			const char* MODEL_FILENAME = {};

	//			MODEL_FILENAME = aString;

	//			//Xファイルの読み込み
	//			D3DXLoadMeshFromX(MODEL_FILENAME,
	//				D3DXMESH_SYSTEMMEM,
	//				pDevice,
	//				NULL,
	//				&g_BlockTexInfo[category].pModel[nCntModel].g_pBuffMatModel,
	//				NULL,
	//				&g_BlockTexInfo[category].pModel[nCntModel].g_dwNumMatModel,
	//				&g_BlockTexInfo[category].pModel[nCntModel].g_pMeshModel);

	//			nCntModel++;
	//		}
	//		else if (strcmp(aString, "END_SCRIPT") == 0)
	//		{
	//			break;
	//		}
	//	}
	//}
	//else
	//{
	//	//メッセージボックス
	//	MessageBox(NULL, "ファイルが開けません。", "エラー(Edit.cpp)", MB_OK);
	//	return;
	//}
	//fclose(pFile);
}
//===============================================================================================================
// エディット2dモードかどうか
//==============================================================================================================
bool GetEdit2d(void)
{
	return EditMode2d;
}
