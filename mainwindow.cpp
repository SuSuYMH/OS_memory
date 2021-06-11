#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
#include<vector>
#include<algorithm>
#include<Qstring>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    num=0;
    KongNode init = { 0,640,640 };
    KongLian.push_back(init);
     ui->choosebox->addItem("First Fit algorithm");
      ui->choosebox->addItem("Best Fit algorithm");
    connect(ui->DemandBT,&QPushButton::clicked,this,&MainWindow::add);
    connect(ui->RecycleBT,&QPushButton::clicked,this,&MainWindow::remove);
    connect(ui->ChooseBT,&QPushButton::clicked,this,&MainWindow::choose);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void  MainWindow::choose()
{
     QString currentstr=ui->choosebox->currentText();
     if( currentstr=="First Fit algorithm")
     {
         Solution=1;
     }
     else
     {
         Solution=0;
     }
      ui->comboBox->clear();//清空目前所有
      ui->listWidget->clear();
}

void MainWindow::remove()
{
     ui->listWidget->clear();//先清空
     QString currentstr=ui->comboBox->currentText();
     int currentnum=currentstr.toInt();
     for (int i = 0; i < Ing.size(); i++)
                 {
                     if (Ing[i].NUM == currentnum)
                     {
                         Recycle(Ing[i]);
                     }
                 }
     for (int i = 0; i < KongLian.size(); i++)
     {
         /*result.sprintf("长度为%d          起始地址为%d         终止地址为%d\n",KongLian[i].length,KongLian[i].start,KongLian[i].end);
         QStringList *mode=new QStringList(result);
         ui->listView->setModel(result);*/
         QString str1=QString::number(KongLian[i].length);
         QString str2=QString::number(KongLian[i].start);
         QString str3=QString::number(KongLian[i].end);
         QString str4=QString::number(i);
         QString str="current block"+str4+"     Free block size:"+str1+"     start address:"+str2+"     end address:"+str3;
         ui->listWidget->insertItem(i,str);
     }
      ui->comboBox->clear();
     for (int i = 0; i < Ing.size(); i++)//更新删除下拉框
     {
         ui->comboBox->addItem(QString::number(Ing[i].NUM));
     }
}

void MainWindow::add()
{
    ui->listWidget->clear();//先清空
   JinCheng CurrentJinCheng;
   CurrentJinCheng.len=ui->lineEdit->text().toInt();
   CurrentJinCheng.NUM = num++;//进程号
   FF(CurrentJinCheng);
   for (int i = 0; i < KongLian.size(); i++)
   {
       /*result.sprintf("长度为%d          起始地址为%d         终止地址为%d\n",KongLian[i].length,KongLian[i].start,KongLian[i].end);
       QStringList *mode=new QStringList(result);
       ui->listView->setModel(result);*/
       QString str1=QString::number(KongLian[i].length);
       QString str2=QString::number(KongLian[i].start);
       QString str3=QString::number(KongLian[i].end);
       QString str4=QString::number(i);
       QString str="current block"+str4+"     Free block size:"+str1+"     start address:"+str2+"     end address:"+str3;
       ui->listWidget->insertItem(i,str);
   }
     ui->comboBox->clear();
   for (int i = 0; i < Ing.size(); i++)//更新删除下拉框
   {
       ui->comboBox->addItem(QString::number(Ing[i].NUM));
   }
}
bool MainWindow:: length(const KongNode& a, const KongNode& b) //按长度排序
{
    return a.length < b.length;
}

bool  MainWindow::start(const KongNode& a, const KongNode& b)//按初始值真排序
{
    return a.start < b.start;
}

void MainWindow::Sort()
{
    if (Solution == 1)
    {
        sort(KongLian.begin(), KongLian.end(), start);//把KongNode按照指针顺序排序
    }
    else
    {
        sort(KongLian.begin(), KongLian.end(), length);
    }
}

int MainWindow::FF(JinCheng & JC)//JC为分配的进程
{
    bool Success = 0;//分配是否成功，没成功则挂到Gua中等待
    for (int i = 0; i < KongLian.size(); i++)
    {
        if (KongLian[i].length >= JC.len)
        {
            Success = 1;
            JC.start = KongLian[i].start;
            JC.end= KongLian[i].start+JC.len;
            if (KongLian[i].length == JC.len)//满了直接删除
            {
                if (i < KongLian.size()-1)//不是最后一个结点的话，把它换成最后一个节点，然后直接删除最后一个节点即可
                    KongLian[i] = KongLian.back();
                KongLian.pop_back();
            }
            else//如果没占满则把这个空闲结点减小
            {
                KongLian[i].start = KongLian[i].start+JC.len;
                KongLian[i].length = KongLian[i].length-JC.len;
            }
            break;
        }
    }
    if (Success == false)//没成功分配挂上去
    {
        Gua.push_back(JC);
    }
    else//成功运行的
    {
        Ing.push_back(JC);
    }
    Sort();
    //sort(KongLian.begin(), KongLian.end(), length);//把KongNode按照length的大小进行排序，每次分配或者回收之后都要进行排序，PS：之后可以把它抽象成一个函数来解决
    //sort(KongLian.begin(), KongLian.end(), start);//把KongNode按照指针顺序排序
    return 0;
}

int MainWindow:: Recycle(JinCheng& JC)//根据之前用户传进来的NUM，找到这个进程，然后把进程的结构体传给这个函数
{
    int xia_kong = -1;//运行进程下面有空，为-1表示没有，否则表示有，且数字就是这个空的编号
    int shang_kong = -1;
    for (int i = 0; i < KongLian.size(); i++)//先总体检查一遍下面有没有空
    {
        if (KongLian[i].start == JC.end)
        {
            KongLian[i].start = JC.start;
            KongLian[i].length += JC.len;
            xia_kong = i;
            break;
        }

    }
    for (int i = 0; i < KongLian.size(); i++)
    {
        if (KongLian[i].end == JC.start)//上面有空
        {
            if (xia_kong == 0)//上有空下没空
            {
                KongLian[i].end = JC.end;
                KongLian[i].length += JC.len;
            }
            else//上下都有空
            {
                KongLian[xia_kong].start = KongLian[i].start;
                KongLian[xia_kong].length += KongLian[i].length;
                if (i < KongLian.size() - 1)//删除这个节点
                    KongLian[i] = KongLian.back();
                KongLian.pop_back();
            }
            shang_kong = 1;
        }
    }
    if((shang_kong == -1)&&(xia_kong == -1))//上下都没空，直接新生成一个空结点
    {
        KongNode Newnode = { JC.start,JC.len,JC.end };
        KongLian.push_back(Newnode);
    }
    JC= Ing.back();
    Ing.pop_back();//删除进程
    Sort();
    //sort(KongLian.begin(), KongLian.end(), length);//把KongNode按照length的大小进行排序
    //sort(KongLian.begin(), KongLian.end(), start);//把KongNode按照指针顺序排序
    for (int i = 0; i < Gua.size(); i++)
    {
        FF(Gua[i]);//看看这些被挂载的玩意能不能运行了啦！
    }
    return 0;
}
