# Tradutor de Assembly para Assembly IA-32
*Lucas Araújo Pena - 13/0056162*

Este programa irá converter um programa em Assembly Inventado,já montado e ligado
no formato objeto, para um programa em Assembly Intel IA-32.

Para compilar, use o seguinte comando:

> g++ main.cpp -o tradutor

Para usar o tradutor, use o seguinte comando:

> ./tradutor sample.obj

Deixei o script que usei para compilar e rodar o trabalho ao mesmo tempo, caso
tenha interesse em usá-lo, use os seguintes comandos:

> chmod +x tradutor.sh
> ./tradutor.sh sample.obj

## Ambiente de desenvolvimento
- Este trabalho foi feito em Windows 11, mas foi compilado, executado e testado em
ambiente Linux Ubuntu, através do WSL.

## Observações sobre a execução

- Os logs e tabelas usadas durante o desenvolvimento foram deixadas no terminal
pois podem ser úteis na correção do trabalho.
- O arquivo gerado tem o nome de out-sample.asm para um programa sample.obj.