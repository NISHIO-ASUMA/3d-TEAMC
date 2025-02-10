//==============================================================================================================
//
// 影[shadow.cpp]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
#include"edit2d.h"
#include "player.h"
#include "camera.h"
#include "input.h"

//**************************************************************************************************************
//マクロ定義
//**************************************************************************************************************
#define MAX_EDIT (256)

//**************************************************************************************************************
//グローバル変数宣言
//**************************************************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEdit2d = NULL; //頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureEdit2d[POLYGON_TYPE_MAX] = {};//テクスチャへのポインタ保存用
EDIT_INFO_2D g_Edit2d[MAX_EDIT];//影の構造体
int g_EditCnt;
int g_ObjCount;

//===============================================================================================================
//影の初期化処理
//===============================================================================================================
void InitEdit2d(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	for (int nCntEdit2d = 0; nCntEdit2d < POLYGON_TYPE_MAX; nCntEdit2d++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			POLYGON_TYPE[nCntEdit2d],
			&g_pTextureEdit2d[nCntEdit2d]);
	}

	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EDIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEdit2d,
		NULL);

	VERTEX_3D* pVtx = NULL;

	for (int nCntEdit2d = 0; nCntEdit2d < MAX_EDIT; nCntEdit2d++)
	{
		g_Edit2d[nCntEdit2d].pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_Edit2d[nCntEdit2d].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Edit2d[nCntEdit2d].fWidth = 100.0f;
		g_Edit2d[nCntEdit2d].fHeight = 100.0f;
		g_Edit2d[nCntEdit2d].fLength = 0.0f;
		g_Edit2d[nCntEdit2d].bUse = false;
		g_Edit2d[nCntEdit2d].fMove = 10.0f;
	}

	g_Edit2d[0].bUse = true;
	g_EditCnt = 0;
	g_ObjCount = 0;

	//頂点バッファをロック
	g_pVtxBuffEdit2d->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEdit2d = 0; nCntEdit2d < MAX_EDIT; nCntEdit2d++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 50.0f);
		pVtx[1].pos = D3DXVECTOR3(50.0f, 0.0f, 50.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(50.0f, 0.0f, 0.0f);

		//各頂点の法線の設定(ベクトルの大きさは１にする必要がある)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffEdit2d->Unlock();
}
//===============================================================================================================
//影の終了処理
//===============================================================================================================
void UninitEdit2d(void)
{
	for (int nCntEdit2d = 0; nCntEdit2d < POLYGON_TYPE_MAX; nCntEdit2d++)
	{
		if (g_pTextureEdit2d[nCntEdit2d] != NULL)
		{
			g_pTextureEdit2d[nCntEdit2d]->Release();
			g_pTextureEdit2d[nCntEdit2d] = NULL;
		}
	}

	//頂点バッファの解放
	if (g_pVtxBuffEdit2d != NULL)
	{
		g_pVtxBuffEdit2d->Release();
		g_pVtxBuffEdit2d = NULL;
	}
}
//===============================================================================================================
//影の更新処理
//===============================================================================================================
void UpdateEdit2d(void)
{
	Camera* pCamera = GetCamera();

	VERTEX_3D* pVtx;

	//頂点バッファをロック
	g_pVtxBuffEdit2d->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Edit2d[g_EditCnt].bUse)
	{
		//移動量の減衰
		g_Edit2d[g_EditCnt].move.x += (0.0f - g_Edit2d[g_EditCnt].move.x) * 0.5f;
		g_Edit2d[g_EditCnt].move.y += (0.0f - g_Edit2d[g_EditCnt].move.y) * 0.5f;
		g_Edit2d[g_EditCnt].move.z += (0.0f - g_Edit2d[g_EditCnt].move.z) * 0.5f;

		//位置の更新
		g_Edit2d[g_EditCnt].pos += g_Edit2d[g_EditCnt].move;

		//上移動
		if (GetKeyboardPress(DIK_W))
		{
			g_Edit2d[g_EditCnt].move.x += sinf(pCamera->rot.y) * g_Edit2d[g_EditCnt].fMove;
			g_Edit2d[g_EditCnt].move.z += cosf(pCamera->rot.y) * g_Edit2d[g_EditCnt].fMove;
		}
		//左移動
		if (GetKeyboardPress(DIK_A))
		{
			g_Edit2d[g_EditCnt].move.x -= cosf(-pCamera->rot.y) * g_Edit2d[g_EditCnt].fMove;
			g_Edit2d[g_EditCnt].move.z -= sinf(-pCamera->rot.y) * g_Edit2d[g_EditCnt].fMove;
		}
		//下移動
		if (GetKeyboardPress(DIK_S))
		{
			g_Edit2d[g_EditCnt].move.x -= sinf(pCamera->rot.y) * g_Edit2d[g_EditCnt].fMove;
			g_Edit2d[g_EditCnt].move.z -= cosf(pCamera->rot.y) * g_Edit2d[g_EditCnt].fMove;
		}
		//右移動
		if (GetKeyboardPress(DIK_D))
		{
			g_Edit2d[g_EditCnt].move.x += cosf(-pCamera->rot.y) * g_Edit2d[g_EditCnt].fMove;
			g_Edit2d[g_EditCnt].move.z += sinf(-pCamera->rot.y) * g_Edit2d[g_EditCnt].fMove;
		}

		//下移動
		if (GetKeyboardPress(DIK_Q))
		{
			g_Edit2d[g_EditCnt].move.y -= g_Edit2d[g_EditCnt].fMove;
		}
		//右移動
		if (GetKeyboardPress(DIK_E))
		{
			g_Edit2d[g_EditCnt].move.y += g_Edit2d[g_EditCnt].fMove;
		}

		if (KeyboardTrigger(DIK_RETURN))
		{
			g_Edit2d[g_EditCnt + 1] = g_Edit2d[g_EditCnt];
			g_Edit2d[g_EditCnt + 1].bUse = true;

			g_EditCnt++;
		}

		// 大きさ変更(拡大)
		if (GetKeyboardPress(DIK_RIGHT))
		{
			g_Edit2d[g_EditCnt].fWidth += 1.0f;
		}
		else if (GetKeyboardPress(DIK_UP))
		{
			g_Edit2d[g_EditCnt].fHeight += 1.0f;
		}

		// 大きさ変更(縮小)
		if (GetKeyboardPress(DIK_LEFT) && g_Edit2d[g_EditCnt].fWidth > 1.0f)
		{
			g_Edit2d[g_EditCnt].fWidth -= 1.0f;
		}
		else if (GetKeyboardPress(DIK_DOWN) && g_Edit2d[g_EditCnt].fHeight > 1.0f)
		{
			g_Edit2d[g_EditCnt].fHeight -= 1.0f;
		}

		// 回転Y
		if (GetKeyboardPress(DIK_Z))
		{
			g_Edit2d[g_EditCnt].rot.y -= 0.001f;
		}
		if (GetKeyboardPress(DIK_C))
		{
			g_Edit2d[g_EditCnt].rot.y += 0.001f;
		}

		// 回転X
		if (GetKeyboardPress(DIK_V))
		{
			g_Edit2d[g_EditCnt].rot.x -= 0.001f;
		}
		if (GetKeyboardPress(DIK_B))
		{
			g_Edit2d[g_EditCnt].rot.x += 0.001f;
		}

		// 回転Z
		if (GetKeyboardPress(DIK_F))
		{
			g_Edit2d[g_EditCnt].rot.z -= 0.001f;
		}
		if (GetKeyboardPress(DIK_G))
		{
			g_Edit2d[g_EditCnt].rot.z += 0.001f;
		}

		// カメラを選択中のオブジェクトの場所へ移動
		if (KeyboardTrigger(DIK_F6))
		{
			pCamera->posV.x = g_Edit2d[g_EditCnt].pos.x;
			pCamera->posV.z = g_Edit2d[g_EditCnt].pos.z;
		}

		//ブロック消去
		if (KeyboardTrigger(DIK_BACKSPACE) && g_EditCnt > 0)
		{
			g_Edit2d[g_EditCnt].bUse = false; // 選択中のオブジェクトを消す
			g_EditCnt--;                    // オブジェクトのカウントを減らす
		}

		if (GetKeyboardPress(DIK_O))
		{
			g_Edit2d[g_EditCnt].fMove += 0.1f; // 移動量変更
		}
		else if (GetKeyboardPress(DIK_I) && g_Edit2d[g_EditCnt].fMove > 0.0f)
		{
			g_Edit2d[g_EditCnt].fMove -= 0.1f; // 移動量変更
		}

		if (KeyboardTrigger(DIK_1)&& g_Edit2d[g_EditCnt].nType < POLYGON_TYPE_MAX - 1)
		{
			g_Edit2d[g_EditCnt].nType++;
		}
		else if (KeyboardTrigger(DIK_2) && g_Edit2d[g_EditCnt].nType > 0)
		{
			g_Edit2d[g_EditCnt].nType--;
		}

		pVtx += 4 * g_EditCnt;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Edit2d[g_EditCnt].pos.x - g_Edit2d[g_EditCnt].fWidth,0.0f, g_Edit2d[g_EditCnt].pos.z + g_Edit2d[g_EditCnt].fHeight);
		pVtx[1].pos = D3DXVECTOR3(g_Edit2d[g_EditCnt].pos.x + g_Edit2d[g_EditCnt].fWidth, 0.0f, g_Edit2d[g_EditCnt].pos.z + g_Edit2d[g_EditCnt].fHeight);
		pVtx[2].pos = D3DXVECTOR3(g_Edit2d[g_EditCnt].pos.x - g_Edit2d[g_EditCnt].fWidth, 0.0f, g_Edit2d[g_EditCnt].pos.z - g_Edit2d[g_EditCnt].fHeight);
		pVtx[3].pos = D3DXVECTOR3(g_Edit2d[g_EditCnt].pos.x + g_Edit2d[g_EditCnt].fWidth, 0.0f, g_Edit2d[g_EditCnt].pos.z - g_Edit2d[g_EditCnt].fHeight);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	g_pVtxBuffEdit2d->Unlock();
}
//===============================================================================================================
//影の描画処理
//===============================================================================================================
void DrawEdit2d(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCntEdit2d = 0; nCntEdit2d < MAX_EDIT; nCntEdit2d++)
	{
		if (!g_Edit2d[nCntEdit2d].bUse)
		{
			continue;
		}

		// 種類を代入
		int nType = g_Edit2d[nCntEdit2d].nType;

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Edit2d[nCntEdit2d].mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Edit2d[nCntEdit2d].rot.y, g_Edit2d[nCntEdit2d].rot.x, g_Edit2d[nCntEdit2d].rot.z);
		D3DXMatrixMultiply(&g_Edit2d[nCntEdit2d].mtxWorld, &g_Edit2d[nCntEdit2d].mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Edit2d[nCntEdit2d].pos.x, g_Edit2d[nCntEdit2d].pos.y, g_Edit2d[nCntEdit2d].pos.z);
		D3DXMatrixMultiply(&g_Edit2d[nCntEdit2d].mtxWorld, &g_Edit2d[nCntEdit2d].mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Edit2d[nCntEdit2d].mtxWorld);

		//頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffEdit2d, 0, sizeof(VERTEX_3D));

		//テクスチャフォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureEdit2d[nType]);
		
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEdit2d * 4, 2);
	}
}
//========================================================================================================
// 書き出し処理
//========================================================================================================
void SaveEdit2d(void)
{
	FILE* pFile;

	pFile = fopen("data\\save2d.txt", "w");

	if (pFile != NULL)
	{
		fwrite(&g_EditCnt, sizeof(int), 0, pFile);

		fprintf(pFile,

			"+==========================================+\n"
			"+	  マップエディター 作成者 吉田祐人      +\n"
			"+==========================================+\n\n");

		for (int nCnt = 0; nCnt < g_EditCnt; nCnt++)
		{
			if (g_Edit2d[nCnt].bUse)
			{
				fprintf(pFile, "POLYGONSET\n");

				fprintf(pFile, "   TEX_TYPE = %d					# [ 2dオブジェクトの種類 ]\n", g_Edit2d[nCnt].nType);

				fprintf(pFile, "   POS = %.1f %.1f %.1f				# [ 位置 ]\n", g_Edit2d[nCnt].pos.x, g_Edit2d[nCnt].pos.y, g_Edit2d[nCnt].pos.z);

				fprintf(pFile, "   ROT = %.1f %.1f %.1f				# [ 向き ]\n", g_Edit2d[nCnt].rot.x, g_Edit2d[nCnt].rot.y, g_Edit2d[nCnt].rot.z);

				fprintf(pFile, "   WIDTH = %.1f						# [ 横幅 ]\n", g_Edit2d[nCnt].fWidth);

				fprintf(pFile, "   HEIGHT = %.1f					# [ 高さ ]\n", g_Edit2d[nCnt].fHeight);

				fprintf(pFile, "END_POLYGONSET\n\n");
			}
		}

		fprintf(pFile, "OBJ_COUNT = %d    # [ 2dオブジェクトの配置数 ]\n", g_EditCnt);
		fprintf(pFile, "END_SCRIPT          # この行は消さないでください");
	}
	else
	{
		//メッセージボックス
		MessageBox(NULL, "ファイルが開けません。", "エラー(SaveEdit2d)", MB_OK);
		return;
	}
	fclose(pFile);

}
//========================================================================================================
// 書き出し処理
//========================================================================================================
void LoadEdit2d(void)
{
	FILE* pFile;

	D3DXVECTOR3 pos, rot;
	int nType = 0;
	D3DXVECTOR2 fLength;
	char skip[3];

	pFile = fopen("data\\save2d.txt", "r");

	if (pFile != NULL)
	{
		char aString[256];

		while (1)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "POLYGONSET") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "TEX_TYPE") == 0)
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

					else if (strcmp(aString, "WIDTH") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &fLength.x);
					}
					else if (strcmp(aString, "HEIGHT") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &fLength.y);
					}
					else if (strcmp(aString, "END_POLYGONSET") == 0)
					{
						SetPolygon(pos, rot, fLength.x, fLength.y, nType);
						break;
					}
				}
			}
			if (strcmp(aString, "OBJ_COUNT") == 0)
			{
				fscanf(pFile, "%s", &skip[0]);
				fscanf(pFile, "%d", &g_ObjCount);
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
		MessageBox(NULL, "ファイルが開けません。", "エラー(LoadEdit2d)", MB_OK);
		return;
	}
	fclose(pFile);
}
//========================================================================================================
// 再読み込み処理
//========================================================================================================
void ReLoadEdit2d(void)
{
	FILE* pFile;

	D3DXVECTOR3 pos, rot;
	int nType = 0;
	D3DXVECTOR2 fLength;
	char skip[3];
	int nIdx = 0;

	pFile = fopen("data\\save2d.txt", "r");

	if (pFile != NULL)
	{
		char aString[256];

		while (1)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "POLYGONSET") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "TEX_TYPE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &g_Edit2d[nIdx].nType);
					}
					else if (strcmp(aString, "POS") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &g_Edit2d[nIdx].pos.x);
						fscanf(pFile, "%f", &g_Edit2d[nIdx].pos.y);
						fscanf(pFile, "%f", &g_Edit2d[nIdx].pos.z);
					}
					else if (strcmp(aString, "ROT") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &g_Edit2d[nIdx].rot.x);
						fscanf(pFile, "%f", &g_Edit2d[nIdx].rot.y);
						fscanf(pFile, "%f", &g_Edit2d[nIdx].rot.z);
					}

					else if (strcmp(aString, "WIDTH") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &g_Edit2d[nIdx].fWidth);
					}
					else if (strcmp(aString, "HEIGHT") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &g_Edit2d[nIdx].fHeight);
					}
					else if (strcmp(aString, "END_POLYGONSET") == 0)
					{
						nIdx++;
						break;
					}
				}
			}
			if (strcmp(aString, "OBJ_COUNT") == 0)
			{
				fscanf(pFile, "%s", &skip[0]);
				fscanf(pFile, "%d", &g_ObjCount);
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
		MessageBox(NULL, "ファイルが開けません。", "エラー(ReLoadEdit2d)", MB_OK);
		return;
	}
	fclose(pFile);

	for (int nCnt = 0; nCnt < g_ObjCount; nCnt++)
	{
		g_Edit2d[g_EditCnt].bUse = true;

		if (nCnt != g_ObjCount - 1)
		{
			g_EditCnt++;
		}
	}
	g_Edit2d[g_EditCnt].bUse = true;
}
//======================================================================================================================
// エディット2dの取得処理
//======================================================================================================================
EDIT_INFO_2D* GetEditInfo2D(void)
{
	return &g_Edit2d[0];
}
//======================================================================================================================
// 配置数の取得
//======================================================================================================================
int GetNum2d(void)
{
	return g_EditCnt;
}

