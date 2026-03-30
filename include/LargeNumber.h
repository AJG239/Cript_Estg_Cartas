#include <cmath>
#include <cstdint>
#include <climits>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>


class LargeNumber{
    public:
        using word = uint64_t; // Utilizamos uint64_t como unidad básica de almcenamiento de palabras de 64 bits.

        std::vector<word> words; // Vector de digitos (el [0] es el menos representativo).
        bool neg; // Saber si es negativo.

        // Funciones de apoyo
        static size_t wordBits(){return sizeof(word) * CHAR_BIT;}

        // Funciones de apoyo para el vector
        size_t size() const {return words.size();} // Devuelve el tamaño del vector
        word& back() {return words.back();}

        word& operator[](size_t i) {return words[i];}
        const word& operator[](size_t i) const {return words[i];}

        void pop_back() {
            words.pop_back(); // Elimina el último elemento del vector
        }

        void push_back(word b){
            words.push_back(b); // Se añade al final del vector el valor
        }

        void resize(size_t s){
            words.resize(s);
        }

        LargeNumber& setNeg(bool n){
            neg = n;
            return *this;
        }
        
        LargeNumber& truncate() {
            while (size() > 0 && words.back() == 0) pop_back();
            return *this;
        }

        // Constructores
        LargeNumber() : neg(false) {}

        LargeNumber(size_t s, word w, bool negativo = false) : words(s, w), neg(negativo) {}

        LargeNumber(const word* a, const word* b, bool negativo = false) : words(a, b), neg(negativo){
            truncate();
        } 

        LargeNumber(const LargeNumber& a) : words(a.words), neg(a.neg) {}

        LargeNumber(int i) : neg(i < 0){
            // Para crear LargeNumber comprobamos que el número que se haya introducido no sea negativo
            // Si es así lo cambiamos para poder trabajar correctamente, sino se da el valor dado
            // Importante --> El vector vacio cuenta como el 0.
            unsigned u = (i < 0) ? -static_cast<unsigned>(i) : static_cast<unsigned>(i);
            if(u > 0) push_back(u); 
        }

        // Funciones Aritmeticas
        /* 
           El objetivo de este función es que cuando se calcula la suma de los dos vectores
           comprobar que existe acarreo, es decir, cuando se excede del valor máximo que puede
           tener hay acarreo
        */
        static word addCarry(word* a, word b){
            return (*a += b) < b; // Permite saber si existe acarreo en el calculo
        }

         /* 
           El objetivo de este función es que cuando se calcula la resta de los dos vectores
           comprobar que existe un préstamo, es decir, cuando se 
        */
        static word subCarry(word* a, word b){
            word temp = *a;
            return(*a = temp - b ) > temp; // Si se cumple esta condición es que existe préstamo
        }

        static LargeNumber& addUnsignedOverwrite(LargeNumber& a, const LargeNumber& b){
            // Comprobamos el tamaño de ambos vectores para asegurar que tengamos espacio suficiente
            size_t size_a = a.size(), size_b = b.size(), size_max = std::max(size_a, size_b);
            a.resize(size_max);
        
            word carry = 0;
            size_t i;
            
            // Sumamos los elementos que contienen palabras
            for (i = 0; i < size_b; i++){
                carry = addCarry(&a[i], carry);
                carry += addCarry(&a[i], b[i]);
            }

            // Propaga el acarreo a los elementos restanttes de a
            for (; i < size_max && carry; i++) carry = addCarry(&a[i], carry);

            // Si después de propagar el acarreo aún queda uno, lo añadimos al final del vector
            if (carry) a.push_back(carry);

            return a.truncate();
        }

        // Versión que no modifica los valores originales. Crea una copia
        static LargeNumber addUnsigned(const LargeNumber& a, const LargeNumber& b){
            LargeNumber r(a);
            return addUnsignedOverwrite(r, b);
        }
        
};

