//==============================================================================================================
//
// �C�[�W���O [easing.h]
// Author: YOSHIDA YUTO
//
//==============================================================================================================
#ifndef _EASING_H_
#define _EASING_H_

#include"main.h"

//**************************************************************************************************************
//�v���g�^�C�v�錾
//**************************************************************************************************************
float EaseInQuad(float t);		// �C�[�W���O(����)
float EaseOutQuad(float t);		// �C�[�W���O(����)
float EaseInOutQuad(float t);	// �C�[�W���O(����������)
float EaseInQuad(float t);      // �C�[�W���O(����)
float EaseInCubic(float t);		// �C�[�W���O(�}���ȉ���)
float EaseOutCubic(float t);	// �C�[�W���O(�}���Ȍ���)
float EaseInOutCubic(float t);	// �C�[�W���O(����������)
float EaseInSine(float t);		// �C�[�W���O(�ɂ₩�ȉ���)
float EaseOutSine(float t);		// �C�[�W���O(�ɂ₩�Ȍ���)
float EaseInOutSine(float t);	// �C�[�W���O(�ɂ₩�ȉ���������)
float EaseBounce(float t);      // �C�[�W���O(�o�E���h)
float SetEase(int EaseFrame, int MaxEaseFrame); // �C�[�W���O�̐ݒ菈��
float EaseOutQuart(float t);	 // �C�[�W���O(����)
float EaseOutQuint(float t);	 // �C�[�W���O(����)
float EaseInOutQuart(float t);	 // �C�[�W���O(��苭������)
float EaseInOutQuint(float t);	 // �C�[�W���O(����ɋ�������)
float EaseInBack(float t);		 // �C�[�W���O(�ŏ��ɏ�����������A�i�s��ɑ��������n�߂�)
float EaseOutBack(float t);		 // �C�[�W���O(�ŏ��ɏ�����������A�i�s��ɑ��������n�߂�)
float EaseInOutBack(float t);	 // �C�[�W���O(�ŏ��ɏ�����������A�i�s��ɑ��������n�߂�)
float EaseInElastic(float t);	 // �C�[�W���O(�U�����铮���ŁA�ŏ��͋}���ɐi�݁A�r���ŐU�����J��Ԃ�)
float EaseOutElastic(float t);	 // �C�[�W���O(�U�����铮���ŁA�ŏ��͋}���ɐi�݁A�r���ŐU�����J��Ԃ�)
float EaseInOutElastic(float t); // �C�[�W���O(�U�����铮���ŁA�ŏ��͋}���ɐi�݁A�r���ŐU�����J��Ԃ�)
#endif