# CAD-19.1

## C:
```
gcc -o matmulc matmulc.c -lm
./matmulc [n] [N] [s] [v]
n : dimensão mínima da matriz
N : dimensão máxima da matriz
s : passo com que n cresce:
while (n <= N){
  ...
  n += s;
}
v : número de vezes que y = A x é calculado para cada valor de n.

Para mais detalhes:
./matmulc -h

arquivos:
"c_results_c.txt" e "c_results_f.txt" sendo a multiplicação por linhas e por colunas, respectivamente.
```

## FORTRAN:
```
gfortran -o matmulf matmul.f95 -mcmodel=large
./matmulf

arquivos:
"f_results_c.txt" e "f_results_f.txt" sendo a multiplicação por linhas e por colunas, respectivamente.
```

## GRÁFICOS (Python 3):

```
python3 "graficos.py" [arquivo_linhas] [arquivo_colunas] [Título do gráfico] (-s [imagem])

exemplo:
python3 "graficos.py" "f_results_c.txt" "f_results_f.txt" "y = A x em FORTRAN"
-> abre grafico em tela do matplotlib

python3 "graficos.py" "c_results_c.txt" "c_results_f.txt" "y = A x em C" -s "c_results.png"
-> salva grafico em arquivo .png
```
