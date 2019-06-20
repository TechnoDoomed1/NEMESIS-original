///-------------------------------------------------------------------------------------------------------------------------
/// [N E M E S I S] Cipher
///-------------------------------------------------------------------------------------------------------------------------
/// Brief explanation on how it works
// Given two keys (Key1 and Key2 - with lengths L1 and L2), we store each of them in the first row of a tringular matrix
// with size L1 and L2 respectively. All new rows of each matrix are then filled with random characters, extrapolated from
// the characters of Key1 and Key2 applying multiplication and addition modulo 256. Given large enough keys, the entropy
// given by all characters of both keys is enough to produce strong pseudo-random characters.
//
// Each byte of data is then encrypted using both matrices, which are the keystreams. The result is a double keystream
// cipher, with Keystream1 and Keystream2 of length LCM(1,...,L1)=LCM(1,...,L1/2) and LCM(1,...,L2)=LCM(1,...,L2/2) each,
// functions that quickly rise to infinity (approx. 2.65^n). This allows to use 2 relatively short passphrases as the main
// security device of the cipher.
//
/// Security of the cipher
// We consider a character alphabet consisting of just letters (both upper and lower case), numbers and whitespaces to be
// used for keys (in reality, a lot more characters can be used). Given an approximation of 64 to those 63, we have that the
// security in bits of the cryptosystem in a brute force attack is 2^b = 64^(L1+L2). This gives us the following formula:
// b = log_2(64)·(L1+L2) = 6·(L1+L2)    ==>    132-bit security = two keys that are each 11 characters long
//
// Of course, given the nature of this cipher, smaller files need pretty larger keys than necessary to ensure security,
// but on the other hand, large files are easier to protect. Several files have been encrypted using the NEMESIS cipher
// and then subjected to randomness tests. All files seem to pass the tests on the DIEHARD and NIST suites.
//
/// Potential attacks on NEMESIS
// This cipher is as of yet unknown, and the author's lack of knowledge makes it rather difficult to back any claims
// concerning cryptanalysis of the cipher. The only claim that can be made has already been made (passes RNG test suites).
//
/// Explaining the encryption algorithm
// In encryption, each byte is encoded by using both keystreams. Each byte is multiplied by a random odd integer factor
// called RK based on the sum of both keystream's current character, then added Keystream1 and substracted Keystream2.
// Then, the result is yet again multiplied by another random odd integer factor.
// Decryption follows the revese approach, which is easy to do since odd integers are multiplication-invertible modulo 256.
//
// This prevents a user with knowledge of the message (but who doesn't know the keys) to recover the keys, since both
// keystreams are used for each byte thrice, once on an addition of unkown value, and twice again on a multiplication.
// It's the multiplication that throws away any hope of altering the message without knowledge of the keys. Any change
// to the ciphertext without knowing the keys results in an unpredicted character during decryption.
//
/// Message Authentication Code
// Upon ending RK+ decryption/encryption, a MAC is given based on the resulting ciphertext. If 2 MACs belonging to the
// same file don't match using the same keys, then the file has been tampered with after encryption.
//
// The probability of editing an encrypted file and getting the same MAC is 1 in 3 billion (3,368,562,317 to be exact),
// since a checksum (CS) of the ciphertext is made, and then CS is returned modulo 251, 241, 239 and 233 (all primes).
// The usage of the MAC warns of message corruption, and further prevents/discourages file tampering.
//
///===================================================================================================================================
///  LIBRARIES AND HEADERS NECESSARY FOR USE
///===================================================================================================================================
#include <stdio.h>
#include <c-upgrade.h>
//      ...which also includes <stdlib.h>, <math.h>, <stdbool.h>, <string.h> and <inttypes.h>

#include "language.h"
#include "functions.h"

///==================================================================================================================================
///  NEMESIS  Console API
///===================================================================================================================================
int main(){
    // Variable types to use.
    int MAC, size, time, bits, estimate, mode;
    char filename[40], input[120];
    byte *data;

    // Classes and objects to use.
    Datafile iofile;
    DUAL_CRYPTOSYSTEM Nemesis;


    ///----------------------------------------------------------------------------------------------------------------------
    /// OPENS THE FILE TO ENCRYPT/DECRYPT
    ///----------------------------------------------------------------------------------------------------------------------
    // Prints title on-screen.
    printf("%s", TITLE_BAR);
    puts("     N E M E S I S");
    printf("%s", TITLE_BAR);

    // Asks for the file to open.
    printf("%s", file_name_eng), scanf("%[^\n]40s", &filename), clear_buffer();
    iofile = Datafile_open(filename, "rb");

    // Ends the application if file couldn't be opened.
    if(iofile.file == NULL){
        printf("%s", file_error_eng);
        end(403);
    }

    // Otherwise, it prints on-screen the size of file in Kb or Mb.
    size = iofile.size;
    printf("%s", file_open_eng);

    if(size >= 800*MEGABYTE){
        printf("%s", file_toobig_eng);
        end(1);
    }
    else if(size>=MEGABYTE){
        printf("%5.3f Mb\n", 1.*size/(1024*1024));
    }
    else{
        printf("%5.3f Kb\n", 1.*size/1024);
    }

    ///----------------------------------------------------------------------------------------------------------------------
    /// WE EXTRACT ALL DATA FROM THE FILE, AND STORE IT IN A BYTE ARRAY
    ///----------------------------------------------------------------------------------------------------------------------
    // Prepare "data" to allocate all bytes from the file.
    data = new_array(size, sizeof(byte));

    // If there wasn't enough memory to do that, end the application.
    if(data == NULL){
        printf("%s", memory_fail_eng);
        Datafile_close(iofile);
        delete(data);
        end(-1);
    }

    // We now read the data from the opened file and assign it to "data".
    fread(data, sizeof(byte), size, iofile.file);

    ///----------------------------------------------------------------------------------------------------------------------
    /// THE USER IS THEN ASKED WHICH MODE TO USE
    ///----------------------------------------------------------------------------------------------------------------------
    // We present the available modes (RK+ Encryption / RK+ Decryption) to the user.
    printf("%s", mode_intro_eng);
    printf("%s", mode_encRKp_eng);
    printf("%s", mode_decRKp_eng);

    // Now the user has to select one of these options;
    do{ scanf("%d", &mode);}while(mode<0 || mode>1);
    clear_buffer();

    ///----------------------------------------------------------------------------------------------------------------------
    /// THEN, WE ASK FOR 2 KEYS THAT WILL GENERATE THE DOUBLE KEYSTREAMS FOR THE NEMESIS CIPHER
    ///----------------------------------------------------------------------------------------------------------------------
    // Estimate the necessary key length for the size of extracted data.
    estimate = estimate_min_pass_length(size);

    // Print the guidelines for key input.
    printf("%s", DIVISION_BAR);
    printf("%s", pass_intro1_eng);
    printf("%s", pass_intro2_eng);

    // Ask for Key1 and gather the user input.
    printf("%s%d%s", pass_phrase1_eng, estimate, pass_phrase2_eng);
    scanf("%[^\n]120s", &input), clear_buffer();
    while(strlen(input)<estimate){
        printf("%s", pass_error_eng);
        scanf("%[^\n]120s", &input), clear_buffer();
    }
    Nemesis.Key1.passphrase = String_create(0, input);

    // Now we do the same for Key2.
    printf("%s%d%s", pass_phrase1_eng, estimate, pass_phrase2_eng);
    scanf("%[^\n]120s", &input), clear_buffer();
    while(strlen(input)<estimate){
        printf("%s", pass_error_eng);
        scanf("%[^\n]120s", &input), clear_buffer();
    }
    Nemesis.Key2.passphrase = String_create(0, input);

    ///----------------------------------------------------------------------------------------------------------------------
    /// TURNING KEY1, KEY2 INTO PROPER KEYSTREAMS
    ///----------------------------------------------------------------------------------------------------------------------
    // We calculate and show to the user the estimate time it will take to perform encryption/decryption.
    // Also shows security in bits.
    bits = 6*(Nemesis.Key1.passphrase.dim + Nemesis.Key2.passphrase.dim);
    time = estimate_aprox_time_encryption(size, bits);
    printf("%s", DIVISION_BAR);
    printf("%s%d bits]\n", bitsec_eng, bits);
    printf("%s %d min %d seg\n", estime_eng, time/60, time%60);

    // Now, we must convert Key1 and K2 into Keystream1 and Keystream2.
    GetKeystreams(&Nemesis);

    ///----------------------------------------------------------------------------------------------------------------------
    /// WE NOW APPLY THE CIPHERING ALGORITHM, AND WRITE THE DATA TO THE FILE
    ///----------------------------------------------------------------------------------------------------------------------
    // Executing the encryption/decryption process, and keeping the MAC it returns.
    MAC = Nemesis_CipherAlgorithm(data, size, &Nemesis, mode);

    // Close file, then open it again for writing. If it couldn't open the file, exit with an error message.
    Datafile_close(iofile);
    iofile = Datafile_open(filename, "wb");

    if(iofile.file == NULL){
        printf("%s", file_error_eng);
        delete(data);
        String_destroy(Nemesis.Key1.passphrase);
        String_destroy(Nemesis.Key2.passphrase);
        end(403);
    }

    // Write all processed data to the file. After it's done, we close the file, this time for good.
    fwrite(data, sizeof(byte), size, iofile.file);
    Datafile_close(iofile);

    // Print MAC on-screen, and exit the application with a success message.
    printf("%s", DIVISION_BAR);
    printf("%s%s %03d-%03d-%03d-%03d\n", mac_reminder1_eng, mac_reminder2_eng, (int)MAC%233, (int)MAC%239, (int)MAC%241, (int)MAC%251);
    printf("%s", DIVISION_BAR);
    printf("%s", nemesis_green_eng);
    printf("%s", nemesis_remind_eng);

    delete(data);
    String_destroy(Nemesis.Key1.passphrase);
    String_destroy(Nemesis.Key2.passphrase);
    end(0);
}
