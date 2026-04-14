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

        static bool guardarBMP(const std::array<int, 52>& baraja, const std::string& ruta) {
            // ── Dimensiones ──
            const int ancho_por_carta = 40, alto_por_carta = 56; // Alto y ancho de cada carta
            const int filas = 4, columnas = 13; // Filas x Columnas de la imagen
            const int padding = 4; // Espacio entre cartas en píxeles

            // Ancho total: padding izquierdo + 13 × (carta + padding)
            const int ancho = columnas * (ancho_por_carta + padding) + padding;

            // Alto total: padding superior + 4 × (carta + padding)
            const int alto  = filas * (alto_por_carta + padding) + padding;

            // Buffer de pixeles --> BGR
            std::vector<uint8_t> pixeles(ancho * alto * 3, 30);

            // Colores de los pixeles --> BGR
            struct Color { uint8_t b, g, r; };
            Color coloresPalo[2] = {
                {100, 100, 100},   // Picas y tréboles --> gris
                {80,  80,  220},   // Corazones y Diamantes --> rojo
            };

            for(int i = 0; i < 52; i++){
                int indice_cartas = baraja[i], palo = indice_cartas / 13, numero = indice_cartas % 13;

                int f = i / columnas, col = i % columnas;

                int pos_x = padding + col * (ancho_por_carta + padding);
                int pos_y = padding + f * (alto_por_carta + padding);

                // Color base del palo
                Color palo_carta = coloresPalo[palo];

                // Pintar cada píxel del rectángulo de la carta
                for (int y = 0; y < alto_por_carta; y++) {
                    for (int x = 0; x < ancho_por_carta; x++) {
                        // Posición absoluta en la imagen
                        int pixel_x = pos_x + x;
                        int pixel_y = pos_y + y;

                        // Comprobar si el píxel pertenece al borde
                        bool borde = (x == 0 || x == ancho_por_carta - 1 || y == 0 || y == alto_por_carta - 1);

                        // Calcular índice en el buffer de píxeles
                        int index = ((pos_y + y) * ancho + (pos_x + x)) * 3;

                        if (borde) {
                            // Borde gris 
                            pixeles[index] = 200;
                            pixeles[index + 1] = 200;
                            pixeles[index + 2] = 200;
                        } else {
                            // Interior: color del palo
                            pixeles[index] = palo_carta.b;
                            pixeles[index + 1] = palo_carta.g;
                            pixeles[index + 2] = palo_carta.r;
                        }
                    }
                }
            }

            return generar_bitmap(ruta, ancho, alto, pixeles);
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

        /*
            Escribe un archivo BMP completo. Un archivo BMP esta formado por una cabecera BMP de 14 bytes, una cabecera DIB de 40 bytes
            y el resto formado por los pixeles de las cartas.

            Importante que el formato de color de un BitMap no es RGB de BGR.
        */
        static bool generar_bitmap(const std::string& ruta, int ancho, int alto, const std::vector<uint8_t>& pixels){
            // Un bitmap establece que las imagenes deben ser múltiplos de 4, por lo que redondeamos el tamaño de bits a un múltiplo de 4
            int tam_fila = ((ancho * 3 + 3) / 4) * 4;
            int tam_imagen = tam_fila * alto; // Tamaño total de la imagen (dimensión total)
            int tam_fichero = 54 + tam_imagen;  // Tamaño total del bitmap --> sumamos 54 por la cabecera (14 bits = BitMap + tamaño + offset) y cabecera DIB (40 bits =  dimensiones + formato) 

            std::ofstream archivo(ruta, std::ios::binary);
            if (!archivo.is_open()) return false;

            // Cabecera BMP
            uint8_t cabecera_BMP[14] = {};
            
            cabecera_BMP[0] = 'B';
            cabecera_BMP[1] = 'M';

            escrbirLeenEnding(cabecera_BMP + 2, tam_fichero);
            escrbirLeenEnding(cabecera_BMP + 10, 54);

            archivo.write(reinterpret_cast<char*>(cabecera_BMP), 14); // Introducción de la cabecera BMP al archivo

            // Cabecera DIB --> información técnica para visualizar la imagen
            uint8_t cabecera_DIB[40] = {};

            escrbirLeenEnding(cabecera_DIB, 40); // Tamaño de la cabecera
            escrbirLeenEnding(cabecera_DIB + 4, ancho); // Ancho en píxeles
            escrbirLeenEnding(cabecera_DIB + 8, alto); // Alto en píxeles

            cabecera_DIB[12] = 1; cabecera_DIB[13] = 0; // Ambos píxeles nos dan los planos de color
            cabecera_DIB[14] = 24; cabecera_DIB[15] = 0; // 3 Bytes por pixel formato BGR

            escrbirLeenEnding(cabecera_DIB + 20, tam_imagen);

            archivo.write(reinterpret_cast<char*>(cabecera_DIB), 40);

            /*
                Datos de cada pixel, cada fila tiene 8 bits disponibles y se escribe de abajo a arriba la imagen
            */

            std::vector<uint8_t> fila(tam_fila, 0);

            for(int i = alto - 1; i >= 0; i--){
                for(int j = 0; j < ancho; j++){
                    int indice = (i * ancho + j) * 3;
                    fila[j * 3] = pixels[indice];
                    fila[j * 3 + 1] = pixels[indice + 1];
                    fila [j * 3 + 2] = pixels[indice + 2];
                }

                archivo.write(reinterpret_cast<char*>(fila.data()), tam_fila);
            }

            archivo.close();
            return true;
        }

        /*
            Escribe un entere de 32 bitsen formato little-endian (bytes bajo primero), BMP exige este formato para todos
            los número de la cabecera.
        */
        static void escrbirLeenEnding(uint8_t* buffer, uint32_t val){
            buffer[0] = val & 0xFF; // Los 8 bytes más bajos    
            buffer[1] = (val >> 8) & 0xFF;
            buffer[2] = (val >> 16) & 0xFF;
            buffer[3] = (val >> 24) & 0xFF;
        }

        /* 
            Números para poder escribirlos en las cartas
        */
        static constexpr uint8_t numeros[][7] = {
        {0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E}, // 0
        {0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E}, // 1
        {0x0E, 0x11, 0x01, 0x06, 0x08, 0x10, 0x1F}, // 2
        {0x0E, 0x11, 0x01, 0x06, 0x01, 0x11, 0x0E}, // 3
        {0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02}, // 4
        {0x1F, 0x10, 0x1E, 0x01, 0x01, 0x11, 0x0E}, // 5
        {0x06, 0x08, 0x10, 0x1E, 0x11, 0x11, 0x0E}, // 6
        {0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08}, // 7
        {0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E}, // 8
        {0x0E, 0x11, 0x11, 0x0F, 0x01, 0x02, 0x0C}, // 9       
        {0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11}, // A (índice 10)      
        {0x07, 0x02, 0x02, 0x02, 0x02, 0x12, 0x0C}, // J (índice 11)   
        {0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D}, // Q (índice 12)
        {0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11}, // K (índice 13)
        };

        /*
            Palos de la baraja
        */
        static constexpr uint8_t palos[][7] = {
            {0x08, 0x1C, 0x3E, 0x7F, 0x3E, 0x08, 0x1C}, // picas
            {0x22, 0x77, 0x7F, 0x7F, 0x3E, 0x1C, 0x08}, // corazones
            {0x08, 0x1C, 0x3E, 0x7F, 0x3E, 0x1C, 0x08}, // rombos
            {0x08, 0x1C, 0x2A, 0x6B, 0x3E, 0x08, 0x1C}, // tréboles
        };
};