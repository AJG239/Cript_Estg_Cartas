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

            json << "] \n";
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

        static Info cargar_JSON(const std::string& filename){
            std::ifstream file(filename);

            if(!file.is_open()) throw std::runtime_error("No se pudo abrir el archivo: " + filename);

            // Iterador que copia desde el principio hasta el final del archivo
            std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

            file.close();

            // Parseamos la información del archivo
            Info info;
            info.mensaje = parsear_string_JSON(data, "mensaje");
            info.cifrado = parsear_string_JSON(data, "cifrado");
            info.key = parsear_string_JSON(data, "key");

            size_t baraja_INICIO = data.find("\"baraja\"");
            if(baraja_INICIO != std::string::npos){
                size_t corchete_inicio = data.find('[', baraja_INICIO);
                size_t corchete_fin = data.find(']', corchete_inicio);

                if(corchete_inicio != std::string::npos && corchete_fin != std::string::npos){
                    std::string baraja_str = data.substr(corchete_inicio + 1, corchete_fin - corchete_inicio - 1); // Extraemos el contenido del array de la baraja
                    std::istringstream ss(baraja_str); // Creamos un stream para procesar el contenido del array
                    std::string token; // Variable para almacenar cada elemento del array
                    int i = 0;

                    while (std::getline(ss, token, ',') && i < 52) {
                        // Limpiar espacios
                        size_t start = token.find_first_not_of(" \t\n\r"); // Encontrar el primer carácter que no sea espacio
                        if (start != std::string::npos)
                            info.baraja[i++] = std::stoi(token.substr(start)); // Convertir el nombre de la carta a su índice correspondiente
                    }
                }
            }
            return info;

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

        /**
            No se literlamente un parser de JSON, pero para lo que queremos hacer, que es cargar la información del JSON, es suficiente.
         */
        static std::string parsear_string_JSON(const std::string&json, const std::string& key){
            std::string busqueda = "\"" + key + "\""; 
           
            size_t posicion = json.find(busqueda);
            if(posicion == std::string::npos) return ""; // Si no se encuentra la clave, devolvemos una cadena vacía

            size_t dosPuntos = json.find(':', posicion); // Buscar el : después de la clave
            size_t comilla1 = json.find('"', dosPuntos + 1); // Primera comilla del valor
            size_t comilla2 = json.find('"', comilla1 + 1);  // Segunda comilla del valor

            return json.substr(comilla1 + 1, comilla2 - comilla1 - 1); // Extraer lo de entre comillas   
        }
};