#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef unsigned long long formatoBinario;
formatoBinario memoria[1000]; 
FILE *arquivo;
char linha[100];

formatoBinario MASCARA_Endereco  = 0b00000000111111111111;
formatoBinario MASCARA_Opcode    = 0b11111111000000000000;
formatoBinario MASCARA_40Bits    = 0b1111111111111111111111111111111111111111;
formatoBinario MASCARA_20Bits    = 0b0000000000000000000011111111111111111111;

formatoBinario MASCARA_00_07Bits = 0b0000000011111111111111111111111111111111;
formatoBinario MASCARA_08_19Bits = 0b1111111100000000000011111111111111111111;
formatoBinario MASCARA_20_27Bits = 0b1111111111111111111100000000111111111111;
formatoBinario MASCARA_28_39Bits = 0b1111111111111111111111111111000000000000;

struct CPU {
    unsigned short PC;  // Program Counter                      12 bits
    
    unsigned short IR;  // Instruction Register                 8  bits
    unsigned short MAR; // Memory Address Register              12 bits
    unsigned short IBR; // Instruction Buffer Register          20 bits
    
    long int MBR;       // Memory Buffer Register               40 bits
    long int AC;        // Acumulador                           40 bits
    long int MQ;        // Multiplicador Quociente              40 bits

    bool LorR;          // 0 instrucao de IR, 1 instr de IBR    1  bit FLAG
    bool leituraCompleta// 0 le so a direita, 1 le esq+dir      1  bit FLAG
} cpu;

void criar_CPU() {
    cpu.AC  = 0b0 << 40ULL;
    cpu.MQ  = 0b0 << 40ULL;
    cpu.MBR = 0b0 << 40ULL;
    cpu.PC  = 0b0 << 12ULL;

    cpu.MAR = 0b0 << 12ULL;
    cpu.IR  = 0b0 <<  8ULL;
    cpu.IBR = 0b0 << 20ULL;

    cpu.LorR = 0;
    cpu.leituraCompleta = 1;
    return;
}

formatoBinario codificador(char linha[100], int informacao){
    /*Responsável por codificar *linha* para o formato correto a ser armazenado na memória*/
    int opcode;
    int informacao_binario = 0; //tanto dado quanto endereco
    formatoBinario palavra;
    long long numero_negativo;
    long long mascara = 0;

    // int resultado;
    // int resto;
    // int flag = 0;
    // int fator_aumento = 1;

    // while(informacao != 1){
    //     resto = informacao % 2;
    //     if(resto == 1){
    //         informacao = informacao - 1;
    //         informacao = informacao / 2;
    //         flag = 1;
    //     }else{
    //         informacao = informacao / 2;
    //         flag = 0;
    //     }

    //     if(flag){
    //         informacao_binario = informacao_binario + fator_aumento;
    //     }

    //     fator_aumento = fator_aumento * 10;
    // }

    // informacao_binario = informacao_binario + fator_aumento;
    
    if(!strcmp(linha, "LOAD MQ")){
        opcode = 0b00001010;
        palavra = opcode << 12ULL;
    }else if(!strcmp(linha, "LOAD MQ.M(X)")){
        opcode = 0b00001001;
        palavra = opcode << 12ULL;
        palavra = palavra | informacao;
    }else if(!strcmp(linha, "STOR M(X)")){
        opcode = 0b00100001;
        palavra = opcode << 12ULL;
        palavra = palavra | informacao;
    }else if(!strcmp(linha, "LOAD M(X)")){
        opcode = 0b00000001;
        palavra = opcode << 12ULL;
        palavra = palavra | informacao;
    }else if(!strcmp(linha, "LOAD-M(X)")){
        opcode = 0b00000010;
        palavra = opcode << 12ULL;
        palavra = palavra | informacao;
    }else if(!strcmp(linha, "LOAD |M(X)|")){
        opcode = 0b00000011;
        palavra = opcode << 12ULL;
        palavra = palavra | informacao;
    }else if(!strcmp(linha, "LOAD-|M(X)|")){
        opcode = 0b00000100;
        palavra = opcode << 12ULL;
        palavra = palavra | informacao;
    }else if(!strcmp(linha, "JUMP M(X,0:19)")){
        opcode = 0b00001101;
        palavra = opcode << 12ULL;
        palavra = palavra | informacao;
    }else if(!strcmp(linha, "JUMP M(X,20:39)")){
        opcode = 0b00001110;
        palavra = opcode << 12ULL;
        palavra = palavra | informacao;
    }else if(!strcmp(linha, "JUMP+M(X,0:19)")){
        opcode = 0b00001111;
        palavra = opcode << 12ULL;
        palavra = palavra | informacao;
    }else if(!strcmp(linha, "JUMP+M(X,20:39)")){
        opcode = 0b00010000;
        palavra = opcode << 12ULL;
        palavra = palavra | informacao;
    }else if(!strcmp(linha, "ADD M(X)")){
        opcode = 0b00000101;
        palavra = opcode << 12ULL;
        palavra = palavra | informacao;
    }else if(!strcmp(linha, "ADD |M(X)|")){
        opcode = 0b00000111;
        palavra = opcode << 12ULL;
        palavra = palavra | informacao;
    }else if(!strcmp(linha, "SUB M(X)")){
        opcode = 0b00000110;
        palavra = opcode << 12ULL;
        palavra = palavra | informacao;
    }else if(!strcmp(linha, "SUB |M(X)|")){
        opcode = 0b00001000;
        palavra = opcode << 12ULL;
        palavra = palavra | informacao;
    }else if(!strcmp(linha, "MUL M(X)")){
        opcode = 0b00001011;
        palavra = opcode << 12ULL;
        palavra = palavra | informacao;
    }else if(!strcmp(linha, "DIV M(X)")){
        opcode = 0b00001100;
        palavra = opcode << 12ULL;
        palavra = palavra | informacao;
    }else if(!strcmp(linha, "LSH")){
        opcode = 0b00010100;
        palavra = opcode << 12ULL;
    }else if(!strcmp(linha, "RSH")){
        opcode = 0b00010101;
        palavra = opcode << 12ULL;
    }else if(!strcmp(linha, "STOR M(X,8:19)")){
        opcode = 0b00010010;
        palavra = opcode << 12ULL;
        palavra = palavra | informacao;
    }else if(!strcmp(linha, "STOR M(X,28:39)")){
        opcode = 0b00010011;
        palavra = opcode << 12ULL;
        palavra = palavra | informacao;
    }else{
        if(linha[0] == '-'){
            numero_negativo = informacao * -1;
            mascara = (1ULL << 40) - 1;
            palavra = numero_negativo & mascara;
        }else{
            palavra = 0ULL;
            palavra = palavra << 39;
            palavra = palavra | informacao;
        }
    }

    return palavra;
}

int gerenciador_memoria(){
    /*Responsável por ler o arquivo e devolver uma linha*/
    FILE *arquivo = fopen("programa.txt", "r");
    char caractere;
    formatoBinario palavra;
    int posicao_dados = 0, posicao_instrucao = 100;
    int flag_instrucao = 0;
    int i = 0;              // indica a posição onde o próximo caractere vai ser inserido
    int fim_informacao = 0; // flag que indica se o endereço já foi extraido
    int informacao = 0;     // pode ser um endereço ou um dado
    int numero;             // buffer que armazena o caractere convertido para inteiro


    if (arquivo == NULL) {
        printf("ERRO : Arquivo invalido/nao encontrado.\n");
        return 1;
    }
    
    while ((caractere = fgetc(arquivo)) != EOF){
        if (caractere == '\r') continue; // Ignora o lixo do Windows
        if(caractere != '\n'){
            if((caractere >= '0' && caractere <= '9') && !fim_informacao) {
                if(informacao){
                    numero = caractere - '0';
                    informacao = informacao * 10 + numero;
                }else{
                    informacao = caractere - '0';
                }
            }else{
                if((caractere == ')' || caractere == ',') && !fim_informacao){
                    fim_informacao = 1;
                    linha[i] = 'X';
                    linha[i + 1] = caractere;
                    i += 2;
                }else{
                    linha[i] = caractere;
                    i++;
                }
            }
        }else{
            linha[i] = '\0';
            i = 0;
            palavra = codificador(linha, informacao);
            if(linha[0] == ' ' || linha[0] == '-'){
                if (posicao_dados == 100) {printf("ERRO LOGICO:\n - Este computador NÃO suporta mais de 100 dados."); }
                memoria[posicao_dados] = palavra;
                posicao_dados++;

            }else{
                
                if(flag_instrucao){ //indica se a posição atual da memória precisa de mais uma instrução
                    memoria[posicao_instrucao] = memoria[posicao_instrucao] | palavra;
                    posicao_instrucao++;
                    flag_instrucao = 0;
                }else{
                    memoria[posicao_instrucao] = palavra << 20ULL;
                    flag_instrucao = 1;
                } //essas verificações não vem se eu já estorei o tamanho da memória ou se passei do limite da area de dados, vou mexer nisso depois
            }
            informacao = 0;
            fim_informacao = 0;
        }
    }

    fclose(arquivo);
    memoria[posicao_instrucao] = 0b0 << 39;
    return 0;
}



void ciclo_busca() {
    if (cpu.leituraCompleta) {
        if (cpu.LorR) {                             // Falta executar instrucao da esquerda
            cpu.IR  = cpu.IBR >> 12ULL;
            cpu.MAR = cpu.IBR & MASCARA_Endereco;

            cpu.PC++;
            cpu.LorR = 0;
        } else {                                    // Leitura de Novas Intrucoes
            cpu.MAR = cpu.PC;
            cpu.MBR = memoria[cpu.MAR];

            cpu.IR  = (cpu.MBR >> 32ULL);
            cpu.MAR = (cpu.MBR >> 20ULL) & MASCARA_Endereco;
            cpu.IBR =  cpu.MBR & MASCARA_20Bits;
            cpu.LorR = 1;
        }
    } else {
        cpu.MAR = cpu.PC;
        cpu.MBR = memoria[cpu.MAR];

        cpu.IR  = cpu.MBR >> 12ULL;
        cpu.MAR = cpu.MBR & MASCARA_Endereco;

        cpu.PC++;
        cpu.LorR = 0;
        cpu.leituraCompleta = 1;
    }
}

void ciclo_execucao() {
    unsigned short opcode = cpu.IR;
    int endereco = cpu.MAR;

    if(opcode == 0b00001010){       // LOAD MQ
        cpu.AC = cpu.MQ;

    }else if(opcode == 0b00001001){ // LOAD MQ.M(X)
        cpu.MQ = memoria[endereco];

    }else if(opcode == 0b00100001){ // STOR M(X)
        memoria[endereco] = cpu.AC;

    }else if(opcode == 0b00000001){ // LOAD M(X)
        cpu.AC = memoria[endereco];

    }else if(opcode == 0b00000010){ // LOAD-M(X)
        cpu.AC = memoria[endereco] * -1;

    }else if(opcode == 0b00000011){ // LOAD |M(X)|
        cpu.AC = abs(memoria[endereco]);

    }else if(opcode == 0b00000100){ // LOAD-|M(X)|
        cpu.AC = abs(memoria[endereco]) * -1;
        
    }else if(opcode == 0b00001101){ // JUMP M(X,0:19)
        cpu.PC = endereco;
        cpu.LorR = 0;

    }else if(opcode == 0b00001110){ // JUMP M(X,20:39)
        cpu.PC = endereco;
        cpu.leituraCompleta = 0;

    }else if(opcode == 0b00001111){ // JUMP+M(X,0:19)
        if (cpu.AC >= 0) {
            cpu.PC = endereco;
            cpu.LorR = 0;
        }

    }else if(opcode == 0b00010000){ // JUMP+M(X,20:39)
        if (cpu.AC >= 0) {
            cpu.PC = endereco;
            cpu.leituraCompleta = 0;
        }

    }else if(opcode == 0b00000101){ // ADD M(X)
        cpu.AC += memoria[endereco];

    }else if(opcode == 0b00000111){ // ADD |M(X)|
        cpu.AC += abs(memoria[endereco]);

    }else if(opcode == 0b00000110){ // SUB M(X)
        cpu.AC -= memoria[endereco];

    }else if(opcode == 0b00001000){ // SUB |M(X)|
        cpu.AC -= abs(memoria[endereco]);

    }else if(opcode == 0b00001011){ // MUL M(X)
        cpu.MQ = (cpu.MQ * memoria[endereco]) & MASCARA_40Bits;  // Nao sei se cpu.MQ * memoria[endereco] cabe na memoria
        cpu.AC = (cpu.MQ * memoria[endereco]) >> 40ULL;

    }else if(opcode == 0b00001100){ // DIV M(X)
        cpu.AC /= memoria[endereco];
        cpu.MQ  %= memoria[endereco];

    }else if(opcode == 0b00010100){ // LSH
        cpu.AC = cpu.AC << 1;
        
    }else if(opcode == 0b00010101){ // RSH
        cpu.AC = cpu.AC >> 1;
        
    }else if(opcode == 0b00010010){ // STOR M(X,8:19)
        int informacao = (cpu.AC & MASCARA_Endereco) << 20ULL;
        memoria[endereco] = memoria[endereco] & MASCARA_08_19Bits;
        memoria[endereco] = memoria[endereco] | informacao;
        
    }else if(opcode == 0b00010011){ // STOR M(X,28:39)
        int informacao = cpu.AC & MASCARA_Endereco;
        memoria[endereco] = memoria[endereco] & MASCARA_28_39Bits;
        memoria[endereco] = memoria[endereco] | informacao;

    }else if(opcode == 0b00000000){

        printf("ERRO : Opcode nao identificado.");
    } 
    return;
}

void ciclo_instrucao(formatoBinario codigo){
    ciclo_busca();
    ciclo_execucao();
}


void cpu_print(int i){
    printf(" - %d esimo(a) execucao.", i);
    printf("\n - MBR : %lu", cpu.MBR);
    printf("\n - AC  : %lu  |  MQ  : %lu", cpu.AC, cpu.MQ);
    printf("\n - IBR : %hu", cpu.IBR);
    printf("\n - IR  : %hu  |  MAR : %hu", cpu.IR, cpu.MAR);
    printf("\n - PC  : %hu", cpu.PC);
}

void main() {
    printf("\n\n ====================== INICIANDO COMPUTADOR IAS ======================\n\n\n");
    
    criar_CPU();
    if (gerenciador_memoria()) return;
    


    int i = 0;
    while (memoria[100 + i] != 0b0000000000000000000000000000000000000000);
        ciclo_instrucao(memoria[100+i]);
        i++;
    
    printf("\n\n\n ========================== FIM DO PROGRAMA ==========================\n\n");
}
