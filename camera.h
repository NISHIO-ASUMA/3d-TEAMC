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
// マクロ定義
//**************************************************************************************************************
#define MAX_KEY (256) // キーの最大数

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
	CAMERAMODE_NORMAL = 0,
	CAMERAMODE_ANIMATION,
	CAMERAMODE_MAX
}CAMERAMODE;

//**************************************************************************************************************
// カメラのアニメーションの種類
//**************************************************************************************************************
typedef enum
{
	CAMERAANIM_ONE = 0,
	CAMERAANIM_SP,
	CAMERAANIM_MAX
}CAMERAANIM;

//**************************************************************************************************************
// カメラのキー構造体
//**************************************************************************************************************
typedef struct
{
	float fPosVX; // 視点X
	float fPosVY; // 視点Y
	float fPosVZ; // 視点Z

	float fPosRX; // 注視点X
	float fPosRY; // 注視点Y
	float fPosRZ; // 注視点Z

	float fRotX; // 向きX
	float fRotY; // 向きY
	float fRotZ; // 向きZ
	int nAnimFrame; // アニメーションのフレーム
	float fDistance;         // 距離
}Anim_Key;

//**************************************************************************************************************
// カメラのアニメーション構造体
//**************************************************************************************************************
typedef struct
{
	Anim_Key Anim_KeyInfo[MAX_KEY];
	int nNumKey; // キーの最大数
	bool bTracking; // 追従するかしないか
	bool bLoopAnimation;     // ループするかどうか
}CameraAnim_Info;

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
	int WaveTIme;               // カメラを揺らす時間

	int AnimType;        // アニメーションの種類
	CameraAnim_Info aAnimInfo[CAMERAANIM_MAX]; // カメラのアニメーションの情報
	int nAnimKey;
	int nNumKey;
	int nCounterAnim;
	bool bEditMode;             // カメラの編集モード
	bool bTracking;             // カメラ追従をするかどうか
	int CameraState;            // カメラの状態
	bool bViewCrossing;         // モデルと交差したか
	int nBlockIdx;              // ブロックのインデックス
	D3DXVECTOR3 rotDest;        // 目標の角度
}Camera;

//**********************
// プロトタイプ宣言
//**********************
void InitCamera(void);		// カメラの初期化
void UninitCamera(void);	// カメラの終了
void UpdateCamera(void);	// カメラの更新
void SetCamera(void);		// カメラの設定
Camera* GetCamera();		// カメラの取得
void StickCamera(void);		// 右スティックのカメラ処理
void MouseWheel(int zDelta);// マウスホイール
void WaveCamera(int WaveTime); // カメラを揺らす処理
void SetAnimation(int nAnimType); // カメラのアニメーション設定
void UpdateEventCamera(void);     // イベントのカメラ
void EventCameraAngle(D3DXVECTOR3 pos); // カメラをどこに向けるか設定
#endif