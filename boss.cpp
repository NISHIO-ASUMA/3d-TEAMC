//============================
//
// ボス [boss.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
// インクルードファイル
//****************************
#include "boss.h"
#include "input.h"
#include "mouse.h"
#include "camera.h"

//****************************
// マクロ定義
//****************************
#define MAX_WORD (256) // 最大の文字数

//****************************
// プロトタイプ宣言
//****************************
void LoadBoss(void);

//****************************
// グローバル変数宣言
//****************************
Boss g_Boss; // ボスの情報

//=============================
// ボスの初期化処理
//=============================
void InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	g_Boss.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 座標
	g_Boss.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// 移動量
	g_Boss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 角度
	g_Boss.bUse = false;						// 未使用状態
	g_Boss.nLife = 20;							// 体力
	g_Boss.state = BOSSSTATE_NORMAL;			// 状態

	LoadBoss(); // ボスのロード

	D3DXMATERIAL* pMat;//マテリアルへのポインタ

	for (int nCntModel = 0; nCntModel < g_Boss.Motion.nNumModel; nCntModel++)
	{
		//マテリアルのデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_Boss.Motion.aModel[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_Boss.Motion.aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//このファイル名を使用してテクスチャを読み込む
				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_Boss.Motion.aModel[nCntModel].pTexture[nCntMat]);
			}
		}
	}

	//頂点座標の取得
	int nNumVtx;//頂点数
	DWORD sizeFVF;//頂点フォーマットのサイズ
	BYTE* pVtxBuff;//頂点バッファへのポインタ

	for (int nCntModel = 0; nCntModel < g_Boss.Motion.nNumModel; nCntModel++)
	{
		//頂点数の取得
		nNumVtx = g_Boss.Motion.aModel[nCntModel].pMesh->GetNumVertices();

		//頂点フォーマットのサイズ取得
		sizeFVF = D3DXGetFVFVertexSize(g_Boss.Motion.aModel[nCntModel].pMesh->GetFVF());

		//頂点バッファのロック
		g_Boss.Motion.aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//頂点座標を比較してボスの最小値,最大値を取得
			if (vtx.x < g_Boss.Motion.aModel[nCntModel].vtxMin.x)
			{
				g_Boss.Motion.aModel[nCntModel].vtxMin.x = vtx.x;
			}
			else if (vtx.y < g_Boss.Motion.aModel[nCntModel].vtxMin.y)
			{
				g_Boss.Motion.aModel[nCntModel].vtxMin.y = vtx.y;
			}
			else if (vtx.z < g_Boss.Motion.aModel[nCntModel].vtxMin.z)
			{
				g_Boss.Motion.aModel[nCntModel].vtxMin.z = vtx.z;
			}
			else if (vtx.x > g_Boss.Motion.aModel[nCntModel].vtxMax.x)
			{
				g_Boss.Motion.aModel[nCntModel].vtxMax.x = vtx.x;
			}
			else if (vtx.y > g_Boss.Motion.aModel[nCntModel].vtxMax.y)
			{
				g_Boss.Motion.aModel[nCntModel].vtxMax.y = vtx.y;
			}
			else if (vtx.z > g_Boss.Motion.aModel[nCntModel].vtxMax.z)
			{
				g_Boss.Motion.aModel[nCntModel].vtxMax.z = vtx.z;
			}

			//頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;

			//サイズに代入
			g_Boss.Motion.aModel[nCntModel].Size.x = g_Boss.Motion.aModel[nCntModel].vtxMax.x - g_Boss.Motion.aModel[nCntModel].vtxMin.x;
			g_Boss.Motion.aModel[nCntModel].Size.y = g_Boss.Motion.aModel[nCntModel].vtxMax.y - g_Boss.Motion.aModel[nCntModel].vtxMin.y;
			g_Boss.Motion.aModel[nCntModel].Size.z = g_Boss.Motion.aModel[nCntModel].vtxMax.z - g_Boss.Motion.aModel[nCntModel].vtxMin.z;
		}

		//頂点バッファのアンロック
		g_Boss.Motion.aModel[nCntModel].pMesh->UnlockVertexBuffer();
	}
}
//=============================
// ボスの終了処理
//=============================
void UninitBoss(void)
{
	for (int nCntModel = 0; nCntModel < g_Boss.Motion.nNumModel; nCntModel++)
	{
		//テクスチャの破棄
		for (int nCntTex = 0; nCntTex < (int)g_Boss.Motion.aModel[nCntModel].dwNumMat; nCntTex++)
		{
			if (g_Boss.Motion.aModel[nCntModel].pTexture[nCntTex] != NULL)
			{
				g_Boss.Motion.aModel[nCntModel].pTexture[nCntTex]->Release();
				g_Boss.Motion.aModel[nCntModel].pTexture[nCntTex] = NULL;
			}
		}

		//メッシュの破棄
		if (g_Boss.Motion.aModel[nCntModel].pMesh != NULL)
		{
			g_Boss.Motion.aModel[nCntModel].pMesh->Release();
			g_Boss.Motion.aModel[nCntModel].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_Boss.Motion.aModel[nCntModel].pBuffMat != NULL)
		{
			g_Boss.Motion.aModel[nCntModel].pBuffMat->Release();
			g_Boss.Motion.aModel[nCntModel].pBuffMat = NULL;
		}
	}
}
//=============================
// ボスの更新処理
//=============================
void UpdateBoss(void)
{
	UpdateMotion(&g_Boss.Motion);
}
//=============================
// ボスの描画処理
//=============================
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxSize;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	if (g_Boss.bUse == true)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Boss.mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Boss.rot.y, g_Boss.rot.x, g_Boss.rot.z);
		D3DXMatrixMultiply(&g_Boss.mtxWorld, &g_Boss.mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Boss.pos.x, g_Boss.pos.y, g_Boss.pos.z);
		D3DXMatrixMultiply(&g_Boss.mtxWorld, &g_Boss.mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Boss.mtxWorld);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//全モデルパーツの描画
		for (int nCntModel = 0; nCntModel < g_Boss.Motion.nNumModel; nCntModel++)
		{
			D3DXMATRIX mtxRotModel, mtxTransform;//計算用
			D3DXMATRIX mtxParent;//親のマトリックス

			//パーツのマトリックスの初期化
			D3DXMatrixIdentity(&g_Boss.Motion.aModel[nCntModel].mtxWorld);

			//パーツの向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_Boss.Motion.aModel[nCntModel].rot.y, g_Boss.Motion.aModel[nCntModel].rot.x, g_Boss.Motion.aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_Boss.Motion.aModel[nCntModel].mtxWorld, &g_Boss.Motion.aModel[nCntModel].mtxWorld, &mtxRotModel);

			//パーツの位置(オフセット)を反映
			D3DXMatrixTranslation(&mtxTransform, g_Boss.Motion.aModel[nCntModel].pos.x, g_Boss.Motion.aModel[nCntModel].pos.y, g_Boss.Motion.aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_Boss.Motion.aModel[nCntModel].mtxWorld, &g_Boss.Motion.aModel[nCntModel].mtxWorld, &mtxTransform);

			//パーツの[親のマトリックス]を設定
			if (g_Boss.Motion.aModel[nCntModel].nIdxModelParent != -1)
			{
				//親モデルがある場合
				mtxParent = g_Boss.Motion.aModel[g_Boss.Motion.aModel[nCntModel].nIdxModelParent].mtxWorld;
			}
			else
			{//親モデルがない場合
				mtxParent = g_Boss.mtxWorld;
			}

			//算出した[パーツのワールドマトリックス]と[親のマトリックス]をかけあわせる
			D3DXMatrixMultiply(&g_Boss.Motion.aModel[nCntModel].mtxWorld,
				&g_Boss.Motion.aModel[nCntModel].mtxWorld,
				&mtxParent);//自分自分親

			//パーツのワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD,
				&g_Boss.Motion.aModel[nCntModel].mtxWorld);

			for (int nCntMat = 0; nCntMat < (int)g_Boss.Motion.aModel[nCntModel].dwNumMat; nCntMat++)
			{
				//マテリアルのデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_Boss.Motion.aModel[nCntModel].pBuffMat->GetBufferPointer();

				D3DXMATERIAL color;

				if (g_Boss.state != BOSSSTATE_DAMAGE)
				{
					//マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				}
				else if (g_Boss.state == BOSSSTATE_DAMAGE)
				{
					color = pMat[nCntMat];

					color.MatD3D.Diffuse.r = 1.0f;
					color.MatD3D.Diffuse.g = 0.3f;
					color.MatD3D.Diffuse.b = 0.3f;
					color.MatD3D.Diffuse.a = 1.0f;

					//マテリアルの設定
					pDevice->SetMaterial(&color.MatD3D);
				}

				//テクスチャの設定
				pDevice->SetTexture(0, g_Boss.Motion.aModel[nCntModel].pTexture[nCntMat]);

				//モデル(パーツ)の描画
				g_Boss.Motion.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
			}
			//マテリアルの設定
			pDevice->SetMaterial(&matDef);
		}
	}
}
//=============================
// ボスの設定処理
//=============================
void SetBoss(D3DXVECTOR3 pos, float speed, int nLife)
{
	if (!g_Boss.bUse)
	{
		g_Boss.pos = pos; // 位置を代入
		g_Boss.Speed = speed; // 足の速さ
		g_Boss.nLife = nLife; // 体力を挿入
		g_Boss.bUse = true;   // 使用状態にする
	}
}

void LoadBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタ

	FILE* pFile; // ファイルのポインタ

	pFile = fopen("data\\MOTION\\boss.txt", "r");

	int nCntMotion = 0;
	char Skip[3];
	int nCntKey = 0;
	int nCntPosKey = 0;
	int nCntRotkey = 0;
	int nCnt = 0;
	int nNumParts = 0;
	int nIdx = 0;

	if (pFile != NULL)
	{//　NULL じゃない
		char aString[MAX_WORD];

		while (1)
		{
			// 読み飛ばし
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "SCRIPT") == 0)
			{// SCRIPTを読み取ったら
				while (1)
				{
					// 読み飛ばし
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NUM_MODEL") == 0)
					{// NUM_MODELを読み取ったら
						fscanf(pFile, "%s", &aString[0]);

						if (strcmp(&aString[0], "=") == 0)
						{// 値を代入
							fscanf(pFile, "%d", &g_Boss.Motion.nNumModel);
						}
					}

					else if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
					{
						// MODEL_FILENAMEを読み取ったら
						fscanf(pFile, "%s", &aString[0]);

						if (strcmp(&aString[0], "=") == 0)
						{// 代入
							// 文字列を読み込む
							fscanf(pFile, "%s", &aString[0]);

							const char* MODEL_FILE = {};	// モデルファイル名格納用の変数

							// 読み込んだ文字列を保存する
							MODEL_FILE = aString;

							//Xファイルの読み込み
							D3DXLoadMeshFromX(MODEL_FILE,
								D3DXMESH_SYSTEMMEM,
								pDevice,
								NULL,
								&g_Boss.Motion.aModel[nCnt].pBuffMat,
								NULL,
								&g_Boss.Motion.aModel[nCnt].dwNumMat,
								&g_Boss.Motion.aModel[nCnt].pMesh);

							nCnt++; // nCntをインクリメント
						}
					}
					else if (strcmp(&aString[0], "CHARACTERSET") == 0)
					{
						while (1)
						{
							// 文字列を読み飛ばす
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "NUM_PARTS") == 0)
							{// NUM_PARTSを読み取ったら

								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "=") == 0)
								{// 値を代入
									fscanf(pFile, "%d", &nNumParts);
								}
							}

							else if (strcmp(&aString[0], "PARTSSET") == 0)
							{
								while (1)
								{
									// 文字列を読み飛ばす
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "INDEX") == 0)
									{// INDEXを読み取ったら

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// 代入
											fscanf(pFile, "%d", &nIdx);
										}
									}

									if (strcmp(&aString[0], "PARENT") == 0)
									{// PARENTを読み取ったら

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// 代入
											// ペアネント
											fscanf(pFile, "%d", &g_Boss.Motion.aModel[nIdx].nIdxModelParent);
										}
									}


									if (strcmp(&aString[0], "POS") == 0)
									{// INDEXを読み取ったら

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// 座標を代入
											fscanf(pFile, "%f", &g_Boss.Motion.aModel[nIdx].offpos.x);
											fscanf(pFile, "%f", &g_Boss.Motion.aModel[nIdx].offpos.y);
											fscanf(pFile, "%f", &g_Boss.Motion.aModel[nIdx].offpos.z);
										}
									}

									if (strcmp(&aString[0], "ROT") == 0)
									{// INDEXを読み取ったら

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// 角度を代入
											fscanf(pFile, "%f", &g_Boss.Motion.aModel[nIdx].rot.x);
											fscanf(pFile, "%f", &g_Boss.Motion.aModel[nIdx].rot.y);
											fscanf(pFile, "%f", &g_Boss.Motion.aModel[nIdx].rot.z);
										}
									}

									if (strcmp(&aString[0], "END_PARTSSET") == 0)
									{// END_PARTSSETを読み取ったら
										// whileを抜ける
										break;
									}

								}// while文末
							}
							else if (strcmp(&aString[0], "END_CHARACTERSET") == 0)
							{
								break;
							}
						}
					}
					else if (strcmp(&aString[0], "MOTIONSET") == 0)
					{// MOTIONSETを読み取ったら


						while (1)
						{
							// 文字を読み飛ばす
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(aString, "LOOP") == 0)
							{// LOOP を読み取ったら
								// 文字を読み飛ばす
								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "=") == 0)
								{// = を読み取ったら
									// 値を代入
									fscanf(pFile, "%d", &g_Boss.Motion.aMotionInfo[nCntMotion].bLoop);
								}
							}

							else if (strcmp(aString, "NUM_KEY") == 0)
							{// NUM_KEYを読み取ったら
								// 文字を読み飛ばす
								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "=") == 0)
								{// = を読み取ったら
									// 値を代入
									fscanf(pFile, "%d", &g_Boss.Motion.aMotionInfo[nCntMotion].nNumkey);
								}

								while (nCntKey < g_Boss.Motion.aMotionInfo[nCntMotion].nNumkey)
								{
									// 文字を読み飛ばす
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(aString, "KEYSET") == 0)
									{// KEYSETを読み取ったら


										while (1)
										{
											// 文字を読み飛ばす
											fscanf(pFile, "%s", &aString[0]);


											if (strcmp(&aString[0], "FRAME") == 0)
											{// FRAMEを読み取ったら
												// 文字を読み飛ばす
												fscanf(pFile, "%s", &aString[0]);

												if (strcmp(&aString[0], "=") == 0)
												{// 値を代入
													fscanf(pFile, "%d", &g_Boss.Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
													break;
												}
											}

										}

										while (1)
										{
											// 文字を読み飛ばす
											fscanf(pFile, "%s", &aString[0]);
											if (strcmp(&aString[0], "KEY") == 0)
											{// KEYを読みとったら
												while (1)
												{
													// 文字を読み飛ばす
													fscanf(pFile, "%s", &aString[0]);

													if (strcmp(&aString[0], "POS") == 0)
													{// POSを読み取ったら
														// 文字を読み飛ばす
														fscanf(pFile, "%s", &aString[0]);

														if (strcmp(&aString[0], "=") == 0)
														{// 値を代入
															fscanf(pFile, "%f", &g_Boss.Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosX);
															fscanf(pFile, "%f", &g_Boss.Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosY);
															fscanf(pFile, "%f", &g_Boss.Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosZ);
															nCntPosKey++;		// インクリメント
														}
													}

													else if (strcmp(&aString[0], "ROT") == 0)
													{// ROTを読み取ったら
														// 文字を読み飛ばす
														fscanf(pFile, "%s", &aString[0]);

														if (strcmp(&aString[0], "=") == 0)
														{// 値を代入
															fscanf(pFile, "%f", &g_Boss.Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotX);
															fscanf(pFile, "%f", &g_Boss.Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotY);
															fscanf(pFile, "%f", &g_Boss.Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotZ);
															nCntRotkey++;		// インクリメント
														}
													}

													else if (strcmp(&aString[0], "END_KEY") == 0)
													{// END_KEYを読み取ったら
														break;
													}
												}
											}
											else if (strcmp(&aString[0], "END_KEYSET") == 0)
											{// END_KEYSETを読み取ったら
												nCntRotkey = 0;
												nCntPosKey = 0;
												nCntKey++;			// インクリメント
												break;
											}


										}

									}

								}
							}

							if (strcmp(&aString[0], "END_MOTIONSET") == 0)
							{// END_MOTIONSETを読み取ったら
								nCntMotion++;		// モーションの更新
								nCntKey = 0;		// 0から始める
								break;
							}
						}
					}

					else if (strcmp(&aString[0], "END_SCRIPT") == 0)
					{
						break;
					}
					else
					{
						continue;
					}
				}// while文末

				break;
			}
		}// while文末
	}
	else
	{
		//メッセージボックス
		MessageBox(NULL, "ファイルが開けません。", "エラー(Boss.cpp)", MB_OK);
		return;
	}
	// ファイルを閉じる
	fclose(pFile);
}
