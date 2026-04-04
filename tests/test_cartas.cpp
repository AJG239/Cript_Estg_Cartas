#pragma once
#include "../include/Cartas.h"
#include <iostream>

int test_cartas(){
    int passed = 0, failed = 0;

    auto check = [&](const std::string& nombre, bool condicion){
        if(condicion){passed++;}
        else{std::cout << " [Error] " << nombre << std::endl; failed++;}
    };

    // Nombres de cartas
    check("carta 0 = AS de picas", Cartas::nombreCarta(0) == "AS de picas");
    check("carta 12 = K de Picas", Cartas::nombreCarta(12) == "K de Picas");
    check("carta 13 = AS de Corazones", Cartas::nombreCarta(13) == "AS de Corazones");
    check("carta 51 = K de Treboles", Cartas::nombreCarta(51) == "K de Treboles");

    // Codificar/decodificar
    auto probar = [&](const std::string& msg, const std::string& tipo, const std::string& clave) {
        auto baraja = Cartas::codificar(msg, tipo, clave);
        std::string decoded = Cartas::decodificar(baraja, tipo, clave);
        return decoded.substr(0, msg.size()) == msg;
    };

    check("hello world", probar("hello world", "none", ""));
    check("una letra", probar("a", "none", ""));
    check("max 45 chars", probar("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrs", "none", ""));

    // Permutación válida
    auto baraja = Cartas::codificar("test");
    std::vector<int> perm(baraja.begin(), baraja.end());
    check("permutacion valida", Permutacion_Lehmer::PermutacioonValida(perm));   
}