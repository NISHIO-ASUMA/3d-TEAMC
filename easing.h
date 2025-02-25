//==============================================================================================================
//
// イージング [easing.h]
// Author: YOSHIDA YUTO
//
//==============================================================================================================
#ifndef _EASING_H_
#define _EASING_H_

#include"main.h"

//**************************************************************************************************************
//プロトタイプ宣言
//**************************************************************************************************************
float EaseInQuad(float t);		// イージング(加速)
float EaseOutQuad(float t);		// イージング(減速)
float EaseInOutQuad(float t);	// イージング(加速→減速)
float EaseInQuad(float t);      // イージング(加速)
float EaseInCubic(float t);		// イージング(急激な加速)
float EaseOutCubic(float t);	// イージング(急激な減速)
float EaseInOutCubic(float t);	// イージング(加速→減速)
float EaseInSine(float t);		// イージング(緩やかな加速)
float EaseOutSine(float t);		// イージング(緩やかな減速)
float EaseInOutSine(float t);	// イージング(緩やかな加速→減速)
float EaseBounce(float t);      // イージング(バウンド)
float SetEase(int EaseFrame, int MaxEaseFrame); // イージングの設定処理
float EaseOutQuart(float t);	 // イージング(減速)
float EaseOutQuint(float t);	 // イージング(減速)
float EaseInOutQuart(float t);	 // イージング(より強い加速)
float EaseInOutQuint(float t);	 // イージング(さらに強い加速)
float EaseInBack(float t);		 // イージング(最初に少し引っ張り、進行後に速く動き始める)
float EaseOutBack(float t);		 // イージング(最初に少し引っ張り、進行後に速く動き始める)
float EaseInOutBack(float t);	 // イージング(最初に少し引っ張り、進行後に速く動き始める)
float EaseInElastic(float t);	 // イージング(振動する動きで、最初は急速に進み、途中で振動を繰り返す)
float EaseOutElastic(float t);	 // イージング(振動する動きで、最初は急速に進み、途中で振動を繰り返す)
float EaseInOutElastic(float t); // イージング(振動する動きで、最初は急速に進み、途中で振動を繰り返す)
#endif