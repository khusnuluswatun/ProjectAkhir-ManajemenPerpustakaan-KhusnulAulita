#include <iostream>
#include <cstdio>
#include <iomanip>
using namespace std;

string keyUnm = "admin";
string keyPwd = "123";
bool saveGak = false;

struct Buku{
	string kode, judul, penulis;
	int tahun, stok;
	Buku *kiri;
	Buku *kanan;
};

struct List{
	Buku *head = NULL;
	Buku *tail = NULL;
};

List bukubuku;
//bag aulita : sorting, editBuku, hapusBuku, pinjam, kembalikan
void loadDataFilePerpustakaan(), menu(), tambahBuku(), tampilkanBuku(), searching(), sorting(), editBuku(), hapusBuku(), pinjam(), kembalikan(), simpanFile(), keluar();

int main(){
	cout << "========================================================" << endl;
	cout << "               SISTEM MANAJEMEN PERPUSTAKAAN           " << endl;
	cout << "========================================================" << endl;
	cout << "\nSilahkan login terlebih dahulu\n";
	string unm, pwd;
	bool check = false;
	while(!check){
		cout << "Username : ";
		cin >> unm;
		cout << "Password : ";
		cin >> pwd;	
		
		if(unm != keyUnm || pwd != keyPwd){
			cout << "Username atau password salah, silahkan coba lagi" << endl;
		}else{
			check = true;
		}
	}
	
	char lanjut = 'y';
	int pilihan;
	
	loadDataFilePerpustakaan();
	
	do{
		system("cls");
		menu();
		cout << "Pilih menu: ";
		cin >> pilihan;
		cout << endl;

		switch(pilihan){
			case 1:
			tambahBuku();
			break;
			case 2:
			tampilkanBuku();
			break;
			case 3:
			searching();
			break;
			case 4:
			sorting();
			break;
			case 5:
			editBuku();
			break;
			case 6:
			hapusBuku();
			break;
			case 7:
			pinjam();
			break;
			case 8:
			kembalikan();
			break;
			case 9:
			simpanFile();
			break;
			case 0:
			keluar();
			break;
			default:
			cout << "Menu tidak ada" <<endl;
			break;
		}
		
		cout << "\nLanjut menu?(y/n): ";
		cin >> lanjut;
	}while(lanjut == 'y' && pilihan != 0);
	if(lanjut == 'n') keluar();
}


void loadDataFilePerpustakaan() {
    FILE *fperpus = fopen("dataPerpustakaan.txt", "r");
    if (fperpus == NULL) return;

    char t_kode[50], t_judul[100], t_penulis[100];
    int t_tahun, t_stok;

    // format: %[^;] = baca string sampai ketemu ';'
    while (fscanf(fperpus, " %[^;];%[^;];%[^;];%d;%d\n", t_kode, t_judul, t_penulis, &t_tahun, &t_stok) != EOF) {
        
        Buku *baru = new Buku;
        baru->kode = t_kode;
        baru->judul = t_judul;
        baru->penulis = t_penulis;
        baru->tahun = t_tahun;
        baru->stok = t_stok;
        baru->kiri = NULL;
        baru->kanan = NULL;

        if (bukubuku.head == NULL) {
            bukubuku.head = bukubuku.tail = baru;
        } else {
            bukubuku.tail->kiri = baru;
            baru->kanan = bukubuku.tail;
            bukubuku.tail = baru;
        }
    }

    fclose(fperpus);
}

void menu(){
	cout << "========================================================" << endl;
	cout << "               SISTEM MANAJEMEN PERPUSTAKAAN           " << endl;
	cout << "========================================================" << endl;
	cout << "1. Tambah buku baru" << endl;
	cout << "2. Tampilkan semua buku" << endl;
	cout << "3. Cari buku" << endl;
	cout << "4. Sorting buku" << endl;
	cout << "5. Edit data buku" << endl;
	cout << "6. Hapus buku" << endl;
	cout << "-----------------------------------------------------" << endl;
	cout << "7. Pinjam buku" << endl;
	cout << "8. Kembalikan buku" << endl;
	cout << "-----------------------------------------------------" << endl;
	cout << "9. Simpan ke file" << endl;
	cout << "0. Keluar" << endl;
	cout << "========================================================" << endl;
}

void tambahBuku(){
	int brp;
	cout << "Mau input berapa buku? ";
	cin >> brp;
	
	for(int i=0;i<brp;i++){
		Buku *baru = new Buku;
		cout << "-----------\nBuku ke-" << (i+1) << endl;
		cout << "Kode: ";
		cin >> baru->kode;
		cout << "Judul: ";
		cin.ignore();
    	getline(cin, baru->judul);
		cout << "Penulis: ";
    	getline(cin, baru->penulis);
		cout << "Tahun: ";
		cin >> baru->tahun;
		cout << "Stok: ";
		cin >> baru->stok;
		
		baru->kiri = NULL;
		baru->kanan = NULL;
		
		if(bukubuku.head == NULL){ //node kosong
			bukubuku.head = baru;
			bukubuku.tail = baru;
		}else{//ada isi - ekor tunjuk baru
			bukubuku.tail->kiri = baru;
			baru->kanan = bukubuku.tail;
			bukubuku.tail = baru;
		}
	}
	saveGak = true;
}

void tampilkanBuku(){
	if(bukubuku.head == NULL){
		cout << "Data buku kosong! " << endl; return;
	}
	
	cout << "-------------------------------------------------------" << endl;
    cout << left << setw(8) << "Kode" << setw(25) << "Judul" << setw(15) << "Penulis" << setw(7) << "Stok" << endl;
	cout << "-------------------------------------------------------" << endl;
	
	Buku *bantu = bukubuku.head;
	while(bantu != NULL){
		cout << left << setw(8) << bantu->kode << setw(25) << bantu->judul << setw(15) << bantu->penulis << setw(7) << bantu->stok << endl;
		bantu = bantu->kiri;
	}
	cout << "-------------------------------------------------------" << endl;
}

void searching(){
	if(bukubuku.head == NULL){
		cout << "Data buku kosong" << endl; return;
	}
	string cari;
	cout << "Kode atau judul buku yang dicari: ";
	cin.ignore();
	getline(cin, cari);
	
	Buku *bantu = bukubuku.head;
	bool ketemu = false;
	while(bantu != NULL){
		if(bantu->kode == cari || bantu->judul == cari){
			if(ketemu == false){			
				cout << "-------------------------------------------------------" << endl;
				cout << left << setw(8) << "Kode" << setw(25) << "Judul" << setw(15) << "Penulis" << setw(7) << "Stok" << endl;
				cout << "-------------------------------------------------------" << endl;	
			}
		cout << left << setw(8) << bantu->kode << setw(25) << bantu->judul << setw(15) << bantu->penulis << setw(7) << bantu->stok << endl;
		ketemu = true;
		}
		bantu = bantu->kiri;
	}

	if(ketemu){
		cout << "-------------------------------------------------------" << endl;	
	}else{
		cout << "Data tidak ditemukan" << endl;	
	}	
}

//semua perintah ini ambil data sama edit data linked list / bukubuku yaa
// saveGak itu nanti biar kalo mau keluar & belum save ke file jadi muncul pertanyaan mau save gak
void sorting(){
	//ini dibuat ada option sorting ascending atau desc yaa
}

void editBuku(){
	saveGak = true;
}
void hapusBuku(){
	saveGak = true;
}
void pinjam(){
	saveGak = true;
}
void kembalikan(){
	saveGak = true;
}
void simpanFile(){
	if(bukubuku.head == NULL){
		cout << "Data buku kosong" << endl; return;
	}
	
	FILE *filePerpus;
	filePerpus = fopen("dataPerpustakaan.txt", "w");
	if(filePerpus == NULL){
		cout << "File tidak bisa dibuka" << endl;return;
	}
	
	Buku *bantu = bukubuku.head;
	while (bantu != NULL) {
		//delimiter ;
        fprintf(filePerpus, "%s;%s;%s;%d;%d\n", bantu->kode.c_str(), bantu->judul.c_str(), bantu->penulis.c_str(), bantu->tahun, bantu->stok);
        
        bantu = bantu->kiri;
    }
    fclose(filePerpus);
    cout << "\nData berhasil disimpan ke file dataPerpustakaan\n";
}
void keluar(){
	char qsave;
	if(saveGak){
		cout << "Perubahan belum disimpan. Apakah akan Anda simpan? (y/n) ";
		cin >> qsave;
		if(qsave == 'y'){
			simpanFile();
		}
	}
	cout << "\nTerima kasih telah menggunakan program ini!" << endl;
	exit(0);
}
