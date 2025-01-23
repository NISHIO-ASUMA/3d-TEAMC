//============================
//
// 壁[wall.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
//インクルードファイル
//****************************
#include "wall.h"
#include "player.h"
#include "input.h"
#include "edit.h"

//****************************
//マクロ定義
//****************************
#define SIZE_VALUE (0.1f) //壁の拡大率

//****************************
//グローバル変数宣言
//****************************
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL; //頂点バッファへのポインタ
//D3DXVECTOR3 g_posWall; //位置
//D3DXVECTOR3 g_rotWall; //向き
//D3DXMATRIX g_mtxWorldWall;//ワールドマトリックス
WALL g_Wall[MAX_WALL];//壁の構造体
int nNumWall;

//=============================
//壁の初期化処理
//=============================
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\wall000.png",
		&g_pTextureWall);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	VERTEX_3D* pVtx = NULL;

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		g_Wall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Wall[nCntWall].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Wall[nCntWall].vtxPos[nCntWall] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Wall[nCntWall].Size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Wall[nCntWall].fA = 1.0f;
		g_Wall[nCntWall].bUse = false;
		g_Wall[nCntWall].fScal = D3DXVECTOR3(0.0f,0.0f,0.0f);//基準を決める
	}
	nNumWall = 0;

	//頂点バッファをロック
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = -WALL_WIDTH;
		pVtx[0].pos.y = WALL_HEIGHT;
		pVtx[0].pos.z = 0.0f;
				   
		pVtx[1].pos.x = WALL_WIDTH;
		pVtx[1].pos.y = WALL_HEIGHT;
		pVtx[1].pos.z = 0.0f;
				   
		pVtx[2].pos.x = -WALL_WIDTH;
		pVtx[2].pos.y = -WALL_HEIGHT;
		pVtx[2].pos.z = 0.0f;
				   
		pVtx[3].pos.x = WALL_WIDTH;
		pVtx[3].pos.y = -WALL_HEIGHT;
		pVtx[3].pos.z = 0.0f;

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

	g_pVtxBuffWall->Unlock();
}
//=============================
//壁の終了処理
//=============================
void UninitWall(void)
{
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	//頂点バッファの解放
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}
//=============================
//壁の更新処理
//=============================
void UpdateWall(void)
{

}
//=============================
//壁の描画処理
//=============================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans,mtxScal;

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_Wall[nCntWall].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Wall[nCntWall].mtxWorld);

			//大きさを反映
			D3DXMatrixScaling(&mtxScal, g_Wall[nCntWall].Size.x, g_Wall[nCntWall].Size.y, g_Wall[nCntWall].Size.z);
			D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxScal);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall[nCntWall].rot.y, g_Wall[nCntWall].rot.x, g_Wall[nCntWall].rot.z);
			D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Wall[nCntWall].pos.x, g_Wall[nCntWall].pos.y, g_Wall[nCntWall].pos.z);
			D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxTrans);
			
			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Wall[nCntWall].mtxWorld);

			//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			//テクスチャフォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWall);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * 4, 2);
		}
	}
}
//=============================
//壁の設定処理
//=============================
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot,float fA, D3DXVECTOR3 Size)
{
	VERTEX_3D* pVtx;

	int nCntWall;

	//頂点バッファをロック
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_Wall[nCntWall].bUse == false)
		{
			g_Wall[nCntWall].pos = pos;
			g_Wall[nCntWall].Size = Size;
			g_Wall[nCntWall].fA = fA;
			g_Wall[nCntWall].rot = rot;

			g_Wall[nCntWall].bUse = true;

			//各頂点の法線の設定(ベクトルの大きさは１にする必要がある)
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			nNumWall++;
			break;
		}
		pVtx += 4;
	}

	g_pVtxBuffWall->Unlock();
}
//==========================
//壁との衝突判定
//==========================
void CollisionWall(void)
{
	VERTEX_3D* pVtx;
	Player* pPlayer = GetPlayer();

	//頂点バッファをロック
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	int nCntPos = 0;
	int nNextWall = 1;

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		if (g_Wall[nCnt].bUse == true&&pPlayer->bDisp==true)
		{
			g_Wall[nCnt].vtxPos[0].x = g_Wall[nCnt].pos.x - cosf(g_Wall[nCnt].rot.y) * WALL_WIDTH;
			g_Wall[nCnt].vtxPos[0].y = g_Wall[nCnt].pos.y + WALL_HEIGHT;
			g_Wall[nCnt].vtxPos[0].z = g_Wall[nCnt].pos.z + sinf(g_Wall[nCnt].rot.y) * WALL_WIDTH;

			g_Wall[nCnt].vtxPos[1].x = g_Wall[nCnt].pos.x + cosf(g_Wall[nCnt].rot.y) * WALL_WIDTH;
			g_Wall[nCnt].vtxPos[1].y = g_Wall[nCnt].pos.y + WALL_HEIGHT;
			g_Wall[nCnt].vtxPos[1].z = g_Wall[nCnt].pos.z - sinf(g_Wall[nCnt].rot.y) * WALL_WIDTH;

			g_Wall[nCnt].vtxPos[2].x = g_Wall[nCnt].pos.x - sinf(g_Wall[nCnt].rot.y) * WALL_WIDTH;
			g_Wall[nCnt].vtxPos[2].y = g_Wall[nCnt].pos.y - WALL_HEIGHT;
			g_Wall[nCnt].vtxPos[2].z = g_Wall[nCnt].pos.z - cosf(g_Wall[nCnt].rot.y) * WALL_WIDTH;

			D3DXVECTOR3 VecALine,VecALine2;//頂点0～頂点1のベクトル
			D3DXVECTOR3 vecWall;

			VecALine = g_Wall[nCnt].vtxPos[1] - g_Wall[nCnt].vtxPos[0];//0-1...
			VecALine2 = g_Wall[nCnt].vtxPos[2] - g_Wall[nCnt].vtxPos[0];

			D3DXVec3Normalize(&VecALine, &VecALine);//正規化
			D3DXVec3Normalize(&VecALine2, &VecALine2);//正規化

			D3DXVECTOR3 PlayerVec;//プレイヤーのベクトル

			D3DXVECTOR3 VecBLine;//壁からプレイヤー

			VecBLine = pPlayer->pos - g_Wall[nCnt].vtxPos[0];//壁からプレイヤーにベクトルを引く

			D3DXVec3Normalize(&VecBLine, &VecBLine);//正規化

			D3DXVECTOR3 Cross = {};//外積

			D3DXVec3Cross(&Cross, &VecALine, &VecBLine);//外積を計算
			D3DXVec3Cross(&vecWall, &VecALine, &VecALine2);

			float Dot = D3DXVec3Dot(&vecWall, &VecBLine);

			//プレイヤーが壁の外に出た
			if (Cross.y < 0)
			{
				pPlayer->move.x = Dot;
				pPlayer->move.z = Dot;

				//プレイヤーの位置を前回の位置に戻す
				pPlayer->pos.x += vecWall.x;
				pPlayer->pos.z += vecWall.z;

				DotWall();//内積
			}
		}
	}
	//頂点バッファをアンロック
	g_pVtxBuffWall->Unlock();
}
//==========================
//壁の内積
//==========================
void DotWall(void)
{
	Player* pPlayer = GetPlayer();

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		D3DXVECTOR3 vecA;
		D3DXVECTOR3 vecLine;
		D3DXVECTOR3 vecLineB;
		D3DXVECTOR3 vecB; // 法線
		D3DXVECTOR3 vecMove;
		D3DXVECTOR3 VecC;
		D3DXVECTOR3 vecWall;
		D3DXVECTOR3 vecCross;

		vecA = pPlayer->pos - pPlayer->posOld;//プレーヤーの移動

		D3DXVec3Normalize(&vecA, &vecA);//正規化

		vecLine = g_Wall[nCnt].vtxPos[1] - g_Wall[nCnt].vtxPos[0]; // ベクトルA
		vecLineB = g_Wall[nCnt].vtxPos[2] - g_Wall[nCnt].vtxPos[0]; // ベクトルB

		D3DXVec3Cross(&vecB, &vecLine, &vecLineB);//外積
		D3DXVec3Normalize(&vecB, &vecB);//ベクトルB

		float fDot = D3DXVec3Dot(&vecA,&vecB);

		vecWall = fDot * vecB;

		D3DXVec3Normalize(&vecWall, &vecWall);

		pPlayer->move.x = vecWall.x;
		pPlayer->move.z = vecWall.z;
	}
}
////========================
////弾の判定
////========================
//void CollisionBullet(BULLET* pBullet)
//{
//	VERTEX_3D* pVtx;
//
//	PLAYER* pPLayer = GetPlayer();
//
//	//頂点バッファをロック
//	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
//	{
//		if (pBullet->bUse == true)
//		{
//			D3DXVECTOR3 VecALine;
//
//			D3DXVECTOR3 pos, pos1;
//
//			VecALine = g_Wall[nCntWall].vtxPos[1] - g_Wall[nCntWall].vtxPos[0];//0-1...
//
//			D3DXVec3Normalize(&VecALine, &VecALine);
//
//			D3DXVECTOR3 PlayerVec;
//
//			D3DXVECTOR3 VecBLine;//壁からプレイヤー
//
//			VecBLine = pBullet->pos - g_Wall[nCntWall].vtxPos[0];
//
//			D3DXVec3Normalize(&VecBLine, &VecBLine);
//
//			D3DXVECTOR3 Cross = {};
//
//			D3DXVec3Cross(&Cross, &VecALine, &VecBLine);
//
//			if (Cross.y < 0)
//			{
//				D3DXVECTOR3 BulletVec,vecC,vecMove;
//				D3DXVECTOR3 WallLineB;
//				D3DXVECTOR3 BulletCross;
//
//				BulletVec = pBullet->pos - pBullet->posOld;
//
//				WallLineB = g_Wall[nCntWall].vtxPos[2] - g_Wall[nCntWall].vtxPos[0];
//
//				D3DXVec3Cross(&BulletCross, &VecALine, &WallLineB);
//
//				D3DXVec3Normalize(&BulletCross, &BulletCross);
//
//				float fDot = D3DXVec3Dot(&BulletCross, &BulletVec);
//
//				vecC = BulletCross * fDot;
//
//				D3DXVec3Normalize(&vecC, &vecC);
//
//				vecMove = BulletVec + (vecC * 2.0f * fDot);
//
//				D3DXVec3Normalize(&vecMove, &vecMove);
//
//				pBullet->move.x = vecMove.x * 20.0f;
//				pBullet->move.z = vecMove.z * 20.0f;
//			}
//		}
//	}
//	
//	g_pVtxBuffWall->Unlock();
//}
//==========================
//壁の取得
//==========================
WALL* GetWall(void)
{
	return &g_Wall[0];
}
//==========================
//壁の数の取得
//=========================
int NumWall(void)
{
	return nNumWall;
}
