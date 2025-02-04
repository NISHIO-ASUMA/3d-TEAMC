//============================
//
//�v���C���[
//Author:YOSHIDA YUUTO
//
//============================

#ifndef _PLAYER_H_
#define _PLAYER_H_

//**************************
//�C���N���[�h�t�@�C��
//**************************
#include"main.h"
#include"model.h"
#include"motion.h"
#include "enemy.h"
#include"item.h"

//**************************
//�}�N����`
//**************************
#define PLAYER_MOVE (1.0f)
#define WIDTH (100.0f) //�ǂ̉���
#define HEIGHT (100.0f) //�ǂ̍���
#define MAX_MODEL (16)
#define MAX_ITEM (256)

//****************************
//���C�t�̎��
//****************************
typedef enum
{
	LIFE_ONE = 0,
	LIFE_TWO,
	LIFE_THREE,
	LIFE_MAX
}LIFE;

//*******************************
// ���C�t�̃e�N�X�`���ݒ�
//*******************************
static const char* LIFE_TYPE[LIFE_MAX] =
{
	"data\\TEXTURE\\Life001.png",
	"data\\TEXTURE\\Life000.png",
	"data\\TEXTURE\\playerLife0000.png",
};

//***************************
//���C�t�̍\����
//***************************
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	bool bUse;
	D3DXCOLOR col;
	int nLife;
	int nType;
	int nDamage;
}LIFEBAR;

//**************************
//�v���C���[�̎��
//**************************
typedef enum
{
	PLAYERHOLD_NO = 0,
	PLAYERHOLD_HOLD,
	PLAYERHOLD_MAX
}PLAYERHOLDTATE;

//**************************
//�v���C���[�̎��
//**************************
typedef enum
{
	PLAYERTYPE_WEPON = 0,
	PLAYERTYPE_NOHAND,
	PLAYERTYPE_MAX
}PLAYERTYPE;

//**************************
//�v���C���[�̃R���{���
//**************************
typedef enum
{
	COMBO_NO = 0,
	COMBO_ATTACK1,
	COMBO_ATTACK2,
	COMBO_ATTACK3,
	COMBO_ATTACK4,
	COMBO_ATTACK5,
	COMBO_MAX
}COMBOSTATE;

//**************************
//�v���C���[�̏��
//**************************
typedef enum
{
	PLAYERSTATE_NORMAL = 0,
	PLAYERSTATE_MOVE,
	PLAYERSTATE_ATTACK,
	PLAYERSTATE_JUMP,
	PLAYERSTATE_LANDING,
	PLAYERSTATE_DAMAGE,
	PLAYERSTATE_FALL,
	PLAYERSTATE_INVISIBLE,
	PLAYERSTATE_NO_WEPON,
	PLAYERSTATE_MAX
}PLAYERSTATE;

//*******************************
//�v���C���[�̕��탂�[�V�����̐�
//*******************************
typedef enum
{
	MOTION_KATANA = 0,
	MOTION_BIGWEPON,
	MOTION_DBHAND,
	MOTION_ONE_HAND,
	MOTION_PIERCING,
	MOTION_SP,
	MOTION_SPHAMMER,
	MOTION_ONEHANDBLOW,
	MOTION_SPPIERCING,
	MOTION_MAX
}WEPONMOTION; // �g���܂킹����͎̂g���܂킷

//**************************
//�v���C���[�\����
//**************************
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 posDest;
	D3DXVECTOR3 posOld;//�O��̈ʒu
	D3DXVECTOR3 Size;//�傫��
	D3DXVECTOR3 rot;//����
	D3DXVECTOR3 rotDestPlayer;//�ړI�̊p�x
	D3DXVECTOR3 move;//�ړ���
	D3DXVECTOR3	vtxMinPlayer;
	D3DXVECTOR3	vtxMaxPlayer;
	D3DXMATRIX mtxWorldPlayer;//���[���h�}�g���b�N�X
	int nIdxShadow;
	bool bJump;
	bool bDisp;
	float PlayerMove;
	MOTION Motion;
	bool bMove;
	PLAYERSTATE state;
	int nCounterState;
	int nLife;
	int nMaxLife;

	D3DXVECTOR3 SwordOffpos;
	D3DXVECTOR3 Swordpos, Swordrot;
	D3DXMATRIX SwordMtx;//����ɂ��郏�[���h�}�g���b�N�X�̃I�t�Z�b�g
	PLAYERHOLDTATE HandState;
	int nCounterAction;
	int PlayerType;
	bool bImpactCollision;
	bool bJumpAttack;
	float fShadowSize;//�e�̑傫��
	int Combostate; // �R���{�̏�Ԃ����ϐ� 
	int ItemIdx;    // �A�C�e���̃C���f�b�N�X�ۊǗp�ϐ�
	float speed;    // ���̑��� 
	int nDamage;    // �v���C���[�̍U���� 
	int nOldItem;
	int WeponMotion; // ���킲�Ƃ̃��[�V����
	int nStockDamage;
	float fStockSpeed;
	bool Itembreak[MAX_ITEM];
	bool FeverMode;
	bool SpMode;
	bool AttackSp;  // sp�U�������Ă��邩
}Player;

//**************************
//�v���g�^�C�v�錾
//**************************
void InitPlayer(void);//�v���C���[�̏���������
void UninitPlayer(void);//�v���C���[�̏I������
void UpdatePlayer(void);//�v���C���[�̍X�V����
void DrawPlayer(void);//�v���C���[�̕`�揈��
Player* GetPlayer(void);//�v���C���[�̎擾����
void SetMtxPos(void);//���[���h�}�g���b�N�X�̃I�t�Z�b�g�ݒ菈��
void HitPlayer(int nDamage);//�v���C���[�̃q�b�g����
bool CollisionItem(int nIdx, float Itemrange, float plrange);
void HitSowrd(ENEMY* pEnemy,int nCntEnemy);
void ThrowItem(void); // �A�C�e���𓊂���
void CollisionPlayer(D3DXVECTOR3* pPos, D3DXVECTOR3* pMove, float PLradius, float ENradius); // �v���C���[�ƓG�̓����蔻��
bool sphererange(D3DXVECTOR3* pPos1,D3DXVECTOR3 *pPos2, float radius1, float radius2); // �~�̓����蔻��
#endif
