#pragma once
#include "Factoradico.h"
#include "LargeNumber.h"
#include "Permutacion_Lehmer.h"
#include "Transformer.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <array>

class Cartas{
    public:
        static const int baraja = 52; // Tamaño de una baraja de cartas

        static std::string nombreCarta(int indice){
            if (indice < 0 || indice >= baraja) return "Error";

            static const std::string palos[] = {"Picas", "Corazones", "Diamantes", "Treboles"};
            static const std::string numeros[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

            int palo = indice / 13; // 0-12 --> picas, 13-25 --> Corazones, 26-38 Diamantes, 38-51 Treboles
            int num = indice % 13; // Número dentro del palo
            
            return numeros[num] + " de " + palos[palo] + " " + ((palo == 1 || palo == 2) ? "roja" : "negra");
        }

        static std::array<int, baraja> BarajaIdentidad(){
            std::array<int, baraja> identidad;
            
            for(int i = 0; i < baraja; i++){
                identidad[i] = i;
            }

            return identidad;
        }
};