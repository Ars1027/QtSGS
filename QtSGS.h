#pragma once

#include "UTF.h"
#include <QtWidgets/QWidget>
#include "ui_QtSGS.h"
#include "Navigation.h"
#include "PassNodeWindow.h"

class QtSGS : public QWidget
{
    Q_OBJECT

public:
    QtSGS(QWidget *parent = Q_NULLPTR);

    void timerEvent(QTimerEvent*);
    void paintEvent(QPaintEvent*);
    void closeEvent(QCloseEvent*);
    bool SearchJudge();//搜索的前置判断
    void clearFinalRoad();//清空导航路线

    void WriteData();//将信息写入日志文件
private:
    Ui::QtSGSClass* ui;
    QImage B_Img;
    QRect BRec;//图片位置
    Road FinalRoad[10 * NodeCount];
    int FinalRoadCount;//总共走过的结点
    int CurrentRoadNum;//当前所处的位置
    double StuX, StuY;//模拟行进的人物坐标
    double StuSpeed;//模拟行进的人物速度
    int waitingCount;//用于计算人物等待的计时器
    bool Simulation;//模拟行进状态。用来限制扫描附近建筑物
    bool NowArea;
    bool RideBike;
    int day,hour, minute, second;//时分秒
    int TimerID;//计时器ID
    Road ScanRoad[B_NodeNum];
    bool ShowScanRoad;
    int ScanRoadCount;
 
public slots:
    void TimerFaster();//加快虚拟时间
    void TimerSlower();//减慢虚拟时间
    void clickScanService();//扫描附近的服务设施
    void clickStartSearch();//单击搜索按钮后触发的函数
    void clickSwitchButton();//单击停止导航触发的函数
    void clickStopSearchButton();
    void changeStartNodeNum();//如果下拉框有任何操作。则更新对应的信息
    void changeEndNodeNum();

    void on_pushButton_clicked();
};
