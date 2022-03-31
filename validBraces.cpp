// validBraces.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <list>
#include <iostream>

bool valid_braces(std::string braces);

int main()
{
    std::string ex1 = "(){}[]";
    std::string ex2 = "(){}[]";
    std::string ex3 = "(){}[]";
    std::string ex4 = "(}";
    std::string ex5 = "[(])";
    std::string ex6 = "[({})](]";

    std::cout << valid_braces(ex1) << std::endl;
    std::cout << valid_braces(ex2) << std::endl;
    std::cout << valid_braces(ex3) << std::endl;
    std::cout << valid_braces(ex4) << std::endl;
    std::cout << valid_braces(ex5) << std::endl;
    std::cout << valid_braces(ex6) << std::endl;
}


bool valid_braces(std::string braces)
{
    char temp;
    std::list<char> bracesStack;
    for (int i = 0; i < braces.size(); i++) {
        if (braces[i] != '(' and braces[i] != '{' and braces[i] != '[') {
            if (!bracesStack.empty()) {
                switch (braces[i]) {
                case ')':
                    temp = bracesStack.front();
                    if (temp == '(') {
                        bracesStack.pop_front();
                        break;
                    }
                case '}':
                    temp = bracesStack.front();
                    if (temp == '{') {
                        bracesStack.pop_front();
                        break;
                    }

                case ']':
                    temp = bracesStack.front();
                    if (temp == '[') {
                        bracesStack.pop_front();
                        break;
                    }
                default:
                    return false;
                }
            }
            else { 
                return false;
            } 
        }
        else {
            bracesStack.push_front(braces[i]);
        }
    }
    if(!bracesStack.empty()){
        return false;
    }
    return true;
}


