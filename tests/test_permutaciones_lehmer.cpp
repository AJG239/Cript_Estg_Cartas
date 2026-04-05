#pragma once
#include "../include/Permutacion_Lehmer.h"
#include <iostream>

int test_permutation_lehmer(){
    int passed = 0, failed = 0;

    auto check = [&](const std::string& nombre, bool condicion){
        if(condicion){passed++;}
        else{std::cout << " [Error] " << nombre << std::endl; failed++;}
    };

    // Número a permutación
    auto perm46 = Permutacion_Lehmer::NumeroAPermutacion(LargeNumber(46), 5);
    check("46 = [1,4,3,0,2]", perm46 == std::vector<int>({1,4,3,0,2}));

    auto perm23 = Permutacion_Lehmer::NumeroAPermutacion(LargeNumber(23), 4);
    check("23 = [3,2,1,0]", perm23 == std::vector<int>({3,2,1,0}));

    // Permutación a número
    auto perm17 = {0,3,4,2,1};
    LargeNumber num_perm17 = Permutacion_Lehmer::PermutacionANumero(perm17);
    check("{0,3,4,2,1} = 17", LargeNumber::comparar_numeros(num_perm17, LargeNumber(17)) == 0);

    auto perm239 = {1,5,4,3,2,0};
    LargeNumber num_perm239 = Permutacion_Lehmer::PermutacionANumero(perm239);
    check("{1,5,4,3,2,0} = 239", LargeNumber::comparar_numeros(num_perm239, LargeNumber(239)) == 0);

    // Comprobar permutación
    check("[2,0,3,1] valida", Permutacion_Lehmer::PermutacioonValida({2,0,3,1}));
    check("[1,0,1,3] invalida", !Permutacion_Lehmer::PermutacioonValida({1,0,1,3}));

    return failed;
}