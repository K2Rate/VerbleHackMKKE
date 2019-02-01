#include <stdio.h>
#include <Windows.h>
#include <TlHelp32.h>


HANDLE GetMKKE()
{
	printf("wait run MKKE.exe ...");

point_get_snapshot:
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnapshot == NULL)
	{
		static int reset_count = 0;
		reset_count++;
		printf("CreateToolhelp32Snapshot failed :( restart ¹%d / 5\n", reset_count);
		Sleep(500);
		if (reset_count == 5)
		{
			exit(-1);
		}
		goto point_get_snapshot;
	}
	
	PROCESSENTRY32 pe32; pe32.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hSnapshot, &pe32) == TRUE)
	{
		while (Process32Next(hSnapshot, &pe32) == TRUE)
		{
			if (stricmp(pe32.szExeFile, "MKKE.exe") == 0)
			{
				printf("\nMKKE.exe finded!");
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
				if (hProcess == NULL) {
					printf("\nMKKE.exe open error :(, run hack how administrator"); Sleep(1000); exit(-1);
				}
				else {
					printf("\nMKKE.exe success opened :)");
					system("cls");
					printf("VerbleHack v0.0\nF12 - Update process\nF1 - Heal me\nF2 - Kill opponent\nF3 - Add time\n");
					return hProcess;
				}
			}
		}
	}

	Sleep(200);
	goto point_get_snapshot;
}

int main()
{
	HANDLE hProcess = GetMKKE();

	while (true)
	{
		if (GetAsyncKeyState(VK_F12))
		{
			hProcess = GetMKKE();
			Beep(1000, 90);
		}
		if (GetAsyncKeyState(VK_F1))
		{
			const DWORD float_1 = 0x3F800000; DWORD my_animal;
			ReadProcessMemory(hProcess, (void*)0x00E8A968, &my_animal, sizeof(DWORD), NULL);
			WriteProcessMemory(hProcess, (void*)(my_animal + 0x78C0), &float_1, sizeof(DWORD), NULL);
		}
		if (GetAsyncKeyState(VK_F2))
		{
			const DWORD float_0 = 0x00000000; DWORD opponent_animal;
			ReadProcessMemory(hProcess, (void*)0x00E8AA24, &opponent_animal, sizeof(DWORD), NULL);
			WriteProcessMemory(hProcess, (void*)(opponent_animal + 0x78C0), &float_0, sizeof(DWORD), NULL);
		}
		if (GetAsyncKeyState(VK_F3) & 1)
		{
			DWORD bytes = 0;
			ReadProcessMemory(hProcess, (void*)0x00E91C80, &bytes, 4, NULL);
			bytes += 60;
			WriteProcessMemory(hProcess, (void*)0x00E91C80, &bytes, 4, NULL);
		}


		Sleep(100);
	}

	CloseHandle(hProcess);
	return 0;
}