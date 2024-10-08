#include "tree.hpp"
using namespace std;

enum Menu {
    TAMBAH = 1,
    CEK_STRUKTUR,
    CEK_DATA,
    EDIT,
    HAPUS,
    KELUAR,
    CEO = 1,
    MANAJER,
    KARYAWAN,
    NAMA = 1,
    ID,
};

void cls() {
    cout << "\033[H\033[2J";
}

void menu() {
    cout << "\"Note : Pilih angka dalam menu dibawah\"" << endl;
    cout << "Ket :" << endl;
    cout << "   1. Tambah Karyawan" << endl;
    cout << "   2. Cek Struktur Karyawan" << endl;
    cout << "   3. Cek Data Karyawan" << endl;
    cout << "   4. Edit Data Karyawan" << endl;
    cout << "   5. Hapus Karyawan" << endl;
    cout << "   6. Keluar" << endl;
    cout << "\nPilihan Anda : ";
}

void cekInput(const string& input, int* k) {
    if(!input.empty()) {
        if(isdigit(input[0])) {
            // Jika input adalah angka
            *k = stoi(input);
        } else {
            *k = 0;
        }
    } else {
        *k = 0;
    }
}

void menu_1(string* input) {
    cout << "Ketik '1' untuk CEO, '2' untuk Manajer, dan '3' untuk Karyawan\n : ";
    getline(cin, *input);
}

void pilih_CEO_Manajer(Node* root, string nama, int a) {
    Node* result = nullptr;
    string nama_;
    while(true) {
        cls();
        root->preOrder();
        if(a == CEO) {
            cout << "Masukkan nama CEO yang akan menjadi atasannya : ";
            getline(cin, nama_);
            result = root->find(nama_);
            if(result != nullptr && result->getStatus(result) == "CEO") {
                break;
            } else {
                cout << "\nNama CEO tidak ditemukan.\nMasukkan nama yang sesuai atau batalkan penambahan." << endl;
                string a;
                cout << "Ketik '0' untuk ulang dan apapun untuk batal\n : ";
                getline(cin, a);
                if(a != "0")
                    return;
            }
        } else if(a == MANAJER) {
            cout << "Masukkan nama Manajer yang akan menjadi atasannya : ";
            getline(cin, nama_);
            result = root->find(nama_);
            if(result != nullptr && result->getStatus(result) == "Manajer") {
                break;
            } else {
                cout << "\nNama Manajer tidak ditemukan.\nMasukkan nama yang sesuai atau batalkan penambahan." << endl;
                string a;
                cout << "Ketik '0' untuk ulang dan apapun untuk batal\n : ";
                getline(cin, a);
                if(a != "0")
                    return;
            }
        }
    }
    result->insert(root, new Node(root->manipulasiNama(root, nama)));
    root->assignStatus();
}

void edit(Node* node, const int& k) {
    string nama_;
    cout << "Masukkan nama karyawan yang akan di edit datanya : ";
    getline(cin, nama_);
    Node* result = node->find(nama_);
    if(result != nullptr) {
        cls();
        if(k == NAMA) {
            result->editNama(node);
        } else if(k == ID) {
            result->editID(node);
        } else {
            result->editNo();
        }
    } else {
        cout << "Nama yang anda masukkan tidak sesuai dengan karyawan manapun." << endl;
        string a;
        cout << "Ketik '0' untuk ulang dan apapun untuk batal\n : ";
        getline(cin, a);
        if(a != "0")
            return;
        edit(node, k);
    }
}

int main() {
    string nama, input;
    int k = 0;

    cls();
    cout <<  "Masukkan nama PEMILIK PERUSAHAAN untuk struktur karyawan anda : ";
    getline(cin, nama);
    Node* root = new Node(nama);
    root->assignStatus();
    while(k != KELUAR) {
        cls();
        menu();
        getline(cin, input);
        cekInput(input, &k);
        switch(k) {
            case TAMBAH: {
                while(true) {
                    cls();
                    menu_1(&input);
                    cekInput(input, &k);
                    if(k == CEO) {   
                        cout << "\n\nMasukkan nama CEO (Pemimpin Perusahaan) untuk struktur karyawan anda : ";
                        getline(cin, nama);
                        root->insert(root, new Node(root->manipulasiNama(root, nama)));
                        root->assignStatus();
                        cout << "\nCEO berhasil ditambahkan";
                        getchar();
                        break;
                    } else if(k == KARYAWAN || k == MANAJER) {
                        string a;
                        if(k == MANAJER) 
                            a = "Manajer";
                        else if(k == KARYAWAN)
                            a = "Karyawan";
                        cout << "\n\nMasukkan nama " << a << " untuk struktur karyawan anda : ";
                        getline(cin, nama);
                        pilih_CEO_Manajer(root, nama, (k - 1));
                        cout << endl << a << " berhasil ditambahkan";
                        getchar();
                        break;
                    } else {
                        cout << "\nMasukkan angka yang sesuai!";
                        getchar();
                    }
                }
                break;
            }
            case CEK_STRUKTUR: {
                cls();
                root->preOrder();
                getchar();
                break;
            }
            case CEK_DATA: {
                cls();
                root->levelOrder();
                getchar();
                break;
            }
            case EDIT: {
                while(true) {
                    cls();
                    root->levelOrder();
                    cout << "\nKetik '1' untuk edit Nama, '2' untuk ID, dan '3' untuk Nomor Telepon\n : ";
                    getline(cin, input);
                    cekInput(input, &k);
                    if(k < 1 || k > 3) {
                        cout << "Pilihan anda tidak terdapat dalam menu.\nTolong pilih menu yang sesuai!";
                        getchar();
                    } else {
                        edit(root, k);
                        cls();
                        root->levelOrder();
                        getchar();
                        break;
                    }
                }
                break;
            }
            case HAPUS: {
                cls();
                root->remove();
                break;
            }
            case KELUAR: {
                cls();
                cout << "Terima kasih\nSemoga amal dan ibadah anda diterima." << endl;
                getchar();
                cls();
                cout << "Panjang umur tentunya :p";
                break;
            }
            default: {
                cout << "\nPilihan anda tidak terdapat dalam menu.\nTolong pilih menu yang sesuai!";
                getchar();
                break;
            }
        }
    }
    return 0;
}