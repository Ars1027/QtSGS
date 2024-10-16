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
    bool SearchJudge();//������ǰ���ж�
    void clearFinalRoad();//��յ���·��

    void WriteData();//����Ϣд����־�ļ�
private:
    Ui::QtSGSClass* ui;
    QImage B_Img;
    QRect BRec;//ͼƬλ��
    Road FinalRoad[10 * NodeCount];
    int FinalRoadCount;//�ܹ��߹��Ľ��
    int CurrentRoadNum;//��ǰ������λ��
    double StuX, StuY;//ģ���н�����������
    double StuSpeed;//ģ���н��������ٶ�
    int waitingCount;//���ڼ�������ȴ��ļ�ʱ��
    bool Simulation;//ģ���н�״̬����������ɨ�踽��������
    bool NowArea;
    bool RideBike;
    int day,hour, minute, second;//ʱ����
    int TimerID;//��ʱ��ID
    Road ScanRoad[B_NodeNum];
    bool ShowScanRoad;
    int ScanRoadCount;
 
public slots:
    void TimerFaster();//�ӿ�����ʱ��
    void TimerSlower();//��������ʱ��
    void clickScanService();//ɨ�踽���ķ�����ʩ
    void clickStartSearch();//����������ť�󴥷��ĺ���
    void clickSwitchButton();//����ֹͣ���������ĺ���
    void clickStopSearchButton();
    void changeStartNodeNum();//������������κβ���������¶�Ӧ����Ϣ
    void changeEndNodeNum();

    void on_pushButton_clicked();
};
