#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void decrypt_files() {
    WIN32_FIND_DATA fd;
    HANDLE h = FindFirstFile("*.kkk", &fd);
    if (h != INVALID_HANDLE_VALUE) {
        do {
            char old_name[MAX_PATH];
            char new_name[MAX_PATH];
            sprintf(old_name, "%s", fd.cFileName);
            strcpy(new_name, old_name);
            
            // .kkk 확장자 제거
            int len = strlen(old_name);
            if (len > 4) new_name[len - 4] = '\0';

            FILE *f = fopen(old_name, "rb+");
            if (!f) continue;

            fseek(f, 0, SEEK_END);
            long s = ftell(f);
            rewind(f);

            unsigned char *b = (unsigned char *)malloc(s);
            fread(b, 1, s, f);

            // 암호화 시 사용한 0x77 키로 다시 XOR하여 복구
            for (int i = 0; i < s; i++) b[i] ^= 0x77;

            rewind(f);
            fwrite(b, 1, s, f);
            fclose(f);
            free(b);

            // 파일명 복구
            rename(old_name, new_name);
            printf("[OK] 복구 완료: %s\n", new_name);
        } while (FindNextFile(h, &fd));
        FindClose(h);
    }
}

int main() {
    printf("--- 복구 툴 실행 (즉시 복구 시작) ---\n");
    decrypt_files();
    printf("\n모든 파일이 정상적으로 복구되었습니다.\n");
    
    // 확인을 위해 3초 대기 후 자동 종료
    Sleep(3000);
    return 0;
}

