//============================
//
// ���[�V����[motion.h]
// Author:YOSHIDA YUUTO
//
//============================

#ifndef _MOTION_H_
#define _MOTION_H_

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include "main.h"
#include"model.h"
#include<stdio.h>

//****************************
//�}�N����`
//****************************
#define FILENAME001 ("data\\ModelPlayer.txt")
#define MAX_PARTS (16)

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
	MOTIONTYPE_JUMPACTION,
	MOTIONTYPE_INVISIBLE,
	MOTIONTYPE_DAMAGE,
	MOTIONTYPE_FALL,
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
}MOTION;

//****************************
//�v���g�^�C�v�錾
//****************************
void UpdateMotion(MOTION* pMotion);
#endif
