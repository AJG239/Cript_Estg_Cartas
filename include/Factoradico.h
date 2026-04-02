#pragma once
#include "LargeNumber.h"
#include <vector>

/*
    Dentro de este método encontramos un gran problema, calcular el factorádico constantemente genera
    una gran carga computacionaly hará que nuestro proyecto se ralentice bastante.
    Para solucionarlo una idea puede ser guardarlo en caché/memoria para poder acceder a estos valores cuando
    necesitemos. Porque también los necesitaremos con el Lehmer-Code y las permutaciones.
*/

class Factoradico{
    public: 
        static std::vector<LargeNumber> cache;

        static void iniciar_Cache(){
            if (!cache.empty()) return; // Comprobamos que exista una cache instanciada, si existe terminamos.

            cache.push_back(LargeNumber(1)); // Añadimos directamente el 0! = 1

            // Se hace como máximo hasta 52 porque solo tendremos 52 cartas en la baraja
            for (int i = 1; i <= 52; i++){
                // Coste computacional muy elevado (mirar algún método más efeciente)
                cache.push_back(LargeNumber::mulLong(cache.back(), LargeNumber(i)));
            }
        }

        static LargeNumber factorial(int n){
            iniciar_Cache();
            return cache[n];    
        }

        static std::vector<LargeNumber> NumeroAFactoradico(LargeNumber num){
            LargeNumber q, r; // Definimos para nuestros calculos el cociente y el resto
            std::vector<LargeNumber> result; // Necesitamos un vector de LargeNumber porque el número que dividimos es del mismo tipo
            int divisor = 1; // Divisor incremental

            while(LargeNumber::comparar_numeros(num, LargeNumber(0))){
                LargeNumber::divMod(num, LargeNumber(divisor), q, r); // Hacemos la división para obtener el cociente y el resto
                result.insert(result.begin(), r); // Insertamos el resto en la primera posición
                num = q; // Cambiamos el número por el cociente para seguir dividiendo
                divisor++; // Incrementamos el divisor
            }

            return result;
        }

        static LargeNumber FactoradicoANumero(const std::vector<LargeNumber>& fact){
            iniciar_Cache(); // Iniciamos cache por si acaso no se ha iniciado antes

            LargeNumber result(0); // Neesitamos instancia el valor a 0 para poder ir sumando los resultados
            int size = static_cast<int>(fact.size()); // Sacamos el tamaño del vector para poder desplazarnos por él

            // Para cada elemento del vector, lo multiplicamos por el factorial y lo sumamos al resultado
            for(int i = 0; i < size; i++){
                result = LargeNumber::addUnsigned(result, LargeNumber::mulLong(fact[i], factorial(size-i-1)));
            }

            return result;
        }

        // Esto se necesita porque estamos trabajando con vectores de 52 elementos y necesita de que se rellenes de ceros
        static void add_cerosFactoradico(std::vector<LargeNumber>& fact, int size){
            while(static_cast<int>(fact.size()) < size){
                fact.insert(fact.begin(), LargeNumber(0)); // Dependiendo del tamaño insertamos tantos ceros como necesitemos
            }
        }
};

// Si no tenemos esta linea no podemos acceder a los valores de cache, es una definción estática.
inline std::vector<LargeNumber> Factoradico::cache;