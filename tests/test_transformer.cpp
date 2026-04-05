#pragma once
#include "../include/Transformer.h"
#include <iostream>

int test_transformer(){
    int passed = 0, failed = 0;

    auto check = [&](const std::string& nombre, bool condicion){
        if(condicion){passed++;}
        else{std::cout << " [Error] " << nombre << std::endl; failed++;}
    };

    // Índices
    check("espacio=0", Transformer::caracterAIndice(' ') == 0);
    check("a=6", Transformer::caracterAIndice('a') == 6);
    check("z=31", Transformer::caracterAIndice('z') == 31);

    // Proceso total de caracteres
    std::string caracteres = Transformer::obtenerCaracteres();
    bool todosOk = true;
    for (int i = 0; i < 32; i++) {
        if (Transformer::caracterAIndice(caracteres[i]) != i) todosOk = false;
        if (Transformer::indiceACaracter(i) != caracteres[i]) todosOk = false;
    }
    check("32 simbolos ida/vuelta", todosOk);

    // Texto a número
    auto texto = Transformer::limpiar_texto("Este 123 texto ES de PrUeBa./");
    auto texto_numero = Transformer::textoALargeNumber(texto);
    auto numero_texto = Transformer::LargeNumberATexto(texto_numero);
    
    check("limpiar texto", texto == "este  texto es de prueba./");
    check("numero no es cero", texto_numero.size() > 0);
    check("ida y vuelta", numero_texto.substr(0, texto.size()) == texto);

    return failed;
}