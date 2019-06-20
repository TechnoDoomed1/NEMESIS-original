/** ==============================================================================================================================
    C-UPGRADE LIBRARY (made by Eric Santiño - aka Doom Missed)
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    This library tries to implement C++ classes to C. Why do this? Because C++ is usually slower, harder to learn/understand,
    and MOSTLY, because it's much less pleasent for the reader to read and understand than C is. Of course, C has its own share
    of problems... tons of them. This library aims to remove several of those problems, and add power to C while doing so.

    NEW ADDITIONS:
        * some new definitions, functions and variable types
        * standard classes a la C++ (list, vector, string, datafile)
        * easier to use dynamic memory allocation and management
        * built-in interactive documentation, which can be called using C_Upgrade_IDC() or CallIDC()

=================================================================================================================================*/
#ifndef C_UPGRADE_H__
#define C_UPGRADE_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

///=================================================================================================================================
/// DEFINITIONS
///=================================================================================================================================
// Useful renaming shortcurts for the programmer.
#define object                      typedef struct
#define int64                       int64_t
#define uint64                      uint64_t
#define d64                         PRI64
#define u64                         PRIu64

#define AND                         &&
#define OR                          ||
#define XOR                         ^
#define EQUALS                      ==

#define end(a)                      await(FALSE); return a;
#define certificate(a)              if(a==NULL){ puts("ERROR - Not enough memory could be allocated"); await(FALSE); return -1;}


// Typical definitions that could be useful for the programmer.
#define STRING_END                 -1
#define INVALID                    -1
#define FALSE                       false
#define TRUE                        true
#define E                           2.71828182845904523536
#define GOLDEN_RATIO                1.61803398874989484820
#define PI                          M_PI


// Other definitions that may come in handy.
#define TITLE_BAR                   "===========================================================================\n"
#define DIVISION_BAR                "---------------------------------------------------------------------------\n"
#define LARGE_UNDER_BAR             "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
#define SHORT_BAR                   "-------------------------------------------\n"
#define UNDER_BAR                   "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
#define BYTE                        256
#define KILOBYTE                    1024
#define MEGABYTE                    1048576
#define GIGABYTE                    1073741824

// Internal definitions for the C-upgrade Library (don't use or change these values).
#define MAX_POINTERS_FOR_KILLER     100000

///=================================================================================================================================
/// NEW VARIABLE TYPES
///=================================================================================================================================
// Defines a byte type (value range is from 0 to 255)
typedef unsigned char byte;

///=================================================================================================================================
/// NEW AUXILIARY FUNCTIONS
///=================================================================================================================================
// Forces the result of operation a%b to be positive.
#define mod(a, b) (a<0? b+(a%b):a%b);

// Custom power function for integral types, because pow() from math.h tends to be error prone.
int intpow (int n, int exponent){
    int k, result = n;

    if(exponent == 0)
        return 1;
    else if(exponent == 1)
        return n;

    for(k=1; k<exponent; k++){
        result *= n;
    }
    return result;
}

// Deduces if given integer number is prime or not.
bool is_prime (int num){
    int k;

    if(num < 2)
        return FALSE;
    else if(num == 2 || num == 3 || num == 5 || num == 7 || num == 11)
        return TRUE;
    else{
        for(k=2; is_prime(k) && k*k<num; k++){
            if(num % k == 0)
                return FALSE;
        }
        return TRUE;
    }
}

// Collects any garbage still left in the standard input stream.
void clear_buffer (){
    char c;

    do{
        c = getchar();
    }while(c != '\n');
}

// Portable code for system("pause").
void await (bool message){
    if(message==TRUE)
        printf("[Press ENTER to continue...]");

    getchar();
}

///=================================================================================================================================
/// DYNAMIC MEMORY ALLOCATION       -   GENERIC
///=================================================================================================================================
// NEW_ARRAY (removes the need for calls to malloc or calloc doing by so internally.)
//----------------------------------------------------------------------------------------
/// new_array: Creates a new array from pointer.
void* new_array (int dim, int size){
    void *parray = NULL;

    parray = malloc(dim * size);
    return parray;
}

// DELETE (frees memory from any dynamically allocated array or matrix). All pointers are also set to NULL.
//-----------------------------------------------------------------------------------------------------------
#define delete(a) free(a); a = NULL;

///---------------------------------------------------------------------------------------------------------------------------------
/// POINTER KILLER
// This new object type can be used if the programmer desires so. Each time an operation allocating dynamic memory is performed,
// the PointerKiller can be told to collect the pointer. Once extermination is ordered through the PointerKiller, all gathered
// pointers are delete'd.
// PointerKiller can keep up to MAX_POINTERS_FOR_KILLER pointers (default 100000).
///---------------------------------------------------------------------------------------------------------------------------------

object PointerKiller_Class{
    void **pointers[MAX_POINTERS_FOR_KILLER];
    int last;
}PointerKiller_Class;
static PointerKiller_Class PointerKiller;

#define PointerKiller_collect(a)    fpointerkiller_collect(&a)
#define PointerKiller_exterminate   fpointerkiller_exterminate
#define PointerKiller_certificate(a) if(a==NULL){ puts("ERROR - Not enough memory could be allocated"); await(FALSE); return -1;} \
                                     else{ fpointerkiller_collect(&a);}


//-------------------------------------------------------------------------
void fpointerkiller_collect (void** pointer){
    if((pointer != NULL) && (PointerKiller.last < MAX_POINTERS_FOR_KILLER)){
        PointerKiller.pointers[PointerKiller.last] = pointer;
        PointerKiller.last++;
    }
}
void fpointerkiller_exterminate (){
    int k;
    for(k=0; k < PointerKiller.last; k++){
        if(PointerKiller.pointers[k] != NULL){
            free(*PointerKiller.pointers[k]);
            *PointerKiller.pointers[k] = NULL;
        }
    }
    PointerKiller.last = 0;
}

///=================================================================================================================================
/// STANDARD CLASSES
///=================================================================================================================================
/// LIST class
object C_List {
    int dim, $RealDimensionForPointer;
    int *list;
}C_List;

#define List                        C_List
#define List_create                 flist_new
#define List_destroy(a)             flist_destroy(&a)
#define List_last(a)                flist_last(&a)
#define List_empty(a)               flist_empty(&a)
#define List_clear(a)               flist_clear(&a)
#define List_erase(a,b)             flist_erase(&a,b)
#define List_swap(a,b,c)            flist_swap(&a,b,c)
#define List_insert(a,b,c)          flist_insert(a,&b,c)
#define List_pushback(a,b)          flist_pushback(a,&b)


/// PSEUDO-CONSTRUCTOR
// Creates a new instance of the C_List class for assignment.
C_List flist_new (int N) {
    int k;
    List baselist;

    baselist.$RealDimensionForPointer = (N<50? 50+N:(3*N)/2);
    baselist.list = new_array(50+N, sizeof(int));

    if(baselist.list != NULL){
        baselist.dim = N;
        for(k=0; k<N; baselist.list[k]=0, k++);
    }
    else
        baselist.dim = 0;

    return baselist;
}

/// List Destructor
// Deletes internal array and sets dimension to 0.
void flist_destroy (C_List* objList) {
    delete(objList->list);
    objList->dim = 0;
}

/// CLASS METHODS
// Returns last member of the list.
int flist_last (C_List* objList){
    return objList->list[objList->dim - 1];
}
// Returns TRUE if list is empty, FALSE otherwise.
bool flist_empty (C_List* objList){
    if(objList->dim == 0)
        return TRUE;
    else
        return FALSE;
}
// All data in the list is erased to 0.
void flist_clear (C_List* objList){
    int k;
    for(k=0; k<objList->dim; objList->list[k++] = 0);
}
// Same as clear(), but from position "pos" until end of list.
void flist_erase (C_List* objList, int pos){
    int k;
    for(k=pos; k<objList->dim; objList->list[k++] = 0);
}
// Swaps two elements of the list, interchanging places.
void flist_swap (C_List* objList, int index1, int index2){
    int temp;

    temp = objList->list[index1];
    objList->list[index1] = objList->list[index2];
    objList->list[index2] = temp;
}
// Insert an element to the list at position "index". All other entries are shifted one position to the right.
// No element is lost in the transition, but may throw an error if memory allocation fails.
void flist_insert (int value, C_List* objList, int index){
    int k;

    if((objList->dim + 1) > objList->$RealDimensionForPointer){
        puts("List_insert(): ERROR, not enough memory");
        return ;
    }

    objList->dim++;

    for(k=index+1; k<=objList->dim; objList->list[k] = objList->list[k-1], k++);
    objList->list[index] = value;
}
// Same as insert, but always on the end of the list.
void flist_pushback (int value, C_List* objList){
    flist_insert(value, objList, objList->dim);
}

///---------------------------------------------------------------------------------------------------------------------------------
/// VECTOR class
object C_Vector {
    int dim, $RealDimensionForPointer;
    double* vect;
}C_Vector;

#define Vector                      C_Vector
#define Vector_create               fvect_new
#define Vector_destroy(a)           fvect_destroy(&a)
#define Vector_last(a)              fvect_last(&a)
#define Vector_empty(a)             fvect_empty(&a)
#define Vector_clear(a)             fvect_clear(&a)
#define Vector_erase(a)             fvect_erase(&a)
#define Vector_swap(a,b,c)          fvect_swap(&a,b,c)
#define Vector_insert(a,b,c)        fvect_insert(a,&b,c)
#define Vector_pushback(a,b)        fvect_pushback(a,&b)
#define Vector_module(a)            fvect_module(&a)


/// PSEUDO-CONSTRUCTOR
// Creates a new instance of the C_Vector class for assignment.
C_Vector fvect_new (int N) {
    int k;
    Vector basevect;

    basevect.$RealDimensionForPointer = (N<50? 50+N:(3*N)/2);
    basevect.vect = new_array(basevect.$RealDimensionForPointer, sizeof(double));

    if(basevect.vect != NULL){
        basevect.dim = N;
        for(k=0; k<N; basevect.vect[k]=0., k++);
    }
    else
        basevect.dim = 0;

    return basevect;
}

/// Vector Destructor
// Deletes internal array and sets dimension to 0.
void fvect_destroy (C_Vector* objVect) {
    delete(objVect->vect);
    objVect->dim = 0;
}

/// CLASS METHODS
// Returns last element of the vector.
double fvect_last (C_Vector* objVect){
    return objVect->vect[objVect->dim - 1];
}
// Returns TRUE if empty, and FALSE otherwise.
bool fvect_empty (C_Vector* objVect){
    if(objVect->dim == 0)
        return TRUE;
    else
        return FALSE;
}
// Clears all elements of the vector by turning them into 0.
void fvect_clear (C_Vector* objVect){
    int k;
    for(k=0; k<objVect->dim; objVect->vect[k++] = 0.);
}
// Same as clear(), but from position "pos" until end of vector.
void fvect_erase (C_Vector* objVect, int pos){
    int k;
    for(k=pos; k<objVect->dim; objVect->vect[k++] = 0.);
}
// Insert an element to the vector at position "index". All other entries are shifted one position to the right.
// No element is lost in the transition, but may throw an error if memory allocation fails.
void fvect_insert (int value, C_Vector* objVector, int index){
    int k;

    if((objVector->dim + 1) > objVector->$RealDimensionForPointer){
        puts("List_insert(): ERROR, not enough memory");
        return ;
    }

    objVector->dim++;

    for(k=index+1; k<=objVector->dim; objVector->vect[k] = objVector->vect[k-1], k++);
    objVector->vect[index] = value;
}
// Same as insert, but always on the end of the vector.
void fvect_pushback (int value, C_Vector* objVector){
    fvect_insert(value, objVector, objVector->dim);
}
// Returns the Eucliden Norm of the vector.
double fvect_module (C_Vector* objVect){
    int k; double result=0.;

    for(k=0; k<objVect->dim; result+=objVect->vect[k]*objVect->vect[k], k++);
    return sqrt(result);
}

///---------------------------------------------------------------------------------------------------------------------------------
/// STRING class
object C_String {
    int dim;
    char* str;
}C_String;

#define String                      C_String
#define String_create               fstr_new
#define String_destroy(a)           fstr_destroy(&a)
#define String_empty(a)             fstr_empty(&a)
#define String_clear(a)             fstr_clear(&a)
#define String_erase(a)             fstr_erase(&a)
#define String_swap(a,b,c)          fstr_swap(&a,b,c)
#define String_find(a,b,c)          fstr_find(&a,b,c)
#define String_extract(a,b,c)       fstr_extract(&a,b,c)
#define String_append(a,b,c)        fstr_append(a,&b,c)
#define String_concatenate(a,b)     fstr_concatenate(&a,&b)
#define String_compare(a,b)         fstr_compare(&a,&b)
#define String_print(a)             printf("%s\n", a.str);

/// PSEUDO-CONSTRUCTOR
// Creates a new instance of the C_String class for assignment.
C_String fstr_new (int N, char* varstr) {
    int k;
    String basestr;

    basestr.dim = (N>0? N:strlen(varstr));
    basestr.str = new_array(basestr.dim + 1, sizeof(char));

    if(basestr.str != NULL){
        if(varstr != NULL){
            for(k=0; varstr!=NULL && varstr[k]!='\0' && k<basestr.dim; basestr.str[k] = varstr[k], k++);
            basestr.str[basestr.dim]='\0';
        }
        else{
            for(k=0; k<basestr.dim; basestr.str[k] = ' ', k++);
            basestr.str[basestr.dim]='\0';
        }
    }
    else
        basestr.dim = 0;

    return basestr;
}

/// String Destructor
// Deletes internal array and sets dimension to 0.
void fstr_destroy (C_String* objString) {
    delete(objString->str);
    objString->dim = 0;
}

/// TYPICAL CLASS METHODS
// Returns character at the end of string.
bool fstr_empty (C_String* objString){
    if(objString->dim == 0)
        return TRUE;
    else
        return FALSE;
}
// All characters in the string become whitespaces.
void fstr_clear (C_String* objString){
    int k;
    for(k=0; k<objString->dim; objString->str[k++] = ' ');
}
// From position "pos" until end of string, clear() happens.
void fstr_erase (C_String* objString, int pos){
    int k;
    for(k=pos; k<objString->dim; objString->str[k++] = ' ');
}
// Swaps place of two characters inside the string.
void fstr_swap (C_String* objString, int index1, int index2){
    int temp;

    temp = objString->str[index1];
    objString->str[index1] = objString->str[index2];
    objString->str[index2] = temp;
}

/// STRING-ONLY CLASS METHODS
// Finds a portion of text inside the string. Value returned may vary, consult the IDC.
int fstr_find (C_String* objString, char* text, int value){
    int i, j, length = strlen(text);

    for(i=0; i<objString->dim; i++){
        for(j=0; i+j<objString->dim && j<length && objString->str[i+j] == text[j]; j++);
        if(j==length && value!=INVALID && value!=0)
            return value;
        else if(j==length)
            return i;
    }
    return INVALID;
}
// Returns FALSE if two strings are different, and TRUE is they are equal.
bool fstr_compare (C_String* objString1, C_String* objString2){
    if(objString1->dim != objString2->dim)
        return FALSE;
    else if(strcmp(objString1->str, objString2->str) != 0)
        return FALSE;
    else
        return TRUE;
}
// Appends a portion of text to a string, and returns the value for assignment.
C_String fstr_append (char* newstr, C_String* objString, int pos){
    int k, length = strlen(newstr);
    C_String new_objString;

    if(pos==STRING_END)
        pos = objString->dim;

    new_objString.dim = objString->dim + length;
    new_objString.str = new_array(new_objString.dim + 1, sizeof(char));

    if(new_objString.str != NULL){
        for(k=0; k<pos; new_objString.str[k] = objString->str[k], k++);
        for(k=pos; k<pos+length; new_objString.str[k] = newstr[k-pos], k++);
        for(k=pos+length; k<new_objString.dim; new_objString.str[k] = objString->str[k-length], k++);
        new_objString.str[new_objString.dim]='\0';
    }
    else{
        new_objString.dim = 0;
    }
    return new_objString;
}
// Concatenates two strings together, and returns the value for assignment.
C_String fstr_concatenate (C_String* string1, C_String* string2){
    return fstr_append(string2->str, string1, STRING_END);
}
// Returns extracted text from first instance of tag "start", to first instance of tag "end", inside passed string.
C_String fstr_extract (C_String* objString, char* tag_start, char* tag_endit){
    int pos_start, pos_endit, k;
    C_String extracted;

    if(tag_start != NULL)
        pos_start = fstr_find(objString, tag_start, 0) + strlen(tag_start);
    else
        pos_start = 0;

    if(tag_endit != NULL)
        pos_endit = fstr_find(objString, tag_endit, 0);
    else
        pos_endit = objString->dim;


    if(pos_start!=INVALID && pos_endit!=INVALID){
        extracted.dim = pos_endit-pos_start;
        if(pos_endit-pos_start > 0){
            extracted.str = new_array(pos_endit-pos_start + 1, sizeof(char));
        }
        else
            extracted.str = NULL;

        if(extracted.str != NULL){
            for(k=pos_start; k<pos_endit; extracted.str[k-pos_start] = objString->str[k], k++);
            extracted.str[extracted.dim]='\0';
        }
        else
            extracted.dim = 0;
    }
    else{
        extracted.dim = 0;
    }
    return extracted;
}

///---------------------------------------------------------------------------------------------------------------------------------
/// DATAFILE class
object C_Datafile {
    FILE* file;
    char* name;
    uint64 size;
}C_Datafile;

#define Datafile                        C_Datafile
#define Datafile_open                   fdatafile_new
#define Datafile_close(a)               fdatafile_destroy(&a)
#define Datafile_print(a,b)             fprintf(a.file, "%s", b)

/// PSEUDO-CONSTRUCTOR
// Opens a file, much like fopen() does.
C_Datafile fdatafile_new (char* filename, char* mode) {
    Datafile(basedatafile);

    basedatafile.file = fopen(filename, mode);
    if(basedatafile.file != NULL){
        fseek(basedatafile.file, 0, SEEK_END);
        basedatafile.size = ftell(basedatafile.file);
        rewind(basedatafile.file);

        basedatafile.name = filename;
    }
    else{
        basedatafile.size = 0;
        basedatafile.name = filename;
    }

    return basedatafile;
}

/// DESTRUCTOR
// Closes file, much like fclose() does.
void fdatafile_destroy (C_Datafile* openfile) {
    delete(openfile->name);
    openfile->size = 0;

    if(openfile->file != NULL)
        fclose(openfile->file);
    openfile->file = NULL;
}

///=================================================================================================================================
/// INTERACTIVE DOCUMENTATION CENTER
///=================================================================================================================================
// The C-upgrade Library comes with a neat function called the "Interactive Documentation Center".
// Just call it in a debugging session of your program to use it. The Interactive Documentation Center (IDC from now on), will
// expect user input, and according to that input, will print on-screen all documentation about the requested data.
//
// The IDC serves as build-in documentation for every new feature that C-upgrade Library has to offer.

void C_Upgrade_IDC (void){
    char input[100]; C_String strin;

    printf("%s", TITLE_BAR);
    puts(">> Welcome to the \"Interactive Documentation Center\" for the C-upgrade lib.");
    puts(">> Enter any of the following to open their respective documentation:");
    puts("   (faq, new, delete, functions, list, vector, string, datafile)\n");
    puts(">> Enter 'quit' or 'exit' anytime to close IDC.");
    printf("%s", TITLE_BAR);
    while(TRUE){
        printf(">>> "), scanf("%[^\n]100s", &input), clear_buffer();
        strin = String_create(0, input);
        printf("%s", TITLE_BAR);

        if((strin.str!=NULL) && (String_find(strin, "quit", 0)!=INVALID))
            break;

        else if((strin.str!=NULL) && (String_find(strin, "exit", 0)!=INVALID))
            break;

        else if((strin.str!=NULL) && (String_find(strin, "faq", 0)!=INVALID)){
            puts("** Frequently Asked Questions **\n");
            puts("1) WHAT'S NEW?");
            puts("   Quite a lot. The C-upgrade library tries to bring some power to the C");
            puts("   language so that it can compete with more modern languages like C++,");
            puts("   but without sacrificing anything and without extra complication. While");
            puts("   it adds nothing new to the language itself, it makes life easier and");
            puts("   is a nifty addition for C. Also, it is completely portable.\n");
            puts("   Among the new additions, we have:");
            puts("   -> Easier way of declaring and managing dynamic memory.");
            puts("   -> strin garbage collection and portable system(\"pause\").");
            puts("   -> Implementation of standard classes similar to the C++ ones.");
            puts("   -> Embedded interactive documentation for the library.\n");
            puts("2) WHY USE IT?");
            puts("   The library itself is small, and requires no changes to how things");
            puts("   are coded or done normally. All features are optional, and are never");
            puts("   forced upon the programmer. Furthermore, all features it provides are");
            puts("   easy to use and pretty similar to the standard classes that C++ use");
            puts("   like vectors, or strings.\n");
            puts("3) HOW DO I USE THESE NEW FEATURES?");
            puts("   For each new feature you would like to use, consult the IDC. All");
            puts("   necessary documentation is archived here.\n");
            puts("4) CAN I DEFINE MY OWN CLASSES?");
            puts("   Yes, and no. C doesn't let anyone define custom classes, so all that");
            puts("   has been done is create structs and defining function renamings to");
            puts("   emulate the functionality of C++ classes. Still, this approach is far");
            puts("   from perfect, and has several limitations:\n");
            puts("   -> All is user-coded, with no inheritance or polymorphism.");
            puts("   -> No overloading of any kind, C doesn't allow it.");
            puts("   -> All class members are public by default.");
            puts("   -> Somewhat different syntax from C++ or Java.");
            puts("   -> All instances of a class must be manually deconstructed.\n");
            puts("   If you are interested in creating a custom class for your project,");
            puts("   type \"tutorial custom classes\" in the IDC. Be advised, it only");
            puts("   explains how to create classes similar to how they were done in");
            puts("   this library. There are better yet more complex methods, just look");
            puts("   in the Internet for a bit.\n");
        }

        else if((strin.str!=NULL) && (String_find(strin, "new", 0)!=INVALID)){
            puts("// AUTOMATIC MEMORY ALLOCATION");
            puts("// Emulating C++ \"new\" operator\n");

            puts("   void* new_array(int dim, int size)");
            printf("   %s", UNDER_BAR);
            puts("   New Function! Creates a dynamic array from pointer. Second parameter");
            puts("   must be \"size\" which should take the form be sizeof(type).\n");

            puts("   byte example;");
            printf("   %s", UNDER_BAR);
            puts("   New variable type! Can hold values from 0 to 255. Specially useful for");
            puts("   raw file modification, like cryptographic applications. It is really");
            puts("   just a renaming of \"unsigned char\" which was a misnomer name anyway.");
        }

        else if((strin.str!=NULL) && (String_find(strin, "delete", 0)!=INVALID)){
            puts("// AUTOMATIC MEMORY ALLOCATION");
            puts("// Emulating C++ \"delete\" operator\n");

            puts("   delete(void* parray)");
            printf("   %s", UNDER_BAR);
            puts("   Macro renaming of \"free\" for array pointers. Has the extra benefit of");
            puts("   also setting the pointer to NULL after calling free(), so it's safe and");
            puts("   doesn't crash for trying to delete already delete'd memory.\n");

            puts("   PointerKiller\t[METHODS: _collect()   _exterminate()]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   New Class! It's a list of all pointer used for allocating dynamic memory.");
            puts("   Each call to PointerKiller_collect(var) will add the \"var\" pointer to");
            puts("   its list. The exterminate() command will 'delete' all pointers in its list.\n");
        }

        else if((strin.str!=NULL) && (String_find(strin, "functions", 0)!=INVALID)){
            puts("   mod (int a, int b)");
            printf("   %s", UNDER_BAR);
            puts("   Macro renaming for a % b, but forces results to be positive. As it's");
            puts("   a macro, only variables (and not expressions) should be passed to it.\n");

            puts("   int intpow (int n, int exponent)");
            printf("   %s", UNDER_BAR);
            puts("   New Function! Use this one instead of math.h pow() to calculate powers");
            puts("   of integers. pow() may fail randomly giving unprecise double results.\n");

            puts("   bool is_prime (int num)");
            printf("   %s", UNDER_BAR);
            puts("   New Function!");
            puts("   Returns TRUE if given number is prime, and returns FALSE if otherwise.\n");

            puts("   void clear_buffer ()");
            printf("   %s", UNDER_BAR);
            puts("   New Function! Collects any garbage left in the standard input stream.");
            puts("   Should have been part of the standard a long time ago, since fflush");
            puts("   is unreliable (catching a trend already?)\n");

            puts("   void await (bool message)");
            printf("   %s", UNDER_BAR);
            puts("   New Function! Portable code that emulates system(\"pause\") by waiting");
            puts("   user input to advance. If message is TRUE, then the user is informed ");
            puts("   that they must press ENTER to continue. Completely portable.\n");
        }

        else if((strin.str!=NULL) && (String_find(strin, "list", 0)!=INVALID)){
            puts("// New Class! (List)");
            printf("%s", SHORT_BAR);
            puts("   (*) New instances of the class can be created with List.");
            puts("   example: List foobar;\n");
            puts("   (*) For destructor and class methods, just use List_function().");
            puts("   example: foobar = List_swap(foobar, 3, 5);\n\n");
            puts("// Class Members");
            printf("%s", SHORT_BAR);
            puts("   List.list\t(internal dynamic INT array)");
            puts("   List.dim\t(size of that array)\n\n");

            puts("// Class Inherent Functions");
            printf("%s", SHORT_BAR);
            puts("   [List\tList_create(int dim)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Sets size of the list, and initializes all elements inside the class");
            puts("   internal array to 0. Any previous data stored is deleted.\n");
            puts("   [void\tList_destroy(List* objList)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Class destructor. Frees internal dynamic memory and resets size to zero.");
            puts("   Always must be called for each instance of the class if you wish to free");
            puts("   its allocated memory.\n");
            puts("   [int\t\tList_last(List* objList)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Returns last member in the list.\n");
            puts("   [bool\tList_empty(List* objList)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Returns TRUE if list.dim = 0, and FALSE if not. Note that newly created");
            puts("   variables without use of the constructor don't have 0 dimension by default.\n");
            puts("   [void\tList_clear(List* objList)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Resets all values in the list to zero.\n");
            puts("   [void\tList_erase(List* objList, int pos)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Resets all values in the list, from position \"pos\" onwards, to zero.\n");
            puts("   [void\tList_swap(List* objList, int pos1, int pos2)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Swaps members inside the list in the specified positions. A permutation");
            puts("   of the original list is obtained this way.\n");
            puts("   [bool\tList_insert(int value, List* objList, int pos)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Insert member \"value\" at position \"pos\" of the list. All members from");
            puts("   that position until the end are shifted one position to the right. If the");
            puts("   allocation was unsuccesful, returns FALSE. On success, returns TRUE.\n");
            puts("   [bool\tList_pushback(int value, List* objList]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Adds element \"value\" to the end of the list. Returns TRUE if allocation");
            puts("   was succesfull, and FALSE on failure.\n");
        }

        else if((strin.str!=NULL) && (String_find(strin, "vector", 0)!=INVALID)){
            puts("// New Class! (Vector)");
            printf("%s", SHORT_BAR);
            puts("   (*) New instances of the class can be created with Vector.");
            puts("   example: Vector test;\n");
            puts("   (*) For destructor and class methods, just use Vector_function().");
            puts("   example: test = Vector_insert(PI, test, 0);\n\n");

            puts("// Class Members");
            printf("%s", SHORT_BAR);
            puts("   Vector.vect\t(internal dynamic DOUBLE array)");
            puts("   Vector.dim\t\t(size of that array)\n\n");

            puts("// Class Inherent Functions");
            printf("%s", SHORT_BAR);
            printf("%s", SHORT_BAR);
            puts("   [double\tVector_module(Vector* objVector)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Returns the euclidean module of the N-dimensional vector, where N is");
            puts("   the number of elements the vector contains.\n");
            puts("   Lists and Vectors share similar functions, to the point they are the");
            puts("   same except for type renaming. Type \"list\" for more information.");
        }

        else if((strin.str!=NULL) && (String_find(strin, "string", 0)!=INVALID)){
            puts("// New Class! (String)");
            printf("%s", SHORT_BAR);
            puts("   (*) New instances of the class can be created with String.");
            puts("   example: String good_day;\n");
            puts("   (*) For destructor and class methods, just use String_function().");
            puts("   example: good_day = String_create(0, \"Have a nice day!\");\n\n");

            puts("// Class Members");
            printf("%s", SHORT_BAR);
            puts("   String.str\t\t(internal dynamic CHAR array)");
            puts("   String.dim\t\t(size of that array)\n\n");

            puts("// Class Inherent Functions");
            printf("%s", SHORT_BAR);
            puts("   [String\tString_create(int dim, char* parray)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Expects size of the list, and if given a dynamic array it takes as much");
            puts("   values as possible from it. Note that if \"dim\" is larger than the size");
            puts("   of parray, results are undefined. Should parray not be NULL, and dim = 0");
            puts("   then dimension is set automatically to acommodate the passed string.\n");
            puts("   [void\tString_destroy(String* objStr)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Class destructor. Frees internal dynamic memory and resets size to zero.");
            puts("   Always must be called for each instance of the class if you wish to free");
            puts("   its allocated memory.\n");
            puts("   [bool\tString_empty(String* objStr)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Returns TRUE if string.dim = 0, and FALSE if not. Note that newly created");
            puts("   variables without use of the constructor don't have 0 dimension by default.\n");
            puts("   [void\tString_clear(String* objStr)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   All characters in the internal char string become whitespace.\n");
            puts("   [void\tString_erase(String* objStr, int pos)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Same as \"String_clear()\", but from \"pos\" until end of string.\n");
            puts("   [void\tString_swap(String* objStr, int pos1, int pos2)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Swaps characters of internal char string at the specified positions.\n\n");
            puts("   [int\t\tString_find(String* objStr, char* str, int value)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Returns \"value\" if it finds that \"str\" is inside the given C_String.");
            puts("   If value is either zero or INVALID (-1), then the position iterator of");
            puts("   objStr in which it was find is returned.\n");
            puts("   [String\tString_extract(C_String* objStr, char* start, char* end)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Returns a String with all data extracted from \"start\" to \"end\" in the");
            puts("   passed string object parameter. Similar to something like this:\n");
            puts("   find(\"<data>x64t</data>\", \"<data>\", \"</data>\") == \"x64t\"\n\n");
            puts("   [bool\tString_compare(String* objStr1, String* objStr2)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Returns TRUE if the strings are equal, and FALSE otherwise.\n");
            puts("   [String\tString_append(char* text, String* objStr, int pos)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Appends char string \"text\" to the passed String at the stated position.");
            puts("   No change is made upon the passed C_String parameter, instead returns a");
            puts("   new string with all the changes.\n");
            puts("   [String\tString_concatenate(String* objStr1, String* objStr2)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Concatenates the strings into a new string which is then returned.\n\n");
            puts("   [void\tString_print(String var)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Prints the String on the screen, ending with a new line character.\n");
        }

        else if((strin.str!=NULL) && (String_find(strin, "datafile", 0)!=INVALID)){
            puts("// New Class! (Datafile)");
            printf("%s", SHORT_BAR);
            puts("   (*) New instances of the class can be created with Datafile.");
            puts("   example: Datafile(inputfile);\n");
            puts("   (*) Class methods. Accessed using Datafile_function().");
            puts("   example: inputfile = Datafile_open(\"data.txt\",\"rb\");\n\n");

            puts("// Class Members");
            printf("%s", SHORT_BAR);
            puts("   Datafile.file\t\t(FILE stream to be opened)");
            puts("   Datafile.name\t\t(char* that keeps name of file)");
            puts("   Datafile.size\t\t(size of the file's data in 64-bit int)\n\n");

            puts("// Class Inherent Functions");
            printf("%s", SHORT_BAR);
            puts("   [Datafile\t\tDatafile_open(char* name, char* mode)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Opens the datafile's stream for I/O.\n");
            puts("   [void\t\tDatafile_close(C_Datafile* objDatafile)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Class destructor.");
            puts("   The PointerKiller object should never be fetched FILE or Datafile objects.");
            puts("   You must still manually close all your files.\n");
            puts("   [void\tDatafile_print(Datafile var, char* str)]");
            printf("   %s", LARGE_UNDER_BAR);
            puts("   Prints char string to datafile.");
        }

        else if((strin.str!=NULL) && (String_find(strin, "tutorial custom classes", 0)!=INVALID)){
            puts("** CREATING YOUR OWN CLASSES **\n");
            puts("   First, you don't need the C-upgrade library to create custom classes");
            puts("   in C language. All this library provides is handy documentation for");
            puts("   learning to do so, as well as providing standard classes a la C++.\n");
            puts("   To create your own class, first you need to understand the approach");
            puts("   followed. There is the base object, which only contains the members.");
            puts("   It is really just a struct. Now, there are two ways of doing this: using");
            puts("   function pointers, and linking those to the real functions, or to use");
            puts("   \"#define\" commands to let the preprocessor do all the work, and not");
            puts("   wasting extra stack or heap memory for no gain at all.\n");
            puts("   Here is the sample code, commented for better understanding:\n\n");
            puts("   // This defines the basic class. In reality it's just a typedef struct");
            puts("   // that has the members of the class we want to create. No methods yet.");
            puts("   // ---------------------------------------------------------------------");
            puts("        typedef struct Class_Name {");
            puts("             /* CLASS MEMBERS */");
            puts("        }Class_Name;\n");
            puts("   // Now we create the constructor, destructor and class methods we want");
            puts("   // for our class, and list them like this:");
            puts("   // ---------------------------------------------------------------------\n");
            puts("   #define Name\t\t\tClass_Name");
            puts("   #define Name_function1\t\treal_name_of_function1");
            puts("   #define Name_function2\t\treal_name_of_function2");
            puts("   #define Name_function3\t\treal_name_of_function3");
            puts("   #define Name_function4\t\treal_name_of_function4");
            puts("   // etc.\n");
            puts("   And that's it. Just that easy. As you can imagine, it's not even hard at");
            puts("   all and complements pretty nicely the way that structs work. Look around");
            puts("   this library for a bit to several examples of how it's done. Have fun!");
            puts("   ADDENDUM: C-Upgrade lets you use \"object\" instead of \"typedef struct\"");
            puts("   for cleaner coding, thanks to \"#define object typdef struct\"\n");
        }
        printf("%s", TITLE_BAR);
        puts(">> (faq, new, delete, functions, list, vector, string, datafile)");
    }
    puts("Thank you for using C-Upgrade Library.\nHave a nice day!");
    String_destroy(strin);
}
#define CallIDC() C_Upgrade_IDC()

///=================================================================================================================================
#endif
