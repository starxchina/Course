//
//  Read_file.cpp
//  代码生成_递归下降_预测分析
//

//

#include "class.h"
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;

Read_File::Read_File()
{
    this->txt_file = "";
    this->in_path = "";
    this->out_path = "";
    
}

void Read_File::Read_txt()
{
    cout<<  "Please input the txt path:";
    cin >>   this->in_path;
    ifstream file(this->in_path);
    
    //判断文件是否打开成功
    if(!file.is_open())
    {
        cout<<"File cannot open,please try again"<<endl;
        exit(EXIT_FAILURE);
    }
    
    //文件已成功打开
    std::string line("");
    
    getline(file,line);
    this->start_status = line[0];   //获得开始符
    
    Grammar_normative_storage temp_for_storage; //将一条文法进行分离，并暂时存储之
    while(getline(file,line))
    {
        this->txt_file += line;
        this->txt_file += "\n";
        
        temp_for_storage.grammar_left = line[0];
        unsigned long temp_for_right = line.length() - 4;
        temp_for_storage.grammar_right = line.substr(4,temp_for_right);
        
        this->storage_grammer.push_back(temp_for_storage);
        
        
    }
    file.close();
    
    cout<<"Read the txt, and we get the grammar as follows:"<<endl;
    cout<<"The start status is \""<<this->start_status<<"\""<<endl;
    cout<<this->txt_file<<endl<<endl;
}

char Read_File::get_start_status()
{
    return this->start_status;
}

std::vector<Grammar_normative_storage> Read_File::get_storage_grammar()
{
    return this->storage_grammer;
}

Read_File::~Read_File()
{
    
}
