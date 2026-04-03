#pragma once
#include <iostream>
#include "../include/LargeNumber.h"
#include "../include/Factoradico.h"
#include "../include/Permutacion_Lehmer.h"
#include "../include/Transformer.h"

void constructore_prueba() {
    std::cout << "Prueba de Constructores" << std::endl;

    // BigNum vacío = 0
    LargeNumber cero;
    std::cout << "cero: size=" << cero.size() << ", neg=" << cero.neg << std::endl;
    // Esperado: size=0, neg=0

    // BigNum desde entero positivo
    LargeNumber a(42);
    std::cout << "a(42): words[0]=" << a[0] << ", size=" << a.size() << std::endl;
    // Esperado: words[0]=42, size=1

    // BigNum desde entero negativo
    LargeNumber b(-7);
    std::cout << "b(-7): words[0]=" << b[0] << ", neg=" << b.neg << std::endl;
    // Esperado: words[0]=7, neg=1

    // Copia
    LargeNumber c(a);
    std::cout << "c(copia de a): words[0]=" << c[0] << std::endl;
    // Esperado: words[0]=42

    // Truncate
    LargeNumber d;
    d.push_back(42);
    d.push_back(0);
    d.push_back(0);
    std::cout << "d antes de truncate: size=" << d.size() << std::endl;  // 3
    d.truncate();
    std::cout << "d despues de truncate: size=" << d.size() << std::endl; // 1
    std::cout << "d[0] = " << d[0] << std::endl; // 42
    // Esperado: size=1, d[0]=42
}

void suma_pruebas(){
    std::cout << "Prueba de funciones:Sumas" << std::endl;

    // Suma con acarreo simple
    LargeNumber number_1(100);
    LargeNumber number_2(200);

    LargeNumber suma = LargeNumber::addUnsigned(number_1, number_2);

    std::cout << "100 + 200 = " << suma[0] << std::endl;

    // Suma con desbordamiento, añadimos el máximo número de bits disponibles
    LargeNumber max_word;

    max_word.push_back(UINT64_MAX); // 2^64 - 1

    LargeNumber number_3(1);

    LargeNumber suma_max = LargeNumber::addUnsigned(max_word, number_3);

    std::cout << suma_max[0] << ";" << suma_max[1] << std::endl;
}

void resta_pruebas(){
    std::cout << "Prueba de funciones: Resta" << std::endl;

    // Resta simple con préstamo
    LargeNumber number_1(500);
    LargeNumber number_2(260);

    LargeNumber resta = LargeNumber::subUnsigned(number_1, number_2);

    std::cout << "500 - 260 = " << resta[0] << std::endl;

    // Resta de varias palabras
    LargeNumber number_3;
   
    number_3.push_back(4);
    number_3.push_back(16);

    LargeNumber number_4(7);

    LargeNumber resta_multi = LargeNumber::subUnsigned(number_3, number_4);

    std::cout << "resultado=" << resta_multi[0] << "; prestamo=" << resta_multi[1] << std::endl;

}

void multiplicacion_prueba(){
    // Multipliación simple
    LargeNumber numero_1(14);
    numero_1.mulWord(456);
    std::cout << numero_1[0] << std::endl;

    // Probando multiplicar un numero muy grande
    LargeNumber factorial_20(1);

    for(int i = 2; i <= 20; i++){
        factorial_20.mulWord(i);
    }

    std::cout << factorial_20[0] << std::endl;

    // Multiplicar dos palabras
    LargeNumber numero_2(98765);
    LargeNumber numero_3(43211);

    LargeNumber result = LargeNumber::mulLong(numero_2, numero_3);

    std::cout << "LargeNumber result:" << result[0] << std::endl;

}

void division_prueba(){
    // División simple
    LargeNumber cociente, resto;
    LargeNumber::divMod(LargeNumber(21), LargeNumber(5), cociente, resto);

    std::cout << "21 / 5 = " << cociente[0] << " resto " << resto[0] << std::endl;

    // División de numero pequeños
    LargeNumber cociente_2;
    uint64_t resto_2;
    LargeNumber::divModHalfWord(LargeNumber(35879), 7, cociente_2, resto_2);
    std::cout << "35879 / 7 = " << cociente_2[0] << " resto " << resto_2 << std::endl;
}

void pruebas_string(){
    LargeNumber factorial_10(1);

    for (int i = 2; i <= 10; i++){
        factorial_10.mulWord(i);
    }

    std::cout << "10! = " << factorial_10 << std::endl;

    LargeNumber cociente, resto;
    LargeNumber::divMod(LargeNumber(17), LargeNumber(5), cociente, resto);
    std::cout << "123 / 6 = " << cociente[0] << " resto " << resto[0] << std::endl;

    std::cout << "100 + 200 = " << LargeNumber::addUnsigned(LargeNumber(100), LargeNumber(200)) << std::endl;
}

void carga_cache(){
    std::cout << "10! = " << Factoradico::factorial(10) << std::endl;

    std::cout << "52! = " << Factoradico::factorial(52) << std::endl;
}

void numero_a_factoradico(){
    std::vector<LargeNumber> f17 = Factoradico::NumeroAFactoradico(LargeNumber(17));
    std::cout << "17 en factoradic: [";
    for (size_t i = 0; i < f17.size(); i++) {
        if (i > 0) std::cout << ", ";
        std::cout << f17[i];
    }
    std::cout << "]" << std::endl;
}

void factoradico_a_numero(){
    std::vector<LargeNumber> f = {LargeNumber(2), LargeNumber(2), LargeNumber(1), LargeNumber(0)};
    LargeNumber num = Factoradico::FactoradicoANumero(f);
    std::cout << "Factoradic [2, 2, 1, 0] a numero: " << num << std::endl;
}


void numero_permutacion(){
    std::vector<int> permLast = Permutacion_Lehmer::NumeroAPermutacion(LargeNumber(46), 5);
    std::cout << "46 con 5 elementos: ";
    Permutacion_Lehmer::printPermutation(permLast);
    std::cout << std::endl;
}


void permutacion_a_numero(){
    std::vector<int> p17 = {0, 3, 4, 2, 1};
    LargeNumber dec_p17 = Permutacion_Lehmer::PermutacionANumero(p17);
    std::cout << "[0,3,4,2,1] -> " << dec_p17 << std::endl;
}

void probar_transformer(){
    std::cout << "Indice de h: " << Transformer::caracterAIndice('h') << std::endl;

    std::cout << "Caracter 23: " << Transformer::indiceACaracter(23) << std::endl;

    std::cout << "Comprobacion de texto con palabra (lince): " << Transformer::texto_valido("lince") << std::endl;

    std::cout << "Limpieza de texto (Me GusTan LAS RosQuilLas.): " << Transformer::limpiar_texto("Me GusTan LAS RosQuilLas.") << std::endl;
}