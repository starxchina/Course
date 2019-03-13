//
//  class.h
//  代码生成_递归下降_预测分析
//

//

#ifndef class_h
#define class_h

#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <stack>
#include <ctype.h>
#include <algorithm>


//文法的存储方式
typedef struct
{
    char grammar_left;  //文法的左部
    std::string grammar_right;  //文法的右部
    
}Grammar_normative_storage;

//first集、follow集的存储方式
typedef struct
{
    char non_terminal_symbol;   //非终结符
    std::string first_set;  //first集合
    std::string follow_set; //follow集合
    
}First_Follow_storage;


//读取文件
class Read_File
{
private:
    std::string txt_file;   //存储文件内容
    std::string in_path;    //存储读取的文件路径
    std::string out_path;   //存储输出文件的路径
    char start_status;      //记录开始符
    std::vector <Grammar_normative_storage> storage_grammer;  //存储规范化的文法
public:
    Read_File();
    void  Read_txt();    //读取文件内容，并返回文件内容到被继承的类中进行赋值
//    void  Write_txt(std::string the_txt_to_write);   //将输出的内容写入文件
    void Grammar_Normative_Storage();   //将文法左部和右部分开存储
    char get_start_status(); //返回开始符
    std::vector<Grammar_normative_storage> get_storage_grammar();   //返回文法的规范存储
    ~Read_File();
};


//求的first，follow，select集合
class First_Follow_Select
{
private:
    std::vector <Grammar_normative_storage> grammar;    //文法
    char start_status;  //开始符
    std::string Vn; //存储非终结符
    std::string Vt; //存储终结符
    std::string unused_status_pool; //无用状态池，供文法在提取左公因子和消除左递归时添加状态
    std::vector <First_Follow_storage> first_follow_set;    ////这是一张表，用于输出显示和存储求解得到的first和follow集
public:
    First_Follow_Select(char start_status,    std::vector<Grammar_normative_storage>grammar);
    void process(); //判断步骤
    void generate_Vn(std::string &str_vn , std::vector <Grammar_normative_storage> grammar_get_vn); //获得Vn
    void generate_Vt(std::string &str_vt , std::vector <Grammar_normative_storage> grammar_get_vt); //获得Vt
    void generate_unused_status_pool(); //生成无用状态池
    void judge_left_common_factor(); //判断是否含有左公因子
    void remove_left_common_factor();   //消除左公因子
    void judge_non_terminal_symbol_get_eplison();   //判断哪些非终结符会推出空
    void str_A_minus_str_B(std::string &str_A, std::string str_B);   //将字符串A中和B中同时有的字符，在A中删去，最后返回A
    bool is_ch_in_string(char ch, std::string str); //判断ch是否在str中
    size_t pos_ch_in_string(char ch , std::string str);    //返回ch在str中的位置
    void judge_left_recursion();    //判断是否含有左递归
    void sort_of_grammar(std::vector<Grammar_normative_storage> &grammar_to_sort);     //对文法进行排序
    void remove_direct_left_recursion(int pos);   //消除直接左递归  i是文法中存在直接左递归的序号
    void print_grammar(std::vector<Grammar_normative_storage> grammar_to_print);   //输出文法
    void print_first_follow_set(std::vector<First_Follow_storage> vec); //输出first follow 表
    void generate_first();   //查找first集
    void set_merge(std::string &str_A,std::string str_B);   //合并A和B集合，最终返回A集合
    void delete_specific_ch(char ch , std::string &str);    //删除str中的ch字符
    void generate_follow();  //查找follow集
    void generate_select();  //查找select集
    char return_start_status();     //返回开始福
    std::vector<Grammar_normative_storage> return_grammar();        //返回文法
    std::vector<First_Follow_storage> return_first_folow_set();     //返回first_follow集
    std::string return_Vn();    //返回Vn
    std::string return_Vt();    //返回Vt
    ~First_Follow_Select();
};


class Recursion_Code
{
private:
    char start_status;  //开始符
    std::vector<Grammar_normative_storage>grammar;  //文法
    std::vector<First_Follow_storage>first_follow_set;  //first_follow表
    std::string Vn; //Vn
public:
    Recursion_Code(char start_status,   std::vector<Grammar_normative_storage>grammar,   std::vector<First_Follow_storage>first_follow_set,  std::string Vn);
    void code_creat();  //  代码生成
    void add_first_content(std::ofstream &file,std::string Vn); //添加非主要头部内容
    void add_main_content(std::ofstream &file,char start_status);   //添加main函数内容
    void add_body_func(std::ofstream &file,  std::string vn_set, std::vector<First_Follow_storage> f_f_s, std::vector<Grammar_normative_storage> temp_grammar);  //函数主体，生成各个函数
    bool is_ch_in_string(char ch, std::string str); //  ch是否在str中
    void set_merge(std::string &str_A,std::string str_B);   //集合合并
    void make_or_struct(std::string &str1 , std::string str2);      //生成if中的或结构
    void inner_bracket_content(std::ofstream &file ,std::string production_first_set);  //生成方括号中的内容
    ~Recursion_Code();
};

class Prediction_analysis
{
private:
    std::vector<Grammar_normative_storage>grammar;  //文法
    std::vector<First_Follow_storage>first_follow_set;  //first_follow表
    std::string Vn; //Vn
    std::string Vt;//Vt
    std::vector< std::vector<std::string> > prediction_table;//存储预测分析表
    char start_status;    //开始符
public:
    Prediction_analysis(std::vector<Grammar_normative_storage>grammar,std::vector<First_Follow_storage>first_follow_set,std::string Vn,std::string Vt,char start_status);
    void create_table();    //生成预测分析表
    std::string get_production_first_set(std::string str_right,    std::string vn_set ,std::vector<First_Follow_storage> f_f_s);    //找到此条产生式的first集合
    void set_merge(std::string &str_A,std::string str_B);   //合并A和B
    bool is_ch_in_string(char ch, std::string str); //ch是否在str中
    void print_prediction_table(std::vector< std::vector<std::string> > prediction_table , std::string Vn, std::string Vt);  //  输出预测分析表
    void deal_with_input_expression();    //处理表达式
    ~Prediction_analysis();
};

#endif /* class_h */
