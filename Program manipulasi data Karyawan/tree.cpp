#ifndef TREE_HPP
#define TREE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <iomanip>
using namespace std;

class Node {
private:
    string nama, status, no_telp;
    vector<Node*> bawahan;
    map<string, string> id;
    Node* atasan;

    void _assignStatus(Node* node, int depth = 0) {
        if(depth == 0) {
            node->status = "Pemilik Perusahaan";
        } else if(depth == 1) {
            node->status = "CEO";
        } else if(depth == 2) {
            node->status = "Manajer";
        } else {
            node->status = "Karyawan";
        }

        for(Node* child : node->bawahan) {
            _assignStatus(child, depth + 1);
        }
    }

    Node* findParent(Node* node, const string& targetname) {
        if(node == nullptr || node->bawahan.empty()) {
            return nullptr;
        }
        for(Node* child : node->bawahan) {
            if(child->nama == targetname) {
                return node;
            }
            Node* parent = findParent(child, targetname);
            if(parent != nullptr) {
                return parent;
            }
        }
        return nullptr;
    }

    void _preOrder(unsigned int depth = 0) {
        string pangkat;
        if(depth != 0) {
            if(depth == 1) {
                pangkat = " - CEO : ";
            } else if(depth == 2) {
                pangkat = "   -- Manajer : ";
            } else if(depth == 3) {
                pangkat = "      --- Karyawan : ";
            }
            cout << pangkat << nama << endl;
        } else {
            if(bawahan.empty()) {
                cout << "   -Belum ada karyawan-" << endl;
                return;
            }
        }
        for(Node* child : bawahan) {
            child->_preOrder(depth + 1);
        }
    }

    void _levelOrder() {
        //Membuat antrian untuk menyimpan node yang akan diproses
        if(bawahan.empty()) {
            cout << "   -Belum ada karyawan-" << endl;
            return;
        }
        queue<Node*> data;
        //Memasukkan node awal ke dalam antrian
        data.push(this);
        //Melakukan penelusuran level order
        while(!data.empty()) {
            //Mengambil node dari depan antrian
            Node* current = data.front();
            data.pop();
            //Menampilkan data anak dari node saat ini
            for(Node* child : current->bawahan) {
                cout << left << "ID: " << setw(5) << child->id[child->nama];
                cout << left << "Nama: " << setw(25) << child->nama;
                cout << left << "Status: " << setw(10) << child->status;
                cout << left << "No Telp: " << child->no_telp << endl;
                //Menambahkan anak-anak node saat ini ke dalam antrian
                data.push(child);
            }
        }
    }

    bool cekID(const string& value) {
        for(const auto& map : id) {
            if(map.second == value) {
                return true; //Jika nilai ditemukan dalam map id di node saat ini
            }
        }

        // Telusuri bawahan secara rekursif
        for(Node* child : bawahan) {
            if(child->cekID(value)) {
                return true; //Jika nilai ditemukan di salah satu bawahan
            }
        }

        return false; //Jika nilai tidak ditemukan di node saat ini atau di bawahnya
    }

public:
    Node(const string name, Node* parent = nullptr) : nama(name), atasan(parent) {}

    string manipulasiID(string uniqueID) {
        //Jika ya, tambahkan (n) ke ID sampai menemukan ID yang unik
        int counter = 1;
        string hold = uniqueID;
        while(cekID(hold)) { //Periksa apakah ID unik
            hold = uniqueID + "(" + to_string(counter++) + ")";
        }
        return hold;
    }

    string manipulasiNama(Node* node, string nama) {
        if(node->find(nama) == nullptr) {
            return nama; // Jika nama unik, kembalikan nama aslinya
        }

        int counter = 1;
        string uniqueName;
        do {
            uniqueName = nama + "(" + to_string(counter++) + ")";
        } while(node->find(uniqueName) != nullptr); // Periksa apakah nama unik

        return uniqueName; // Kembalikan nama yang telah dimodifikasi
    }

    void insert(Node* root, Node* node) {
        string uniqueID;
        node->atasan = this;
        bawahan.push_back(node);
        cout << "Masukkan ID            : ";
        getline(cin, uniqueID);
        node->id[node->nama] = root->manipulasiID(uniqueID);
        cout << "Masukkan Nomor Telepon : ";
        getline(cin, node->no_telp);
    }

    void assignStatus() {
        _assignStatus(this);
    }

    Node* find(const string& targetName) {
        if(nama == targetName) {
            return this;
        }
        for(Node* child : bawahan) {
            Node* result = child->find(targetName);
            if(result != nullptr) {
                return result;
            }
        }
        return nullptr;
    }

    void remove() {
        string nama_;
        while(true) {
            cout << "\033[H\033[2J";
            preOrder();
            cout << "Masukkan nama karyawan yang akan anda hapus : ";
            getline(cin, nama_);
            Node* result = find(nama_);
            if(result != nullptr) {
                // Jika node yang akan dihapus adalah CEO atau Manager
                if(result->status != "Pemilik Perusahaan") {
                    if(result->bawahan.empty()) {
                        Node* parent = findParent(this, nama_);
                        if(parent != nullptr) {
                            auto it = parent->bawahan.begin();
                            while (it != parent->bawahan.end()) {
                                if ((*it)->nama == nama_) {
                                    // Atur atasan karyawan yang dihapus ke nullptr
                                    (*it)->atasan = nullptr;
                                    // Hapus node karyawan dari struktur pohon
                                    it = parent->bawahan.erase(it);
                                    cout << "\nKaryawan " << nama_ << " berhasil dihapus." << endl;
                                } else {
                                    ++it;
                                }
                            }
                            // Perbarui status di seluruh pohon setelah penghapusan
                            assignStatus();
                            getchar();
                            break;
                        }
                    } else {
                        cout << "\nTidak dapat menghapus " << nama_ << "." << endl;
                        cout << "Hapus bawahan terlebih dahulu atau pilih menu edit jika ingin mengganti." << endl;
                    }
                } else {
                    cout << "\nTidak dapat menghapus Pemilik Perusahaan.";
                }
            } else {
                cout << "\nNama yang anda masukkan tidak terdapat dalam daftar.";
            }
            getchar();
            string a;
            cout << "\n\nKetik '0' untuk ulang dan apapun untuk batal\n : ";
            getline(cin, a);
            if(a != "0")
                return;
        }
    }

    void editNama(Node* root) {
        string nama_;
        cout << "Masukkan nama baru : ";
        getline(cin, nama_);
        string ID = id[nama];
        id.erase(nama);
        nama = manipulasiNama(root, nama_);
        id[nama] = ID;
    }

    void editID(Node* root) {
        string ID;
        cout << "Masukkan ID baru : ";
        getline(cin, ID);
        id.erase(nama);
        id[nama] = root->manipulasiID(ID);
    }

    void editNo() {
        string No;
        cout << "Masukkan Nomor Telepon baru : ";
        getline(cin, No);
        no_telp = No;
    }

    void preOrder() {
        cout << "Struktur Karyawan Pak/Bu " << nama << " : " << endl;
        cout << "----------------------------------------------------------\n";
        _preOrder();
    }

    void levelOrder() {
        cout << "Data Karyawan Pak/Bu " << nama << " : " << endl;
        cout << "----------------------------------------------------------\n";
        _levelOrder();
    }

    string getStatus(Node* node) {
        return node->status;
    }

    ~Node() {
        for(Node* child : bawahan) {
            delete child;
        }
    }
};

#endif
