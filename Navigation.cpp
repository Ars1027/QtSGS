#include "Navigation.h"
#include <iostream>
using namespace std;

//随机化的启发式搜索算法
DFS::DFS()
{
	std::ifstream INF;
	// 读取地图文件并存储到MapNodes中，每个节点包含其坐标(LX, LY)和四个连接的节点及其连接距离、是否允许骑行、拥挤系数
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
	// 计算相邻节点之间的距离并存储在ConnectionNodeDistance中。如果节点不连接，则将距离设置为MaxValue
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

void DFS::Reset()//重置蚂蚁信息---除了地图结点信息，全部重置
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



int DFS::SelectNextCityNum(bool ride)//随机性地选择下一个前进的结点（四个临近点）
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
	NextNodeNum = rand() % Count; // 随机性
	return Buffer[NextNodeNum];
}

void DFS::MoveAnt(bool ride, int nowTime)//移动蚂蚁位置，并计算移动距离和花费时间
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
	CityVisited[CurrentCity] = true; // 将该节点标记为已访问
	TravelCityCount++;
}

void DFS::Search(int start, int finish, bool ride, int nowTime)//根据布尔变量判断是否更改默认移动速度。
{
	CurrentCity = start;
	TravelCityNum[TravelCityCount] = CurrentCity;
	TravelCityCount++;
	CityVisited[CurrentCity] = true;
	if (ride)
		MoveSpeed = 2.4;
	else
		MoveSpeed = 1.2;
	// 调用MoveAnt让蚂蚁移动，知道无法前进或者抵达终点（类似dfs）
	while (CurrentCity != finish && CurrentCity != -1 && TravelCityCount < NodeCount)
		MoveAnt(ride, nowTime);
}

Navigation::Navigation()
{

}

Navigation::~Navigation()
{

}


void Navigation::Search1(int start, int finish, bool bike, int nowTime)//同一校区间两点的最短路径
{
		BestRoad.MoveDistance = MaxValue;//先将最佳路径设置为最大值。
		for (int i = 0; i < IteCount; i++)
		{
			SearchRoad.Reset();//初始化
			SearchRoad.Search(start, finish, bike, nowTime);//搜索起点到终点的最短路径，false表示不骑车。
			// 当找到比当前最佳解更短的路径时，更新BestAnt
			if (SearchRoad.CurrentCity == finish && SearchRoad.MoveDistance < BestRoad.MoveDistance)
			{
				BestRoad.MoveDistance = SearchRoad.MoveDistance;
				BestRoad.TravelCityCount = SearchRoad.TravelCityCount;
				for (int j = 0; j < NodeCount * 2; j++)
					BestRoad.TravelCityNum[j] = SearchRoad.TravelCityNum[j];
			}
		}
}

