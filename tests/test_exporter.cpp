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

     // ── BMP: guardar ──
    bool bmpOk = Exporter::guardarBMP(info.baraja, "output/images/test_export.bmp");
    check("BMP guardado", bmpOk);

    // ── BMP: verificar que el archivo existe y tiene contenido ──
    std::ifstream bmpFile("output/images/test_export.bmp", std::ios::binary | std::ios::ate);
    check("BMP archivo existe", bmpFile.is_open());
    check("BMP tiene contenido", bmpFile.tellg() > 54);  // Al menos las cabeceras

    // ── BMP: verificar cabecera ──
    if (bmpFile.is_open()) {
        bmpFile.seekg(0);  // Volver al inicio
        char firma[2];
        bmpFile.read(firma, 2);
        check("BMP firma 'BM'", firma[0] == 'B' && firma[1] == 'M');
    }

    // ── BMP: baraja por defecto también funciona ──
    auto barajaDefault = Cartas::BarajaIdentidad();
    bool bmpDefault = Exporter::guardarBMP(barajaDefault, "output/images/test_default.bmp");
    check("BMP baraja default", bmpDefault);

    return failed;
}