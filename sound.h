//=============================================================================
//
// サウンド処理 [sound.h]
// Author : Asuma Nishio
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************
// インクルードファイル宣言
//*****************************
#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_TITLE_BGM = 0,	// タイトルBGM
	SOUND_LABEL_TUTORIAL_BGM,	// チュートリアルBGM
	SOUND_LABEL_GAME_BGM,		// ゲームメインBGM
	SOUND_LABEL_RESULT_BGM,		// リザルトBGM
	SOUND_LABEL_RANKING_BGM,	// ランキングBGM
	SOUND_LABEL_JUMP_SE,		// ジャンプSE
	SOUND_LABEL_SELECT_SE,		// 選択キーSE
	SOUND_LABEL_ENTER_SE,		// 決定音SE
	SOUND_LABEL_ACTION_SE,		// 剣SE
	SOUND_LABEL_BAT_SE,			// 木製バット系統SE
	SOUND_LABEL_HAMMER_SE,		// 大型武器系統SE
	SOUND_LABEL_ITEM_SE,		// アイテム取得SE
	SOUND_LABEL_DAMAGE_SE,		// ダメージSE
	SOUND_LABEL_MAX
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
