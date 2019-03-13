//
//  main.cpp
//  algorithm3:delete the unstopped production
//

//

#include "class_fun.h"

int main(int argc, const char * argv[]) {
    Delete_Unstoppable_Production test;
    test.read_the_txt("/Users/legendary/Desktop/delete_production.txt");
    test.get_VN_and_VT();
    test.delete_substring();
    test.delete_unused_character();
    test.print();
    return 0;
}
