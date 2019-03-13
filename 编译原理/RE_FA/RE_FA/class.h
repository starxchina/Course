//
//  class.h
//  RE_FA
//

//

#ifndef class_h
#define class_h
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <stack>
#include <algorithm>

//use in RE2FA
typedef struct node_re2fa
{
    char letter_re;    //record the mark in the RE
    char front_status; //record the head of a connect
    char final_status; //record the tail of a connect
    struct node_re2fa *next;
    
}NODE;

//read the txt file
class READTXT
{
private:
    std::string txt_content;  //content of the file
public:
    READTXT();
    void getTxtContent();
    std::string getContent();
    std::string getErasedContent();
    ~READTXT();
};




//RE-->FA
class RE2FA:private READTXT
{
private:
    READTXT file;
    std::string txt_file;
    std::string fa_connect; //storage the connect of FA
    NODE *header = new NODE; //record the node marked in the RE
    NODE *tail;
    char mark_letter; //mark in the RE
    char status;    //record the status in the FA
public:
    RE2FA();
    void transform();   //tramsform RE to FA
    void deal_loop(int pos);    //deal with the form of "_*"
    void deal_parentheses(int front,int last);  //deal with the parentheses
    void deal_or(int &start,int &last);   //deal with "|"
    void deal_series(int &start,int &last);//deal with series of expression which do not have "|" and "*"
    void getfa_con(){std::cout<<this->fa_connect;}
    ~RE2FA();
    
};







//FA-->RE
class FA2RE:private READTXT
{
private:
    READTXT file;
    std::string txt_file;
    char    mark_letter;
    std::stack <std::string>   stk_lower;
    std::string str_RE;
public:
    FA2RE();
    void transform();
    void deal_loop();
    void deal_or();
    std::string deal_series_in_or(char start_status,   std::string final_set  ,char &end_status);
    bool is_in_it(char ch,  std::string str);
    void deal_final_series();
    void deal_stack_lower();
    void output();
    ~FA2RE();
};


#endif /* class_h */
