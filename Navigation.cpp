#include "Navigation.h"
#include <iostream>
using namespace std;

//�����������ʽ�����㷨
DFS::DFS()
{
	std::ifstream INF;
	// ��ȡ��ͼ�ļ����洢��MapNodes�У�ÿ���ڵ����������(LX, LY)���ĸ����ӵĽڵ㼰�����Ӿ��롢�Ƿ��������С�ӵ��ϵ��
	INF.open("./Resources/Maptest.txt");
	for (int i = 0; i < NodeCount; i++)
	{
		INF >> MapNodes[i].Name;
		INF >> MapNodes[i].LX >> MapNodes[i].LY;
		INF >> MapNodes[i].PX >> MapNodes[i].PY;
		for (int j = 0; j < 4; j++)
		{
			INF >> MapNodes[i].ConnectionNodeNum[j];
			MapNodes[i].Crowed = 1.0;
			MapNodes[i].RideAllow = true;
		}
		INF >> MapNodes[i].Crowed;
		INF >> MapNodes[i].RideAllow;
		INF >> MapNodes[i].ServiceNode;
		CityVisited[i] = false;
	}
	INF.close();
	// �������ڽڵ�֮��ľ��벢�洢��ConnectionNodeDistance�С�����ڵ㲻���ӣ��򽫾�������ΪMaxValue
	int x1, x2, y1, y2;
	int a, b;
	for (int m = 0; m < NodeCount; m++)
	{
		for (int n = 0; n < 4; n++)
		{
			if (MapNodes[m].ConnectionNodeNum[n] != -1)
			{
				x1 = MapNodes[m].LX;
				x2 = MapNodes[MapNodes[m].ConnectionNodeNum[n]].LX;
				y1 = MapNodes[m].LY;
				y2 = MapNodes[MapNodes[m].ConnectionNodeNum[n]].LY;
				a = x1 - x2;
				b = y1 - y2;
				MapNodes[m].ConnectionNodeDistance[n] = sqrt(a * a + b * b);
			}
			else
				MapNodes[m].ConnectionNodeDistance[n] = MaxValue;
		}
	}
	
	for (int p = 0; p < 2 * NodeCount; p++)
		TravelCityNum[p] = -1;
	TravelCityCount = 0;
	CurrentCity = -1;
	MoveSpeed = 1.2;
	MoveDistance = 0.0;
	MoveTime = 0.0;
}

DFS::~DFS()
{

}

void DFS::Reset()//����������Ϣ---���˵�ͼ�����Ϣ��ȫ������
{
	for (int i = 0; i < NodeCount; i++)
		CityVisited[i] = false;
	for (int p = 0; p < 2 * NodeCount; p++)
		TravelCityNum[p] = -1;
	TravelCityCount = 0;
	CurrentCity = -1;
	MoveSpeed = 1.2;
	MoveDistance = 0.0;
	MoveTime = 0.0;
}



int DFS::SelectNextCityNum(bool ride)//����Ե�ѡ����һ��ǰ���Ľ�㣨�ĸ��ٽ��㣩
{
	int NextNodeNum = -1, Buffer[4] = { -1, -1, -1, -1 }, Count = 0;
	for (int i = 0; i < 4; i++)
	{
		if (MapNodes[CurrentCity].ConnectionNodeNum[i] != -1 && CityVisited[MapNodes[CurrentCity].ConnectionNodeNum[i]] == false)
		{
			if(ride && MapNodes[CurrentCity].RideAllow)
				Buffer[Count++] = MapNodes[CurrentCity].ConnectionNodeNum[i];
			else if(!ride)
				Buffer[Count++] = MapNodes[CurrentCity].ConnectionNodeNum[i];
		}
	}
	if(Count == 0)
		return NextNodeNum;
	NextNodeNum = rand() % Count; // �����
	return Buffer[NextNodeNum];
}

void DFS::MoveAnt(bool ride, int nowTime)//�ƶ�����λ�ã��������ƶ�����ͻ���ʱ��
{
	int NextNodeNum = SelectNextCityNum(ride);
	if (NextNodeNum == -1)
	{
		CurrentCity = NextNodeNum;
		return;
	}
	for(int i = 0; i < 4; i++)
		if (NextNodeNum == MapNodes[CurrentCity].ConnectionNodeNum[i])
		{
			double Speed = MoveSpeed / MapNodes[MapNodes[CurrentCity].ConnectionNodeNum[i]].Crowed;
			MoveDistance += MapNodes[CurrentCity].ConnectionNodeDistance[i];
			MoveTime += MapNodes[CurrentCity].ConnectionNodeDistance[i] / Speed;
			break;
		}
	TravelCityNum[TravelCityCount] = NextNodeNum;
	CurrentCity = NextNodeNum;
	CityVisited[CurrentCity] = true; // ���ýڵ���Ϊ�ѷ���
	TravelCityCount++;
}

void DFS::Search(int start, int finish, bool ride, int nowTime)//���ݲ��������ж��Ƿ����Ĭ���ƶ��ٶȡ�
{
	CurrentCity = start;
	TravelCityNum[TravelCityCount] = CurrentCity;
	TravelCityCount++;
	CityVisited[CurrentCity] = true;
	if (ride)
		MoveSpeed = 2.4;
	else
		MoveSpeed = 1.2;
	// ����MoveAnt�������ƶ���֪���޷�ǰ�����ߵִ��յ㣨����dfs��
	while (CurrentCity != finish && CurrentCity != -1 && TravelCityCount < NodeCount)
		MoveAnt(ride, nowTime);
}

Navigation::Navigation()
{

}

Navigation::~Navigation()
{

}


void Navigation::Search1(int start, int finish, bool bike, int nowTime)//ͬһУ������������·��
{
		BestRoad.MoveDistance = MaxValue;//�Ƚ����·������Ϊ���ֵ��
		for (int i = 0; i < IteCount; i++)
		{
			SearchRoad.Reset();//��ʼ��
			SearchRoad.Search(start, finish, bike, nowTime);//������㵽�յ�����·����false��ʾ���ﳵ��
			// ���ҵ��ȵ�ǰ��ѽ���̵�·��ʱ������BestAnt
			if (SearchRoad.CurrentCity == finish && SearchRoad.MoveDistance < BestRoad.MoveDistance)
			{
				BestRoad.MoveDistance = SearchRoad.MoveDistance;
				BestRoad.TravelCityCount = SearchRoad.TravelCityCount;
				for (int j = 0; j < NodeCount * 2; j++)
					BestRoad.TravelCityNum[j] = SearchRoad.TravelCityNum[j];
			}
		}
}

