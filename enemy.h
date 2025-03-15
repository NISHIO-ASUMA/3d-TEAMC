//==============================================================================================================
//
// �G[enemy.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

#ifndef _ENEMY_H_
#define _ENEMY_H_

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include"main.h"
#include"motion.h"

//**************************************************************************************************************
//�}�N����`
//**************************************************************************************************************
#define MAX_ENEMY (100)
#define ENEMYFILE001 ("data\\enemy.txt")
#define ENEMYFILE002 ("data\\enemy2.txt")
#define ENEMYFILE003 ("data\\enemy3.txt")
#define TERRITTORYPOS_ONE (D3DXVECTOR3(-611.0f,0.0f,-679.0f)) // �e���g���[1
#define TERRITTORYPOS_TWO (D3DXVECTOR3(-873.0f,0.0f,671.0f)) // �e���g���[2
#define TERRITTORYPOS_THREE (D3DXVECTOR3(730.0f,0.0f,1157.0f)) // �e���g���[3
#define TERRITTORYPOS_FOUR (D3DXVECTOR3(783.0f,0.0f,-810.0f)) // �e���g���[4

//**************************************************************************************************************
//�G�̍U�����
//**************************************************************************************************************
typedef enum
{
	ENEMYATTACK_NO = 0,
	ENEMYATTACK_ATTACK,
	ENEMYATTACK_MAX
}ENEMYATTACK;

//**************************************************************************************************************
//�G�̏��
//**************************************************************************************************************
typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_AGENT,
	ENEMYSTATE_ATTACK,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_DEATH,
	ENEMYSTATE_MAX
}ENEMYSTATE;

//**************************************************************************************************************
//�G�̎��
//**************************************************************************************************************
typedef enum
{
	ENEMYTYPE_ONE = 0, 
	ENEMYTYPE_TWO,
	ENEMYTYPE_THREE,
	ENEMYTYPE_FOUR,
	ENEMYTYPE_FIVE,
	ENEMYTYPE_SIX,
	ENEMYTYPE_SEVEN,
	ENEMYTYPE_MAX
}ENEMYTYPE;

//**************************************************************************************************************
// �G�̃e���g���[�̍\����
//**************************************************************************************************************
typedef struct
{
	int nNumEnemy;		// �G���e���g���[�ɉ��̂��邩
	int nNumber;		// �ǂ̃e���g���[��
	int CylinderIdx;	// �V�����_�[�̃C���f�b�N�X
	D3DXVECTOR3 pos;    // �e���g���[�̈ʒu
	bool bBoss;         // �{�X���o�Ă邩�o�ĂȂ���
	bool bUse;			// �g�p���
}Territory;

//**************************************************************************************************************
//�G�̍\����
//**************************************************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;//�ʒu
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;//�ړ���
	D3DXVECTOR3 offpos, offrot;
	D3DXVECTOR3 rot;//����
	//D3DXVECTOR3 vtxMin,vtxMax;
	//D3DXVECTOR3 Size;
	D3DXMATRIX mtxWorldEnemy;
	float fDistance;
	int nType;
	bool bUse;//�g�p���Ă��邩�ǂ���
	MOTION Motion;
	D3DXVECTOR3 Size;
	D3DXVECTOR3 vtxMin,vtxMax;
	int nLife;
	int nMaxLife;
	int nDamage;
	ENEMYSTATE state;
	int nCounterState;
	int nIdxShadow;
	int nIdxLifeBar,nIdxLifeFrame;
	int nIdxUi;
	float fMove;//�����_��
	int nCountAction;
	int AttackState;
	float Speed; // ���̑���
	D3DXVECTOR3 rotDest; // �ړI�̊p�x
	D3DXVECTOR3 PosDest; // �ړI�̈ʒu
	int nIdxMap;
	int HitStopCount;    // �q�b�g�X�g�b�v
	int TerritoryNumber; // �e���g���[�̔ԍ�
	bool isKillCount;    // �L���J�E���g���ł��邩���f
	int nIdxtarget;		 // �^�[�Q�b�g�̃C���f�b�N�X
}ENEMY;

//**************************************************************************************************************
//�v���g�^�C�v�錾
//**************************************************************************************************************
void InitEnemy(void);//�G�̏���������
void UninitEnemy(void);//�G�̏I������
void UpdateEnemy(void);//�G�̍X�V����
void DrawEnemy(void);//�G�̕`�揈��
ENEMY* GetEnemy(void);//�G�̎擾����
void HitEnemy(int nCnt,int nDamage);//�G�̃q�b�g����
void SetEnemy(D3DXVECTOR3 pos,int nType,int nLife,float Speed, int TerritoryNumber);//�G�̐ݒ菈��
void SpawnEnemy(int nSpawner, int TerritoryIdx);
int GetNumEnemy(void);//�G�̑����擾����
bool CollisionView(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, float fRange, float viewAngle); // ���E�̔���
float SetAngle(D3DXVECTOR3* pRot, D3DXVECTOR3* pPos);
//bool CollisionLine(D3DXVECTOR3* pFirstPos, D3DXVECTOR3* pEndPos,float fRadius); // ���Ƌ��̓����蔻��
void SetSoundWepon(int nType);									  // ���킪�G�ɓ����������̉��̐ݒ菈��
void SetCreateWeponSound(int nType);							  // �������킪�����������̃T�E���h�ݒ菈��
void UpdateEnemySpawn(void);									  // �G�̃X�|�i�[�̏���
int GetTerritoryBossPos(void);                                    // �{�X���ǂ��ɔ���������
void SetEnableBossTerritory(bool bEnable);						  // �{�X������e���g���[�̏���
void SetUpHitStop(int *pHitStopCount);								  // �q�b�g�X�g�b�v�̐ݒ菈��
#endif
