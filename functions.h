/** ===============================================================================================================================
    FUNCTIONS HEADER
==================================================================================================================================*/
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <c-upgrade.h>
#define TIME_MODIFIER_CONSTANT 2270

//==================================================================================================================================
//  GLOBAL VARIABLES - ODD INTEGERS AND INVERSE TABLES FOR RK+ ENCRYPTION/DECRYPTION
//==================================================================================================================================
int inverse[128]={1, 171, 205, 183, 57, 163, 197, 239, 241, 27, 61, 167, 41, 19, 53, 223, 225, 139, 173, 151, 25, 131, 165, 207,
209, 251, 29, 135, 9, 243, 21, 191, 193, 107, 141, 119, 249, 99, 133, 175, 177, 219, 253, 103, 233, 211, 245, 159, 161, 75, 109,
87, 217, 67, 101, 143, 145, 187, 221, 71, 201, 179, 213, 127, 129, 43, 77, 55, 185, 35, 69, 111, 113, 155, 189, 39, 169, 147, 181,
95, 97, 11, 45, 23, 153, 3, 37, 79, 81, 123, 157, 7, 137, 115, 149, 63, 65, 235, 13, 247, 121, 227, 5, 47, 49, 91, 125, 231, 105,
83, 117, 31, 33, 203, 237, 215, 89, 195, 229, 15, 17, 59, 93, 199, 73, 51, 85, 255};

int oddnum[128]={1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53, 55, 57, 59,
61, 63, 65, 67, 69, 71, 73, 75, 77, 79, 81, 83, 85, 87, 89, 91, 93, 95, 97, 99, 101, 103, 105, 107, 109, 111, 113, 115, 117, 119,
121, 123, 125, 127, 129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151, 153, 155, 157, 159, 161, 163, 165, 167, 169, 171,
173, 175, 177, 179, 181, 183, 185, 187, 189, 191, 193, 195, 197, 199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221, 223,
225, 227, 229, 231, 233, 235, 237, 239, 241, 243, 245, 247, 249, 251, 253, 255};

//==================================================================================================================================
//  NEW STRUCT OBJECTS
//==================================================================================================================================
object Key{
    C_String passphrase;
    byte keystream[60][120];
}KEY;

object DualCryptosystem{
    KEY Key1, Key2;
}DUAL_CRYPTOSYSTEM;

///=================================================================================================================================
///  [N E M E S I S]   CRYPTOSYSTEM FUNCTIONS
///=================================================================================================================================

void GetKeystreams (DUAL_CRYPTOSYSTEM* Nemesis){
    int length1, length2, aux, i, j, k;

    // First row of each composite Keystream is extracted from the keys.
    for(k=0; k<Nemesis->Key1.passphrase.dim; Nemesis->Key1.keystream[0][k] = Nemesis->Key1.passphrase.str[k], k++);
    for(k=0; k<Nemesis->Key2.passphrase.dim; Nemesis->Key2.keystream[0][k] = Nemesis->Key2.passphrase.str[k], k++);

    // Extract each key's length from the strings.
    length1 = Nemesis->Key1.passphrase.dim;
    length2 = Nemesis->Key2.passphrase.dim;

    // Sums and multiplies members from both keys to generate Keystream1. Each round has 1 byte less.
    for(i=1; i<((length1/2)+(length1%2)); i++){
        for(j=0; j<(length1-i); j++){
            aux  = Nemesis->Key1.keystream[i-1][j] * Nemesis->Key1.keystream[i-1][length1-i];
            aux += Nemesis->Key1.keystream[i-1][j+1] * Nemesis->Key1.keystream[i-1][(j-1<0? 0:j-1)];

            for(k=0; k<length2; k++){
                aux += Nemesis->Key2.keystream[0][k] * Nemesis->Key2.keystream[0][(k+j)%length2] * Nemesis->Key1.keystream[0][(aux+k)%length1];
            }
            Nemesis->Key1.keystream[i][j] = aux % BYTE;
        }
    }
    // The same is then done again to generate Keystream2.
    for(i=1; i<((length2/2)+(length2%2)); i++){
        for(j=0; j<(length2-i); j++){
            aux  = Nemesis->Key2.keystream[i-1][j] * Nemesis->Key2.keystream[i-1][length2-i];
            aux += Nemesis->Key2.keystream[i-1][j+1] * Nemesis->Key2.keystream[i-1][(j-1<0? 0:j-1)];

            for(k=0; k<length1; k++){
                aux += Nemesis->Key1.keystream[0][k] * Nemesis->Key1.keystream[0][(k+j)%length2] * Nemesis->Key2.keystream[0][(aux+k)%length1];
            }
            Nemesis->Key2.keystream[i][j] = aux % BYTE;
        }
    }
}
///---------------------------------------------------------------------------------------------------------------------------------

int Nemesis_CipherAlgorithm (byte* data, int size, DUAL_CRYPTOSYSTEM* Nemesis, int mode){
    int DB, k, checksum = 0, mb = 0;
    int length1, length2, stop1, stop2, RK, Keystream1_Byte, Keystream2_Byte;

    // Extract each key's length from the strings.
    length1 = Nemesis->Key1.passphrase.dim, stop1 = (length1/2)+(length1%2);
    length2 = Nemesis->Key2.passphrase.dim, stop2 = (length2/2)+(length2%2);

    /// MAIN ALGORITHM. Uses both Keystream1 and Keystream2 to encrypt/decrypt the data.
    for(DB=0; DB<size; DB++){
        // We get the corresponding bytes from each keystream.
        Keystream1_Byte = 0, Keystream2_Byte = 0;

        for(k=0; k<stop1; k++){
            Keystream1_Byte += Nemesis->Key1.keystream[k][DB%(length1-k)];
        }
        for(k=0; k<stop2; k++){
            Keystream2_Byte += Nemesis->Key2.keystream[k][DB%(length2-k)];
        }

        // Then, we calculate the RK factor. After that, we either encrypt or decrypt our data.
        Keystream1_Byte %= BYTE;
        Keystream2_Byte %= BYTE;
        RK = ((Keystream1_Byte + Keystream2_Byte)%BYTE)/2;

            /// RK+ Encryption
            if(mode == 0){
                RK = (oddnum[RK]*data[DB] +Keystream1_Byte -Keystream2_Byte)*oddnum[127-RK];
                data[DB] = mod(RK, BYTE);

                checksum += data[DB];
            }
            /// RK+ Decryption
            else{
                checksum += data[DB];

                RK = (data[DB]*inverse[127-RK] -Keystream1_Byte +Keystream2_Byte)*inverse[RK];
                data[DB] = mod(RK, BYTE);
            }

        // Keeps track of how many megabytes have been processed.
        if(DB && (DB%MEGABYTE == 0))
            printf("%s %3d\n", proc_mega_eng, ++mb);
    }

    // Return checksum of encrypted/decrypted file to create MAC (Message Authentication Code).
    return checksum;
}

//==================================================================================================================================
//  AUXILIARY FUNCTIONS
//==================================================================================================================================
/** Estimates the necessary key-length to secure the message by using  LCM(N,...,1) = LCM(N,...,N/2) ~= 2.65^N  */
int estimate_min_pass_length (int length){
    int k; double f=2.65;

    for(k=2; f<length; f*=2.65, k++);
    return k;
}

/** Estimates the time it would take to encrypt the data on the computer the NEMESIS cypher was compiled. */
int estimate_aprox_time_encryption (int length, int bits){
    return 1+((length/1024)*bits)/(TIME_MODIFIER_CONSTANT*1024);
}

///---------------------------------------------------------------------------------------------------------------------------------
#endif
