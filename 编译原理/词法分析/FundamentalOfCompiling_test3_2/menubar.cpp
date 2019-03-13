#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_Import_file_triggered()
{   //打开文件
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Text file"), "", tr("Text Files (*.txt)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
           return;
    QTextStream in(&file);
    QString displayString;
    while (!in.atEnd())
    {
        in.setCodec("GBK");//不清楚为什么有的可以，有的不可以，windows系统此行取消注释，unix系统本行注释掉（自己测试一下）
        QString line = in.readLine();
        displayString.append(line+"\n");

    }
    ui->textEdit_1->clear();
    ui->textEdit_1->setPlainText(displayString);
    //随手关门
    file.close();
}

void MainWindow::on_actionpreprocessing_triggered()
{
    //从textEdit1中一个字符一个字符的读
    this->pProject = 0;
    //没有查到如何从textEdit中逐个字符读取，所以直接将之保存在字符串中
    QString str_text_1 = ui->textEdit_1->toPlainText();
    //将字符串中的数据读入resourceProject中
    while(str_text_1[pProject]!='$'){
        this->resourceProject[pProject] = str_text_1[pProject];
        this->pProject++;
    }
    this->resourceProject[pProject] = '$';
    this->pProject++;
    this->resourceProject[pProject] = '\0';

    this->filterResource();

    QString str_text_2;
    for(int i=0;i<pProject;i++){
        str_text_2[i] = resourceProject[i];
    }
    str_text_2[pProject] = '\0';
    ui->textEdit_2->setPlainText(str_text_2);
}

void MainWindow::filterResource(){
    QChar tempString[10000]; //暂时的字符串
    int count = 0,j;
    for (int i = 0; i <= this->pProject; i++){
        if (this->resourceProject[i] == '/' && this->resourceProject[i + 1] == '/'){
            //若为单行注释“//”,则去除注释后面的东西，直至遇到回车换行
            while (this->resourceProject[i] != '\n'){
                i++;//向后扫描
            }
         }
         if (this->resourceProject[i] == '/'&&this->resourceProject[i + 1] == '*'){
             //若为多行注释“/*....*/”则去除该内容
             j = i;
             i += 2;
             while (this->resourceProject[i] != '*' || this->resourceProject[i + 1] != '/'){
                 i++;//继续扫描
                 if (this->resourceProject[i] =='\0'){
                     ui->textEdit_2->append("\n注释出错，存在没有找到 */！！！\n");//此时对其继续进行词法分析
                     //cout << "注释出错，存在没有找到 */！！！" << endl; //此时对其继续进行词法分析
                     break;
                  }
             }
             if (this->resourceProject[i] == '\0')
                 i = j; //此时对其继续分析
             else
                 i += 2;//跨过“*/”
          }
          if (this->resourceProject[i] != '\n'&&this->resourceProject[i] != '\t'&&this->resourceProject[i] != '\v'&&this->resourceProject[i] != '\r'){
              //若出现无用字符，则过滤；否则加载
              tempString[count++] = this->resourceProject[i];
           }
     }
     tempString[count] = '\0';
     //产生净化之后的源程序,即将净化无用字符后的字符串赋值给原字符串
     for(int i=0;i <= count; i++){
         this->resourceProject[i] = tempString[i];
     }
     this->pProject = count;
}



int MainWindow::searchReserve(QString s){
    for (int i = 0; i < 32; i++)
                  {
                      if (exitWord[i].compare(s) == 0)
                          {//若成功查找，则返回种别码
                              return i + 1;//返回种别码
                          }
                 }
             return -1;//否则返回-1，代表查找不成功，即为标识符
}

void MainWindow::Scanner()
              {//根据DFA的状态转换图设计
                  int i, count = 0;//count用来做token[]的指示器，收集有用字符
                  QChar ch;//作为判断使用
                  ch = resourceProject[pProject];
                  while (ch == ' ')
                      {//过滤空格，防止程序因识别不了空格而结束
                          pProject++;
                          ch = resourceProject[pProject];//resourceProject中存储的是预处理完的序列
                      }
                 //每次收集前先清零
                 token = "";
                  if (resourceProject[pProject].isLetter())
                      {//开头为字母
                         token[count++] = resourceProject[pProject];//收集
                          pProject++;//下移
                         while (resourceProject[pProject].isLetter() || resourceProject[pProject].isDigit())
                            {//后跟字母或数字
                                token[count++] = resourceProject[pProject];//收集
                                 pProject++;//下移
                              }//多读了一个字符既是下次将要开始的指针位置
                         //token[count] = '\0';
                          syn = searchReserve(token);//查表找到种别码
                        if (syn == -1)
                              {//若不是保留字则是标识符
                                  syn = 100;//标识符种别码
                              }
                         return;
                   }
                  else if (resourceProject[pProject].isDigit() || resourceProject[pProject] == '.')
                  {//首字符为数字
                      int cnt(0);
                      if (resourceProject[pProject] == '.') cnt = 1;
                      while (resourceProject[pProject].isDigit() || resourceProject[pProject] == '.')
                      {//后跟数字或小数点
                          token[count++] = resourceProject[pProject];//收集
                          pProject++;
                          if (resourceProject[pProject] == '.') { cnt++; }//如果出现了小数点则加一
                      }//多读了一个字符既是下次将要开始的指针位置
                      if (cnt > 1) { //出现两个或多个小数点
                          //cout << "小数格式有误!!!" << endl;
                          ui->textEdit_3->append("\n小数格式有误!!!\n");
                          //因为此输入非法，该标识符需清零
                          token="";

                      }
                      else if (cnt == 1 && count == 1) //出现只有一个小数点的情况
                      {
                          //cout << "小数格式有误!!!" << endl;
                          ui->textEdit_3->append("\n小数格式有误!!!\n");
                          //因为此输入非法，该标识符需清零
                          token="";
                      }
                      else {  //正常情况
                          //token[count] = '\0';
                          syn = 99;//常数种别码
                      }
                  }
                  else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ';' || ch == '(' || ch == ')' || ch == '^'
                  || ch == ',' || ch == '\"' || ch == '\'' || ch == '~' || ch == '#' || ch == '%' || ch == '['
                 || ch == ']' || ch == '{' || ch == '}' || ch == '\\' || ch == '.' || ch == '\?' || ch == ':')
                     {//若为运算符或者界符，查表得到结果
                         token[0] = resourceProject[pProject];
                          //token[1] = '\0';//形成单字符串
                          for (i = 0; i<36; i++)
                             {//查运算符界符表
                                 if (token.compare(operatorOrDelimiter[i]) == 0)
                                     {
                                          syn = 33 + i;//获得种别码，使用了一点技巧，使之呈线性映射
                                          break;//查到即推出
                                      }
                              }
                         pProject++;//指针下移，为下一扫描做准备
                          return;
                      }
                  else  if (resourceProject[pProject] == '<')
                      {//<,<=,<<
                        pProject++;//后移，超前搜索
                          if (resourceProject[pProject] == '=')
                             {
                                  syn = 38;
                              }
                          else if (resourceProject[pProject] == '<')
                              {//左移
                                  pProject--;
                                  syn = 58;
                              }
                         else
                              {
                                  pProject--;
                                  syn = 37;
                             }
                          pProject++;//指针下移
                         return;
                      }
                 else  if (resourceProject[pProject] == '>')
                      {//>,>=,>>
                         pProject++;
                       if (resourceProject[pProject] == '=')
                            {
                                 syn = 40; //>=符号
                              }
                          else if (resourceProject[pProject] == '>')
                              {
                                  syn = 59; //右移符号
                              }
                          else
                             {
                                  pProject--;
                                  syn = 39; //只是>符号
                             }
                          pProject++;
                          return;
                      }
                   else  if (resourceProject[pProject] == '=')
                     {//=.==
                        pProject++;
                         if (resourceProject[pProject] == '=') //==号
                              {
                                  syn = 42;
                              }
                          else
                              {
                                  pProject--;
                                  syn = 41;  //赋值号
                              }
                          pProject++;
                          return;
                      }
                  else  if (resourceProject[pProject] == '!')
                      {//!,!=
                         pProject++;
                         if (resourceProject[pProject] == '=')
                              {
                                syn = 43;//！=号
                              }
                          else
                              {
                                  syn = 68;
                                  pProject--; //!号
                             }
                          pProject++;
                          return;
                      }
                  else  if (resourceProject[pProject] == '&')
                     {//&,&&
                         pProject++;
                          if (resourceProject[pProject] == '&')
                              {
                                  syn = 53;
                              }
                          else
                              {
                                  pProject--;
                                  syn = 52;
                              }
                         pProject++;
                          return;
                      }
                 else  if (resourceProject[pProject] == '|')
                     {//|,||
                           pProject++;
                          if (resourceProject[pProject] == '|') //|
                             {
                                  syn = 55;
                              }
                          else
                              {
                                  pProject--;  // ||号
                                  syn = 54;
                              }
                          pProject++;
                          return;
                      }
                 else  if (resourceProject[pProject] == '$')
                     {//结束符
                          syn = 0;//种别码为0
                     }
                  else
                     {//不能被以上词法分析识别，则出错。
                      ui->textEdit_3->append("\nerror:无法识别\n");
                      //cout << "error:无法识别" << endl;
                      syn = 0;
                      resourceProject[pProject] = '$';
                      return ;
                      }
              }


void MainWindow::on_actiontokeb_sequence_triggered()
{
    Initialize();
    this->pProject = 0;
    while(this->syn != 0){
        //启动扫描
        Scanner();
        if (this->syn == 100)
                         {//标识符
                             for (int i = 0; i < 1000; i++)
                             {//插入标识符表中
                                 if (IDentifierTbl[i].compare(token) == 0)
                                 {//已在表中
                                     break;
                                 }
                                 if (IDentifierTbl[i].isNull())
                                 {//查找空间
                                     IDentifierTbl[i] = token;
                                     //strcpy(IDentifierTbl[i], token);
                                     break;
                                 }
                             }
                             //cout << "(标识符：" << token << ")" << endl;
                             //qDebug()<<"(标识符：" << token << ")" ;
                             ui->textEdit_3->append("(标识符:"+token+")\n");

                         }
          else if (syn >= 1 && syn <= 32)
                         {//保留字
                             //cout << "(" << exitWord[syn - 1] << "--)" << endl;
                             //qDebug()<< "(" << exitWord[syn - 1] << "--)" ;
                             ui->textEdit_3->append("("+exitWord[syn - 1]+"--)\n");

                         }
          else if (syn == 99)
                         {//const 常数
                             //cout << "(常数 ," << token << ")" << endl;
                             //qDebug()<<"(常数 ," << token << ")";
                             ui->textEdit_3->append("(常数，"+token+")\n");

                         }
          else if (syn >= 33 && syn <= 68)
                         {
                             //cout << "(" << operatorOrDelimiter[syn - 33] << ",   --)" << endl;
                             //qDebug()<<"(" << operatorOrDelimiter[syn - 33] << ",   --)" ;
                             ui->textEdit_3->append("("+operatorOrDelimiter[syn - 33]+",  --)\n");
                         }
    }
    for (int i = 0; i < 100; i++)
                    {//插入标识符表中
                        //cout << "(第" << i + 1 << "个标识符：," << IDentifierTbl[i] <<")"<< endl;
                        qDebug()<<"(第" << i + 1 << "个标识符：," << IDentifierTbl[i] <<")";
                        ui->textEdit_3->append("(第" + QString::number(i+1,10) + "个标识符：," + IDentifierTbl[i] +")\n");
                    }
    QString filename="/Users/legendary/Desktop/Data.txt";
    QFile file( filename );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << ui->textEdit_3->toPlainText() << endl;
    }
}
