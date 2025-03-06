//==============================================================================================================
//
// モーション[motion.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _MOTION_H_
#define _MOTION_H_

//**************************************************************************************************************
//インクルードファイル
//**************************************************************************************************************
#include "main.h"
#include"model.h"
#include<stdio.h>

//**************************************************************************************************************
//マクロ定義
//**************************************************************************************************************
#define FILENAME001 ("data\\ModelPlayer.txt")
#define MAX_PARTS (16)

#define KEY_FIRST (0)  // 0番目のキー
#define KEY_ONE (1)    // 1番目のキー
#define KEY_TWO (2)  // 2番目のキー
#define KEY_THREE (3)  // 3番目のキー
#define KEY_FOUR (4)  // 4番目のキー
#define KEY_FIVE (5)  // 5番目のキー
#define KEY_SIX (6)  // 6番目のキー
#define KEY_SEVEN (7)  // 7番目のキー
#define KEY_EIGHT (8)  // 8番目のキー
#define KEY_NINE (9)  // 9番目のキー
#define KEY_TEN (10) // 10番目のキー
#define KEY_EREVEN (11) // 11番目のキー
#define KEY_TWELVE (12) // 12番目のキー
#define KEY_THIRTEEN (13) // 13番目のキー
#define KEY_FOURTEEN (14) // 14番目のキー
#define KEY_FIFTEEN (15) // 15番目のキー
#define KEY_SIXTEEN (16) // 16番目のキー
#define KEY_SEVENTEEN (17) // 17番目のキー
#define KEY_EIGHTEEN (18) // 18番目のキー
#define KEY_NINETEEN (19) // 19番目のキー
#define KEY_TWENTY (20) // 20番目のキー

//**************************
//キャラの種類
//**************************
typedef enum
{
	LOADTYPE_PLAYER = 0,
	LOADTYPE_ENEMY,
	LOADTYPE_MAX
}LOADTYPE;

//**************************
//モデルのモーション
//**************************
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,
	MOTIONTYPE_MOVE,
	MOTIONTYPE_ACTION,
	MOTIONTYPE_JUMP,
	MOTIONTYPE_LANDING,
	MOTIONTYPE_ACTION2,
	MOTIONTYPE_ACTION3,
	MOTIONTYPE_ACTION4,
	MOTIONTYPE_DEATH,
	MOTIONTYPE_MAX
}MOTIONTYPE;

//**************************
//キー構造体
//**************************
typedef struct
{
	float fPosX;//位置X
	float fPosY;//位置Y
	float fPosZ;//位置Z

	float fRotX;//向きX
	float fRotY;//向きY
	float fRotZ;//向きZ

}KEY;

//**************************
//キー情報構造体
//**************************
typedef struct
{
	int nFrame;//再生フレーム
	KEY aKey[MAX_PARTS];//各キーパーツのキー要素
}KEY_INFO;


//**************************
//モーション情報構造体
//**************************
typedef struct
{
	bool bLoop;//ループするかどうか
	int nNumkey;//キーの総数
	KEY_INFO aKeyInfo[32];//キー情報
	int nStartKey, nEndKey, nStartFrame,nEndFrame;//判定を始めるキーの変数
}MOTION_INFO;

//**************************
//モーション構造体
//**************************
typedef struct
{
	MODEL aModel[MAX_PARTS];					// モデル(パーツ)
	int nNumModel;								// モデル(パーツ)の総数(実際に使うモデルの数)
	MOTION_INFO aMotionInfo[MOTIONTYPE_MAX];	// モーション情報
	int nNumMotion;								// モーションの総数
	MOTIONTYPE motionType;						// モーションの種類
	bool bLoopMotion;							// ループするかどうか
	int nNumKey;								// キーの総数
	int nKey;									// 現在のキーNo.
	int nCountMotion;							// モーションのカウンター
	int nextKey;                                // 次のキー

	MOTIONTYPE motiontypeBlend;					// ブレンドの種類
	bool bBlendMotion;							// ブレンドがあるかどうか
	bool bLoopMotionBlend;						// ループするかどうか
	bool bFinishMotion;							// モーションが終わったかどうか
	bool bFirstMotion;                          // モーションが始まったフラグ
	int nNumKeyBlend;							// ブレンドモーションの最大のキー
	int nKeyBlend;								// ブレンドモーションの現在のキー
	int nNextKeyBlend;							// ブレンドモーションの次のキー
	int nCounterMotionBlend;					// ブレンドのカウンター

	int nFrameBlend;							// ブレンドのフレーム数
	int nCounterBlend;							// ブレンドカウンター

}MOTION;

//**************************************************************************************************************
//プロトタイプ宣言
//**************************************************************************************************************
void UpdateMotion(MOTION* pMotion);
void SetMotion(MOTION* pMotion,MOTIONTYPE motiontypeBlend, bool Blend, int nFrameBlend);
void NormalizeRotation(float *pRotX, float* pRotY, float* pRotZ);
bool CheckActionMotion(MOTION* pMotion);																	 // モーションがアクションかじゃないか
#endif
