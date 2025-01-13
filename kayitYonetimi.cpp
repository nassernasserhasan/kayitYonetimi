/* 
  ogrenci Bilgileri:
  Ad Soyad: [Adinizi Buraya Yazin]
  Numara: [Numaranizi Buraya Yazin]
  Proje Adi: Kayit Yonetim Sistemi

  Proje Aciklamasi:
  Bu proje, bir dosya uzerinde kayit, arama, guncelleme, silme ve listeleme islemlerini gerceklestirmek icin tasarlanmistir. Kullanici dostu bir menu ile kullanici, ihtiyac duydugu islemleri kolayca gerceklestirebilir.
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

// Veri yapisi tanimlamasi
struct Kayit {
    int id;
    char isim[50];
    float puan;
};

// Yeni kayit ekleme
void kayitEkle(const char* dosyaAdi) {
    ofstream dosya(dosyaAdi, ios::binary | ios::app);
    if (!dosya) {
        cout << "Dosya acilirken hata olustu!" << endl;
        return;
    }

    Kayit kayit;
    cout << "Kayit ID'sini giriniz: ";
    cin >> kayit.id;
    cin.ignore();
    cout << "Kayit ismini giriniz: ";
    cin.getline(kayit.isim, 50);
    cout << "Puani giriniz: ";
    cin >> kayit.puan;

    dosya.write(reinterpret_cast<char*>(&kayit), sizeof(kayit));
    dosya.close();
    cout << "Kayit basariyla eklendi!" << endl;
}

// Tum kayitlari goruntuleme
void kayitlariGoster(const char* dosyaAdi) {
    ifstream dosya(dosyaAdi, ios::binary);
    if (!dosya) {
        cout << "Dosya acilirken hata olustu!" << endl;
        return;
    }

    Kayit kayit;
    cout << left << setw(10) << "ID" << setw(20) << "İsim" << setw(10) << "Puan" << endl;
    cout << "----------------------------------------" << endl;

    while (dosya.read(reinterpret_cast<char*>(&kayit), sizeof(kayit))) {
        cout << left << setw(10) << kayit.id << setw(20) << kayit.isim << setw(10) << kayit.puan << endl;
    }

    dosya.close();
}

// Kayit arama
void kayitAra(const char* dosyaAdi) {
    ifstream dosya(dosyaAdi, ios::binary);
    if (!dosya) {
        cout << "Dosya acilirken hata olustu!" << endl;
        return;
    }

    int arananId;
    cout << "Aranacak kayit ID'sini giriniz: ";
    cin >> arananId;

    Kayit kayit;
    bool bulundu = false;

    while (dosya.read(reinterpret_cast<char*>(&kayit), sizeof(kayit))) {
        if (kayit.id == arananId) {
            cout << "Kayit bulundu!" << endl;
            cout << "ID: " << kayit.id << "\nİsim: " << kayit.isim << "\nPuan: " << kayit.puan << endl;
            bulundu = true;
            break;
        }
    }

    if (!bulundu) {
        cout << "Kayit bulunamadi!" << endl;
    }

    dosya.close();
}

// Kayit guncelleme
void kayitGuncelle(const char* dosyaAdi) {
    fstream dosya(dosyaAdi, ios::binary | ios::in | ios::out);
    if (!dosya) {
        cout << "Dosya acilirken hata olustu!" << endl;
        return;
    }

    int arananId;
    cout << "Guncellenecek kayit ID'sini giriniz: ";
    cin >> arananId;

    Kayit kayit;
    bool bulundu = false;

    while (dosya.read(reinterpret_cast<char*>(&kayit), sizeof(kayit))) {
        if (kayit.id == arananId) {
            cout << "Yeni bilgileri giriniz:" << endl;
            cout << "Kayit ismini giriniz: ";
            cin.ignore();
            cin.getline(kayit.isim, 50);
            cout << "Puani giriniz: ";
            cin >> kayit.puan;

            dosya.seekp(-static_cast<int>(sizeof(kayit)), ios::cur);
            dosya.write(reinterpret_cast<char*>(&kayit), sizeof(kayit));
            cout << "Kayit basariyla guncellendi!" << endl;
            bulundu = true;
            break;
        }
    }

    if (!bulundu) {
        cout << "Kayit bulunamadi!" << endl;
    }

    dosya.close();
}

// Kayit silme
void kayitSil(const char* dosyaAdi) {
    ifstream dosya(dosyaAdi, ios::binary);
    if (!dosya) {
        cout << "Dosya acilirken hata olustu!" << endl;
        return;
    }

    ofstream tempDosya("temp.dat", ios::binary);
    int silinecekId;
    cout << "Silinecek kayit ID'sini giriniz: ";
    cin >> silinecekId;

    Kayit kayit;
    bool bulundu = false;

    while (dosya.read(reinterpret_cast<char*>(&kayit), sizeof(kayit))) {
        if (kayit.id == silinecekId) {
            bulundu = true;
        } else {
            tempDosya.write(reinterpret_cast<char*>(&kayit), sizeof(kayit));
        }
    }

    dosya.close();
    tempDosya.close();

    remove(dosyaAdi);
    rename("temp.dat", dosyaAdi);

    if (bulundu) {
        cout << "Kayit basariyla silindi!" << endl;
    } else {
        cout << "Kayit bulunamadi!" << endl;
    }
}

// Ortalama hesaplama
void ortalamaHesapla(const char* dosyaAdi) {
    ifstream dosya(dosyaAdi, ios::binary);
    if (!dosya) {
        cout << "Dosya acilirken hata olustu!" << endl;
        return;
    }

    Kayit kayit;
    float toplam = 0;
    int sayac = 0;

    while (dosya.read(reinterpret_cast<char*>(&kayit), sizeof(kayit))) {
        toplam += kayit.puan;
        sayac++;
    }

    dosya.close();

    if (sayac > 0) {
        cout << "Ortalama puan: " << toplam / sayac << endl;
    } else {
        cout << "Kayit bulunamadi!" << endl;
    }
}

int main() {
    const char* dosyaAdi = "kayitlar.dat";
    int secim;

    do {
        cout << "\n==== Menu ====" << endl;
        cout << "1. Yeni kayit ekle" << endl;
        cout << "2. Tum kayitlari goster" << endl;
        cout << "3. Kayit ara" << endl;
        cout << "4. Kayit guncelle" << endl;
        cout << "5. Kayit sil" << endl;
        cout << "6. Ortalama hesapla" << endl;
        cout << "0. cikis" << endl;
        cout << "Seciminizi yapin: ";
        cin >> secim;

        switch (secim) {
            case 1: kayitEkle(dosyaAdi); break;
            case 2: kayitlariGoster(dosyaAdi); break;
            case 3: kayitAra(dosyaAdi); break;
            case 4: kayitGuncelle(dosyaAdi); break;
            case 5: kayitSil(dosyaAdi); break;
            case 6: ortalamaHesapla(dosyaAdi); break;
            case 0: cout << "Hosca kalin!" << endl; break;
            default: cout << "Gecersiz secim!" << endl; break;
        }
    } while (secim != 0);

    return 0;
}
