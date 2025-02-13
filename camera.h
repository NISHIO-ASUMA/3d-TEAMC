//============================================================================================================
//
// カメラ処理 [camera.h]
// Author: TEAM_C
//
//============================================================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//**************************************************************************************************************
// インクルードファイル宣言
//**************************************************************************************************************
#include "main.h"

//**************************************************************************************************************
// カメラ列挙型を定義
//**************************************************************************************************************
typedef enum
{
	CAMERATYPE_PLAYER = 0,
	CAMERATYPE_CRFAT,
	CAMERATYPE_MAX
}CAMERATYPE;

//**************************************************************************************************************
// カメラ列挙型を定義
//**************************************************************************************************************
typedef enum
{
	CAMERAMODE_NONE = 0,
	CAMERAMODE_PLAYER,
	CAMERAMODE_MAX
}CAMERAMODE;

//**************************************************************************************************************
// カメラ構造体を定義
//**************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 posV;			// 視点
	D3DXVECTOR3 posR;			// 注視点
	D3DXVECTOR3 vecU;			// 上方向ベクトル
	D3DXMATRIX mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX mtxView;			// ビューマトリックス
	D3DXVECTOR3 rot;			// 角度
	float fDistance;			// 視点から注視点までの距離
	D3DXVECTOR3 posVDest;		// 目的の視点
	D3DXVECTOR3 posRDest;		// 目的の注視点
	CAMERAMODE g_CameraMode;
	D3DVIEWPORT9 viewport;		// ビューポート
	float oldDistance;          // 保存用の変数
	float Direction;            // カメラのうごく方向
}Camera;

//**********************
// プロトタイプ宣言
//**********************
void InitCamera(void);		// カメラの初期化
void UninitCamera(void);	// カメラの終了
void UpdateCamera(void);	// カメラの更新
void SetCamera(int nCnt);		// カメラの設定
Camera* GetCamera();		// カメラの取得
void StickCamera(void);		// 右スティックのカメラ処理
void MouseWheel(int zDelta);// マウスホイール

void InitCameraTex();	  // カメラに映すテクスチャの初期化処理
void UninitCameraTex();	  // カメラに映すテクスチャの終了処理
void DrawCameraTex();	  // カメラに映すテクスチャの描画処理
int SetTexCamera(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fWidth, float fHeight); // 設定
void DeleteTex(int nIdx);

#endif