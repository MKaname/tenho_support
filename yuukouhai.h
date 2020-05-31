
#pragma once
#ifndef __YUUKOUHAIKEISAN
#define __YUUKOUHAIKEISAN

#include <stdio.h>
#include <string.h>
#include <vector>
using namespace std;

//�L���v�����߂�N���X
class Yuukouhai{

private:
	int NormalSyanten();//�ʏ��V�����e��
	int KokusiSyanten();//���m�V�����e��
	int TiitoituSyanten();//�`�[�g�C�V�����e��
	void mentu_cut(int);//�����c�����֐�
	void taatu_cut(int);//�^�[�c�����֐�

	int mentu;		//�����c��
	int toitu;		//�g�C�c��
	int kouho;		//�^�[�c��
	int fuurosuu;		//�t�[����
	int temp;		//�V�����e�����i�v�Z�p�j
	int syanten_normal;	//�V�����e�����i���ʗp�j

	void NotKoritu();
	int tehai[38];
	int not_koritu[38];//�Ǘ��v�łȂ��v�i�[�p

	bool error_check();

public:

	bool NormalYuukou (vector<int> NormalYuukou[]);//�ʏ��L���v
	bool TiitoiYuukou (vector<int> TiitoiYuukou[]);//���m�L���v
	bool KokusiYuukou (vector<int> KokusiYuukou[]);//�`�[�g�C�L���v
	void set_tehai(int t[]);//��v�Z�b�g
};


#endif