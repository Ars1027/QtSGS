#include "QtSGS.h"

#include "PassNodeWindow.h"

int StartNodeNum = -1, EndNodeNum = -1;//����յ�����
std::vector<std::pair<int, double>> PassNodeNum;//;�������


QtSGS::QtSGS(QWidget* parent)
    : QWidget(parent), ui(new Ui::QtSGSClass)
{
    ui->setupUi(this);
    B_Img = QImage("./Resources/STUMap.png");
    BRec = QRect(80, 0,1436, 1032);
    NowArea = true;
    Simulation = false;
    ShowScanRoad = false;
    CurrentRoadNum = 0;
    day = 1;
    hour = 6;
    minute = 0;
    second = 0;
    StuX = -50;
    StuY = -50;
    StuSpeed = 1.2;
    FinalRoadCount = 0;
    ScanRoadCount = 0;
    ui->changeTimeF->setEnabled(true);
    ui->changeTimeS->setEnabled(false);
    ui->StopSearch->setEnabled(false);
    ui->ScanService->setEnabled(true);

    connect(ui->changeTimeF, SIGNAL(clicked()), this, SLOT(TimerFaster()));
    connect(ui->changeTimeS, SIGNAL(clicked()), this, SLOT(TimerSlower()));
    connect(ui->StartSearch, SIGNAL(clicked()), this, SLOT(clickStartSearch()));
    connect(ui->ScanService, SIGNAL(clicked()), this, SLOT(clickScanService()));
    connect(ui->StartBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeStartNodeNum()));
    connect(ui->EndBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(changeEndNodeNum()));
    connect(ui->StopSearch, SIGNAL(clicked()), this, SLOT(clickStopSearchButton()));
    TimerID = startTimer(100);
}

void QtSGS::timerEvent(QTimerEvent* e)
{
    if (e->timerId() == TimerID)
    {
        second++;
        if (second == 60)
        {
            second = 0;
            minute++;
            if (minute == 60)
            {
                minute = 0;
                hour++;
                if (hour == 24)
                {
                    hour = 0;
                    day++;
                    if (day == 8)
                        day = 1;
                }
            }
        }

        if (Simulation) // �����ʼģ���н�
        {
            if (FinalRoad[CurrentRoadNum].NodeNum < B_NodeNum)
                NowArea = true;
            else
                NowArea = false;

            if (CurrentRoadNum >= FinalRoadCount)
            {
                // ģ�����
                Simulation = false;
                ui->StartSearch->setEnabled(true);
                ui->StopSearch->setEnabled(false);
                ui->ScanService->setEnabled(true);
                clearFinalRoad();
                StuX = -50;
                StuY = -50;
                CurrentRoadNum = 0;
                FinalRoadCount = 0;
            }
            else
            {
                // ��֡�ƶ�ѧ��λ��
                double deltaX = FinalRoad[CurrentRoadNum + 1].LX - StuX;
                double deltaY = FinalRoad[CurrentRoadNum + 1].LY - StuY;
                double distance = sqrt(deltaX * deltaX + deltaY * deltaY);

                if (distance > 0)
                {
                    double Speed = StuSpeed / FinalRoad[CurrentRoadNum].Crowed;
                    double moveX = (deltaX / distance) * Speed;
                    double moveY = (deltaY / distance) * Speed;

                    StuX += moveX;
                    StuY += moveY;

                    // �ж��Ƿ񵽴���һ���ڵ�
                    if (fabs(deltaX) <= fabs(moveX) && fabs(deltaY) <= fabs(moveY))
                    {
                        StuX = FinalRoad[CurrentRoadNum + 1].LX;
                        StuY = FinalRoad[CurrentRoadNum + 1].LY;
                        CurrentRoadNum++;
                    }
                }
                else
                {
                    StuX = FinalRoad[CurrentRoadNum + 1].LX;
                    StuY = FinalRoad[CurrentRoadNum + 1].LY;
                    CurrentRoadNum++;
                }
            }
        }
    }
    repaint();
}

void QtSGS::paintEvent(QPaintEvent* e)
{
    QPainter pt(this);

    // ����ͼƬ
    pt.drawImage(BRec, B_Img);
    QImage startImg("./Resources/Start.ico");
    QImage endImg("./Resources/End.ico");
    QImage BikeImg("./Resources/Bike.ico");
    QImage StuImg("./Resources/Student.ico");
    QImage landmarkImg("./Resources/landmark.ico");

    if (Simulation)
    {
        // �������յ�
        pt.drawImage(FinalRoad[0].LX - 10, FinalRoad[0].LY - 10, startImg); // �������
        pt.drawImage(FinalRoad[FinalRoadCount - 1].LX - 10, FinalRoad[FinalRoadCount - 1].LY - 10, endImg); // �����յ�

        // ��ѧ��λ��

        // ����
        if (!RideBike) 
        {
            pt.drawImage(StuX - 20, StuY - 20, StuImg);
            QPen pen(QColor("#1819ff"), 4, Qt::SolidLine);
            pt.setPen(pen);
        }
        // �ﳵ   
        else 
        {
            pt.drawImage(StuX - 20, StuY - 20, BikeImg);
            QPen pen(QColor("#93ec2e"), 4, Qt::SolidLine);
            pt.setPen(pen);
        }
            

        
        // ���Ƶ�ǰѧ��λ�õ���һ������·��
        if (CurrentRoadNum < FinalRoadCount - 1)
        {
            pt.drawLine(StuX, StuY, FinalRoad[CurrentRoadNum + 1].LX, FinalRoad[CurrentRoadNum + 1].LY);
        }

        // ���ƴӵ�ǰ�ڵ�֮���·��
        for (int i = CurrentRoadNum + 1; i < FinalRoadCount - 1; i++)
        {
            if (NowArea && ((FinalRoad[i].NodeNum >= B_NodeNum || FinalRoad[i].NodeNum == -1) || (FinalRoad[i + 1].NodeNum >= B_NodeNum || FinalRoad[i + 1].NodeNum == -1)))
                break;
            else if (!NowArea && ((FinalRoad[i].NodeNum < B_NodeNum || FinalRoad[i].NodeNum == -1) || (FinalRoad[i + 1].NodeNum < B_NodeNum || FinalRoad[i + 1].NodeNum == -1)))
                break;
            pt.drawLine(FinalRoad[i].LX, FinalRoad[i].LY, FinalRoad[i + 1].LX, FinalRoad[i + 1].LY);
        }
    }

    // ������Ҫ�ص�
    if (ShowScanRoad)
    {
        for (int i = 0; i < B_NodeNum; i++)
        {
            if (ScanRoad[i].NodeNum == -1)
                break;
            else
            {
                pt.drawImage(ScanRoad[i].LX - 15, ScanRoad[i].LY - 15, landmarkImg);
            }
        }
    }
}



void QtSGS::closeEvent(QCloseEvent* e)
{
}

bool QtSGS::SearchJudge()//������ǰ���ж�
{
    if (ui->NavigationBox->currentIndex() == 0)
    {
        QMessageBox::information(this, "����", QString("��ѡ�񵼺�����"));
        return false;
    }
    
    else if (ui->NavigationBox->currentIndex() == 1 || ui->NavigationBox->currentIndex() == 2)
    {
        //�ж�����յ��Ƿ�Ϊ�ջ��Ƿ���ͬ
        if (ui->StartBox->currentIndex() == 0 || ui->EndBox->currentIndex() == 0)
        {
            QMessageBox::information(this, "����", QString("��ѡ��ǰλ�ú��յ�λ��"));
            return false;
        }
        if (ui->StartBox->currentIndex() == ui->EndBox->currentIndex())
        {
            QMessageBox::information(this, "����", QString("��ǰλ�ú��յ�λ�ò�����ͬ"));
            return false;
        }
        //�ж�;��λ��������յ���ͬ
        for (int i = 0; i < PassNodeNum.size(); i++) 
        {
            if (StartNodeNum == PassNodeNum[i].first || EndNodeNum == PassNodeNum[i].first)
            {
                QMessageBox::information(this, "����", QString("��ǰλ�ú��յ�λ�ò��ܺ�;������ͬ"));
                return false;
            }
        }
    }
    return true;
}

void QtSGS::clearFinalRoad()//��յ���·��
{
    for (int i = 0; i < 2 * NodeCount; i++)
        FinalRoad[i].NodeNum = -1;//����NodeNumֵ�жϵ����Ƿ����
    FinalRoadCount = 0;
    ScanRoadCount = 0;
    ShowScanRoad = false;
    for (int j = 0; j < B_NodeNum; j++)
        ScanRoad[j].NodeNum = -1;
}



void QtSGS::WriteData()//����Ϣд����־�ļ�
{
    std::ofstream OF("./Data.db", std::ios::app);
    if (!OF)
    {
        OF.close();
        return;
    }
    Navigation* N = new Navigation();
    OF << "������ʼ---->";
    for (int i = 0; i < FinalRoadCount; i++)
    {
        OF << N->BestRoad.MapNodes[FinalRoad[i].NodeNum].Name << "--->";
    }
    OF << "��������";
    OF << std::endl << std::endl;
    OF.close();
    delete N;
}

void QtSGS::TimerFaster()//�ӿ�����ʱ��
{
    killTimer(TimerID);
    TimerID = startTimer(5);
    ui->changeTimeF->setEnabled(false);
    ui->changeTimeS->setEnabled(true);
}

void QtSGS::TimerSlower()//��������ʱ��
{
    killTimer(TimerID);
    TimerID = startTimer(100);
    ui->changeTimeF->setEnabled(true);
    ui->changeTimeS->setEnabled(false);
}

void QtSGS::clickScanService()//ɨ�踽���ķ�����ʩ
{
    clearFinalRoad();
    if (ui->StartBox->currentIndex() == 0)
    {
        QMessageBox::information(this, "����", QString("��ѡ��ǰλ��"));
        return;
    }
    Navigation* na = new Navigation();
    if (NowArea)
    {
        for (int i = 0; i < B_NodeNum; i++)
            if (na->BestRoad.MapNodes[i].ServiceNode && sqrt(pow(na->BestRoad.MapNodes[StartNodeNum].PX - na->BestRoad.MapNodes[i].PX, 2) +
                pow(na->BestRoad.MapNodes[StartNodeNum].PY - na->BestRoad.MapNodes[i].PY, 2)) <= 150)
            {
                ScanRoad[ScanRoadCount].NodeNum = i;
                ScanRoad[ScanRoadCount].LX = 40 + na->BestRoad.MapNodes[i].PX;
                ScanRoad[ScanRoadCount].LY = 1000 - na->BestRoad.MapNodes[i].PY;
                ScanRoadCount++;
            }
    }
    delete na;
    ShowScanRoad = true;
}

void QtSGS::clickStartSearch() {
    if (!SearchJudge()) return;
    srand((unsigned)time(NULL));

    clearFinalRoad();

    bool isRideBike = (ui->NavigationBox->currentIndex() == 2); // 2Ϊ���в���
    double speed = isRideBike ? 2.4 : 1.2; // �ٶȣ�����2.4������1.2

    // ����ʱ��Ҫ��֤�����յ��Ƿ���������
    if (isRideBike) {
        Navigation* na = new Navigation();
        if (!na->BestRoad.MapNodes[StartNodeNum].RideAllow || !na->BestRoad.MapNodes[EndNodeNum].RideAllow) {
            QMessageBox::information(this, "����", QString("��ǰλ�û��յ�λ�ò���������"));
            return;
        }
        for (int i = 0; i < PassNodeNum.size(); i++)
        {
            if (!na->BestRoad.MapNodes[PassNodeNum[i].first].RideAllow )
            {
                QMessageBox::information(this, "����", QString(";����λ�ò���������"));
                return;
            }
        }
        delete na;
    }

    // �ж� PassNodeNum �Ƿ�Ϊ��
    if (PassNodeNum.empty()) {
        // ���;��������Ϊ�գ�ֱ�Ӵ���㵽�յ�
        Navigation* n = new Navigation();
        n->Search1(StartNodeNum, EndNodeNum, isRideBike, (day - 1) * 86400 + hour * 3600 + minute * 60 + second);

        // ��������·������ÿ���ڵ���Ϣ�洢�� FinalRoad ��
        for (int i = 0; i < n->BestRoad.TravelCityCount; i++) {
            FinalRoad[FinalRoadCount].NodeNum = n->BestRoad.TravelCityNum[i];
            FinalRoad[FinalRoadCount].LX = 40 + n->BestRoad.MapNodes[n->BestRoad.TravelCityNum[i]].LX;
            FinalRoad[FinalRoadCount].LY = 1000 - n->BestRoad.MapNodes[n->BestRoad.TravelCityNum[i]].LY;
            FinalRoad[FinalRoadCount].Crowed = n->BestRoad.MapNodes[n->BestRoad.TravelCityNum[i]].Crowed;
            FinalRoad[FinalRoadCount].WaitCount = 0;
            FinalRoadCount++;
        }

        // �����յ���Ϣ
        FinalRoad[FinalRoadCount].NodeNum = FinalRoad[FinalRoadCount - 1].NodeNum;
        FinalRoad[FinalRoadCount].LX = 40 + n->BestRoad.MapNodes[FinalRoad[FinalRoadCount].NodeNum].PX;
        FinalRoad[FinalRoadCount].LY = 1000 - n->BestRoad.MapNodes[FinalRoad[FinalRoadCount].NodeNum].PY;
        FinalRoad[FinalRoadCount].Crowed = FinalRoad[FinalRoadCount - 1].Crowed;
        FinalRoad[FinalRoadCount].WaitCount = 0;

        Simulation = true;
        CurrentRoadNum = 0;
        StuX = FinalRoad[0].LX;
        StuY = FinalRoad[0].LY;
        StuSpeed = speed;
        RideBike = isRideBike;

        delete n;
        ui->StartSearch->setEnabled(false);
        ui->StopSearch->setEnabled(true);
        ui->ScanService->setEnabled(false);

        WriteData();
        repaint();
        return;
    }

    // ����㵽��һ��;����
    Navigation* n = new Navigation();
    n->Search1(StartNodeNum, PassNodeNum[0].first, isRideBike, (day - 1) * 86400 + hour * 3600 + minute * 60 + second);

    // ����;�������飬�������·���滮
    for (int i = 0; i < PassNodeNum.size(); i++) {
        for (int j = 0; j < n->BestRoad.TravelCityCount; j++) {
            FinalRoad[FinalRoadCount].NodeNum = n->BestRoad.TravelCityNum[j];
            FinalRoad[FinalRoadCount].LX = 40 + n->BestRoad.MapNodes[n->BestRoad.TravelCityNum[j]].LX;
            FinalRoad[FinalRoadCount].LY = 1000 - n->BestRoad.MapNodes[n->BestRoad.TravelCityNum[j]].LY;
            FinalRoad[FinalRoadCount].Crowed = n->BestRoad.MapNodes[n->BestRoad.TravelCityNum[j]].Crowed;
            FinalRoad[FinalRoadCount].WaitCount = 0;
            FinalRoadCount++;
        }
        if (i + 1 < PassNodeNum.size()) {
            delete n;
            n = new Navigation();
            n->Search1(PassNodeNum[i].first, PassNodeNum[i + 1].first, isRideBike, (day - 1) * 86400 + hour * 3600 + minute * 60 + second);
        }
    }

    // �������һ��;���㵽�յ�
    Navigation* na = new Navigation();
    na->Search1(PassNodeNum[PassNodeNum.size() - 1].first, EndNodeNum, isRideBike, (day - 1) * 86400 + hour * 3600 + minute * 60 + second);

    for (int i = 0; i < na->BestRoad.TravelCityCount; i++) {
        FinalRoad[FinalRoadCount].NodeNum = na->BestRoad.TravelCityNum[i];
        FinalRoad[FinalRoadCount].LX = 40 + na->BestRoad.MapNodes[na->BestRoad.TravelCityNum[i]].LX;
        FinalRoad[FinalRoadCount].LY = 1000 - na->BestRoad.MapNodes[na->BestRoad.TravelCityNum[i]].LY;
        FinalRoad[FinalRoadCount].Crowed = na->BestRoad.MapNodes[na->BestRoad.TravelCityNum[i]].Crowed;
        FinalRoad[FinalRoadCount].WaitCount = 0;
        FinalRoadCount++;
    }

    // �����յ���Ϣ
    FinalRoad[FinalRoadCount].NodeNum = FinalRoad[FinalRoadCount - 1].NodeNum;
    FinalRoad[FinalRoadCount].LX = 40 + na->BestRoad.MapNodes[FinalRoad[FinalRoadCount].NodeNum].PX;
    FinalRoad[FinalRoadCount].LY = 1000 - na->BestRoad.MapNodes[FinalRoad[FinalRoadCount].NodeNum].PY;
    FinalRoad[FinalRoadCount].Crowed = FinalRoad[FinalRoadCount - 1].Crowed;
    FinalRoad[FinalRoadCount].WaitCount = 0;

    Simulation = true;
    CurrentRoadNum = 0;
    StuX = FinalRoad[0].LX;
    StuY = FinalRoad[0].LY;
    StuSpeed = speed;
    RideBike = isRideBike;

    delete n;
    delete na;
    ui->StartSearch->setEnabled(false);
    ui->StopSearch->setEnabled(true);
    ui->ScanService->setEnabled(false);

    WriteData();
    repaint();
}

void QtSGS::clickSwitchButton()
{
    if (NowArea)
        NowArea = false;
    else
        NowArea = true;
    repaint();
}
 
void QtSGS::clickStopSearchButton()
{
    this->Simulation = false;
    ui->StartSearch->setEnabled(true);
    ui->StopSearch->setEnabled(false);
    ui->ScanService->setEnabled(true);
}

void QtSGS::changeStartNodeNum()//������������κβ���������¶�Ӧ����Ϣ
{
    StartNodeNum = (ui->StartBox->currentIndex()) - 1;
}

void QtSGS::changeEndNodeNum()
{
    EndNodeNum = (ui->EndBox->currentIndex()) - 1;
    
}



void QtSGS::on_pushButton_clicked()
{
    PassNodeWindow * window = new PassNodeWindow;
    window->show();
}

PassNodeWindow::PassNodeWindow(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

PassNodeWindow::~PassNodeWindow()
{}

void PassNodeWindow::on_cancelButton_clicked()
{
    this->close();
}

void PassNodeWindow::on_confirmButton_clicked()
{
    QList<QCheckBox*> checkboxList = this->findChildren<QCheckBox*>();
    DFS* d = new DFS();
    PassNodeNum.clear();
    for (int i = 0; i < checkboxList.size(); i++)
    {

        if (checkboxList[i]->isChecked())
        {
            int dx = d->MapNodes[i].LX - d->MapNodes[StartNodeNum].LX;
            int dy = d->MapNodes[i].LY - d->MapNodes[StartNodeNum].LY;
            double distance = sqrt(dx * dx + dy * dy);
            PassNodeNum.push_back({ i, distance });
        }
    }
    std::sort(PassNodeNum.begin(), PassNodeNum.end(), [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
        return a.second < b.second; // ����pair�е�doubleֵ��С��������
        });
    this->close();
}
