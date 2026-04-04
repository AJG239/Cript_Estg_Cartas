#pragma once
#include "../include/LargeNumber.h"
#include <iostream>

int test_largenumber(){
    int passed = 0, failed = 0;

    auto check = [&](const std::string& nombre, bool condicion){
        if(condicion){passed++;}
        else{std::cout << " [Error] " << nombre << std::endl; failed++;}
    };

    check("Constructor vacío", LargeNumber().size() == 0);
    check("Constructor int posiitvo", LargeNumber(23)[0] == 23);
    check("Constructor int negativo", LargeNumber(-9)[0] == -9);

    // Suma con acarreo simple
    LargeNumber suma = LargeNumber::addUnsigned(LargeNumber(127), LargeNumber(581)); 
    check("127 + 581 = 708", suma[0] == 708);


    // Suma con desbordamiento
    LargeNumber max_word;
    max_word.push_back(UINT64_MAX); // 2^64 - 1
    LargeNumber number_3(1);
    LargeNumber suma_acarreo = LargeNumber::addUnsigned(max_word, LargeNumber(1));
    check("UINT64_MAX + 1: size=2", suma_acarreo.size() == 2);
    check("UINT64_MAX + 1: words[0]=0", suma_acarreo[0] == 0);
    check("UINT64_MAX + 1: words[1]=1", suma_acarreo[1] == 1);

    // Resta
    LargeNumber resta = LargeNumber::subUnsigned(LargeNumber(439), LargeNumber(327));
    check("439 - 327 = 112", resta[0] == 127);

    // Multiplicación
    LargeNumber multiplicacion = LargeNumber::mulLong(LargeNumber(98765), LargeNumber(43211));
    check("98765 * 43211 = 4267734415", multiplicacion[0] == 4267734415);

    LargeNumber fact52(1);
    for (int i = 2; i <= 52; i++) fact52.mulWord(i);
    check("52! tiene 4 palabras", fact52.size() == 4);

    // División 5125 y 4
    LargeNumber cociente, resto;
    LargeNumber::divMod(LargeNumber(21), LargeNumber(5), cociente, resto);
    check("cociente de 21/5 = 4", cociente[0] == 4);
    check("resto de 21%5 = 1", resto[0] == 1);

    LargeNumber cociente_2;
    uint64_t resto_2;
    LargeNumber::divModHalfWord(LargeNumber(35879), 7, cociente_2, resto_2);
    check("cociente de 35879/7 = 5125", cociente_2[0] == 5125);
    check("resto de 35879%7 = 4", resto_2 == 4);

    return failed;
}