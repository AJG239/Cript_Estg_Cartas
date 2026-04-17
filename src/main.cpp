#include <iostream>
#include <string>
#include <filesystem>
#include "../prueba.cpp"
#include "../include/Cartas.h"
#include "../include/Cifrados.h"
#include "../include/Exporter.h"
#include "../include/Factoradico.h"
#include "../include/LargeNumber.h"
#include "../include/Permutacion_Lehmer.h"
#include "../include/Transformer.h"

// Esto nos permite antes de volver a ejecutar la pantalla inicial, mandarle como preprocesado un clean
void limpiarPantalla(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pausar() {
    std::cout << "\nPulsa Enter para continuar...";
    std::cin.ignore();
    std::cin.get();
}

void mostrarBaraja(const std::array<int, 52>& baraja) {
    // Mostrar como cuadrícula 13×4
    for (int i = 0; i < 52; i++) {
        std::string nombre = Cartas::nombreCarta(baraja[i]);
        std::cout << nombre;
        for (size_t j = nombre.size(); j < 22; j++) std::cout << " ";

        if ((i + 1) % 13 == 0) std::cout << std::endl;
    }
}

void codificarMensaje(){
    limpiarPantalla();

    std::cout << "CODIFICAR MENSAJE DE BARAJA" << std::endl;
    std::cout << "Maximo de 45 cracteres por mensaje" << std::endl;

    std::string mensaje;
    std::getline(std::cin, mensaje);

    if (mensaje.empty()) {
        std::cout << "  Error: el mensaje no puede estar vacio" << std::endl;
        pausar();
        return;
    }   

    std::string limpio = Transformer::limpiar_texto(mensaje);
    std::cout << "Texto limpio: \"" << limpio << "\"" << std::endl;

    if (limpio.empty()) {
        std::cout << "Error: el mensaje no contiene caracteres validos" << std::endl;
        pausar();
        return;
    }

    std::cout << "Longitud: " << limpio.size() << "/45 caracteres" << std::endl;

    if(limpio.size() > 45){
        std::cout << "El text supero el tamaño máximo, ser reducirá a 45 caracteres" << std::endl;
    }

    // Codificar
    std::cout << "\n  Codificando..." << std::endl;
    auto baraja = Cartas::codificar(limpio);

    // Mostrar resultado
    std::cout << "\n  Baraja resultante:" << std::endl;
    std::cout << "  ----------------------------------------" << std::endl;
    mostrarBaraja(baraja);
    std::cout << "  ----------------------------------------" << std::endl;

    pausar();
}

int main(){
    int op = -1;

    while(op != 0){
        limpiarPantalla();

        std::cout << "1. Codificar mensaje" << std::endl;

        std::cout << "0. Salir" << std::endl;

        std::cin >> op;
        std::cin.ignore();

        switch(op){
            case 1: codificarMensaje(); break;
        }
    }

    return 0;
}