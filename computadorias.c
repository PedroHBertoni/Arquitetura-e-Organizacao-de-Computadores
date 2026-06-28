#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef unsigned long long formatoBinario;
formatoBinario memoria[1000];
FILE *arquivo;
char linha[100];

struct CPU {
    unsigned short PC;  // Program Counter
    unsigned short IR;  // Instruction Register
    unsigned short MAR; // Memory Address Register
    long int MBR;       // Memory Buffer Register
    long int AC;        // Acumulador
    long int M;         // Multiplicador
    long int R;         // Resto da divisão
    bool C;             // Carry (0 ou 1)
    bool N;             // 0 (positivo) ou 1 (negativo)
    bool Z;             // 0 (diferente de zero) ou 1 (zero)
};

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
    int teste = 0;

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
                teste = posicao_dados;
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
    return 0;
}

void executor(formatoBinario codigo, struct CPU cpu) {
    formatoBinario opcode = codigo >> 12ULL;
    formatoBinario mascara = (1ULL << 12)-1;
    
    if(codigo == 0b00001010){       // LOAD MQ
        cpu.AC = cpu.M;

    }else if(opcode == 0b00001001){ // LOAD MQ.M(X)
        formatoBinario mascara = (1ULL << 12)-1;
        int informacao = codigo & mascara;
        cpu.M = memoria[informacao];

    }else if(opcode == 0b00100001){ // STOR M(X)
        formatoBinario mascara = (1ULL << 12)-1;
        int informacao = codigo & mascara;
        memoria[informacao] = cpu.AC;

    }else if(opcode == 0b00000001){ // LOAD M(X)
        formatoBinario mascara = (1ULL << 12)-1;
        int informacao = codigo & mascara;
        cpu.AC = memoria[informacao];

    }else if(opcode == 0b00000010){ // LOAD-M(X)
        formatoBinario mascara = (1ULL << 12)-1;
        int informacao = codigo & mascara;
        cpu.AC = memoria[informacao] * -1;

    }else if(opcode == 0b00000011){ // LOAD |M(X)|
        formatoBinario mascara = (1ULL << 12)-1;
        int informacao = codigo & mascara;
        cpu.AC = abs(memoria[informacao]);

    }else if(opcode == 0b00000100){ // LOAD-|M(X)|
        formatoBinario mascara = (1ULL << 12)-1;
        int informacao = codigo & mascara;
        cpu.AC = abs(memoria[informacao]) * -1;
        
    }else if(opcode == 0b00001101){ // JUMP M(X,0:19)   TODOS OS JUMPS TENHO Q REVER AINDA
        formatoBinario mascara = (1ULL << 12)-1;
        int informacao = codigo & mascara;
        cpu.PC = informacao;

    }else if(opcode == 0b00001110){ // JUMP M(X,20:39)
        formatoBinario mascara = (1ULL << 12)-1;
        int informacao = codigo & mascara;
        cpu.PC = informacao;

    }else if(opcode == 0b00001111){ // JUMP+M(X,0:19)
        if (cpu.AC >= 0) {
            formatoBinario mascara = (1ULL << 12)-1;
            int informacao = codigo & mascara;
            cpu.PC = informacao;
        }

    }else if(opcode == 0b00010000){ // JUMP+M(X,20:39)
        if (cpu.AC >= 0) {
            formatoBinario mascara = (1ULL << 12)-1;
            int informacao = codigo & mascara;
            cpu.PC = informacao;
        }

    }else if(opcode == 0b00000101){ // ADD M(X)
        formatoBinario mascara = (1ULL << 12)-1;
        int informacao = codigo & mascara;
        cpu.AC += memoria[informacao];

    }else if(opcode == 0b00000111){ // ADD |M(X)|
        formatoBinario mascara = (1ULL << 12)-1;
        int informacao = codigo & mascara;
        cpu.AC += abs(memoria[informacao]);


    }else if(opcode == 0b00000110){ // SUB M(X)
        
    }else if(opcode == 0b00001000){ // SUB |M(X)|
        
    }else if(opcode == 0b00001011){ // MUL M(X)
        
    }else if(opcode == 0b00001100){ // DIV M(X)
        
    }else if(opcode == 0b00010100){ // LSH
        
    }else if(opcode == 0b00010101){ // RSH
        
    }else if(opcode == 0b00010010){ // STOR M(X,8:19)
        
    }else if(opcode == 0b00010011){ // STOR M(X,28:39)
        
    }else{
        // if(linha[0] == '-'){
        //     numero_negativo = informacao * -1;
        //     mascara = (1ULL << 40) - 1;
        //     palavra = numero_negativo & mascara;
        // }else{
            formatoBinario palavra = codigo;
        // }
    }
}

/*
void load_MQ () {
    AC = M; }
void load_MQ_Mx () {
    M = Memoria[0]; }
void stor_Mx () {
    Memoria[0] = M; }
void load_Mx () {
    AC = Memoria[0]; }
void load_menos_Mx () {
    AC -= Memoria[0] }
void load_abs_Mx () {
    AC = 0 + Memoria[0]; }
void load_abs_menos_Mx () {
    AC -= 0 + Memoria[0]; }

void jump_Mx_esq () {} 
void jump_Mx_dir () {}

void jump_cond_Mx_esq () {}
void jump_cond_Mx_dir () {}

void add_Mx () {}
void add_abs_Mx () {}
void sub_Mx () {}
void sub_abs_Mx () {}
void mul_Mx () {}
void div_Mx () {}
void lsh () {}
void rsh () {}

void stor_Mx_esq () {}
void stor_Mx_dir () {}
*/



void main() {

gerenciador_memoria();

}
