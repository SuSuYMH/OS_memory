# 说明文档

### 项目简介

操作系统第二个项目，1953714杨茗淏，指导教师王东青

### 项目功能

可以选择动态分区中的首次适应算法或最佳适应算法进行内存动态分区的模拟。每次操作可以选择进行分配作业或者回收作业，分配作业要提供作业所需要的内存空间大小，回收作业要提供作业的编号。每次在分配任务或者回收任务结束后，列表都会列出当前空闲分区的情况，包括每一块的大小、起始位置、终止位置，以及当前正在执行的作业数和被挂起的作业数。

### 项目环境

开发环境：C++、QT

运行环境：已经把程序运行所需要的动态库打包到文件夹中，点击.exe即可运行

### 功能实现

##### 类成员设计

* 最主要类成员函数

```c++
//进程分配函数
int FF(JinCheng & JC, bool AlreadyGua)//JC为分配的进程
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
		if (AlreadyGua == 0)//如果之前就已经阻塞就不需要把它再挂到阻塞数组中
		{
			Gua.push_back(JC);
		}	
	}
	else//成功运行的
	{
		if (AlreadyGua == 0)
		{
			Ing.push_back(JC);
		}
		else
		{
			Ing.push_back(JC);//把JC挂到运行数组
			JC = Gua.back();
			Gua.pop_back();
		}
	}
Sort();//重新对空闲分区连进行排序
return 0;
}
```
进程分配函数FF()，传进去一个进程结构体引用和一个bool类型变量，bool类型变量AlreadyGua是用来记录当前传进来的进程是刚进来的进程还是被阻塞中的进程。先假设没有分配成功。依次遍历整个空闲分区数组，如果有满足的就将进程分配进去，并减小该分区的大小，同时把传进来的进程设置为运行状态，挂在运行进程数组中（如果之前处于阻塞态，则把他从阻塞数组中取出，再存放在运行数组中）。分配完成后重新根据用户选择的算法按照空闲分区的大小（最佳适应算法）或者空闲分区的首地址（首次适应算法）对空闲分区链进行排序。

```c++
//进程回收函数
int Recycle(JinCheng& JC)//根据之前用户传进来的NUM，找到这个进程，然后把进程的结构体传给这个函数
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
			if (xia_kong == -1)//上有空下没空
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
	if ((shang_kong == -1) && (xia_kong == -1))//上下都没空，直接新生成一个空结点
	{
		KongNode Newnode = { JC.start,JC.len,JC.end };
		KongLian.push_back(Newnode);
	}
	JC = Ing.back();
	Ing.pop_back();//删除进程
	Sort();
	//sort(KongLian.begin(), KongLian.end(), length);//把KongNode按照length的大小进行排序
	//sort(KongLian.begin(), KongLian.end(), start);//把KongNode按照指针顺序排序
	for (int i = 0; i < Gua.size(); i++)
	{
		FF(Gua[i], 1);//看看这些被挂载的玩意能不能运行了啦！
	}
	return 0;
}
```
进程回收函数Recycle()，在主运行循环中已经根据用户传进来的进程号找到了对应的运行进程，函数参数JC即为该进程的引用。回收进程即把它从运行数组中去掉,并把对应的内存释放出来.内存释放有四种情况,运行进程的上面有空区则和上面合并为一个空闲分区,下面有空去则和下面合并,上下都有空则一起合并,上下都没空就仅仅释放内存即可.然后每次释放完内存还要遍历阻塞进程看是否有进程可以被加进来.

* 类成员函数

```c++
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
    };

    int Solution;//选择算法
    int num;
   std:: vector<KongNode> KongLian;//空闲分区链
   std:: vector<JinCheng> Gua;//等待进程
   std:: vector<JinCheng> Ing;//正在进行的进程
```

其中KongNode结构体用来记录当前的一个空闲分区的情况，start用来记录当前空闲分区的内存起始位置，end用来记录当前空闲分区的内存终止位置，length用来记录当前空闲分区的长度。在程序运行时会生成一个结构体数组也即空闲分区表的逻辑实现。

JingCheng结构体用来记录作业的情况，每次添加一个新进程就会给进程一个进程号NUM方便管理，len记录进程所需内存长度，start和end在进程内存分配成功后会记录进程再内存中的起始位置和终止位置。

Solution用来记录当前选择的算法，num用来记录最初到现在所有要被分配的作业数（已经运行完的、运行中的、等待运行的作业的总和）。

##### 主循环设计

每次从UI中接受数据传入相应的变量中,进行判断后进行内存分配或者内存回收的操作.

* 其中绑定了三个按钮与相应的操作

```c++
connect(ui->DemandBT,&QPushButton::clicked,this,&MainWindow::add);
connect(ui->RecycleBT,&QPushButton::clicked,this,&MainWindow::remove);
connect(ui->ChooseBT,&QPushButton::clicked,this,&MainWindow::choose);
```

* 选择两种算法中的一种,每次重新选择的时候都会重置所有变量为初值

  ```c++
  //选择算法
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
        Ing.clear();
        Gua.clear();
        KongLian.clear();
        num=0;
        KongNode init = { 0,640,640 };
        KongLian.push_back(init);
  }
  ```

* add()和remove()函数把要申请的内存大小或想要回收的作业号传给相关变量,在调用FF()函数或者Recycle()函数进行作业的分配或回收.(函数主要功能就是为了传参,篇幅很长的原因是QT需要转变变量类型进行输出)

  ```c++
  //回收作业
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
       QString str_run=QString::number(Ing.size());
       QString str_gua=QString::number(Gua.size());
       QString str_num="Number of currently running jobs:"+str_run+"        Number of currently blocked jobs:"+str_gua;
       ui->listWidget->insertItem(0,str_num);
       for (int i = 0; i < KongLian.size(); i++)
       {
           QString str1=QString::number(KongLian[i].length);
           QString str2=QString::number(KongLian[i].start);
           QString str3=QString::number(KongLian[i].end);
           QString str4=QString::number(i);
           QString str="current block"+str4+"     Free block size:"+str1+"     start address:"+str2+"     end address:"+str3;
           ui->listWidget->insertItem(i,str);
       }
         ui->comboBox->clear();//先删除下拉框
       for (int i = 0; i < Ing.size(); i++)//更新删除下拉框
       {
           ui->comboBox->addItem(QString::number(Ing[i].NUM));
       }
  }
  
  //分配作业
  void MainWindow::add()
  {
      ui->listWidget->clear();//先清空
     JinCheng CurrentJinCheng;
     CurrentJinCheng.len=ui->lineEdit->text().toInt();
     CurrentJinCheng.NUM = num++;//进程号
     FF(CurrentJinCheng,0);
     QString str_run=QString::number(Ing.size());
     QString str_gua=QString::number(Gua.size());
     QString str_num="Number of currently running jobs:"+str_run+"        Number of currently blocked jobs:"+str_gua;
     ui->listWidget->insertItem(0,str_num);
     for (int i = 0; i < KongLian.size(); i++)
     {
         QString str1=QString::number(KongLian[i].length);
         QString str2=QString::number(KongLian[i].start);
         QString str3=QString::number(KongLian[i].end);
         QString str4=QString::number(i);
         QString str="current block"+str4+"     Free block size:"+str1+"     start address:"+str2+"     end address:"+str3;
         ui->listWidget->insertItem(i,str);
     }
       ui->comboBox->clear();//先删除下拉框
     for (int i = 0; i < Ing.size(); i++)//更新删除下拉框
     {
         ui->comboBox->addItem(QString::number(Ing[i].NUM));
     }
  }
  ```

### 项目小结

由于时间仓促,QT晦涩难懂,导致做出的界面不够美观.同时仅仅以列表的形式表现空闲分区连不够直观.这些均需要改进!