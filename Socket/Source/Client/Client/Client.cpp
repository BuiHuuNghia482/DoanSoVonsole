// Client.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Client.h"
#include "afxsock.h"
#include<string.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// The one and only application object
CWinApp theApp;
using namespace std;
int main(){
	int nRetCode = 0;
	HMODULE hModule = ::GetModuleHandle(nullptr);
	if (hModule != nullptr) {
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0)){
			// TODO: change error code to suit your needs
			wprintf(L"Fatal Error: MFC initialization failed\n");
			nRetCode = 1;
		}
		else{
			// TODO: code your application's behavior here.
			CSocket client;
			char sAdd[1000];
			unsigned int port = 1234; //Cung port voi server
			AfxSocketInit(NULL);

			//1. Tao socket
			client.Create();

			// Nhap dic chi IP cua server
			printf("\n Nhap dia chi IP cua server: ");
			gets_s(sAdd);
			if (client.Connect(CA2W(sAdd), port)){
				
				printf("\n Client da ket noi toi server");
				// gia su tao ten thanh cong
				int numbers1= 1;
				//Nhan tu server keu dac ten
				do {
					char id[100];
					client.Receive(&id, sizeof(id), 0);
					printf(" \nServer :  %s\n", id);
					printf("user : ");
					// user dac ten gui qua server
					char m[11];
					gets_s(m);
					client.Send(&m, sizeof(m), 0);
					//xem thong bao tu server tao nickname thanh cong khong
					client.Receive((char*)&numbers1, sizeof(int), 0);
					if (numbers1 == 0) {
						printf("\t\t\tBan tao nickname k hop le ! hoac trung ten voi nguoi khac !\n");
					}
					else {
						printf("\t\t\tBan Tao nickname thanh cong ! chuc mung %s !\n", m);
					}
					
				} while (numbers1 == 0);
				// xem thong bao bat dau tro choi
				char thongbao[20];
				client.Receive(&thongbao, sizeof(thongbao), 0);
				printf("\t\t\t\t%s\n", thongbao);
				int n1, n2, n3;
				bool fal = false;
				do {
					char tbDoan[22];
					client.Receive(&tbDoan, sizeof(tbDoan), 0);
					printf("Server : %s\n", tbDoan);
					int numbers;
					//Gui so thu 1 cho server
					do {
						printf("Nhap so : ");
						scanf("%d", &numbers);
					} while (numbers < 10000 || numbers>99999);
					client.Send((char*)&numbers, sizeof(int), 0);
					//nhan ket qua tu server
					client.Receive((char*)&n1, sizeof(int), 0);
					client.Receive((char*)&n2, sizeof(int), 0);
					printf("Server : %d la so luong chu so dung,  %d la so luong chu so dung vi tri\n", n1, n2);
					if (n1 == 5 && n2 == 5) {
						char tbDoan[40];
						client.Receive(&tbDoan, sizeof(tbDoan), 0);
						printf("Server : %s\n", tbDoan);
						fal = true;
						break;
					}
					client.Receive((char*)&n3, sizeof(int), 0);
					if (n3 == 20){
						char tbDoan[100];
						client.Receive(&tbDoan, sizeof(tbDoan), 0);
						printf("Server :\t Nguoi choi nickname  %s  da chien thang \n", tbDoan);
						fal = true;
						break;
					}
					// cho may khac biet la co nguoi da doan dung k can doan nua
				} while (fal== false);
			}
			client.Close();
		}
	}
	else{
		// TODO: change error code to suit your needs
		wprintf(L"Fatal Error: GetModuleHandle failed\n");
		nRetCode = 1;
	}
	return nRetCode;
}