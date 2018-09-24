// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"
#include "afxsock.h"
#include<string.h>
#include<stdlib.h>
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// viet them mot so ham de su dung cho chuong trinh
struct QuanLyTen {
	char a[11];
};
bool KTraTenCoHopLe(char m[11]);
bool KTraTenCoTrungTen(QuanLyTen*,int,char c[11]);
bool KTraChuoi(char m[11], char n[11]);
void nhap(char a[11], char d[11]);
int SoLuongDoanDung(int, int);
int SoLuongViTriDoanDung(int, int);

// The one and only application object
CWinApp theApp;
using namespace std;
//Kiem tra co phai la so nguyen to hay khong
int main(){
    int nRetCode = 0;
    HMODULE hModule = ::GetModuleHandle(nullptr);
	if (hModule != nullptr) {
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0)) {
			// TODO: change error code to suit your needs
			wprintf(L"Fatal Error: MFC initialization failed\n");
			nRetCode = 1;
		}
		else {
			CSocket server, s;
			unsigned int port = 1234;
			AfxSocketInit(NULL);
			server.Create(port);
			server.Listen(5);
			//Nhap so luong client
			printf("\n Nhap so luong Client: ");
			int num_client;
			scanf("%d", &num_client);
			printf("\n Dang lang nghe ket noi tu Client...\n");
			//Tao mang chua cac socket client
			CSocket *sockClients = new CSocket[num_client];
			for (int i = 0; i < num_client; i++) {
				server.Accept(sockClients[i]);
				printf("Da tiep nhan client %d/%d\n", i + 1, num_client);
			}
			//tao mang dong chua ten
			QuanLyTen *ten = new QuanLyTen[num_client];
			// keu tung client ket noi va tao ten name.
			for (int i = 0; i < num_client; i++) {
				bool temp = false;
				do {
					//server gui client yeu cau nhap ten
					char m[100] = "Moi nhap ten Nickname('a'...'z', 'A'...'Z', '0'...'9' và dai khong qua 10 ky tu)";
					sockClients[i].Send(&m, sizeof(m), 0);
					// nhan ten user
					char id[11];
					sockClients[i].Receive(&id, sizeof(id), 0);
					printf("User %d: %s\n", i + 1, id);
					if (KTraTenCoHopLe(id) == true) {
						if (i == 0) {
							nhap(ten[i].a, id);
							temp = true;
						}
						else if (i != 0) {
							if (KTraTenCoTrungTen(ten, num_client, id)) {
								nhap(ten[i].a, id);
								temp = true;
							}
							else {
								temp = false;
							}
						}
					}
					// neu tao ten loi thi gui 0 cho client biet de client tao ten moi.
					int numbers;
					if (temp == false) {
						numbers = 0;
					}
					else if (temp == true) {
						numbers = 1;
					}
					sockClients[i].Send((char*)&numbers, sizeof(int), 0);
				} while (temp == false);
			}
			//server gui toi may client tro choi bat dau
			for (int i = 0; i < num_client; i++) {
				//server gui client yeu cau nhap ten
				char nn[20] = "Tro choi bat dau";
				sockClients[i].Send(&nn, sizeof(nn), 0);
			}
			//server tao 5 so ngau nhien
			srand((unsigned int)time(NULL));
			int So = rand() % 90000 + 10000;
			printf("So phat sinh ngau nhien : %d\n", So);
			//so cac client gui
			int numbers, soluongdung, dungVitri;
			int vitri, lan = 1;
			bool falg = false;
			do {
				printf("Du Doan lan thu : %d\n",lan);
				for (int i = 0; i < num_client; i++) {
					// gui thong bao keu client nhap so doan
					char tbDoan[22] = "Moi ban du doan 5 so";
					sockClients[i].Send(&tbDoan, sizeof(tbDoan), 0);
					// nhan thong diep tu client
					sockClients[i].Receive((char*)&numbers, sizeof(int), 0);
					printf("%s :%d\n", ten[i].a, numbers);
					soluongdung = SoLuongDoanDung(So, numbers);
					sockClients[i].Send((char*)&soluongdung, sizeof(int), 0);
					dungVitri = SoLuongViTriDoanDung(So, numbers);
					sockClients[i].Send((char*)&dungVitri, sizeof(int), 0);
					if (soluongdung == 5 && dungVitri == 5) {
						vitri = i;
						char ketqua1[40]; // chua ten nickname nguoi thang cuoc.
						nhap(ketqua1, ten[vitri].a);
						char tbchonguoithang[40] = "Chuc Mung Ban La Nguoi Chien Thang";
						sockClients[vitri].Send(&tbchonguoithang, sizeof(tbchonguoithang), 0);
						for (int i = 0; i < vitri; i++) {
							int ketqua = 20;
							sockClients[i].Send((char*)&ketqua, sizeof(int), 0);
							sockClients[i].Send(&ketqua, sizeof(ketqua), 0);
						}
						for (int i = vitri+1; i < num_client; i++) {
							int ketqua = 20;
							sockClients[i].Send((char*)&ketqua, sizeof(int), 0);
							sockClients[i].Send(&ketqua, sizeof(ketqua), 0);
						}
						falg = true;
						break;
					}
				}
				lan++;
			} while (falg == false);
			for (int i = 0; i < num_client; i++) {
				sockClients[i].Close();
			}
			getchar();
			server.Close();
		}
	}
    else{
        // TODO: change error code to suit your needs
        wprintf(L"Fatal Error: GetModuleHandle failed\n");
        nRetCode = 1;
    }

    return nRetCode;
}

bool KTraTenCoHopLe(char m[11]){
	int dem = 0;
	int n = strlen(m);
	for (int i = 0;i < n;i++) {
		if ((m[i] >= 48 && m[i] <= 57) || (m[i] >= 65 && m[i] <= 90) ||
			(m[i] >= 97 && m[i] <= 122) || m[i] == 32) {
			dem++;
		}
		else {
			return false;
		}
	}
	if (n == dem) {
		return true;
	}
	return false;
}
bool KTraTenCoTrungTen(QuanLyTen* d, int b, char c[11]){
	for (int i = 0; i < b; i++) {
		if (strcmp(d[i].a, c) == 0) {
			return false;
		}
	}
		return true;
}
bool KTraChuoi(char m[11], char n[11]) {
	int a = strlen(m);
	int b = strlen(n);
	int dem = 0;
	if (a == b) {
		for (int i = 0;i < a;i++) {
			if (m[i] == n[i]) {
				dem++;
			}
		}
		if (dem == a || dem == b) {
			return true;
		}
	}
	return false;
}
void nhap(char a[11], char d[11]) {
	int c = strlen(d);
	int j = 0;
	for (int i = 0;i < c;i++) {
		a[j++] = d[i];
	}
	a[j] = '\0';
}
int SoLuongDoanDung(int a, int b) {
	int *c = new int[5]; // luu gia tri a vao mang server
	int *d = new int[5];// luu gia tri b vso mang client
	int t, m, dem = 0;// bien tam
	for (int i = 4; i >= 0; i--) {
		t = a % 10;
		c[i] = t;
		a = a / 10;
		m = b % 10;
		d[i] = m;
		b = b / 10;
	}
	// kiem tra so phat sinh cua server co trung nhau khong
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (c[i] == d[j]) {
				dem++;
				break;
			}
		}
	}
	return dem;
}
int SoLuongViTriDoanDung(int a, int b) {
	int *c = new int[5]; // luu gia tri a vao mang server
	int *d = new int[5];// luu gia tri b vso mang client
	int t, m, dem = 0;// bien tam
	for (int i = 4; i >= 0; i--) {
		t = a % 10;
		c[i] = t;
		a = a / 10;
		m = b % 10;
		d[i] = m;
		b = b / 10;
	}
	// kiem tra so phat sinh cua server co trung nhau khong
	for (int i = 0; i < 5; i++) {
		if (c[i] == d[i]) {
			dem++;
		}
	}
	return dem;
}