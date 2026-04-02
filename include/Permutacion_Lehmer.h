#pragma once
#include "LargeNumber.h"
#include "Factoradico.h"
#include <vector>
#include <algorithm>

class Permutacion_Lehmer {
public:
    static std::vector<int> NumeroAPermutacion(const LargeNumber& num, int size){
        std::vector<LargeNumber> fact = Factoradico::NumeroAFactoradico(num); // Transformamos los números dados en factoradico
        Factoradico::add_cerosFactoradico(fact, size); // Añadimos todos los ceros que se nos indiquen

        // Esta lista nos ayudará a saber que posición del vector principal esta disponible, permitiendonos poder utilizar el código de Lehmer
        std::vector<int> lista_lehmer;
        // Rellenamos el vector con la cantidad total de elementos que tenemos como máximo del tamaño
        for(int i = 0; i < size; i++) lista_lehmer.push_back(i);

        //Construimos la permutacion
        std::vector<int> permutacion;
        for(size_t i = 0; i < fact.size(); i++){
            /*  
                Existía un problema. No se puede transformar de forma directa un LargeNumber a un 
                numero entero, por lo que necesitabamos un método que lo acercase.
                Porque pueden pasar dos casos que el LargeNumber sea demasiado perqueño y necesitemos que
                sea más grande o transformalo directo.
                Creamos to double que nos permite pasar un LargeNumber a double.
            */ 
            int inidice = static_cast<int>(fact[i].toDouble());
            permutacion.push_back(lista_lehmer[inidice]);
            lista_lehmer.erase(lista_lehmer.begin() + inidice);
        }

        return permutacion;
    }

    static void printPermutation(const std::vector<int>& perm){
        std::cout << "[";

        for(int i = 0; i < perm.size(); i++){
            if(i > 0) std::cout << ",";
            std::cout << perm[i];
        }

        std::cout << "]";
    }
};