// a program to generate a table of number representations in different bases
#include <stdio.h>

int main(void) {
    printf("Power | Decimal | Octal | Quaternary | Binary\n");
    printf("------|---------|-------|------------|----------------\n");
    
    for (int i = 0; i <= 12; i++) {
        int value = 1 << i;
        
        printf("2^%-3d | %-7d | %-5o | ", i, value, value);
        
        // Print quaternary (base-4)
        int temp = value;
        int quat[20];
        int quat_digits = 0;
        
        do {
            quat[quat_digits++] = temp % 4;
            temp /= 4;
        } while (temp > 0);
        
        for (int j = quat_digits - 1; j >= 0; j--) {
            printf("%d", quat[j]);
        }
        
        printf("%*s | ", 10 - quat_digits, "");
        
        // Print binary
        for (int j = 12; j >= 0; j--) {
            if (value & (1 << j)) {
                printf("1");
            } else if (j < i) {
                printf("0");
            }
        }
        
        printf("\n");
    }
    
    return 0;
}
