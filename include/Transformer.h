#pragma once
#include "LargeNumber.h"
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <stdexcept>

class Transformer{
    public:
        static const int bits_por_caracter = 5; // Cada carácter usa 5 bits
        static const int tamano_afabeto = 32; // 2^5 = 32 caracteres;
        static const int bits_totales = 255; // log_2(52!)
        static const int max_caracteres = 45; // 225 / 5 = 45
        

        // El alfabeto: 32 caracteres exactos
        static std::string obtenerCaracteres() {
            return " .,-\"/abcdefghijklmnopqrstuvwxyz";
        }

        // Convertimos cada carácter al índice de cada uno entre 0-31
        static int caracterAIndice(char c) {
            char lower = std::tolower(c); // Trabajamos con todos los valores en minúsculas

            std::string caracteres = obtenerCaracteres(); // Necesitamos los caracteres para poder trabajar
            size_t pos_caracter = caracteres.find(lower); // Buscamos el carácter que nos ha dado, lo pasamos a minúsculas porque en nuestro string de caracteres no hay mayúsuclas

            // Comprobamos que el carácter que estemos utilizando este presente, por ejmplo la ñ es un carácter no valido
            if(pos_caracter == std::string::npos){
                std::cout << "Error: caracter '" << c << "' no esta en el alfabeto." << std::endl;
                return -1;
            }

            // Si encontramos el caractere devolmemos
            return static_cast<int>(pos_caracter);
        }

        // Convertimos los índice dados a carácteres para poder leerlo
        static char indiceACaracter(int indice){
            std::string caracteres = obtenerCaracteres(); // Pedimos los carácteres para poder trabajar

            // Comprobamos que el índice este dentro del límite del tamaño del alfabeto
            if(indice < 0 || indice >= tamano_afabeto){
                std::cout << "Error: indice " << indice << " fuera del rango permitido." << std::endl;
                return -1;
            }

            // Buscamos el carácter y lo devolvemos
            return caracteres[indice];
        }

        // Función auxiliar para comprobar si el texto es válido, es una comprobación de que noo haya carácteres inválidos
        static bool texto_valido(const std::string& text) {
            std::string caracteres = obtenerCaracteres(); // Pedimos los carácteres para poder trabajr
            
            // Comprobamos para carácter en minúscula que existe dentro de nuestros caráctere permitidos
            for(int i = 0; i < text.size(); i++){
                char c = text[i];
                if(caracteres.find(std::tolower(c)) == std::string::npos) return false;
            }
            return true;
        }

        // Para poder trabajar de forma más sencilla convertimos el texto a minúscula y en caso de que haya carácteres inválidos
        static std::string limpiar_texto(const std::string& text){
            std::string caracteres = obtenerCaracteres(); // Pedimos los carácteres para poder trabajar

            std::string text_limpio; // Texto que utilizaremos para realizar todo el procesado

            // Añadimos cada carácter en minúscula que pertenezca a nuestro string al texto_limpio
            for(int i = 0; i < text.size(); i++){
                char lower = std::tolower(text[i]); 

                if(caracteres.find(lower) != std::string::npos) text_limpio += lower;
            }

            // Asegurarse de que no supera nuestro máximo, si es así generá un error. Para ello lo reducimos hasta el máximo permitido.
            if(text_limpio.size() > max_caracteres){
                text_limpio = text_limpio.substr(0, max_caracteres);
            }

            return text_limpio;
        }
};