/* ----------------------------------------------------------------------------
Isletim Sistemleri 2024 Guz Donemi Proje Odevi GRUP 59 
 * B221210101 Yalkin Can Yaliniz
 * B221210062 Mehmet Horuz
 * B211210056 Ismail Furkan Bilen
 * G221210043 Kadir Erim
 * G221210017 Sehrisu Ustuner
 ----------------------------------------------------------------------------*/
#include <stdio.h>   
#include <stdlib.h>  
#include "increment.h"

/*
    increment fonksiyonu: parametre olarak aldığı sayıyı 1 arttırır.
*/
int increment(int number) {
    return number + 1;
}

/*
    increment uygulaması :
    Kullanıcıdan bir tamsayı alır, 1 arttırır ve ekrana basar.
*/
int main() {
    int sayi;
    if (scanf("%d", &sayi) != 1) {
        fprintf(stderr, "Hata: Bir sayı okunamadı.\n");
        return EXIT_FAILURE;
    }
    printf("%d\n", increment(sayi));
    return 0;
}
