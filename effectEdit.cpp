////============================
////
//// パーティクル[particle.cpp]
//// Author:YOSHIDA YUUTO
////
////============================
//
////****************************
////インクルードファイル
////****************************
//#include "input.h"
//#include "player.h"
//#include "camera.h"
//#include "effect.h"
//#include "effectEdit.h"
//#include "meshfield.h"
//#include <stdio.h>
//
////****************************
////マクロ定義
////****************************
//#define BULLET_WIDTH (25.0f) //横幅
//#define BULLET_HEIGHT (25.0f) //高さ
//#define MAX_EFFECTEDIT (256) //弾の最大数
//#define MAX_MOVE (5.0f) //移動量
//
////****************************
////グローバル変数宣言
////****************************
////SETPARTICLE g_EditEffe[MAX_EFFECTEDIT];
//int g_nCntEffect;
//int g_bEnableLife;
//
////============================
////弾の初期化処理
////============================
//void InitEffectEdit(void)
//{
//	InitParticle();
//
//	InitEffect();
//
//	InitCamera();
//
//	InitMeshField();
//
//	LPDIRECT3DDEVICE9 pDevice;
//
//	pDevice = GetDevice();
//
//	for (int nCnt = 0; nCnt < MAX_EFFECTEDIT; nCnt++)
//	{
//		g_EditEffe[nCnt].pos = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
//		g_EditEffe[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		g_EditEffe[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		g_EditEffe[nCnt].nLife = 10;
//		g_EditEffe[nCnt].nType = 0;
//		g_EditEffe[nCnt].fSize = 10.0f;
//		g_EditEffe[nCnt].nNumParticle = 10;
//		g_EditEffe[nCnt].ColorType = COLORCHANGE_R;
//		g_EditEffe[nCnt].AngleMax = 628;
//		g_EditEffe[nCnt].AngleMin = -314;
//		g_EditEffe[nCnt].fSpeed = 1.0f;
//		g_EditEffe[nCnt].decfAlv = 0.02f;
//		g_EditEffe[nCnt].dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//		g_EditEffe[nCnt].bUse = false;
//	}
//	g_bEnableLife = false;
//
//	g_nCntEffect = 0;
//	g_EditEffe[0].bUse = true;
//
//	SetParticle(D3DXVECTOR3(g_EditEffe[g_nCntEffect].pos.x, g_EditEffe[g_nCntEffect].pos.y, g_EditEffe[g_nCntEffect].pos.z),
//		D3DXVECTOR3(g_EditEffe[g_nCntEffect].move.x, g_EditEffe[g_nCntEffect].move.y, g_EditEffe[g_nCntEffect].move.z),
//		g_EditEffe[g_nCntEffect].nLife,
//		g_EditEffe[g_nCntEffect].col,
//		0,
//		g_EditEffe[g_nCntEffect].fSize,
//		g_EditEffe[g_nCntEffect].nNumParticle,
//		g_EditEffe[g_nCntEffect].AngleMax,
//		g_EditEffe[g_nCntEffect].AngleMin,
//		g_EditEffe[g_nCntEffect].fSpeed,
//		g_EditEffe[g_nCntEffect].dir,
//		g_EditEffe[g_nCntEffect].decfAlv,
//		g_EditEffe[g_nCntEffect].EffectTex,
//		false);
//}
////============================
////弾の終了処理
////============================
//void UninitEffectEdit(void)
//{
//	UninitEffect();
//
//	UninitMeshField();
//
//}
////============================
////弾の更新処理
////============================
//void UpdateEffectEdit(void)
//{
//	CAMERA* pCamera = GetCamera();
//
//	UpdateEffect();
//
//	UpdateCamera();
//
//	// 粒子の数変更
//	if (GetKeyboardPress(DIK_R) && g_EditEffe[g_nCntEffect].nNumParticle < 30)
//	{
//		g_EditEffe[g_nCntEffect].nNumParticle++;
//	}
//	else if (GetKeyboardPress(DIK_T) && g_EditEffe[g_nCntEffect].nNumParticle > 1)
//	{
//		g_EditEffe[g_nCntEffect].nNumParticle--;
//	}
//
//	// 粒子の数変更
//	if (GetKeyboardPress(DIK_F) && g_EditEffe[g_nCntEffect].fSize < 30.0f)
//	{
//		g_EditEffe[g_nCntEffect].fSize++;
//	}
//	else if (GetKeyboardPress(DIK_G) && g_EditEffe[g_nCntEffect].fSize > 2.0f)
//	{
//		g_EditEffe[g_nCntEffect].fSize--;
//	}
//
//	// rgba変更
//	if (KeyboardTrigger(DIK_V) && g_EditEffe[g_nCntEffect].ColorType < COLORCHANGE_MAX - 1)
//	{
//		g_EditEffe[g_nCntEffect].ColorType++;
//	}
//	else if (KeyboardTrigger(DIK_B) && g_EditEffe[g_nCntEffect].ColorType > 0)
//	{
//		g_EditEffe[g_nCntEffect].ColorType--;
//	}
//
//	// 色変更加算
//	if (GetKeyboardPress(DIK_UP))
//	{
//		switch (g_EditEffe[g_nCntEffect].ColorType)
//		{
//		case COLORCHANGE_R:
//			g_EditEffe[g_nCntEffect].col.r += 0.01f;
//			break;
//		case COLORCHANGE_G:
//			g_EditEffe[g_nCntEffect].col.g += 0.01f;
//			break;
//		case COLORCHANGE_B:
//			g_EditEffe[g_nCntEffect].col.b += 0.01f;
//			break;
//		case COLORCHANGE_A:
//			g_EditEffe[g_nCntEffect].col.a += 0.01f;
//			break;
//		default:
//			break;
//		}
//	}
//
//	// 色変更減算
//	else if (GetKeyboardPress(DIK_DOWN))
//	{
//		switch (g_EditEffe[g_nCntEffect].ColorType)
//		{
//		case COLORCHANGE_R:
//			g_EditEffe[g_nCntEffect].col.r -= 0.01f;
//			break;
//		case COLORCHANGE_G:
//			g_EditEffe[g_nCntEffect].col.g -= 0.01f;
//			break;
//		case COLORCHANGE_B:
//			g_EditEffe[g_nCntEffect].col.b -= 0.01f;
//			break;
//		case COLORCHANGE_A:
//			g_EditEffe[g_nCntEffect].col.a -= 0.01f;
//			break;
//		default:
//			break;
//		}
//	}
//
//	// 色の制限
//	if (g_EditEffe[g_nCntEffect].col.r > 1.0f)
//	{
//		g_EditEffe[g_nCntEffect].col.r = 1.0f;
//	}
//	if (g_EditEffe[g_nCntEffect].col.g > 1.0f)
//	{
//		g_EditEffe[g_nCntEffect].col.g = 1.0f;
//	}
//	if (g_EditEffe[g_nCntEffect].col.b > 1.0f)
//	{
//		g_EditEffe[g_nCntEffect].col.b = 1.0f;
//	}
//	if (g_EditEffe[g_nCntEffect].col.a > 1.0f)
//	{
//		g_EditEffe[g_nCntEffect].col.a = 1.0f;
//	}
//	// 色の制限
//	if (g_EditEffe[g_nCntEffect].col.r < 0.1f)
//	{
//		g_EditEffe[g_nCntEffect].col.r = 0.1f;
//	}
//	if (g_EditEffe[g_nCntEffect].col.g < 0.1f)
//	{
//		g_EditEffe[g_nCntEffect].col.g = 0.1f;
//	}
//	if (g_EditEffe[g_nCntEffect].col.b < 0.1f)
//	{
//		g_EditEffe[g_nCntEffect].col.b = 0.1f;
//	}
//	if (g_EditEffe[g_nCntEffect].col.a < 0.1f)
//	{
//		g_EditEffe[g_nCntEffect].col.a = 0.1f;
//	}
//
//	// アングル変更
//	if (GetKeyboardPress(DIK_Y) && g_EditEffe[g_nCntEffect].AngleMax > 1)
//	{
//		g_EditEffe[g_nCntEffect].AngleMax --;
//	}
//	else if(GetKeyboardPress(DIK_U) && g_EditEffe[g_nCntEffect].AngleMax < 628)
//	{
//		g_EditEffe[g_nCntEffect].AngleMax++;
//	}
//
//	// アングル変更
//	if (GetKeyboardPress(DIK_I) && g_EditEffe[g_nCntEffect].AngleMin > -314)
//	{
//		g_EditEffe[g_nCntEffect].AngleMin--;
//	}
//	else if (GetKeyboardPress(DIK_O) && g_EditEffe[g_nCntEffect].AngleMin < 0)
//	{
//		g_EditEffe[g_nCntEffect].AngleMin++;
//	}
//
//	// 移動量の変更
//	if (GetKeyboardPress(DIK_H) && g_EditEffe[g_nCntEffect].fSpeed > -100.0f)
//	{
//		g_EditEffe[g_nCntEffect].fSpeed -= 0.1f;
//	}
//	else if (GetKeyboardPress(DIK_J) && g_EditEffe[g_nCntEffect].fSpeed < 100.0f)
//	{
//		g_EditEffe[g_nCntEffect].fSpeed += 0.1f;
//	}
//
//	// 寿命の変更
//	if (GetKeyboardPress(DIK_K) && g_EditEffe[g_nCntEffect].nLife < 400)
//	{
//		g_EditEffe[g_nCntEffect].nLife ++;
//	}
//	else if (GetKeyboardPress(DIK_L) && g_EditEffe[g_nCntEffect].nLife > 10)
//	{
//		g_EditEffe[g_nCntEffect].nLife--;
//	}
//
//	// 重力の変更
//	// 加算
//	if (GetKeyboardPress(DIK_1))
//	{
//		g_EditEffe[g_nCntEffect].dir.x += 0.1f;
//	}
//	else if (GetKeyboardPress(DIK_2))
//	{
//		g_EditEffe[g_nCntEffect].dir.y += 0.1f;
//	}
//	else if (GetKeyboardPress(DIK_3))
//	{
//		g_EditEffe[g_nCntEffect].dir.z += 0.1f;
//	}
//	// 減算
//	else if (GetKeyboardPress(DIK_4))
//	{
//		g_EditEffe[g_nCntEffect].dir.x -= 0.1f;
//	}
//	else if (GetKeyboardPress(DIK_5))
//	{
//		g_EditEffe[g_nCntEffect].dir.y -= 0.1f;
//	}
//	else if (GetKeyboardPress(DIK_6))
//	{
//		g_EditEffe[g_nCntEffect].dir.z -= 0.1f;
//	}
//
//	if (GetKeyboardPress(DIK_N) && g_EditEffe[g_nCntEffect].decfAlv < 0.1f)
//	{
//		g_EditEffe[g_nCntEffect].decfAlv += 0.001f;
//	}
//	else if (GetKeyboardPress(DIK_M) && g_EditEffe[g_nCntEffect].decfAlv > 0.0001f)
//	{
//		g_EditEffe[g_nCntEffect].decfAlv -= 0.0001f;
//	}
//
//	// 移動処理
//	if (GetKeyboardPress(DIK_W))
//	{
//		g_EditEffe[g_nCntEffect].pos.x += sinf(pCamera->rot.y) * 10.0f;
//		g_EditEffe[g_nCntEffect].pos.z += cosf(pCamera->rot.y) * 10.0f;
//	}
//	else if (GetKeyboardPress(DIK_S))
//	{
//		g_EditEffe[g_nCntEffect].pos.x -= sinf(pCamera->rot.y) * 10.0f;
//		g_EditEffe[g_nCntEffect].pos.z -= cosf(pCamera->rot.y) * 10.0f;
//	}
//	else if (GetKeyboardPress(DIK_A))
//	{
//		g_EditEffe[g_nCntEffect].pos.x -= cosf(pCamera->rot.y) * 10.0f;
//		g_EditEffe[g_nCntEffect].pos.z += sinf(pCamera->rot.y) * 10.0f;
//	}
//	else if (GetKeyboardPress(DIK_D))
//	{
//		g_EditEffe[g_nCntEffect].pos.x += cosf(pCamera->rot.y) * 10.0f;
//		g_EditEffe[g_nCntEffect].pos.z -= sinf(pCamera->rot.y) * 10.0f;
//	}
//	else if (GetKeyboardPress(DIK_Q))
//	{
//		g_EditEffe[g_nCntEffect].pos.y += 1.0f;
//	}
//	else if (GetKeyboardPress(DIK_E))
//	{
//		g_EditEffe[g_nCntEffect].pos.y -= 1.0f;
//	}
//
//	if (KeyboardTrigger(DIK_F2) && g_EditEffe[g_nCntEffect].EffectTex < EFFECT_MAX - 1)
//	{
//		g_EditEffe[g_nCntEffect].EffectTex++;
//	}
//	else if (KeyboardTrigger(DIK_F3) && g_EditEffe[g_nCntEffect].EffectTex > 0)
//	{
//		g_EditEffe[g_nCntEffect].EffectTex--;
//	}
//
//	// 設置
//	if (KeyboardTrigger(DIK_RETURN))
//	{
//		g_EditEffe[g_nCntEffect + 1].pos = g_EditEffe[g_nCntEffect].pos;
//		g_EditEffe[g_nCntEffect + 1].bUse = true;
//
//		SetParticle(D3DXVECTOR3(g_EditEffe[g_nCntEffect].pos.x, g_EditEffe[g_nCntEffect].pos.y, g_EditEffe[g_nCntEffect].pos.z),
//			D3DXVECTOR3(g_EditEffe[g_nCntEffect].move.x, g_EditEffe[g_nCntEffect].move.y, g_EditEffe[g_nCntEffect].move.z),
//			g_EditEffe[g_nCntEffect].nLife,
//			g_EditEffe[g_nCntEffect].col,
//			0,
//			g_EditEffe[g_nCntEffect].fSize,
//			g_EditEffe[g_nCntEffect].nNumParticle,
//			g_EditEffe[g_nCntEffect].AngleMax,
//			g_EditEffe[g_nCntEffect].AngleMin,
//			g_EditEffe[g_nCntEffect].fSpeed,
//			g_EditEffe[g_nCntEffect].dir,
//			g_EditEffe[g_nCntEffect].decfAlv,
//			g_EditEffe[g_nCntEffect].EffectTex,
//			false);
//
//		g_nCntEffect++;
//	}
//
//	if (KeyboardTrigger(DIK_F7))
//	{
//		SaveEffect();
//	}
//	else if (KeyboardTrigger(DIK_F8))
//	{
//		LoadEffect();
//	}
//
//	SetParticle2(g_EditEffe[g_nCntEffect]);
//
//	UpdateParticle();
//
//}
////============================
////弾の描画処理
////============================
//void DrawEffectEdit(void)
//{
//	DrawMeshField();
//
//	DrawEffect();
//}
////============================
////弾の描画処理
////============================
//void SetEffectEdit(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fSize)
//{
//	for (int nCnt = 0; nCnt < MAX_EFFECTEDIT; nCnt++)
//	{
//		if (g_EditEffe[nCnt].bUse == false)
//		{
//			g_EditEffe[nCnt].pos = pos;
//			g_EditEffe[nCnt].fSize = fSize;
//			g_EditEffe[nCnt].move = move;
//			g_EditEffe[nCnt].col = col;
//			g_EditEffe[nCnt].nLife = nLife;
//			g_EditEffe[nCnt].bUse = true;
//
//			break;
//		}
//	}
//}
////============================
//// パーティクルのセーブ処理
////============================
//void SaveEffect(void)
//{
//	FILE* pFile; // ファイルのポインタ宣言
//
//	pFile = fopen("data\\saveparticle.txt", "w");
//
//	if (pFile != NULL)
//	{
//		fwrite(&g_nCntEffect, sizeof(int), 0, pFile);
//
//		fprintf(pFile, "+============================================+\n");
//		fprintf(pFile, "+   パーティクルエディター 作成者 吉田祐人   +\n");
//		fprintf(pFile, "+============================================+\n");
//
//		SetParticle(D3DXVECTOR3(g_EditEffe[g_nCntEffect].pos.x, g_EditEffe[g_nCntEffect].pos.y, g_EditEffe[g_nCntEffect].pos.z),
//			D3DXVECTOR3(g_EditEffe[g_nCntEffect].move.x, g_EditEffe[g_nCntEffect].move.y, g_EditEffe[g_nCntEffect].move.z),
//			g_EditEffe[g_nCntEffect].nLife,
//			g_EditEffe[g_nCntEffect].col,
//			0,
//			g_EditEffe[g_nCntEffect].fSize,
//			g_EditEffe[g_nCntEffect].nNumParticle,
//			g_EditEffe[g_nCntEffect].AngleMax,
//			g_EditEffe[g_nCntEffect].AngleMin,
//			g_EditEffe[g_nCntEffect].fSpeed,
//			g_EditEffe[g_nCntEffect].dir,
//			g_EditEffe[g_nCntEffect].decfAlv,
//			g_EditEffe[g_nCntEffect].EffectTex,
//			false);
//
//		for (int nCnt = 0; nCnt < g_nCntEffect; nCnt++)
//		{
//			if (g_EditEffe[nCnt].bUse)
//			{
//				fprintf(pFile, "PARTICLESET\n");
//
//				fprintf(pFile, "    POS = %3.2f %3.2f %3.2f					# 位置\n",g_EditEffe[nCnt].pos.x, g_EditEffe[nCnt].pos.y, g_EditEffe[nCnt].pos.z);
//
//				fprintf(pFile, "    MOVE = %3.2f %3.2f %3.2f					# 移動量\n", g_EditEffe[nCnt].move.x, g_EditEffe[nCnt].move.y, g_EditEffe[nCnt].move.z);
//
//				fprintf(pFile, "    LIFE = %d								# 寿命\n", g_EditEffe[nCnt].nLife);
//								    
//				fprintf(pFile, "    COLOR = %3.2f %3.2f %3.2f %3.2f				# 色\n", g_EditEffe[nCnt].col.r, g_EditEffe[nCnt].col.g,g_EditEffe[nCnt].col.b,g_EditEffe[nCnt].col.a);
//								    
//				fprintf(pFile, "    TYPE = %d								# パーティクルの種類\n", g_EditEffe[nCnt].nType);
//								    
//				fprintf(pFile, "    SIZE = %3.2f						# パーティクルの大きさ\n", g_EditEffe[nCnt].fSize);
//								    
//				fprintf(pFile, "    NUM_PARTICLE = %d						# パーティクルの粒子の数\n", g_EditEffe[nCnt].nNumParticle);
//								    
//				fprintf(pFile, "    ANGLE_MAX = %d							# パーティクルの角度のMAX\n", g_EditEffe[nCnt].AngleMax);
//								    
//				fprintf(pFile, "    ANGLE_MIN = %d							# パーティクルの角度のMIN\n", g_EditEffe[nCnt].AngleMin);
//								    
//				fprintf(pFile, "    SPEED = %3.2f							# パーティクルの速さ\n", g_EditEffe[nCnt].fSpeed);
//								    
//				fprintf(pFile, "    DIR = %3.2f %3.2f %3.2f					# パーティクルの重力XYZ\n", g_EditEffe[nCnt].dir.x, g_EditEffe[nCnt].dir.y, g_EditEffe[nCnt].dir.z);
//								    
//				fprintf(pFile, "    DECREMENT = %f							# パーティクルのアルファ値の減少の速さ\n", g_EditEffe[nCnt].decfAlv);
//								    
//				fprintf(pFile, "    EFFECT_TEXTPE = %d						# パーティクルのテクスチャの種類\n", g_EditEffe[nCnt].EffectTex);
//								    
//				fprintf(pFile, "    BLIFE = %d								# パーティクルの寿命をつけるかつけないか\n", g_EditEffe[nCnt].bLifedecrease);
//
//				fprintf(pFile, " END_PARTICLESET\n\n");
//			}
//		}
//
//		fprintf(pFile, " END_SCRIPT\n");
//	}
//	else
//	{
//		return;
//	}
//	fclose(pFile);
//
//}
////============================
////エフェクトのロード
////============================
//void LoadEffect(void)
//{
//	FILE* pFile; // ファイルのポインタ宣言
//
//	pFile = fopen("data\\saveparticle.txt", "r");
//
//	char skip[3];
//	D3DXVECTOR3 pos(0.0f,0.0f,0.0f);
//	D3DXVECTOR3 move(0.0f, 0.0f, 0.0f);
//	D3DXVECTOR3 dir(0.0f, 0.0f, 0.0f);
//
//	int nNumParticle = 0;
//	int AngleMax = 0;
//	int AngleMin = 0;
//	int nType = 0;
//	int EffectTex = 0;
//	int nLife = 0;
//
//	D3DXCOLOR col(0.0f, 0.0f, 0.0f, 0.0f);
//
//	float fSize = 0.0f;
//	float fSpeed = 0.0f;
//	float Dec = 0.0f;
//
//	bool EnableLife = false;
//	int bLife = 1;
//
//	if (pFile != NULL)
//	{
//		char aString[1000];
//
//		while (1)
//		{
//			fscanf(pFile, "%s", &aString[0]);
//
//			if (strcmp(aString, "PARTICLESET") == 0)
//			{
//				while (1)
//				{
//					fscanf(pFile, "%s", &aString[0]);
//
//					if (strcmp(aString, "POS") == 0)
//					{
//						fscanf(pFile, "%s", &skip[0]);
//						fscanf(pFile, "%f", &pos.x);
//						fscanf(pFile, "%f", &pos.y);
//						fscanf(pFile, "%f", &pos.z);
//					}
//					else if (strcmp(aString, "MOVE") == 0)
//					{
//						fscanf(pFile, "%s", &skip[0]);
//						fscanf(pFile, "%f", &move.x);
//						fscanf(pFile, "%f", &move.y);
//						fscanf(pFile, "%f", &move.z);
//					}
//					else if (strcmp(aString, "LIFE") == 0)
//					{
//						fscanf(pFile, "%s", &skip[0]);
//						fscanf(pFile, "%d", &nLife);
//					}
//					else if (strcmp(aString, "COLOR") == 0)
//					{
//						fscanf(pFile, "%s", &skip[0]);
//						fscanf(pFile, "%f", &col.r);
//						fscanf(pFile, "%f", &col.g);
//						fscanf(pFile, "%f", &col.b);
//						fscanf(pFile, "%f", &col.a);
//					}
//					else if (strcmp(aString, "TYPE") == 0)
//					{
//						fscanf(pFile, "%s", &skip[0]);
//						fscanf(pFile, "%d", &nType);
//					}
//					else if (strcmp(aString, "SIZE") == 0)
//					{
//						fscanf(pFile, "%s", &skip[0]);
//						fscanf(pFile, "%f", &fSize);
//					}
//					else if (strcmp(aString, "NUM_PARTICLE") == 0)
//					{
//						fscanf(pFile, "%s", &skip[0]);
//						fscanf(pFile, "%d", &nNumParticle);
//					}
//					else if (strcmp(aString, "ANGLE_MAX") == 0)
//					{
//						fscanf(pFile, "%s", &skip[0]);
//						fscanf(pFile, "%d", &AngleMax);
//					}
//					else if (strcmp(aString, "ANGLE_MIN") == 0)
//					{
//						fscanf(pFile, "%s", &skip[0]);
//						fscanf(pFile, "%d", &AngleMax);
//					}
//					else if (strcmp(aString, "SPEED") == 0)
//					{
//						fscanf(pFile, "%s", &skip[0]);
//						fscanf(pFile, "%f", &fSpeed);
//					}
//					else if (strcmp(aString, "DIR") == 0)
//					{
//						fscanf(pFile, "%s", &skip[0]);
//						fscanf(pFile, "%f", &dir.x);
//						fscanf(pFile, "%f", &dir.y);
//						fscanf(pFile, "%f", &dir.z);
//					}
//
//					else if (strcmp(aString, "DECREMENT") == 0)
//					{
//						fscanf(pFile, "%s", &skip[0]);
//						fscanf(pFile, "%f", &Dec);
//					}
//					else if (strcmp(aString, "EFFECT_TEXTPE") == 0)
//					{
//						fscanf(pFile, "%s", &skip[0]);
//						fscanf(pFile, "%d", &EffectTex);
//					}
//					else if (strcmp(aString, "BLIFE") == 0)
//					{
//						fscanf(pFile, "%s", &skip[0]);
//						fscanf(pFile, "%d", &bLife);
//
//						if (bLife == 0)
//						{
//							EnableLife = true;
//						}
//						else if (bLife == 1)
//						{
//							EnableLife = false;
//						}
//					}
//					else if (strcmp(aString, "END_PARTICLESET") == 0)
//					{
//						SetParticle(pos, move, nLife, col, nType, fSize, nNumParticle, AngleMax, AngleMin, fSpeed, dir, Dec, EffectTex, EnableLife);
//						break;
//					}
//				}
//			}
//
//			if (strcmp(aString, "END_SCRIPT") == 0)
//			{
//				break;
//			}
//		}
//	}
//	else
//	{
//		return;
//	}
//	fclose(pFile);
//}
//
////============================
////エフェクト編集の取得
////============================
//SETPARTICLE* GetEditEffect(void)
//{
//	return &g_EditEffe[0];
//}
