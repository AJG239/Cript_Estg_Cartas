#include <iostream>
#include "../include/LargeNumber.h"

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