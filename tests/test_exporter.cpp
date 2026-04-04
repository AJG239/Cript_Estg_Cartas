#pragma once
#include "../include/Exporter.h"
#include <iostream>

int test_exporter(){
    int passed = 0, failed = 0;

    auto check = [&](const std::string& nombre, bool condicion){
        if(condicion){passed++;}
        else{std::cout << " [Error] " << nombre << std::endl; failed++;}
    };
}