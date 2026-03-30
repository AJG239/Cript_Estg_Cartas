#include <iostream>
#include "../include/LargeNumber.h"

void constructore_prueba() {
    std::cout << "=== Constructores ===" << std::endl;

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
    std::cout << "=== Sumas ===" << std::endl;

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