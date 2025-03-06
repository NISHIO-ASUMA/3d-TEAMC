//==============================================================================================================
//
// ���[�V����[motion.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _MOTION_H_
#define _MOTION_H_

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "main.h"
#include"model.h"
#include<stdio.h>

//**************************************************************************************************************
//�}�N����`
//**************************************************************************************************************
#define FILENAME001 ("data\\ModelPlayer.txt")
#define MAX_PARTS (16)

#define KEY_FIRST (0)  // 0�Ԗڂ̃L�[
#define KEY_ONE (1)    // 1�Ԗڂ̃L�[
#define KEY_TWO (2)  // 2�Ԗڂ̃L�[
#define KEY_THREE (3)  // 3�Ԗڂ̃L�[
#define KEY_FOUR (4)  // 4�Ԗڂ̃L�[
#define KEY_FIVE (5)  // 5�Ԗڂ̃L�[
#define KEY_SIX (6)  // 6�Ԗڂ̃L�[
#define KEY_SEVEN (7)  // 7�Ԗڂ̃L�[
#define KEY_EIGHT (8)  // 8�Ԗڂ̃L�[
#define KEY_NINE (9)  // 9�Ԗڂ̃L�[
#define KEY_TEN (10) // 10�Ԗڂ̃L�[
#define KEY_EREVEN (11) // 11�Ԗڂ̃L�[
#define KEY_TWELVE (12) // 12�Ԗڂ̃L�[
#define KEY_THIRTEEN (13) // 13�Ԗڂ̃L�[
#define KEY_FOURTEEN (14) // 14�Ԗڂ̃L�[
#define KEY_FIFTEEN (15) // 15�Ԗڂ̃L�[
#define KEY_SIXTEEN (16) // 16�Ԗڂ̃L�[
#define KEY_SEVENTEEN (17) // 17�Ԗڂ̃L�[
#define KEY_EIGHTEEN (18) // 18�Ԗڂ̃L�[
#define KEY_NINETEEN (19) // 19�Ԗڂ̃L�[
#define KEY_TWENTY (20) // 20�Ԗڂ̃L�[

//**************************
//�L�����̎��
//**************************
typedef enum
{
	LOADTYPE_PLAYER = 0,
	LOADTYPE_ENEMY,
	LOADTYPE_MAX
}LOADTYPE;

//**************************
//���f���̃��[�V����
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
//�L�[�\����
//**************************
typedef struct
{
	float fPosX;//�ʒuX
	float fPosY;//�ʒuY
	float fPosZ;//�ʒuZ

	float fRotX;//����X
	float fRotY;//����Y
	float fRotZ;//����Z

}KEY;

//**************************
//�L�[���\����
//**************************
typedef struct
{
	int nFrame;//�Đ��t���[��
	KEY aKey[MAX_PARTS];//�e�L�[�p�[�c�̃L�[�v�f
}KEY_INFO;


//**************************
//���[�V�������\����
//**************************
typedef struct
{
	bool bLoop;//���[�v���邩�ǂ���
	int nNumkey;//�L�[�̑���
	KEY_INFO aKeyInfo[32];//�L�[���
	int nStartKey, nEndKey, nStartFrame,nEndFrame;//������n�߂�L�[�̕ϐ�
}MOTION_INFO;

//**************************
//���[�V�����\����
//**************************
typedef struct
{
	MODEL aModel[MAX_PARTS];					// ���f��(�p�[�c)
	int nNumModel;								// ���f��(�p�[�c)�̑���(���ۂɎg�����f���̐�)
	MOTION_INFO aMotionInfo[MOTIONTYPE_MAX];	// ���[�V�������
	int nNumMotion;								// ���[�V�����̑���
	MOTIONTYPE motionType;						// ���[�V�����̎��
	bool bLoopMotion;							// ���[�v���邩�ǂ���
	int nNumKey;								// �L�[�̑���
	int nKey;									// ���݂̃L�[No.
	int nCountMotion;							// ���[�V�����̃J�E���^�[
	int nextKey;                                // ���̃L�[

	MOTIONTYPE motiontypeBlend;					// �u�����h�̎��
	bool bBlendMotion;							// �u�����h�����邩�ǂ���
	bool bLoopMotionBlend;						// ���[�v���邩�ǂ���
	bool bFinishMotion;							// ���[�V�������I��������ǂ���
	bool bFirstMotion;                          // ���[�V�������n�܂����t���O
	int nNumKeyBlend;							// �u�����h���[�V�����̍ő�̃L�[
	int nKeyBlend;								// �u�����h���[�V�����̌��݂̃L�[
	int nNextKeyBlend;							// �u�����h���[�V�����̎��̃L�[
	int nCounterMotionBlend;					// �u�����h�̃J�E���^�[

	int nFrameBlend;							// �u�����h�̃t���[����
	int nCounterBlend;							// �u�����h�J�E���^�[

}MOTION;

//**************************************************************************************************************
//�v���g�^�C�v�錾
//**************************************************************************************************************
void UpdateMotion(MOTION* pMotion);
void SetMotion(MOTION* pMotion,MOTIONTYPE motiontypeBlend, bool Blend, int nFrameBlend);
void NormalizeRotation(float *pRotX, float* pRotY, float* pRotZ);
bool CheckActionMotion(MOTION* pMotion);																	 // ���[�V�������A�N�V����������Ȃ���
#endif
