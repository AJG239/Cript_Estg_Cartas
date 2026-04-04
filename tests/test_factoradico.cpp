#pragma once
#include "../include/Factoradico.h"
#include <iostream>

int test_factoradico(){
    int passed = 0, failed = 0;

    auto check = [&](const std::string& nombre, bool condicion){
        if(condicion){passed++;}
        else{std::cout << " [Error] " << nombre << std::endl; failed++;}
    };

    // Cache de factoriales
    check("0! = 1", LargeNumber::comparar_numeros(Factoradico::factorial(0), LargeNumber(1)));
    check("5! = 120", LargeNumber::comparar_numeros(Factoradico::factorial(5), LargeNumber(120))); 
    
    // Número a factorádico
    auto f17 = Factoradico::NumeroAFactoradico(LargeNumber(17));
    check("17 fact: size=4", f17.size() == 4);
    check("17 fact: [0]=2", LargeNumber::comparar_numeros(f17[0], LargeNumber(2)));
    check("17 fact: [1]=2", LargeNumber::comparar_numeros(f17[1], LargeNumber(2)));
    check("17 fact: [2]=1", LargeNumber::comparar_numeros(f17[2], LargeNumber(1)));
    check("17 fact: [3]=0", LargeNumber::comparar_numeros(f17[3], LargeNumber(0)));

    // Factorádico a número
    std::vector<LargeNumber> f = {LargeNumber(2), LargeNumber(2), LargeNumber(1), LargeNumber(0)};
    check("[2, 2, 1, 0] = 17", LargeNumber::comparar_numeros(Factoradico::FactoradicoANumero(f), LargeNumber(17)));

    // Añadir ceros
    std::vector<LargeNumber> small = {LargeNumber(1), LargeNumber(0)};
    Factoradico::add_cerosFactoradico(small, 5);
    check("Pad a 5: size=5", small.size() == 5);
    check("Pad no cambia valor", LargeNumber::comparar_numeros(Factoradico::FactoradicoANumero(small), LargeNumber(1)));

    return failed;
}
