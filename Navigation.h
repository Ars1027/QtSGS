#ifndef _NAVIGATION_H_
#define _NAVIGATION_H_

#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <time.h>

#define B_NodeNum			63	
#define NodeCount			63	
#define MaxValue			99999999//初始最短距离或最短时间
#define IteCount			100000	//迭代次数

struct Node
{
	std::string Name;//结点名字
	int LX, LY, PX, PY;//逻辑位置和实际位置
	int ConnectionNodeNum[4];//周围的结点编号
	double ConnectionNodeDistance[4];//到周围结点的距离
	double Crowed;//该结点的拥挤程度
	bool RideAllow;//该结点是否允许骑行自行车
	bool ServiceNode;
};

struct Road
{
	int NodeNum;//结点编号
	int LX, LY;//结点位置
	double Crowed;//该节点的拥挤程度
	int WaitCount;//等待时长计数器。用来表示模拟行进的人物在对应位置的等待时间
};

class DFS
{
public:
	Node MapNodes[NodeCount];//所有的节点信息
	bool CityVisited[NodeCount];//所有结点的访问信息
	int TravelCityNum[NodeCount * 10];//走过的结点信息
	int TravelCityCount;//走过的所有的结点数量----存储最佳路径时可以用来作为中断标志，进行多个校区之间的导航连接
	int CurrentCity;//当前所在结点编号
	double MoveSpeed;//移动速度--用于计算时间------最终速度为默认移动速度除以结点方向上的拥挤程度
	double MoveDistance;//移动的距离
	double MoveTime;//移动花费的时间
	DFS();
	~DFS();
	void Reset();//重置信息---除了地图结点信息，全部重置
	int SelectNextCityNum(bool);//选择下一个前进的结点----先寻找到一条路径到达终点，然后再计算距离和时间的优先程度
	void MoveAnt(bool, int);//移动当前位置，并计算移动距离和花费时间
	void Search(int, int, bool, int);//搜索一条起点到终点的路径出来。根据布尔变量判断是否更改移动速度
};

class Navigation
{
public:
	DFS SearchRoad;//搜索路径
	DFS BestRoad;//存储最佳路径
	Navigation();
	~Navigation();
	void Search1(int, int, bool, int);//最短路径。搜索任意不同两点之间的最短路径路线出来
};

#endif