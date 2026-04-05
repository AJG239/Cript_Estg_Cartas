#pragma once
#include "../include/Exporter.h"
#include <iostream>
#include <filesystem>

int test_exporter(){
    int passed = 0, failed = 0;
    std::filesystem::create_directories("output/data"); // Crear el directorio si no existe

    auto check = [&](const std::string& nombre, bool condicion){
        if(condicion){passed++;}
        else{std::cout << " [Error] " << nombre << std::endl; failed++;}
    };

    Exporter::Info info;
    info.mensaje = "test exporter";
    info.cifrado = "xor";
    info.key = "clave";
    info.baraja = Cartas::codificar(info.mensaje, info.cifrado, info.key);

    // JSON export
    bool jsonOk = Exporter::guardar_JSON(info, "output/data/test_export.json");
    check("JSON guardado", jsonOk);

    return failed;
}