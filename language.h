/** ==============================================================================================================================
    LANGUAGE DEFINITION HEADER
=================================================================================================================================*/
#ifndef LANGUAGE_H_
#define LANGUAGE_H_

//==================================================================================================================================
//  ENGLISH STRINGS
//==================================================================================================================================
#define file_name_eng       ">> Please, introduce the filename (extension included) to work with:\n"
#define file_error_eng      "File error - Couldn't open file with designated filename.\n"
#define file_open_eng       ">> Opening file of "
#define file_toobig_eng     "The file you tried to open is too big for C to handle.\nThis program has been terminated.\n"

#define memory_fail_eng     "Memory error - File is too long for RAM to handle.\n"

#define mode_intro_eng      "\n>> ENCRYPTION MODE - Select one of the following operating modes.\n"
#define mode_encRKp_eng     "    0: RK+ Encryption\n"
#define mode_decRKp_eng     "    1: RK+ Decryption\n"

#define pass_intro1_eng     "   (Combine letters, numbers and symbols to avoid dictionary attacks.\n"
#define pass_intro2_eng     "    Also, avoid using keys with repetitive patterns.)\n\n"
#define pass_phrase1_eng    ">> Introduce an encryption key of at least "
#define pass_phrase2_eng    " characters:\n"
#define pass_error_eng      ">> The key you entered is not long enough. Try again:\n"

#define bitsec_eng          "\n>> Preparing cycle of cryptographic keys... [SECURITY: "
#define estime_eng          ">> Starting data processing... ESTIMATED TIME:"
#define proc_mega_eng       "Processed megabytes:"

#define mac_reminder1_eng   ">> Message Authentication Code (if the decrypted message's code is any\n"
#define mac_reminder2_eng   "   different, the file has been modified):"

#define nemesis_error_eng   "ERROR - Nemesis couldn't complete the requested process.\n"
#define nemesis_green_eng   ">> Nemesis has finished succesfully.\n  * IMPORTANT! Remember both keys or you won't be able to decrypt your files.\n"
#define nemesis_remind_eng  "    Also, don't forget to write down the MAC somewhere! *\n"

//==================================================================================================================================
//  SPANISH STRINGS
//==================================================================================================================================
#define file_name_spa       ">> Por favor, introduce el nombre del fichero de texto (con la extension):\n"
#define file_error_spa      "Error de archivo - No se pudo leer el archivo especificado.\n"
#define file_open_spa       ">> Abriendo archivo de "
#define file_toobig_spa     "Archivo demasiado grande para lenguaje de programacion C.\nPrograma terminado.\n"

#define memory_fail_spa     "Error de memoria - Archivo demasiado largo para la RAM.\n"

#define mode_intro_spa      "\n>> MODO DE ENCRIPTACION - Selecciona uno de los siguientes modos.\n"
#define mode_encRKp_spa     "    0: Encriptacion RK+\n"
#define mode_decRKp_spa     "    1: Desencriptacion RK+\n"

#define pass_intro1_spa     "   (Usa letras, numeros y simbolos para evitar ataques de diccionario.\n"
#define pass_intro2_spa     "    Evita tambien el uso de claves con patrones repetitivos.)\n\n"
#define pass_phrase1_spa    ">> Escribe una 1a clave de encriptacion de al menos "
#define pass_phrase2_spa    " caracteres:\n"
#define pass_error_spa      ">> La clave no es lo suficientemente larga. Prueba de nuevo:\n"

#define bitsec_spa          "\n>> Preparando ciclo de claves criptograficas... [SEGURIDAD: "
#define estime_spa          ">> Empezando procesado de datos... TIEMPO ESTIMADO: "
#define proc_mega_spa       "Megabytes procesados:"

#define mac_reminder1_spa   ">> Codigo de autentificacion (si el codigo del mensaje desencriptado\n"
#define mac_reminder2_spa   "   no coincide, el fichero ha sido editado): "

#define nemesis_error_spa   "ERROR - No se pudo completar la (des)encriptacion.\n"
#define nemesis_green_spa   ">> Programa terminado con exito.\n  * IMPORTANTE! Recuerda ambas claves o no podras recuperar tus datos.\n"
#define nemesis_remind_spa  "    Recuerda tambien apuntar el MAC! *\n"

///---------------------------------------------------------------------------------------------------------------------------------
#endif
