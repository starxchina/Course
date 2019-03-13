//
//  main.cpp
//  类C转C
//

//

#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <regex>


typedef struct{
    std::string var_name;//variable's name
    std::string var_type;//variable's type
    std::string var_value;//variable's value
}Storage_variable;

Storage_variable var_array[100];//storage the variable list
int var_name_count = -1;//record the array_name's subscript
int var_type_count = -1;//record the array_type's subscript
int var_value_count = -1;//record the array_type's subscript
std::string code_txt;//read the code in
std::ifstream file_txt;
std::ofstream file_save;

void Init()
{//initialize
    std::string str_line;//read the txt line by line
    file_txt.open("/Users/legendary/Desktop/test_c.txt");
    if(!file_txt)
    {
        std::cout<<"cannot open the file"<<std::endl;
        exit(0);
    }
    while(getline(file_txt, str_line))
    {
        code_txt.append(str_line + "\n");
    }
    file_txt.close();
}

int str_into_array(std::string str)
{//storage every variable into var_array
    int count_str=0;
    std::regex str_regex_expr("([_A-Za-z]{1}[\\w]*|([0-9\\.]+))");
    std::string::const_iterator iter_begin = str.begin();
    std::string::const_iterator iter_end = str.end();
    std::smatch match_result;
    while(std::regex_search(iter_begin, iter_end, match_result, str_regex_expr))
    {
        //std::cout << "\nMatch: " << match_result[0];
        var_name_count++;
        var_array[var_name_count].var_name = match_result[0];
        count_str++;
        iter_begin = match_result[0].second; //更新搜索起始位置
    }
    std::string str_type;
    if(!atof(var_array[var_name_count].var_name.c_str()))
    {//is string
        str_type = "string";
    }
    else
    {//not string
        if(atoi(var_array[var_name_count].var_name.c_str()) < atof(var_array[var_name_count].var_name.c_str()))
        {//is the float
            str_type = "float";
        }
        else{
            //int
            str_type = "int";
        }
    }
    
    for(int i=0;i<count_str-1;i++)
    {
        var_type_count++;
        var_array[var_type_count].var_type = str_type;
        var_value_count++;
        var_array[var_value_count].var_value = var_array[var_name_count].var_name;
        
    }
    var_name_count--;
    return count_str-2;
}

void find_series_assignment()
{
    std::regex find_regex_expr("([_A-Za-z]{1}[\\w]*=)([_A-Za-z]{1}[\\w]*=)+((\"[\\w]+\")|([0-9\\.]+));"); //regular expression   only string and int/float/double
    std::string::const_iterator iter_begin = code_txt.begin();
    std::string::const_iterator iter_end = code_txt.end();
    std::smatch match_result;
    while(std::regex_search(iter_begin, iter_end, match_result, find_regex_expr))
    {
        //std::cout << "\nMatch: " << match_result[0];
        int record_num = str_into_array(match_result[0]);//record the number of identifier
        //transform
        std::string str_exchange;
        for(int i=var_name_count - record_num; i <= var_name_count;i++)
        {
            if(var_array[i].var_type=="string")
            {
            str_exchange.append(var_array[i].var_type+" "+var_array[i].var_name+"="+"\""+var_array[i].var_value+"\""+";");
            }
            else
            {
                str_exchange.append(var_array[i].var_type+" "+var_array[i].var_name+"="+var_array[i].var_value+";");
            }
        }
        std::string str_t;
        std::string oppo = match_result[0];//临时变量
        std::regex find_string(oppo);
        std::regex_replace(std::back_inserter(str_t), code_txt.begin(), code_txt.end(), find_string, str_exchange);
        code_txt = str_t;
        std::cout<<code_txt<<std::endl;
        iter_begin = match_result[0].second; //更新搜索起始位置
        
    }

}

void find_a_swap_b()
{
    std::regex find_regex_expr ("([_A-Za-z]{1}[\\w]*)(\\<\\-\\-\\>)([_A-Za-z]{1}[\\w]*)");
    std::string fmt("swap($1,$3)");
    code_txt = std::regex_replace(code_txt,find_regex_expr,fmt);
    //std::cout<<code_txt;
}

void find_array_assignment()
{//n[0...5]=m[1...6];
    std::regex find_regex_expr ("([_A-Za-z]{1}[\\w]*)\\[(\\d+)\\.\\.\\.(\\d+)\\]=([_A-Za-z]{1}[\\w]*)\\[(\\d+)\\.\\.\\.(\\d+)\\];");//([_A-Za-z]{1}[\\w]*)(\\[)(\\d+)(\\.\\.\\.)(\\d+)(\\])(=)([_A-Za-z]{1}[\\w]*)(\\[)(\\d+)(\\.\\.\\.)(\\d+)(\\])
    std::string fmt("for(int i=$2,j=$5;i<=$3;i++){$1[i]=$4[j];}");
    code_txt = std::regex_replace(code_txt,find_regex_expr,fmt);
    //std::cout<<code_txt;
}

void regular_expression()
{//sequence exchange
    //a = b = c = "abc";
    find_series_assignment();
    //a<-->b;
    find_a_swap_b();
    //n[0...5]=m[1...6];
    find_array_assignment();
}

void save(){
    file_save.open("/Users/legendary/Desktop/test_c_save.txt");
    file_save<<code_txt;
    file_save.close();
}

int main(int argc, const char * argv[])
{
    Init();
    std::cout<<code_txt;
    regular_expression();
    std::cout<<std::endl<<std::endl<<code_txt<<std::endl;
    save();
    return 0;
}
