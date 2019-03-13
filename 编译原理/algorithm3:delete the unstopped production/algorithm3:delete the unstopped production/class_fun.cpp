//
//  class_fun.cpp
//  algorithm3:delete the unstopped production
//
//

#include <stdio.h>
#include "class_fun.h"
Delete_Unstoppable_Production::Delete_Unstoppable_Production()
{
    str_txt = "";
}

void Delete_Unstoppable_Production::read_the_txt(std::string file_path)
{
    //read the txt
    std::ifstream file_txt;
    std::string str_line;
    file_txt.open(file_path);
    if(!file_txt)
    {
        std::cout<<"Cannot open the file"<<std::endl;
        exit(0);
    }
    while(getline(file_txt,str_line))
    {
        this->str_txt.append(str_line + "\n");
    }
    file_txt.close();
    //std::cout<<str_txt;
}

void Delete_Unstoppable_Production::get_VN_and_VT()
{
    int count_VN = -1;//record the array subscript of VN
    int count_VT = -1;//record the array subscript of VT
    //VN[0] and VT[0] record its length
    char big_letter;//record the non-terminating charcter temporarily
    //build VN and VT
    for(int i=0; (i< this->str_txt.length()) && this->str_txt[i] != '\n';i++)
    {
        //judge  the non-terminating character
        big_letter = this->str_txt[i];
        i = i + 2;//jump the "->"
        while(true)
        {
            if(this->str_txt[i] == '\n')
            {
                break;
            }
            i++;
        }
        //now the i point at '\n'
        //go back to get terminating characters
        bool no_upper = true;
        int j=i-1;
        for(; this->str_txt[j] != '>' && this->str_txt[j] != '|'; j--)
        {
            if(isupper(this->str_txt[j]))
            {
                no_upper = false;
                break;
            }
        }
        if(no_upper)
        {
            j++;
            this->VN[++count_VN] = big_letter;
            this->VT[++count_VT] = this->str_txt.substr(j,i-j);
        }
    }
    this->VN[++count_VN] = 'S';//in order to delete substring conveniently
    
    this->VN_length = count_VN+1;
    this->VT_length = count_VT+1;
}

void Delete_Unstoppable_Production::delete_substring()
{
    int count_start = 0;
    int count_end = 0;
    std::string new_str_txt;
    while(count_start < this->str_txt.length())
    {
        if(is_in_VN(this->str_txt[count_start]))
        {//in the VN
            while(this->str_txt[count_end] != '\n')
                ++count_end;
            new_str_txt.append(this->str_txt.substr(count_start,count_end - count_start +1));
        }
        else
        {
            while(this->str_txt[count_end] != '\n')
                ++count_end;
        }
        
        count_start = ++count_end;
    }
    this->str_txt = new_str_txt;
}

void Delete_Unstoppable_Production::delete_unused_character()
{
    //std::cout<<str_txt;
    std::string new_str_txt_d = "";
    char record;
    for(int i=0; i<this->str_txt.length(); i++)
    {
        record = this->str_txt[i];
        if(isupper(record))
        {
            if(is_in_VN(record))
            {
                new_str_txt_d += record;
            }
            
        }
        else
        {
            new_str_txt_d += record;
        }
    }
    this->str_txt = new_str_txt_d;
}

bool Delete_Unstoppable_Production::is_in_VN(char big_letter)
{
    for(int i=0; i< VN_length ; i++)
    {
        if(big_letter == VN[i])
            return true;
    }
    return false;
}
void Delete_Unstoppable_Production::print()
{
    std::cout<<str_txt;
}

Delete_Unstoppable_Production::~Delete_Unstoppable_Production()
{
    
}
