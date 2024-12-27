/* ----------------------------------------------------------------------------
Isletim Sistemleri 2024 Guz Donemi Proje Odevi GRUP 59 
 * B221210101 Yalkin Can Yaliniz
 * B221210062 Mehmet Horuz
 * B211210056 Ismail Furkan Bilen
 * G221210043 Kadir Erim
 * G221210017 Sehrisu Ustuner
 ----------------------------------------------------------------------------*/

#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>      // printf, perror vb.
#include <stdlib.h>     // exit, malloc vb.
#include <unistd.h>     // fork, exec, pipe vb.
#include <sys/types.h>  // pid_t, size_t vb.
#include <sys/wait.h>   // wait, waitpid
#include <signal.h>     // signal, sigaction
#include <string.h>     // strcmp, strtok vb.
#include <errno.h>      // errno

#define MAX_GIRIS_UZUNLUK 1024
#define MAX_KELIME_SAYISI 100

extern pid_t arkaPlanPIDler[100];
extern int arkaPlanIslemSayisi;
extern int quitAktif; // 'quit' komutu aktif mi kontrolü için bayrak

/*
 * Kullanıcıya prompt gösteren fonksiyon
 */
void promptYaz();

/*
 * Noktalı virgül ile ayrılmış komutları bölerek dizi halinde döndüren fonksiyon
 */
char** noktaliVirgulIleBol(char *girdi, int *komutSayisi);
/*
 * Boşluklara göre bir komutu kelimelere ayıran fonksiyon
 */
char** kelimelereBol(char *komut);

/*
 * Boşlukları temizleyen fonksiyon
 */
char* bosluklariTemizle(char *str);
/*
 * Tek parçalı (tekli) komutları çalıştıran fonksiyon
 */
void komutCalistirTekli(char **kelimeler, int arkaPlanda);

/*
 * Herhangi bir komut satırını yorumlayan fonksiyon
 */
void komutYorumla(char *komutSatiri);
/*
 * Giriş yönlendirme yapan fonksiyon
 */
int girisYonlendirme(char **kelimeler);
/*
 * Çıkış yönlendirme yapan fonksiyon
 */
int cikisYonlendirme(char **kelimeler);

/*
 * Birden fazla komutu pipe ile bağlayarak çalıştıran fonksiyon
 */
void boruCalistir(char **parcalar, int parcaSayisi, int arkaPlanda);

/*
 * Pipe (|) ile ayrılmış komutları parçalara ayıran fonksiyon
 */
char** boruIleBol(char *komut, int *parcaSayisi);

/*
 * Arka plandaki işlemleri sürekli kontrol eden ve tamamlandıklarında bildiren fonksiyon
 */
void arkaPlanIslemleriniKontrolEt();

/* 
 * SIGCHLD sinyali geldiğinde çağrılacak handler fonksiyonu 
 * Arka plandaki süreçlerin tamamlanmasını yakalar ve durumlarını bildirir.
 */
void arkaPlanBitisHandler();


/*
 * Programı sonlandıran fonksiyon
 * Arka plandaki tüm süreçleri bekler ve çıkış yapar.
 */
void programiSonlandir();

/*
 * Kabuk uygulamasını başlatan fonksiyon
 */
void kabukCalistir(void);







#endif 