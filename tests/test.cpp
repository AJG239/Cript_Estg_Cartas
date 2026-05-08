#include <iostream>
#include "test_cartas.cpp"
#include "test_cifrado.cpp"
#include "test_exporter.cpp"
#include "test_factoradico.cpp"
#include "test_largenumber.cpp"
#include "test_permutaciones_lehmer.cpp"
#include "test_transformer.cpp"

int main() {
    int fallos_totales = 0;

    fallos_totales += test_largenumber();
    fallos_totales += test_factoradico();
    fallos_totales += test_permutation_lehmer();
    fallos_totales += test_transformer();
    fallos_totales += test_cifrado();
    fallos_totales += test_cartas();
    fallos_totales += test_exporter();

    if (fallos_totales == 0) {
        std::cout << "  TODOS LOS TESTS PASADOS" << std::endl;
    } else {
        std::cout << "  " << fallos_totales << " TESTS FALLIDOS" << std::endl;
    }

    return fallos_totales;
}