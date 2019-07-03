# Image Segmentation
Implementation of Relabel to Front Algorithm on a graph of pixels for image segmentation

Detailed description of the algorithm: [http://www.euroinformatica.ro/documentation/programming/!!!Algorithms_CORMEN!!!/DDU0165.html]

A figura da direita
mostra a imagem segmentada, onde os píxeis brancos representam píxeis que fazem parte do
cenário e os píxeis cinzentos representam píxeis de primeiro plano.

![alt text](https://i.ibb.co/NVhHjcC/image-2019-07-03-19-29-24.png)

Para uma imagem com dimensões m × n cada píxel é identificado por um par (i, j) com
0 ≤ i < m e 0 ≤ j < n. O conjunto dos píxeis é representado pela letra P. Uma segmentação
consiste na divisão dos píxeis no conjunto dos píxeis de primeiro plano L e no conjunto de
píxeis de cenário C. Nenhum píxel deve ser simultanemente de primeiro plano e de cenário,
i.e., L∩C = 0/. Nenhum píxel deve ser omitido, i.e., P = L∪C. Cada píxel de primeiro plano,
p ∈ L, contribui com um peso ℓp para o peso total. Cada píxel de cenário, p ∈ C, contribui com
um peso cp para o peso total. Existem ainda pesos associados às relações de vizinhança. Cada
píxel é vizinho do píxel acima, do píxel abaixo, do píxel à esquerda e do píxel à direita.
A cada relação de vizinhança v é associado um peso fv. Quando existe uma relação de
vizinhança v entre dois píxeis p e p
′
em que p ∈ L e p
′ ∈ C o custo fv é adicionado ao peso
total. A letra V representa o conjunto das relações de vizinhança com a propriedade anterior.

Exemplo de uma segmentação. Os pesos de primeiro plano
ℓp estão nas zonas cinzentas dos píxeis, à esquerda. Os pesos de cenário cp estão nas zonas
brancas dos píxeis, à direita. Os pesos das relações de vizinhança estão sobre as respectivas
relações.

![alt text](https://i.ibb.co/MD8hqb7/image-2019-07-03-19-35-10.png)
