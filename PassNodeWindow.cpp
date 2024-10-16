#include "PassNodeWindow.h"
#include "QtSGS.h"

 

PassNodeWindow::PassNodeWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(on_cancelButton_clicked()));
}

PassNodeWindow::~PassNodeWindow()
{}

void PassNodeWindow::on_cancelButton_clicked()
{
	this->close();
}

void PassNodeWindow::on_confirmButton_clicked(int *p)
{
    for (int i = 0; i < 36; i++)
    {
       *(p+i) = -1;
    }
    
    QList<QCheckBox*> checkboxList = this->findChildren<QCheckBox*>();
    for (int i = 0; i < checkboxList.size(); i++)
    {
        //connect(checkboxList[i], &QCheckBox::clicked, this, [=]()
            //{
                
        if (checkboxList[i]->isChecked())
            *(p+i) = i;
                //你需要执行的槽函数内容
            //});
    }
	this->close();
}
