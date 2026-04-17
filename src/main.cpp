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

std::string elegirCifrado() {
    std::cout << "\n  Cifrados disponibles:" << std::endl;
    std::cout << "  1. Sin cifrado" << std::endl;
    std::cout << "  2. XOR" << std::endl;
    std::cout << "  3. Desplazamiento" << std::endl;
    std::cout << "  Opcion: ";

    int opcion;
    std::cin >> opcion;
    std::cin.ignore();

    switch (opcion) {
        case 1: return "none";
        case 2: return "xor";
        case 3: return "desplazamiento";
        default: return "none";
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

    // Elegir cifrado
    std::string cifrado = elegirCifrado();
    std::string clave = "";

    if (cifrado != "none") {
        std::cout << "  Clave: ";
        std::getline(std::cin, clave);
        clave = Transformer::limpiar_texto(clave);
    }

    // Codificar
    std::cout << "\n  Codificando..." << std::endl;
    auto baraja = Cartas::codificar(limpio, cifrado, clave);

    // Mostrar resultado
    std::cout << "\n  Baraja resultante:" << std::endl;
    std::cout << "  ----------------------------------------" << std::endl;
    mostrarBaraja(baraja);
    std::cout << "  ----------------------------------------" << std::endl;

    // Preguntar si quiere exportar
    std::cout << "\n¿Quieres exportar los resultads?" << std::endl;
    std::cout << "1. Guardar JSON" << std::endl;
    std::cout << "2. Guardar imagen BMP" << std::endl;
    std::cout << "3. Guardar ambos" << std::endl;
    std::cout << "4. No exportar" << std::endl;
    std::cout << "Opcion: ";

    int exportar;
    std::cin >> exportar;
    std::cin.ignore();

    if (exportar == 1 || exportar == 3) {
        std::filesystem::create_directories("../output/data");
        std::cout << "Nombre del archivo JSON (sin extension): ";
        std::string nombreJSON;
        std::getline(std::cin, nombreJSON);

        Exporter::Info info{limpio, cifrado, clave, baraja};
        std::string rutaJSON = "../output/data/" + nombreJSON + ".json";

        if (Exporter::guardar_JSON(info, rutaJSON)) {
            std::cout << "JSON guardado en: " << rutaJSON << std::endl;
        } else {
            std::cout << "Error al guardar JSON" << std::endl;
        }
    }

    if (exportar == 2 || exportar == 3) {
        std::filesystem::create_directories("../output/images");
        std::cout << "Nombre del archivo BMP (sin extension): ";
        std::string nombreBMP;
        std::getline(std::cin, nombreBMP);

        std::string rutaBMP = "../output/images/" + nombreBMP + ".bmp";

        if (Exporter::guardarBMP(baraja, rutaBMP)) {
            std::cout << "BMP guardado en: " << rutaBMP << std::endl;
        } else {
            std::cout << "Error al guardar BMP" << std::endl;
        }
    }

    pausar();
}

void decodificarMensaje(){

}

void informacion(){

}

void pruebaApp(){

}

int main(){
    int op = -1;

    while(op != 0){
        limpiarPantalla();

        std::cout << "TFG: Criptografia en metodos de baraja \n" << std::endl;
        std::cout << "Menu prinicipal" << std::endl;

        std::cout << "1. Codificar mensaje" << std::endl;
        std::cout << "2. Decodificar mensaje" << std::endl;
        std::cout << "3. Informacion del sistema" << std::endl;
        std::cout << "4. Prueba" << std::endl;

        std::cout << "0. Salir" << std::endl;

        std::cin >> op;
        std::cin.ignore();

        switch(op){
            case 1: codificarMensaje(); break;
            case 2: decodificarMensaje(); break;
            case 3: informacion(); break;
            case 4: pruebaApp(); break;
            case 0: std::cout << "\n Gracias por utilizar la aplicacion.\n" << std::endl; break;
            default: std::cout << "  Opcion no valida" << std::endl; break;
        }
    }

    return 0;
}