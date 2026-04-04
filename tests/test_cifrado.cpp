#pragma once
#include "../include/Cifrados.h"
#include <iostream>

int test_cifrado(){
    int passed = 0, failed = 0;

    auto check = [&](const std::string& nombre, bool condicion){
        if(condicion){passed++;}
        else{std::cout << " [Error] " << nombre << std::endl; failed++;}
    };
}