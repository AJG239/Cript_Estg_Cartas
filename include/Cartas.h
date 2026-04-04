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

            static const std::string palos[] = {"Picas", "Corazones", "Rombos", "Treboles"};
            static const std::string numeros[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

            int palo = indice / 13; // 0-12 --> picas, 13-25 --> Corazones, 26-38 Rombos, 38-51 Treboles
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

        static std::array<int, baraja> codificar(const std::string& text){
            std::string texto_limipo = Transformer::limpiar_texto(text);

            LargeNumber text_numero = Transformer::textoALargeNumber(texto_limipo);

            std::vector<int> numero_permutacion = Permutacion_Lehmer::NumeroAPermutacion(text_numero, baraja);

            std::array<int, baraja> texto_codificado;
            for(int i = 0; i < baraja; i++){
                texto_codificado[i] = numero_permutacion[i];
            }

            return texto_codificado;
        }

        static std::string decodificar(const std::array<int, baraja>& texto_codificado){
            std::vector<int> permutacion(texto_codificado.begin(), texto_codificado.end());

            if(!Permutacion_Lehmer::PermutacioonValida(permutacion)) return "Error: la baraja no es una permutacion valida";

            LargeNumber permutacion_numero = Permutacion_Lehmer::PermutacionANumero(permutacion);

            std::string numero_texto = Transformer::LargeNumberATexto(permutacion_numero);

            return numero_texto;
        }

        static std::string barajaATecto(const std::array<int, baraja>& texto_baraja){
            std::string texto;
            
            for(int i = 0; i < baraja; i++){
                if (i > 0) texto += " ";
                texto += nombreCarta(texto_baraja[i]);
            }

            return texto;
        }
};