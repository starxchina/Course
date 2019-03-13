//
//  fun_READTXT.cpp
//  RE_FA
//

//

#include "class.h"
#include <iostream>

READTXT::READTXT()
{
    txt_content = "";
}

void READTXT::getTxtContent()
{
    std::string txt_path;
    std::cout<<"Please input the path of the file:";
    std::cin>>txt_path;
    std::ifstream file(txt_path);
    if(!file.is_open())
    {
        std::cout<<"file cannot open"<<std::endl;
        exit(EXIT_FAILURE);
    }
    std::string line;
    while(getline(file,line))
    {
        this->txt_content += line;
    }
    file.close();
}

std::string READTXT::getContent()
{
    return this->txt_content;
}

std::string READTXT::getErasedContent()
{
    std::string str_A$B  =   "";    //to record the converted value
    int temp_length = static_cast<int>(this->txt_content.length());
    temp_length = temp_length / 8;  //A--$-->B
    for(int i=0;    i<temp_length;  i++)
    {
        str_A$B += this->txt_content[i*8];   //A
        str_A$B += this->txt_content[i*8+3];  //$
        str_A$B += this->txt_content[i*8+7]; //B
    }
    return str_A$B;
}

READTXT::~READTXT()
{
    
}
