/*  userspace_ioctl.c – процесс, позволяющий контролировать модуль ядра  
 *  с помощью ioctl.
 * 	
 *  До этого момента можно было использовать для ввода и вывода cat.
 *  Теперь необходимо использовать ioctl, для чего нужно написать свой  
 *  процесс.
 */ 
 
/* Детали устройства, такие как номера ioctl и старший файл устройства.  */ 
#include "chardev.h" 
 
#include <stdio.h> /* Стандартный ввод-вывод. */ 
#include <fcntl.h> /* Открытие. */ 
#include <unistd.h> /* Закрытие. */ 
#include <stdlib.h> /* Выход. */ 
#include <sys/ioctl.h> /* ioctl */ 
 
/* Функции для вызовов ioctl. */ 
 
int ioctl_set_msg(int file_desc, char *message) 
{ 
    int ret_val; 
 
    ret_val = ioctl(file_desc, IOCTL_SET_MSG, message); 
 
    if (ret_val < 0) { 
        printf("ioctl_set_msg failed:%d\n", ret_val); 
    } 
 
    return ret_val; 
} 
 
int ioctl_get_msg(int file_desc) 
{ 
    int ret_val; 
    char message[100] = { 0 }; 

    ret_val = ioctl(file_desc, IOCTL_GET_MSG, message); 
 
    if (ret_val < 0) { 
        printf("ioctl_get_msg failed:%d\n", ret_val); 
    } 
    printf("get_msg message:%s", message); 
 
    return ret_val; 
} 
 
int ioctl_get_nth_byte(int file_desc) 
{ 
    int i, c; 
 
    printf("get_nth_byte message:"); 
 
    i = 0; 
    do { 
        c = ioctl(file_desc, IOCTL_GET_NTH_BYTE, i++); 
 
        if (c < 0) { 
            printf("\nioctl_get_nth_byte failed at the %d'th byte:\n", i); 
            return c; 
        } 
 
        putchar(c); 
    } while (c != 0); 
 
    return 0; 
} 

int ioctl_find_char(int file_desc, char tofind) 
{ 
    int i, c; 

    c = ioctl(file_desc, IOCTL_FIND_СHAR, tofind); 

    if (c < 0) { 
        printf("\nIOCTL_FIND_СHAR nothing to find\n"); 
        return c; 
    } 
    printf("\nIOCTL_FIND_СHAR char %c found %d times", tofind, c);
 
    return 0; 
}

int ioctl_find_substring(int file_desc, char* tofind) 
{ 
    int i, c; 
 
    printf("get_nth_byte message:"); 
    c = ioctl(file_desc, IOCTL_FIND_STR, tofind); 

    if (c < 0) { 
        printf("\nIOCTL_FIND_STR nothing to find\n"); 
        return c; 
    } 
    printf("\nIOCTL_FIND_STR substring %s found at %d position", tofind, c);
 
    return 0; 
} 

char buffer[100]; 
/* Main – вызов функций ioctl. */ 
int main(void) 
{ 
    int file_desc, ret_val; 
    char *msg = "Message passed by ioctl\n"; 
 
    file_desc = open(DEVICE_PATH, O_RDWR); 
    if (file_desc < 0) { 
        printf("Can't open device file: %s, error:%d\n", DEVICE_PATH, 
               file_desc); 
        exit(EXIT_FAILURE); 
    } 
 
    char choice;
    while (1) {
        printf("\nМеню пользователя:\n");
        printf("q - выход\n");
        printf("w - ввести строку\n");
        printf("r - вывести строку\n");
        printf("c - найти символ в строке\n");
        printf("s - найти подстроку в строке\n");
        printf("d - вывести байты строки\n");

        printf("Введите символ: ");
        scanf(" %c", &choice);

        switch (choice) {
            case 'q':
                close(file_desc);
                return 0;
            case 'w':
                scanf("%s", buffer);
                ioctl_set_msg(file_desc, buffer);
                break;
            case 'r':
                ioctl_get_msg(file_desc);
                break;
            case 'c':
                char temp;
		printf("Введите символ: ");
                scanf(" %c", &temp);
                ioctl_find_char(file_desc, temp);
                break;
            case 's':
                scanf("%s", buffer);
                ioctl_find_substring(file_desc, buffer);
                break;
            case 'd':
                ret_val = ioctl_get_nth_byte(file_desc); 
                break;
            default:
                printf("Неверный символ, попробуйте снова\n");
        }
    }
    close(file_desc); 
    return 0;
}
