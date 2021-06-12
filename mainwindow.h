#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<iostream>
#include<vector>
#include<algorithm>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    struct KongNode
    {
        int start;
        int length;
        int end;
    };

    struct JinCheng
    {
        int NUM;//进程号
        int len;//进程所需长度
        int start;
        int end;
        bool AlreadyGua=0;
    };

    int Solution;//选择算法
    int num;
   std:: vector<KongNode> KongLian;//空闲链
   std::  vector<JinCheng> Gua;//等待进程
   std:: vector<JinCheng> Ing;//正在进行的进程

   void Sort();//加入或移除任务后，按哪种方式对空闲块进行排序
   void add();
   void remove();
   void choose();
   int FF(JinCheng& JC,bool AlreadyGua);
   int Recycle(JinCheng& JC);
  static  bool  length(const KongNode& a, const KongNode& b) ;//按长度给空闲分区排序
   static bool  start(const KongNode& a, const KongNode& b);//按起始地址给空闲分区排序


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
