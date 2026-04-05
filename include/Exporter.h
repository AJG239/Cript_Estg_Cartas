#pragma once
#include "Cartas.h"
#include <string>
#include <vector>
#include <fstream> 
#include <sstream>
#include <cstdint>

class Exporter{
    public: 
        struct Info {
            std::string mensaje;
            std::string cifrado;
            std::string key;
            std::array<int, 52> baraja;
        };

        static std::string toJSON(const Info& informacion){
            std::ostringstream json;

            // Seguimos la estructura de un JSON para cada una de las lineas del string
            json << "{\n";
            json << " \"message\" : \"" << caracteres_especiales_JSON(informacion.mensaje) << "\", \n"; // "message" : "mensaje"; 
            json << "\"cifrado\" : \"" << caracteres_especiales_JSON(informacion.cifrado) << "\", \n";
            json << "\"key\" : \"" << caracteres_especiales_JSON(informacion.key) <<"\", \n";
            json << "\"baraja\" : [";
            
            for(int i = 0; i < 52; i++){
                if(i > 0) json << ", ";
                json << "\"" << Cartas::nombreCarta(informacion.baraja[i]) << "\"";
            }

            json << "], \n";
            json << "}";

            return json.str();  
        }

        static bool guardar_JSON(const Info& info, const std::string& filename){
            std::ofstream file(filename); // Creamos el archivo si no existe o lo abrimos para escribir en él.
            if(!file.is_open()) return false;

            file << toJSON(info);
            file.close();

            return true;
        }

    private:
        /*
            Para poder guardar el archivo comoo un JSON y que la interpretación de carácteres no de error
            tenemos que duplicar los distintos elementos para que no los tome como errores al guardarlo.
        */ 
        static std::string caracteres_especiales_JSON(const std::string& a){
            std::string caracter_especial;

            for(size_t i = 0; i < a.size(); i++){
                char caract = a[i];
                switch(caract){
                    case '"': caracter_especial += "\\\""; break;
                    case '\\': caracter_especial += "\\\\"; break;
                    case '\n': caracter_especial += "\\n"; break;
                    case '\t': caracter_especial += "\\t"; break; 
                    default: caracter_especial += caract;
                }
            }

            return caracter_especial;
        }
};