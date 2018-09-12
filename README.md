# Programação Paralela
Trabalhos desenvolvidos na disciplina optativa de programação paralela em CCT - UDESC
## Instalação
```
$ git clone https://github.com/brunats/programacao_paralela
$ cd programacao_paralela
$ make clean
$ make
```

## Exemplo
Matrizes de 3x3, 1 thread, Operação Determinante:
```
$ ./main 3 3 1 1
```
Matrizes de 1000x1000 e 10 testes, cada teste acrescentando 1 thread a cada teste, operacao determinante:
```
$ for i in `seq 1 10`; do ./main 1000 1000 $i 1 ; done
```

### Autores:
```
Bruna Tavares Silva       @brunats
Christopher Renkavieski   @ChrisRenka
```
