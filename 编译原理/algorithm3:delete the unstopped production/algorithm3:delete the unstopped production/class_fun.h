//
//  class_fun.h
//  algorithm3:delete the unstopped production
//

//

#ifndef class_fun_h
#define class_fun_h
#include <string>
#include <iostream>
#include <fstream>
#include <regex>

#define N 10 //define the size of VN and VT

class Delete_Unstoppable_Production
{
public:
    Delete_Unstoppable_Production();
    void read_the_txt(std::string file_path);//input the txt's path
    void get_VN_and_VT();//get terminating character and non_terminating character
    void delete_substring();//delete the unstoppable production
    bool is_in_VN(char big_letter);//judge the capital letter whether in the VN
    void delete_unused_character();//delete the capital letter unused
    void print();
    ~Delete_Unstoppable_Production();
private:
    std::string str_txt;//storage the txt file from the file path
    char VN[N];//storage the non-terminating character
    std::string VT[N];//storage the terminating characters
    int VN_length;//storage the length of VN(one more than actual)
    int VT_length;//storage the length if VT(one more than actual)
};



#endif /* class_fun_h */


