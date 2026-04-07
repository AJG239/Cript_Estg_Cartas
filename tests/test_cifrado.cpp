#pragma once
#include "../include/Cifrados.h"
#include "../include/Cartas.h"
#include <iostream>

int test_cifrado(){
    int passed = 0, failed = 0;

    auto check = [&](const std::string& nombre, bool condicion){
        if(condicion){passed++;}
        else{std::cout << " [Error] " << nombre << std::endl; failed++;}
    };

    // Vamos a crear una función que genere todos los pasos para que no sea muy extenso
    auto probarCifrado = [&](const std::string& tipo, const std::string& mensaje, const std::string& clave){
        std::string mensaje_limpio = Transformer::limpiar_texto(mensaje);
        auto baraja = Cartas::codificar(mensaje_limpio, tipo, clave);
        std::string mensaje_decodificado = Cartas::decodificar(baraja, tipo, clave);
        return mensaje_decodificado.substr(0, mensaje_limpio.size()) == mensaje_limpio;
    };

    // Sin cifrado
    check("SinCifrado: Primer texto de prueba", probarCifrado("none", "Primer texto de prueba", ""));
    check("SinCifrado: LA CALCULADORA", probarCifrado("none", "hola", ""));

    // XOR
    check("XORCifrado: valiente/clave", probarCifrado("xor", "valiente", "clave"));
    check("XORCifrado: mensaje secreto/cartas", probarCifrado("xor", "mensaje secreto", "cartas"));
   
    // Desplazamiento
    check("DesplazamientoCifrado: valiente/clave", probarCifrado("desplazamiento", "hello", "clave"));
    check("DesplazamientoCifrado: mensaje secreto/cartas", probarCifrado("desplazamiento", "mensaje secreto", "cartas"));

    return failed;
}