#pragma once
#include "LargeNumber.h"
#include "Factoradico.h"
#include "Transformer.h"
#include <string>
#include <memory>
#include <vector>

/*
    Esta clase será abstracta para que se ejecute solo cuando sea necesario, se la invocará y 
    se destruirá para evitar un gran consumo de memoria
*/

class Cifrados {
    public: 
        // Establecemos Cifrados como abstracto e incluimos ~ para que cuando se llame al constructor también se llame al destructor
        virtual ~Cifrados() = default; 

        // Nombre para identificar el cifrado
        virtual std::string nombre() const = 0;

        // Cifra un número de 225 bits
        virtual LargeNumber cifrado(const LargeNumber& a, const std::string& key) const = 0;

        // Descrifra un LargeNumber de 225 bits
        virtual LargeNumber descifrado(const LargeNumber& a, const std::string& key) const = 0;

        // Indica si el cifrado requiere de una clave
        virtual bool clave() const = 0;
};

class SinCifrado : public Cifrados {
    public:
    // Instanciamos el nombre y la clave, pero como es sin cifrado devolvemos vacio
    std::string nombre() const override { return "none"; }
    bool clave() const override { return false; }

    LargeNumber cifrado(const LargeNumber& a, const std::string& key) const override{
        return a;
    }

    LargeNumber descifrado(const LargeNumber& a, const std::string& key) const override{
        return a;
    }

};

/*
    XORCipher — Cifrado XOR bit a bit
    XOR es una operación lógica que compara dos bits:
     0 XOR 0 = 0 ; 0 XOR 1 = 1 ; 1 XOR 0 = 1 ; 1 XOR 1 = 0

    Propiedad clave: aplicar XOR dos veces con la misma clave devuelve el valor original (es involutivo).
    mensaje XOR clave = cifrado ; cifrado XOR clave = mensaje
    Por eso cifrar y descifrar son la misma operación. Es el método original del proyecto DeckCrypt (Falcon).
*/

class XORCifrado : public Cifrados {
    public:
        std::string nombre() const override { return "xor";}
        bool clave() const override { return true; }

        LargeNumber cifrado(const LargeNumber& a, const std::string& key) const override {
            if(key.empty()) return a;
            LargeNumber keyNum = Transformer::textoALargeNumber(Transformer::limpiar_texto(key));
            return Transformer::xorLargeNumbers(a, keyNum);
        }
        
        LargeNumber descifrado(const LargeNumber& a, const std::string& key) const override {
            // XOR es simétrico: descifrar = cifrar de nuevo
            return cifrado(a, key);
        }
};

/*
    Factory que nos permite generar un método de cifrado dependiendo del tipo que necesitemos en determinado momento.
    Gracias a unique_ptr podemos generar la clase sin necesidad de instaciarlo con new y delete. De esta forma cuando cumpla 
    su función se eliminará y no tendremos fugas de memoria.
*/
class CifradoFactory {
    public: 
        static std::unique_ptr<Cifrados> create(const std::string& name) {
            if (name == "none") return std::make_unique<SinCifrado>();
            if (name == "xor") return std::make_unique<XORCifrado>();
            throw std::invalid_argument("Cifrado desconocido: " + name);
        }

        static std::vector<std::string> tipos_de_cifrado(){
            return {"none", "xor"};
        }
};