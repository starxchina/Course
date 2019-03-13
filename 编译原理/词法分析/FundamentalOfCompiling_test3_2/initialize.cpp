#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::Initialize(){
    syn = -1;

    exitWord[0] = "auto";
    exitWord[1] = "break";
    exitWord[2] = "case";
    exitWord[3] = "char";
    exitWord[4] = "const";
    exitWord[5] = "continue";
    exitWord[6] = "default";
    exitWord[7] = "do";
    exitWord[8] = "double";
    exitWord[9] = "else";
    exitWord[10] = "enum";
    exitWord[11] = "extern";
    exitWord[12] = "float";
    exitWord[13] = "for";
    exitWord[14] = "goto";
    exitWord[15] = "if";
    exitWord[16] = "int";
    exitWord[17] = "long";
    exitWord[18] = "register";
    exitWord[19] = "return";
    exitWord[20] = "short";
    exitWord[21] = "signed";
    exitWord[22] = "sizeof";
    exitWord[23] = "static";
    exitWord[24] = "struct";
    exitWord[25] = "switch";
    exitWord[26] = "typedef";
    exitWord[27] = "union";
    exitWord[28] = "unsigned";
    exitWord[29] = "void";
    exitWord[30] = "volatile";
    exitWord[31] = "while";

    operatorOrDelimiter[0] = "+";
    operatorOrDelimiter[1] = "-";
    operatorOrDelimiter[2] = "*";
    operatorOrDelimiter[3] = "/";
    operatorOrDelimiter[4] = "<";
    operatorOrDelimiter[5] = "<=";
    operatorOrDelimiter[6] = ">";
    operatorOrDelimiter[7] = ">=";
    operatorOrDelimiter[8] = "=";
    operatorOrDelimiter[9] = "==";
    operatorOrDelimiter[10] = "!=";
    operatorOrDelimiter[11] = ";";
    operatorOrDelimiter[12] = "(";
    operatorOrDelimiter[13] = ")";
    operatorOrDelimiter[14] = "^";
    operatorOrDelimiter[15] = ",";
    operatorOrDelimiter[16] = "\"";
    operatorOrDelimiter[17] = "\'";
    operatorOrDelimiter[18] = "#";
    operatorOrDelimiter[19] = "&";
    operatorOrDelimiter[20] = "&&";
    operatorOrDelimiter[21] = "|";
    operatorOrDelimiter[22] = "||";
    operatorOrDelimiter[23] = "%";
    operatorOrDelimiter[24] = "~";
    operatorOrDelimiter[25] = "<<";
    operatorOrDelimiter[26] = ">>";
    operatorOrDelimiter[27] = "[";
    operatorOrDelimiter[28] = "]";
    operatorOrDelimiter[29] = "{";
    operatorOrDelimiter[30] = "}";
    operatorOrDelimiter[31] = "\\";
    operatorOrDelimiter[32] = ".";
    operatorOrDelimiter[33] = "\?";
    operatorOrDelimiter[34] = ":";
    operatorOrDelimiter[35] = "!";


}
