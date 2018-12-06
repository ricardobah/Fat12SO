#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define CABECAS 2
#define TRILHAS 80
#define SETORES_POR_TRILHA 18
#define BYTES_POR_SETOR 512

struct {
    unsigned char salto[3];
    unsigned char nome_oem[8];
    unsigned char bytes_por_setor[2];
    unsigned char setores_por_cluster[1];
    unsigned char setores_reservados[2];
    unsigned char copias_da_fat[1];
    unsigned char entradas_diretorio_raiz[2];
    unsigned char numero_de_setores[2];
    unsigned char descritor_de_midia[1];
    unsigned char setores_por_copia_da_fat[2];
    unsigned char setores_por_trilha[2];
    unsigned char numero_de_cabecas[2];
    unsigned char setores_ocultos[2];
    unsigned char codigo_de_boot[480];
    unsigned char assinatura_de_boot[2];
} BOOT;

int main(int argc, char *argv[])
{
    int fd;
    int i;
    int n;
    char c;
    
    // Codigo para depuracao.
    /*
    printf("argc = %d\n", argc);
    
    for (i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }
    */
    
    // Abre o arquivo do dispositivo DISCO FLEXIVEL
    // de 1.44 MB.
    fd = open(argv[1], O_RDWR | O_CREAT);
    
    c = 0;
    
    // Preencho com zeros.
    for (i = 0; i < CABECAS * TRILHAS * SETORES_POR_TRILHA * BYTES_POR_SETOR; i++) {
        n = write(fd, &c, sizeof(char));
    }
    
    memcpy(BOOT.salto, (char *)("\xeb\x1c\x90"), sizeof(BOOT.salto));
    memcpy(BOOT.nome_oem, (char *)("FAT12   "), sizeof(BOOT.nome_oem));
    memcpy(BOOT.bytes_por_setor, (char *)("\x00\x02"), sizeof(BOOT.bytes_por_setor));
    memcpy(BOOT.setores_por_cluster, (char *)("\x01"), sizeof(BOOT.setores_por_cluster));
    memcpy(BOOT.setores_reservados, (char *)("\x01\x00"), sizeof(BOOT.setores_reservados));
    memcpy(BOOT.copias_da_fat, (char *)("\x02"), sizeof(BOOT.copias_da_fat));
    memcpy(BOOT.entradas_diretorio_raiz, (char *)("\xe0\x00"), sizeof(BOOT.entradas_diretorio_raiz));
    memcpy(BOOT.numero_de_setores, (char *)("\x40\x0b"), sizeof(BOOT.numero_de_setores));
    memcpy(BOOT.descritor_de_midia, (char *)("\xf0"), sizeof(BOOT.descritor_de_midia));
    memcpy(BOOT.setores_por_copia_da_fat, (char *)("\x09\x00"), sizeof(BOOT.setores_por_copia_da_fat));
    memcpy(BOOT.setores_por_trilha, (char *)("\x12\x00"), sizeof(BOOT.setores_por_trilha));
    memcpy(BOOT.numero_de_cabecas, (char *)("\x02\x00"), sizeof(BOOT.numero_de_cabecas));
    memcpy(BOOT.setores_ocultos, (char *)("\x00\x00"), sizeof(BOOT.setores_ocultos));
    memcpy(BOOT.codigo_de_boot, (char *)("\xeb\xfe\x90"), sizeof(BOOT.codigo_de_boot));
    memcpy(BOOT.assinatura_de_boot, (char *)("\x55\xaa"), sizeof(BOOT.assinatura_de_boot));
    
    printf("sizeof(BOOT) = %ld\n", sizeof(BOOT));
    
    lseek(fd, 0, SEEK_SET);
    
    n = write(fd, &BOOT, sizeof(BOOT));
    
    fd = close(fd);
    
    return 0;
}
