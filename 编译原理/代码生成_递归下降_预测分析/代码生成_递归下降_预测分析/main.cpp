//
//  main.cpp
//  代码生成_递归下降_预测分析
//

//

#include "class.h"

//流程的全过程
void process()
{
    /*
     1、读取文件
     2、判断是否为LL（1）文法
     3、是，则进行递归下降子程序的生成和预测分析表的生成
        不是，则进行非LL（1）到LL（1）的转换
     */
    Read_File read_file;
    read_file.Read_txt();   //读取文件，并将文法规范化存储
    First_Follow_Select transform_to_ll1(read_file.get_start_status(),read_file.get_storage_grammar());
    transform_to_ll1.process();
    Recursion_Code subcode(transform_to_ll1.return_start_status(),transform_to_ll1.return_grammar(),transform_to_ll1.return_first_folow_set(),transform_to_ll1.return_Vn());//递归下降子程序
    subcode.code_creat();
    
    Prediction_analysis prediction_analysis_table(transform_to_ll1.return_grammar(),transform_to_ll1.return_first_folow_set(),transform_to_ll1.return_Vn(),transform_to_ll1.return_Vt(),transform_to_ll1.return_start_status());
    prediction_analysis_table.create_table();
    prediction_analysis_table.deal_with_input_expression(); //  输入表达式
    getchar();
    
}

int main(int argc, const char * argv[]) {
    process();
    return 0;
}
