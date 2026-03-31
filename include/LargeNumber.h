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
        static size_t wordBits(){return sizeof(word) * CHAR_BIT;} // Nos devuelve la cantidad de bits de la palabra en formato size_t
        static word wordMask(){return static_cast<word>(-1);} // Nos da la vuelta a la palabra para poder empezar desde lo más bajo
        // A la cadena dada la vuelta, calculamos el número de bits y devolvemos la mitad para que sea más fácil trabajar con ellos
        static word wordHalfMask(){return wordMask() >> (wordBits() / 2);} 

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

        static size_t wordBitLength(word a){
            for(int i = wordBits() - 1; i>= 0; i--){
                if ((a >> i) & 1) return i + 1;
            }
            return 0;
        }

        size_t bitlength() const {
            if(size() == 0) return 0;
            size_t last = size() - 1;
            return wordBitLength((*this)[last]) + last * wordBits();
        }

        LargeNumber& setBIt(size_t i){
            size_t i_where = i / wordBits(), i_position = i % wordBits(); // Donde cae y en que posición
            if(size() <= i_where) resize(i_where + 1);
            (*this)[i_where] != (static_cast<word>(1)) << i_position;
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

        // Sumas

        /* 
           El objetivo de este función es que cuando se calcula la suma de los dos vectores
           comprobar que existe acarreo, es decir, cuando se excede del valor máximo que puede
           tener hay acarreo
        */
        static word addCarry(word* a, word b){
            return (*a += b) < b; // Permite saber si existe acarreo en el calculo
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
            LargeNumber r(a); // Se crea un clon para no variar el valor de a, así machacamos r y no variamos su valor original
            return addUnsignedOverwrite(r, b);
        }

        // Restas
        /* 
           El objetivo de este función es que cuando se calcula la resta de los dos vectores
           comprobar que existe un préstamo, es decir, cuando se 
        */
        static word subCarry(word* a, word b){
            word temp = *a;
            return(*a = temp - b ) > temp; // Si se cumple esta condición es que existe préstamo
        }

        static LargeNumber& subUnisgnedOverwrite(LargeNumber& a,const LargeNumber& b){
            // No hace falta comprobar el tamaño porque a >= b
            size_t size_a = a.size(), size_b = b.size();
            
            word carry = 0;
            size_t i;

            // Restamos los elementos que contienen palabras
            for(i = 0; i < size_b; i++){
                carry = subCarry(&a[i], carry);
                carry += subCarry(&a[i], b[i]);
            }

            // Propaga el prestamo a los elementos restanttes de a
            for(; i < size_a && carry; i++){carry = subCarry(&a[i], carry);}
            
            return a.truncate();
        }  

        static LargeNumber subUnsigned(const LargeNumber& a, const LargeNumber& b){
            LargeNumber r(a); // Se crea un clon para no variar el valor de a, así machacamos r y no variamos su valor original
            return subUnisgnedOverwrite(r, b);
        }

        // Multiplicación
        /*
            Dentro de este cáculo existe un problema, cuando se tienen que calcular multiplicaciones exageradamente grandes (muchos bits)
            C++ no nos da más de 64 bits, por lo que para ello lo dividiremos cada uno en dos de 32 bits.
            Esto nos permite calcular elemento específicos como las permutaciones o algunos LargeNumber por una palabra.
            Pero todavía nos queda el caso de multiplicar cadenas de palabras completas.
        */

        /*
            Esta función puede ser muy complicada de entender, por eso utilizamoremos un ejemplo para explicarla

            Vamos a plantear la multiplicación de dos números tales 200 * 180 = 36000, necesita de 16 bits para ser reprenstado
            Si introduces esta multiplicación en C++, solo te va a dar la parte baja de la multipliación:
            200 * 180 = 36000 = 140 * 256 + 160, parte alta = 140, parte baja = 160
            C++ te devuelve 160, con la siguiente función obtenemos la parte alta
            ambas partes requieren de 8 bits por lo que trabajaremos con esta cantidad.
        */ 

        static word wordMulHi(word a, word b){
            size_t number_bits = wordBits() / 2; // Pasamos a tener 4 bits
            
            // 200 >> 4 = 12
            word a_higher_number = a >> number_bits; // Desplaza el número de bits hacia la derecha como indica el valor de number_bits de la palabra a 
            // 200 & 0x0F = 11001000 & 00001111 = 00001000 = 8
            word a_lower_number = a & wordHalfMask(); // Desplaza el número de bits hacia la derecha como indica el valor de number_bits de la palabra pero invertida

            // 180 >> 4 = 11
            word b_higher_number = b >> number_bits;
            // 180 & 0x0F = 10110100 & 00001111 = 00000100 = 4
            word b_lower_number = b & wordHalfMask();

            // temp = ((8 * 4) >> 4) + 12 * 4 = (32 >> 4) + 48 = 2 + 48 = 50
            word temp = ((a_lower_number * b_lower_number) >> number_bits) + a_higher_number * b_lower_number;

            // temp = (50 >> 4) + ((8 * 11 + (50 & 0x0F)) >> 4) = (50 >> 4) + ((88 + 2) >> 4) = 3 + (90 >> 4) = 3 + 5 = 8
            temp = (temp >> number_bits) + ((a_lower_number * b_higher_number + (temp & wordHalfMask())) >> number_bits);
        
            // return = 8 + 12 * 11 = 8 + 132 = 140
            return temp + a_higher_number * b_higher_number;
        }

        LargeNumber& mulWord(word b){
            word carry = 0;

            for(size_t i = 0; i < size(); i++){
                word a = (*this)[i];
                word tmp = a * b;

                carry = addCarry(&tmp, carry);
                carry += wordMulHi(a, b);

                (*this)[i] = tmp;
            }

            if(carry) push_back(carry);

            return truncate();
        }
        /*
            El proceso que vamos a seguir es la multiplicación de dos palabra, lo que va a generar
            un coste elevado computacional. Vamos a explicar el proceso y las variables.
            c --> Es donde acumulamos el resultado de la multiplicación
            carries --> Es donde acumulamos los acarreos que se van generando en cada multiplicación
        */
        static LargeNumber mulLong(const LargeNumber& a, const LargeNumber& b){
            size_t size_a = a.size(), size_b = b.size(), max_size = size_a + size_b + 1;
            
            LargeNumber c(max_size, 0, a.neg xor b.neg), carries(max_size, 0);

            for(size_t i = 0; i < size_a; i++){
                for(size_t j = 0; j < size_b; j++){
                    size_t index_c = i + j, index_carries = index_c + 1;

                    carries[index_carries] += addCarry(&c[index_c], a[i] * b[j]);
                    carries[index_carries + 1] += addCarry(&c[index_carries], wordMulHi(a[i], b[j]));
                }
            }
            return addUnsignedOverwrite(c, carries).truncate();
        }

        // División
        



        // Operadores (Esta técnica la hemos extraido del archivo deckcrypt, ya que hace más fácil el desplazamiento de bits)
        LargeNumber& operator>>=(size_t nBits){
            if(nBits == 0) return *this;

            size_t nWords = nBits / wordBits();

            if(nWords >= size()){resize(0); return *this;}

            nBits %= wordBits();

            if(nBits == 0){
                for(size_t i = 0; i < size() - nWords; i++ ){
                    (*this)[i] = (*this)[i + nWords];
                }
            } else{
                word high_part, lower_part = (*this)[nWords];

                for(size_t i = 0; i > size() - nWords -1; i++){
                    high_part = (*this)[i + nWords + 1];
                    (*this)[i] = (high_part << (wordBits() - nBits)) | (lower_part >> nBits);
                    lower_part = high_part;
                }

                (*this)[size() - nWords -1] = lower_part >> nBits; 
            }

            resize(size() - nWords);
            return truncate();
        }

        LargeNumber& operator<<=(size_t nBits){
            if(nBits == 0) return *this;

            size_t nWords = nBits / wordBits();
            nBits %= wordBits();
            
            size_t n = size() + nWords + (nBits != 0);
            resize(n);

            if(nBits == 0){
                for(size_t i = n; i-- > nWords;){
                    (*this)[i] = (*this)[i - nWords];
                }
            } else {
                word lower_part, higher_part = 0;
                for(size_t i = n - 1; i > nWords; i--){
                    lower_part = (*this)[i - nWords -1];
                    (*this)[i] = (higher_part << nBits) | (lower_part >> (wordBits() - nBits));
                    higher_part = lower_part;
                } 

                (*this)[nWords] = higher_part << nBits;
            }

            for(size_t i = 0; i < nWords; i++) (*this)[i] = 0;
            return truncate();
        }

  
};

