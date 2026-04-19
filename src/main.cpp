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
    std::cout << "\nCifrados disponibles:" << std::endl;
    std::cout << "1. Sin cifrado" << std::endl;
    std::cout << "2. XOR" << std::endl;
    std::cout << "3. Desplazamiento" << std::endl;
    std::cout << "Opcion: ";

    int op;
    std::cin >> op;
    std::cin.ignore();

    switch (op) {
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
        std::cout << "Error: el mensaje no puede estar vacio" << std::endl;
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
    std::cout << "\nCodificando..." << std::endl;
    auto baraja = Cartas::codificar(limpio, cifrado, clave);

    // Mostrar resultado
    std::cout << "\nBaraja resultante:" << std::endl;
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
    limpiarPantalla();

    std::cout << "DECODIFICAR BARAJA A MENSAJE" << std::endl;

    std::cout << "1. Cargar desde archivo JSON" << std::endl;
    std::cout << "2. Introducir indices manualmente" << std::endl;
    std::cout << "Opcion: ";

    int op;
    std::cin >> op;
    std::cin.ignore();

    std::array<int, 52> baraja;
    std::string cifrado = "none";
    std::string clave = "";

    if (op == 1) {
        std::cout << "Ruta del archivo JSON: ";
        std::string ruta;
        std::getline(std::cin, ruta);

        try {
            Exporter::Info info = Exporter::cargar_JSON(ruta);

            baraja = info.baraja;
            cifrado = info.cifrado;
            clave = info.key;

            std::cout << "Sesion cargada correctamente" << std::endl;
            std::cout << "Cifrado: " << cifrado << std::endl;

            if (!clave.empty()) std::cout << "  Clave: " << clave << std::endl;
        } catch (const std::exception& e) {
            std::cout << "  Error: " << e.what() << std::endl;

            pausar();
            return;
        }
    } else {
        std::cout << "Introduce 52 indices separados por comas:" << std::endl;
        std::cout << "> ";

        std::string csv;
        std::getline(std::cin, csv);

        std::istringstream ss(csv);
        std::string token;

        int i = 0;
        while (std::getline(ss, token, ',') && i < 52) {
            size_t start = token.find_first_not_of(" ");
            if (start != std::string::npos)
                baraja[i++] = std::stoi(token.substr(start));
        }

        if (i != 52) {
            std::cout << "Error: se necesitan exactamente 52 indices" << std::endl;
            pausar();
            return;
        }

        cifrado = elegirCifrado();
        if (cifrado != "none") {
            std::cout << "  Clave: ";

            std::getline(std::cin, clave);
            clave = Transformer::limpiar_texto(clave);
        }
    }

    std::cout << "\nBaraja:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    mostrarBaraja(baraja);
    std::cout << "----------------------------------------" << std::endl;

    std::string mensaje = Cartas::decodificar(baraja, cifrado, clave);

    std::cout << "\nMensaje decodificado:" << std::endl;
    std::cout << "\"" << mensaje << "\"" << std::endl;

    pausar();
}

void informacion(){
    limpiarPantalla();

    std::cout << "INFORMACION DE LA APP" << std::endl;

    std::cout << "Cartas en la baraja: " << 52 << std::endl;
    std::cout << "Caracteres maximos: 45" << std::endl;
    std::cout << "Alfabeto:\"" << Transformer::obtenerCaracteres() << "\"" << std::endl;

    std::cout << "\n  Cifrados disponibles:" << std::endl;
    auto disponibles = CifradoFactory::tipos_de_cifrado();
    for (const auto& nombre : disponibles) {
        auto c = CifradoFactory::create(nombre);
        std::cout << "- " << nombre;
        if (c->clave()) std::cout << " (requiere clave)";
        std::cout << std::endl;
    }

    std::cout << "\nFlujo de la app:" << std::endl;
    std::cout << "Texto -> Binario (225 bits) -> [Cifrado] -> Decimal" << std::endl;
    std::cout << "-> Factoradico -> Lehmer -> Permutacion de 52 cartas" << std::endl;

    pausar();
}

void pruebaApp(){
    limpiarPantalla();
    std::cout << "Prueba de sistema \n" << std::endl;

    std::cout << "Aviso: La prueba no guarda los resultados ni codifica el mensaje \n" << std::endl;

    std::cout << "Mensaje: ";
    std::string mensaje;
    std::getline(std::cin, mensaje);

    std::string limpio = Transformer::limpiar_texto(mensaje);

    // Paso 1: Texto limpio
    std::cout << "\n=== Paso 1: Limpiar texto ===" << std::endl;
    std::cout << "Original:  \"" << mensaje << "\"" << std::endl;
    std::cout << "Limpio:    \"" << limpio << "\"" << std::endl;
    std::cout << "Longitud:  " << limpio.size() << "/45" << std::endl;

    // Paso 2: Texto a número
    std::cout << "\n=== Paso 2: Texto a numero (225 bits) ===" << std::endl;

    LargeNumber numero = Transformer::textoALargeNumber(limpio);

    std::cout << "Numero: " << numero << std::endl;

    // Paso 3: Número a sistema factorial
    std::cout << "\n=== Paso 3: Numero a sistema factorial ===" << std::endl;

    auto fact = Factoradico::NumeroAFactoradico(numero);

    std::cout << "Numero factorial (" << fact.size() << " digitos): [";

    for (size_t i = 0; i < fact.size() && i < 10; i++) {
        if (i > 0) std::cout << ", ";
        std::cout << fact[i];
    }

    if (fact.size() > 10) std::cout << ", ...";

    std::cout << "]" << std::endl;

    // Paso 4: Número factorial a permutación
    std::cout << "\n=== Paso 4: Numero factorial a permutacion (Lehmer) ===" << std::endl;

    auto perm = Permutacion_Lehmer::NumeroAPermutacion(numero, 52);

    std::cout << "Permutacion: [";

    for (size_t i = 0; i < 10; i++) {
        if (i > 0) std::cout << ", ";
        std::cout << perm[i];
    }

    std::cout << ", ...]" << std::endl;

    // Paso 5: Permutación a cartas
    std::cout << "\n=== Paso 5: Permutacion a cartas ===" << std::endl;
    std::array<int, 52> baraja;

    for (int i = 0; i < 52; i++) baraja[i] = perm[i];
    mostrarBaraja(baraja);

    // Paso 6: Verificación inversa
    std::cout << "\n=== Paso 6: Verificacion inversa ===" << std::endl;

    LargeNumber numBack = Permutacion_Lehmer::PermutacionANumero(perm);
    
    std::string textoBack = Transformer::LargeNumberATexto(numBack);
    std::cout << "Texto recuperado: \"" << textoBack << "\"" << std::endl;

    bool ok = (textoBack.substr(0, limpio.size()) == limpio);
    std::cout << "Verificacion: " << (ok ? "CORRECTA" : "FALLO") << std::endl;

    pausar();
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
            default: std::cout << "Opcion no valida" << std::endl; break;
        }
    }

    return 0;
}