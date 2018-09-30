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
Ajustar a quantidade de linhas, colunas e iterções no arquivo [mandelbrot.in](maratona/black-scholes/black-scholes.in).
Ajustar a quantidade de execuções dentro do arquivo [mandelbrot.cpp](mandelbrot/mandelbrot.cpp) (*linha: 70*).
```
$ cd mandelbrot
$ make clean
$ make
$ ./mandelbrot < mandelbrot.in
```

## Exemplo Black-Sholes
Ajustar o tamanho do N no arquivo [black-sholes.in](maratona/black-scholes/black-scholes.cpp).
Ajustar a quantidade de execuções dentro do arquivo [black-scholes.cpp](maratona/black-scholes/black-scholes.cpp)(*linha: 70*).

```
$ cd maratona/black-sholes/
$ make clean
$ make
$ ./black-sholes < black-sholes.in
```
Ao executar o *./black-sholes < black-sholes.in*, o algoritmo irá executar 10 vezes o Black-Sholes para cada quantidade de threads, a cada thread ele irá calcular a média e desvio padrão.

## Exemplo Cholesky

## Exemplo MPI
Instalação MPI:
```
$ tar xzf openmpi-3.1.2.tar.gz
$ cd openmpi-3.1.2/
$ ./configure
  ... check at the end that the summary shows the features you want to see enabled. For this tutorial PCI support will be useful.
$ make
$ sudo make install
```


### Autores:
* **Bruna Tavares Silva**       [@brunats](https://github.com/brunats)
* **Christopher Renkavieski**   [@ChrisRenka](https://github.com/ChrisRenka)
