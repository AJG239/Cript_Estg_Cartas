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
    DesplazamientoCifrado --> A diferencia de los desplazamientos normales que mueven una cantidad
    de letras en el orden que se le indica. Aquí desplazamos nuestros 225 bits sumandole la clave dada.
    En caso de que se exceda, se calcula en módulo de 52! para obtener el valor a esconder.
*/

class DesplazamientoCifrado : public Cifrados {
    public:
        std::string nombre() const override {return "desplazamiento";}
        bool clave() const override {return true;}

        LargeNumber cifrado(const LargeNumber& a, const std::string& key) const override {
            if(key.empty()) return a;

            LargeNumber keyNum = Transformer::textoALargeNumber(Transformer::limpiar_texto(key));

            // Para no salirnos del rango permitido, tomaremos 52! como el módulo
            LargeNumber modulo = Factoradico::factorial(52);

            // Sumamos ambos elementos para generar el desplazamiento
            LargeNumber result = LargeNumber::addUnsigned(a, keyNum);

            // Debemos comprobar que no se pase de 52!, si se pasa calculamos el resto 
            if (LargeNumber::comparar_numeros(result, modulo) >= 0){
                LargeNumber cociente, resto;
                LargeNumber::divMod(result, modulo, cociente, resto);
                result = resto;
            }
            
            return result;
        }
        
        LargeNumber descifrado(const LargeNumber& a, const std::string& key) const override {
            if(key.empty()) return a;

            LargeNumber keyNum = Transformer::textoALargeNumber(Transformer::limpiar_texto(key));
            LargeNumber modulo = Factoradico::factorial(52);

            // Existen dos casos que a >= key, se resta de forma directa. Si a < key, significa que hemos encontrado el resto por lo que necesitamos recalcularla
            if (LargeNumber::comparar_numeros(a, keyNum) >= 0){
                return LargeNumber::subUnsigned(a, keyNum); 
            } else{
                // Calculamos la diferencia existente entre los valores dados y luego se lo quitamos al módulo para encontrar el verdadero valor que estamos buscando
                LargeNumber diferencia = LargeNumber::subUnsigned(keyNum, a);
                return LargeNumber::subUnsigned(modulo, diferencia);
            }
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
            if (name == "desplazamiento") return std::make_unique<DesplazamientoCifrado>();
            throw std::invalid_argument("Cifrado desconocido: " + name);
        }

        static std::vector<std::string> tipos_de_cifrado(){
            return {"none", "xor", "desplazamiento"};
        }
};