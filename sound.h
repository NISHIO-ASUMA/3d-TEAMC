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
	SOUND_LABEL_SP_SE,			// SP攻撃SE
	SOUND_LABEL_CRAFT,			// 武器作成SE
	SOUND_LABEL_FEVER,			// フィーバー開始SE
	SPUND_LABEL_WEPONBREAK,     // 武器破壊SE
	SOUND_LABEL_EVENTSE,        // イベント発生音
	SOUND_LABEL_AVOIDSE,        // 回避SE
	SOUND_LABEL_TUTOCLEARSE,    // チュートリアルクリア音
	SOUND_LABEL_WEPONFIRESE,    // 炎武器音
	SOUND_LABEL_NOWEPON,        // 武器持ってないとき
	SOUND_LABEL_WATERWEPON,     // 水武器音
	SOUND_LABEL_SPEARWEPON,     // 槍の武器音
	SOUND_LABEL_METALWEPON,     // 鉄の武器音
	SOUND_LABEL_HARISENWEPON,   // ハリセンの音
	SOUND_LABEL_TELPHONWEPON,   // 電柱の音
	SOUND_LABEL_MUSICWEPON,     // 音楽系統の音
	SOUND_LABEL_ICEWEPONSE,     // 氷武器
	SOUND_LABEL_LIFERECOVERY,   // 回復音
	SOUND_LABEL_ITEMTHROW,      // 投げアイテム音
	SOUND_LABEL_FALSEBOSS_SE,   // ボス死亡SE音
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
