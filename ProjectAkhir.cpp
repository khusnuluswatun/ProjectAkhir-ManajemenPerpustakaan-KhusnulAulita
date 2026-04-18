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

struct Peminjaman{
	string nama, judul, tanggal;
	Peminjaman *next;
};

struct List{
	Buku *head = NULL;
	Buku *tail = NULL;
};

struct ListPeminjaman{
	Peminjaman *head = NULL;
	Peminjaman *tail = NULL;
};

List bukubuku;
ListPeminjaman dataPinjam;

//bag aulita : sorting, editBuku, hapusBuku, pinjam, kembalikan
void loadDataFilePerpustakaan(), menu(), tambahBuku(), tampilkanBuku(), searching(), sorting(), editBuku(), hapusBuku(), loadDataFilePeminjam(), pinjam(), tampilkanPeminjaman(), kembalikan(), simpanPeminjaman(), sortPeminjaman(), simpanFile(), simpanSemua(), keluar(), bubbleSort(int pilih), selectionSort(int pilih), swapData(Buku *a, Buku *b);

int main(){
	cout << "=======================================================" << endl;
	cout << "               SISTEM MANAJEMEN PERPUSTAKAAN           " << endl;
	cout << "=======================================================" << endl;
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
	loadDataFilePeminjam();
	
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
			tampilkanPeminjaman();
			break;
			case 9:
			kembalikan();
			break;
			case 10:
			simpanSemua();
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
	cout << "=======================================================" << endl;
	cout << "               SISTEM MANAJEMEN PERPUSTAKAAN           " << endl;
	cout << "=======================================================" << endl;
	cout << "1. Tambah buku baru" << endl;
	cout << "2. Tampilkan semua buku" << endl;
	cout << "3. Cari buku" << endl;
	cout << "4. Sorting buku" << endl;
	cout << "5. Edit data buku" << endl;
	cout << "6. Hapus buku" << endl;
	cout << "-----------------------------------------------------" << endl;
	cout << "7. Pinjam buku" << endl;
	cout << "8. Data buku dipinjam" << endl;
	cout << "9. Kembalikan buku" << endl;
	cout << "-----------------------------------------------------" << endl;
	cout << "10. Simpan ke file" << endl;
	cout << "0. Keluar" << endl;
	cout << "=======================================================" << endl;
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
	saveGak = true;
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
void swapData(Buku *a, Buku *b){
    swap(a->kode, b->kode);
    swap(a->judul, b->judul);
    swap(a->penulis, b->penulis);
    swap(a->tahun, b->tahun);
    swap(a->stok, b->stok);
}

void sorting(){
	//ini dibuat ada option sorting ascending atau desc yaa
	int pilih, urutan; 
	cout << "+------------------------------------+" << endl;
	cout << "|               SORTING              |" << endl;
	cout << "+------------------------------------+" << endl;
	cout << " Sorting berdasarkan: " << endl;
	cout << " 1. Kode" << endl;
	cout << " 2. Judul" << endl;
	cout << " +-----------------------------------+" << endl;
	cout << "Pilih: ";
	cin >> pilih;
	cout << "\n+------------------------------------+" << endl;
	cout << "|               SORTING              |" << endl;
	cout << "+------------------------------------+" << endl;
	cout << " Sorting secara: " << endl;
	cout << " 1. Ascending" << endl;
	cout << " 2. Descending" << endl;
	cout << "+------------------------------------+" << endl;
	cout << "Pilih: ";
	cin >> urutan;
	
	if (urutan == 1) {
		bubbleSort(pilih);
		cout << "Data berhasil diurutkan secara Ascending" << endl;
	} else if (urutan == 2) {
		selectionSort(pilih);
		cout << "Data berhasil diurutkan secara Descending" << endl;
	} else {
		cout << "Pilihan tidak tersedia." << endl;
		return;
	}
}

//ini ascending
void bubbleSort(int pilih) {
	if(bukubuku.head == NULL) return;
	bool swapped;
	Buku *i, *j;
	
	do {
		swapped = false;
		i = bukubuku.head;
		while(i->kiri != NULL) {
			j = i->kiri;
			bool kondisi = false;
			if(pilih == 1){
				kondisi = (i->kode > j->kode);
			}else if(pilih == 2) {
				kondisi = (i->judul > j->judul);
			}
			if(kondisi){
				swapData(i, j);
				swapped = true;
			}
			i = i->kiri;
		}
	} while (swapped);
}

//ini descendingg
void selectionSort(int pilih){
	if(bukubuku.head == NULL) return;
	Buku *i, *j, *max;
	
	for(i = bukubuku.head; i != NULL; i = i->kiri){
		max = i;
		for(j= i->kiri; j!= NULL; j= j->kiri) {
			bool kondisi = false;
			
			if(pilih == 1){
				kondisi = (j->kode > max-> kode);
			}else if(pilih == 2){
				kondisi = (j->judul > max->judul);
			}
			if(kondisi){
				max = j;
			}
		}
	if(max!=i){
		swapData(i, max);
	}
}
}

void editBuku(){
	if(bukubuku.head == NULL) {
		cout << "Data buku kosong" << endl;
		return;
	}
	string cari;
	cout << "Kode atau judul buku yang akan diedit: ";
	cin.ignore();
	getline(cin, cari);
	
	Buku *bantu = bukubuku.head;
	bool ketemu = false;
	while(bantu != NULL){
		if(bantu->kode == cari || bantu->judul == cari){
			int pilih;
			ketemu = true;
			cout << "\nData Buku Ditemukan: " << endl;			
			cout << "-------------------------------------------------------" << endl;
			cout << left << setw(8) << "Kode" << setw(25) << "Judul" << setw(15) << "Penulis" << setw(7) << "Stok" << endl;
			cout << "-------------------------------------------------------" << endl;
			cout << left << setw(8) << bantu->kode << setw(25) << bantu->judul << setw(15) << bantu->penulis << setw(7) << bantu->stok << endl;
			cout << "-------------------------------------------------------" << endl;
			cout << "\n<<------------- Pilih Yang Ingin Diedit ------------->>" << endl;
			cout << "1.Kode" << endl;
			cout << "2.Judul" << endl;
			cout << "3.Nama Penulis" << endl;
			cout << "4.Stok" << endl;
			cout << "-------------------------------------------------------" << endl;
			cout << "Pilihan: ";
			cin >> pilih;
			cin.ignore();
			
			switch(pilih){
				case 1: cout << "Kode baru: ";
						getline(cin, bantu->kode);break;
				case 2: cout << "Judul baru: ";
						getline(cin, bantu->judul);break;
				case 3: cout << "Penulis: ";
						getline(cin, bantu->penulis);break;
				case 4: cout << "Stok baru: ";
						cin >> bantu->stok; break;
				default: cout << "Pilihan tidak tersedia" << endl;
				return;
			}
				
			cout << "\nData berhasil diupdate." << endl;
			return;
		}
		bantu = bantu->kiri;
	}
	if(!ketemu){
		cout<< "Data Buku Tidak Ditemukan" << endl;	
		}
	saveGak = true;
}

void hapusBuku(){
		if(bukubuku.head == NULL) {
		cout << "Data buku kosong" << endl;
		return;
	}
	string cari;
	cout << "Kode atau judul buku yang akan dihapus: ";
	cin.ignore();
	getline(cin, cari);
	
	Buku *hapus = bukubuku.head;
	while(hapus != NULL){
		if(hapus->kode == cari || hapus->judul == cari){
			cout << "\nData Buku Ditemukan: " << endl;			
			cout << "-------------------------------------------------------" << endl;
			cout << left << setw(8) << "Kode" << setw(25) << "Judul" << setw(15) << "Penulis" << setw(7) << "Stok" << endl;
			cout << "-------------------------------------------------------" << endl;
			cout << left << setw(8) << hapus->kode << setw(25) << hapus->judul << setw(15) << hapus->penulis << setw(7) << hapus->stok << endl;
			Peminjaman *cek = dataPinjam.head;
			while(cek != NULL){
				if(cek->judul == hapus->judul){
					cout << "\nBuku sedang dipinjam, tidak bisa dihapus." << endl;
					return;
				}
				cek =cek ->next;
			}
			char konfirmasi;
			cout << "Yakin ingin menghapus buku '" << hapus->judul << "'? (y/n): ";
			cin >> konfirmasi;
			if(konfirmasi != 'y'&& konfirmasi != 'Y'){
				cout << "Penghapusan dibatalkan." << endl;
				return;
			}
			
			if(bukubuku.head == bukubuku.tail){
                bukubuku.head = bukubuku.tail = NULL;
            } else if(hapus == bukubuku.head){
                bukubuku.head = hapus->kiri;
                bukubuku.head->kanan = NULL;
            }else if(hapus == bukubuku.tail){
                bukubuku.tail = hapus->kanan;
                bukubuku.tail->kiri = NULL;
            }else{
                hapus->kanan->kiri = hapus->kiri;
                hapus->kiri->kanan = hapus->kanan;
            }
            delete hapus;
            cout << "Data berhasil dihapus!\n";
            return;
        }
        hapus = hapus->kiri;
	}
	cout << "Data Buku Tidak Ditemukan" << endl;
	saveGak = true;
}

void loadDataFilePeminjam(){
	FILE *fpinjam = fopen("dataPeminjaman.txt", "r");
	if(fpinjam == NULL) return;
	char t_nama[50], t_judul[100], t_tanggal[50];
	
	while(fscanf(fpinjam, " %[^;];%[^;];%[^\n]\n", t_nama, t_judul, t_tanggal) != EOF){
		Peminjaman *baru = new Peminjaman;
		baru->nama = t_nama;
		baru->judul= t_judul;
		baru->tanggal= t_tanggal;
		baru->next = NULL;
				if(dataPinjam.head == NULL){
					dataPinjam.head = dataPinjam.tail = baru;
				}else{
					dataPinjam.tail->next = baru;
					dataPinjam.tail = baru;
				}
	}
	fclose(fpinjam);
}
	
void pinjam(){
	if(bukubuku.head == NULL) {
		cout << "Data buku kosong" << endl;
		return;
	}
	string cari;
	cout << "Kode atau judul buku yang ingin dipinjam: ";
	cin.ignore();
	getline(cin, cari);
	
	Buku *bantu = bukubuku.head;
	while(bantu != NULL){
		if(bantu->kode == cari || bantu->judul == cari){
			if(bantu->stok > 0) {
				bantu->stok--;
				Peminjaman *baru = new Peminjaman;
				cout << "\n<<-------- INPUT DATA PEMINJAMAN BUKU -------->>\n";
				cout << " Nama Peminjam              : ";
				getline(cin, baru->nama);
				cout << " Tanggal Pinjam (yyyy-mm-dd): ";
				getline(cin, baru->tanggal);
				baru->judul = bantu->judul;
				baru->next = NULL;
				if(dataPinjam.head == NULL){
					dataPinjam.head = dataPinjam.tail = baru;
				}else{
					dataPinjam.tail->next = baru;
					dataPinjam.tail = baru;
				}
				cout << "\nBuku '" << bantu->judul << "' berhasil dipinjam!" << endl;
				cout << "+----------------+" << endl;
				cout << "| Sisa stok: " << bantu->stok <<" |" << endl;
				cout << "+----------------+" << endl;
			}else{
				cout << "\nStok habis, buku tidak bisa dipinjam." << endl;
			}
			saveGak = true;
			return;
		}
		bantu = bantu->kiri;
	}
	cout << "Data buku tidak ditemukan." << endl;
}

void tampilkanPeminjaman(){
	sortPeminjaman();
	if(dataPinjam.head == NULL){
		cout << "<---- Tidak ada buku yang sedang dipinjam ---->" << endl;
		return;
	}
	cout << "Data Buku Dipinjam: " << endl;
	cout << "-------------------------------------------------------" << endl;
	cout << left << setw(15) << "Nama" << setw(25) << "Judul Buku" << setw(15) << "Tanggal" << endl;
	cout << "-------------------------------------------------------" << endl;
	Peminjaman *bantu = dataPinjam.head;
	while(bantu != NULL){
		cout << left << setw(15) << bantu->nama << setw(25) << bantu->judul << setw(15) << bantu->tanggal << endl;
		bantu = bantu->next;
}
cout << "-------------------------------------------------------" << endl;
}

//ini sort untuk data peminjaman biar rapi aja, ga aku panggil dimenu
void sortPeminjaman(){
    if(dataPinjam.head == NULL) return;
    bool swapped;
    Peminjaman *i, *j;

    do{
        swapped = false;
        i = dataPinjam.head;

        while(i->next != NULL){
            j = i->next;
            if(i->tanggal > j->tanggal){
                swap(i->nama, j->nama);
                swap(i->judul, j->judul);
                swap(i->tanggal, j->tanggal);
                swapped = true;
            }
            i = i->next;
        }
    }while(swapped);
}

void kembalikan(){
	if(dataPinjam.head == NULL){
		cout << "Tidak ada data peminjaman." << endl;
		return;
	}
	string judul, nama;
	cout << "\n<<-------- INPUT DATA PENGEMBALIAN BUKU -------->>\n";
	cout << "Masukkan nama peminjam : ";
	cin.ignore();
	getline(cin, nama);
	cout << "Masukkan judul buku    : ";
	getline(cin, judul);
	Peminjaman *bantu = dataPinjam.head;
	Peminjaman *prev = NULL;
	
	while(bantu != NULL){
		if(bantu->judul == judul && bantu->nama == nama){
			Buku *buku = bukubuku.head;
			while(buku != NULL){
				if(buku->judul == judul){
					buku->stok++;
					break;
				}
				buku = buku->kiri;
			}
			
			if(prev == NULL){
				dataPinjam.head = bantu->next;
			}else{
				prev->next =bantu->next;
			}
		
			if(bantu == dataPinjam.tail){
				dataPinjam.tail = prev;
			}
			delete bantu;
			cout << "Buku berhasil dikembalikan." << endl;
			saveGak = true;
			return;
		}
		prev = bantu;
		bantu = bantu->next;
	}
	cout << "Data tidak ditemukan." << endl;
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
}

void simpanPeminjaman(){
	if(dataPinjam.head == NULL){
		cout << "Tidak ada yang meminjam." << endl; return;
	}
	
	FILE *filePeminjam;
	filePeminjam = fopen("dataPeminjaman.txt", "w");
	if(filePeminjam == NULL){
		cout << "File tidak bisa dibuka." << endl; return;
	}
	
	Peminjaman *bantu = dataPinjam.head;
	while(bantu != NULL){
		fprintf(filePeminjam, "%s;%s;%s\n", bantu->nama.c_str(), bantu->judul.c_str(), bantu->tanggal.c_str());
		
		bantu = bantu->next;
	}
	fclose(filePeminjam);
} 

void simpanSemua(){
	simpanFile();
	
	if(dataPinjam.head != NULL){
		simpanPeminjaman();
		cout << "\nBerhasil disimpan difile dataPerpustakaan.txt dan dataPeminjaman.txt"<< endl;
	}else{
		cout << "\nData buku berhasil disimpan difile dataPerpustakaan.txt (belum ada peminjaman)." << endl;
	}	
	saveGak = false;	
}	
	
void keluar(){
	char qsave;
	if(saveGak){
		cout << "Perubahan belum disimpan. Apakah akan Anda simpan? (y/n) ";
		cin >> qsave;
		if(qsave == 'y'){
			simpanSemua();
		}
	}
	cout << "\n+-----------------------------------------------------+" << endl;
	cout << "|     Terima kasih telah menggunakan program ini!     |" << endl;
	cout << "+-----------------------------------------------------+" << endl;
	exit(0);
}
