# Basit Kabuk Uygulaması (Shell)

## Proje Hakkında
Bu proje, işletim sistemleri dersi kapsamında geliştirilen bir basit **kabuk (shell)** uygulamasıdır. Uygulama, Linux ortamında çalışmakta ve temel Linux komutlarını çalıştırma, boru (pipe), arka plan işlemleri ve giriş/çıkış yönlendirmesi gibi özellikleri destekleyen bir komut satırı arayüzü sunmaktadır.

## Proje İçeriği
### Temel Özellikler:
- **Komut Yorumlama:** Kullanıcıdan alınan komutları doğru şekilde çalıştırır.
- **Boru (Pipe) Desteği:** Birden fazla komutun çıkışını ve girişini bağlama imkanı sunar.
- **Arka Plan İşlemleri:** Komutları arka planda çalıştırabilir ve tamamlandıklarında kullanıcıyı bilgilendirir.
- **Giriş/Çıkış Yönlendirme:** Komutların giriş ve çıkışını dosyalara yönlendirme yeteneğine sahiptir.
- **Özel Sinyal İşleme:** SIGCHLD gibi sinyalleri yakalar ve yönetir.
- **Program Sonlandırma:** Arka planda çalışan işlemleri bekleyip temiz bir şekilde sonlandırır.

### Dosya Açıklamaları
- **shell.c ve shell.h:**
  - Kabuk uygulamasının ana işlevlerini içerir.
  - Komut çalıştırma, boru ve giriş/çıkış yönlendirme işlevleri bu dosyalarda tanımlanmıştır.
- **main.c:**
  - Programın giriş noktasıdır. `kabukCalistir` fonksiyonunu çağırarak kabuk uygulamasını başlatır.
- **increment.c ve increment.h:**
  - Boru işlemleri için kullanılan örnek bir komut uygulamasıdır. Girdi olarak aldığı sayıları bir artırarak çıktıya yazar.
- **Makefile:**
  - Proje dosyalarını kolayca derlemek ve temizlemek için kullanılır. `make` komutuyla tüm uygulamalar derlenebilir.
- **README.md:**
  - Proje hakkında detaylı bilgi verir.

## Özellikler
### 1. Temel Komut İşleme
Kullanıcıdan bir komut alır ve çalıştırır. Örneğin:


> ls -l



### 2. Giriş ve Çıkış Yönlendirme
Bir komutun girişini veya çıktısını dosyalara yönlendirme yeteneği sağlar. Örneğin:


> cat < file.txt
file.txt’nin içinde ne varsa ekrana yazdırılır.
> cat < nofile.txt
nofile.txt giriş dosyası bulunamadı.

> cat file1.txt > file2.txt


Bu komut, file1.txt dosyasının içeriğini file2.txt dosyasına yazar.

### 3. Boru (Pipe) Desteği
Boru operatörü | kullanılarak birden fazla komutun çıktısını birbirine bağlama desteği sunar. Örneğin:


> echo 12 | increment | grep 13


Bu komut, echo çıktısını increment ile işler ve ardından grep ile filtreler.

>find /etc | grep ssh | grep conf

Bu görevde, komut promptu görüntülenmeden önce, pipe içindeki tüm komutların sonlanması gerektir.
İlgili LINUX system çağrıları: pipe, dup2. Ayrıca proseslerin stdin ve stdout dosya descriptörlerine de bi

### 4. Arka Plan İşlemleri
Bir komut sonuna & eklenerek arka planda çalıştırılabilir. Arka plandaki işlemler tamamlandığında kullanıcı bilgilendirilir. Örneğin:

> sleep 5 &
> cat file.txt
file.txt’nin içindekiler ekrana yazdırılır.
> [24617] retval: 0
Burada, ikinci prompt kabuk tarafından derhal yazdırılır, oysa pid ve dönüş değeri arka plan işleminin sonlandırılmasından
sonra yazdırılır.

### 5. Çoklu Komut İşleme
Noktalı virgül (;) kullanılarak birden fazla komut ardışık olarak çalıştırılabilir. Örneğin:


> ls -l; echo "Bitti!"


> echo 12; sleep 2; echo 13
12
13
Komutlar soldan sağa doğru yürütülür. Örneğin,
komut ilk önce 12 yazdirir, 2 saniye uyuyacak ve sonra 13 yazdirir.



### 6. Çıkış Komutu
quit komutu ile kabuk sonlandırılır. Kabuk sonlanmadan önce arka planda çalışan tüm işlemler tamamlanır ve kullanıcı bilgilendirilir:


> quit
Kabuk sonlandiriliyor...



## Kullanım
Kabuk çalıştırıldığında > ile başlayan bir komut satırı sunar. Kullanıcı bu satıra komut yazarak işlemleri gerçekleştirebilir.

## Katkıda Bulunanlar

    Grup üyeleri:
    - B221210101 Yalkin Can Yaliniz
    - B221210062 Mehmet Horuz
    - B211210056 Ismail Furkan Bilen
    - G221210043 Kadir Erim
    - G221210017 Sehrisu Ustuner 