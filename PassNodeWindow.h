#pragma once

#include <QWidget>
#include "ui_PassNodeWindow.h"

class PassNodeWindow : public QWidget
{
	Q_OBJECT

public:
	PassNodeWindow(QWidget *parent = nullptr);
	~PassNodeWindow();

public slots:
	void on_cancelButton_clicked();
	void on_confirmButton_clicked();

private:
	Ui::PassNodeWindowClass ui;
};
