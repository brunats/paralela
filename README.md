# Programação Paralela
Trabalhos desenvolvidos na disciplina optativa de programação paralela em CCT - UDESC
## Instalação
```
$ git clone https://github.com/brunats/programacao_paralela
$ cd programacao_paralela
```

## Exemplo Matriz
Matrizes de 3x3, 1 thread, Operação Determinante:
```
$ cd matrix
$ make clean
$ make
$ ./main 3 3 1 1
```
Matrizes de 1000x1000 e 10 testes, cada teste acrescentando 1 thread a cada teste, operacao determinante:
```
$ for i in `seq 1 10`; do ./main 1000 1000 $i 1 ; done
```

## Exemplo Mandelbrot
Ajustar o tamanho do fractal de mandelbrot e o número de iterações no arquivo [mandelbrot.in](mandelbrot.in).
Ajustar a quantidade de execuções dentro do arquivo [main.cpp](main.cpp)(*l.62*).
```
$ cd mandelbrot
$ make clean
$ make
$ ./mandelbrot < mandelbrot.in
```

## Exemplo Black-Sholes
Ajustar o tamanho do n no arquivo [black-sholes.in](black-sholes.in).
Ajustar a quantidade de execuções dentro do arquivo [main.cpp](main.cpp)(*linha: 70*).

```
$ cd maratona/black-sholes/
$ make clean
$ make
$ ./black-sholes < black-sholes.in
```

### Autores:
* **Bruna Tavares Silva**       @brunats
* **Christopher Renkavieski**   @ChrisRenka
