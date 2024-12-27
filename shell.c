/* ----------------------------------------------------------------------------
Isletim Sistemleri 2024 Guz Donemi Proje Odevi GRUP 59 
 * B221210101 Yalkin Can Yaliniz
 * B221210062 Mehmet Horuz
 * B211210056 Ismail Furkan Bilen
 * G221210043 Kadir Erim
 * G221210017 Sehrisu Ustuner
 ----------------------------------------------------------------------------*/


/******************************************************************************
 * shell.c
 *
 *  İşletim Sistemleri dersi Proje Ödevi için basit bir kabuk (shell) örneği.
 *
 *****************************************************************************/

#include <stdio.h>      // printf, fgets vb.
#include <stdlib.h>     // exit, malloc, free vb.
#include <unistd.h>     // fork, exec, pipe, dup2, chdir vb.
#include <sys/wait.h>   // wait, waitpid vb.
#include <string.h>     // strtok, strcmp vb.
#include <fcntl.h>      // open, O_RDONLY, O_WRONLY vb.
#include <errno.h>      // errno, perror
#include <signal.h>     // sigaction, kill, SIGCHLD, SIGINT vb.
#include "shell.h"      // Shell ile ilgili prototipler

#define MAX_GIRIS_UZUNLUK 1024
#define MAX_KELIME_SAYISI 100
/*
    Arka plan işlemlerini takip etmek için bir dizi ve sayaç
    - arkaPlanPIDler: Arka planda çalışan proseslerin PID'lerini saklar
    - arkaPlanIslemSayisi: Arka planda çalışan proses sayısını takip eder
*/
pid_t arkaPlanPIDler[100];
int arkaPlanIslemSayisi = 0;
int quitAktif = 0;
int pipeAktif = 0;

/* ----------------------------------------------------------------------------
 * 1) Prompt 
 * Kullanıcıya komut girişi için prompt gösterir.
 * ----------------------------------------------------------------------------*/
void promptYaz() {
    printf("> "); // Prompt sembolü
    fflush(stdout); // Çıktıyı hemen terminale gönder
}

/* ----------------------------------------------------------------------------
 * Noktalı virgülle (;) ayrılan komutları bölmek için fonksiyon
 * Örnek: "echo 12; sleep 2; echo 13"
 * ----------------------------------------------------------------------------*/
char** noktaliVirgulIleBol(char *girdi, int *komutSayisi) {
    // Komutları depolamak için dinamik olarak bellek ayır
    char **komutlar = malloc(sizeof(char*) * MAX_KELIME_SAYISI);
    memset(komutlar, 0, sizeof(char*) * MAX_KELIME_SAYISI);

    int index = 0;
    char *token = strtok(girdi, ";");// Noktalı virgül ile böl
    while(token != NULL) {
        // Her bir komutu ayrı bir dizi elemanına kopyala
        komutlar[index] = malloc(strlen(token) + 1);
        strcpy(komutlar[index], token);
        index++;
        token = strtok(NULL, ";"); 
    }
    *komutSayisi = index; // Toplam komut sayısını güncelle
    return komutlar;
}
/* ----------------------------------------------------------------------------
 * Boşluklara göre (space, tab) bir komutu kelimelere ayırma
 * Örn: "ls -l /home"
 * ----------------------------------------------------------------------------*/
char** kelimelereBol(char *komut) {
    char **kelimeler = malloc(sizeof(char*) * MAX_KELIME_SAYISI);
    memset(kelimeler, 0, sizeof(char*) * MAX_KELIME_SAYISI);

    int index = 0;
    char *token = strtok(komut, " \t");  // Boşluk ve tab ile böl
    while(token != NULL) {
        kelimeler[index] = token; // Her kelimeyi diziye ata
        index++;
        token = strtok(NULL, " \t");
    }
    kelimeler[index] = NULL;  
    return kelimeler;
}
/* ----------------------------------------------------------------------------
 * Boşlukları (leading ve trailing) temizleyen yardımcı fonksiyon 
 * (sleep 5 & gibi komutların sonunda '&' kalması için önemlidir).
 * ----------------------------------------------------------------------------*/
char* bosluklariTemizle(char *str) {
    // Baş taraftaki boşlukları atla
    while(*str == ' ' || *str == '\t' || *str == '\n') {
        str++;
    }
    // Son taraftaki boşlukları atla ve string sonunu belirle
    char *son = str + strlen(str) - 1;
    while(son > str && (*son == ' ' || *son == '\t' || *son == '\n')) {
        *son = '\0';
        son--;
    }
    return str;
}

/* ----------------------------------------------------------------------------
 * Tek parçalı (tekli) komut icrası 
 * (örn: "ls -l", "sleep 2", "cat file.txt" vb.)
 * ----------------------------------------------------------------------------*/
void komutCalistirTekli(char **kelimeler, int arkaPlanda) {
    pid_t pid = fork();
    if(pid < 0) {
        perror("fork hata");
        return;
    }
    else if(pid == 0) {
        // Çocuk
        // Girdi / Çıkış yönlendirme
        girisYonlendirme(kelimeler);
        cikisYonlendirme(kelimeler);

        // Komutu çalıştır
        execvp(kelimeler[0], kelimeler);
        perror("Komut calistirilamadi");
        exit(EXIT_FAILURE);
    } else {
        // Ebeveyn
        if(arkaPlanda) {
            // Arka plan: beklenmeyecek
            arkaPlanPIDler[arkaPlanIslemSayisi++] = pid;
        } else {
            // Ön plan: bekle
            waitpid(pid, NULL, 0);
        }
    }
}
/* ----------------------------------------------------------------------------
 * Herhangi bir komut satırını yorumlayan fonksiyon:
 *  - "quit" kontrolü
 *  - sonda '&' var mı?
 *  - pipe var mı?
 *  - değilse tekli komut
 * ----------------------------------------------------------------------------*/
void komutYorumla(char *komutSatiri) {
    // Boşlukları temizle
    komutSatiri = bosluklariTemizle(komutSatiri);

    // Boşsa dön
    if(strlen(komutSatiri) == 0) return;

    // "quit" mi?
    if(strcmp(komutSatiri, "quit") == 0) {
        programiSonlandir();
        return;
    }

    // Arka plan mı diye bak
    int arkaPlanda = 0;
    int len = strlen(komutSatiri);
    if(len > 0 && komutSatiri[len - 1] == '&') { // Komutun sonunda '&' varsa
        // Sondaki & kaldır
        komutSatiri[len - 1] = '\0';  // '&' karakterini kaldır
        // tekrar trim
        komutSatiri = bosluklariTemizle(komutSatiri);
        arkaPlanda = 1;
    }

    // Pipe var mı?
    int parcaSayisi = 0;
    char **parcalar = boruIleBol(komutSatiri, &parcaSayisi);

    if(parcaSayisi > 1) {
        // Birden fazla pipe parçası
        boruCalistir(parcalar, parcaSayisi, arkaPlanda); // Piping işlemini gerçekleştir
    } else {
        // Tek parça
        // Kelimelere böl
        char *temizKomut = bosluklariTemizle(parcalar[0]);
        char **kelimeler = kelimelereBol(temizKomut);
        komutCalistirTekli(kelimeler, arkaPlanda); // Tekli komutu çalıştır

    }

    // Bellek temizliği
    for(int i = 0; i < parcaSayisi; i++) {
        free(parcalar[i]); // Her bir parça için ayrılan belleği serbest bırak 
    }
    free(parcalar);// Parçalar dizisini serbest bırak
}

/* ----------------------------------------------------------------------------
 * Giriş yönlendirme (komut < dosya)
 * Komut içinde '<' operatörü kullanılarak dosya girdisi yapılır.
 * ----------------------------------------------------------------------------*/
int girisYonlendirme(char **kelimeler) {
    int i = 0;
    while(kelimeler[i] != NULL) {
        if(strcmp(kelimeler[i], "<") == 0) { // '<' operatörü bulunursa
            if(kelimeler[i+1] == NULL) { // Dosya adı belirtilmemişse hata
                fprintf(stderr, "Giris dosyasi belirtilmemis.\n");
                return -1;
            }
            int fd = open(kelimeler[i+1], O_RDONLY); // Dosyayı oku modunda aç
            if(fd < 0) { // Dosya açılamadıysa hata mesajı
                fprintf(stderr, "%s giris dosyasi bulunamadi.\n", kelimeler[i+1]);
                return -1;
            }
            dup2(fd, STDIN_FILENO); // Dosya deskriptörünü standart girdi olarak ayarla
            close(fd); //  deskriptörünü kapatt

            // "< dosya" kelimelerini diziden çıkaralım
            while(kelimeler[i+2] != NULL) {
                kelimeler[i] = kelimeler[i+2];
                i++;
            }
            kelimeler[i] = NULL; // Diziyi sonlandır
            break;
        }
        i++;
    }
    return 0;
}