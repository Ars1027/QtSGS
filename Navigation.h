#ifndef _NAVIGATION_H_
#define _NAVIGATION_H_

#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <time.h>

#define B_NodeNum			63	
#define NodeCount			63	
#define MaxValue			99999999//��ʼ��̾�������ʱ��
#define IteCount			100000	//��������

struct Node
{
	std::string Name;//�������
	int LX, LY, PX, PY;//�߼�λ�ú�ʵ��λ��
	int ConnectionNodeNum[4];//��Χ�Ľ����
	double ConnectionNodeDistance[4];//����Χ���ľ���
	double Crowed;//�ý���ӵ���̶�
	bool RideAllow;//�ý���Ƿ������������г�
	bool ServiceNode;
};

struct Road
{
	int NodeNum;//�����
	int LX, LY;//���λ��
	double Crowed;//�ýڵ��ӵ���̶�
	int WaitCount;//�ȴ�ʱ����������������ʾģ���н��������ڶ�Ӧλ�õĵȴ�ʱ��
};

class DFS
{
public:
	Node MapNodes[NodeCount];//���еĽڵ���Ϣ
	bool CityVisited[NodeCount];//���н��ķ�����Ϣ
	int TravelCityNum[NodeCount * 10];//�߹��Ľ����Ϣ
	int TravelCityCount;//�߹������еĽ������----�洢���·��ʱ����������Ϊ�жϱ�־�����ж��У��֮��ĵ�������
	int CurrentCity;//��ǰ���ڽ����
	double MoveSpeed;//�ƶ��ٶ�--���ڼ���ʱ��------�����ٶ�ΪĬ���ƶ��ٶȳ��Խ�㷽���ϵ�ӵ���̶�
	double MoveDistance;//�ƶ��ľ���
	double MoveTime;//�ƶ����ѵ�ʱ��
	DFS();
	~DFS();
	void Reset();//������Ϣ---���˵�ͼ�����Ϣ��ȫ������
	int SelectNextCityNum(bool);//ѡ����һ��ǰ���Ľ��----��Ѱ�ҵ�һ��·�������յ㣬Ȼ���ټ�������ʱ������ȳ̶�
	void MoveAnt(bool, int);//�ƶ���ǰλ�ã��������ƶ�����ͻ���ʱ��
	void Search(int, int, bool, int);//����һ����㵽�յ��·�����������ݲ��������ж��Ƿ�����ƶ��ٶ�
};

class Navigation
{
public:
	DFS SearchRoad;//����·��
	DFS BestRoad;//�洢���·��
	Navigation();
	~Navigation();
	void Search1(int, int, bool, int);//���·�����������ⲻͬ����֮������·��·�߳���
};

#endif