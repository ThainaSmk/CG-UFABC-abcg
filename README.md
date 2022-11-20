# Aluno: Thaina Abade Siemerink - RA: 11118815

-----------------------------------------
### Primeira entrega: "Roll the Dices"
Interface para rolagem de dados comumente utilizados em mesas de RPG. (D4, D6, D8, D12, D100). Possibilitando uma rolagem múltipla de dados.
Foi implementado seguindo os exemplos da aula "tictactoe" e lendo a documentação da ImGui.

Link para aplicação rodando em WebAssembly: [link](https://thainasmk.github.io/CG-UFABC-abcg/rollTheDices/)

------------------------------------------
### Segunda entrega: "PONG!"
Implementação de um jogo clássico PONG! para dois jogadores. O jogador da esquerda (Rosa) pode controlar a raquete com o W e S e o jogador da direita (Roxo) controla com as setas para cima e para baixo. O jogo acaba quando alguém fizer 10 pontos e reinicia.

A implementação seguiu os exemplos da aula de Asteroids, criando uma classe Bola, Raquete do Jogador 1 e Raquete do Jogador 2. A cor da bola mudando de acordo com a direção além de estético tem utilidade prática no código, pois havia um bug durante o update que poderia causar o registro de colisão de forma anormal, repetidas vezes - fazendo com que a bola "entrasse" na raquete. Com isso, ao registrar o primeiro impacto a bola fica com a mesma cor da raquete e registra a colisão apenas uma vez. Essa solução foi implementada na chamada do update da bola (em bola.cpp) na função "setCorBola" passando um vetor RGBA e alterando a propriedade m_color da Bola.

Os demais itens do projeto foram implementados de forma simples, a colisão verifica se a bola está rebatendo no topo e no piso da janela para inverter a direção de Y da bola e verifica se está batendo nos limites da raquete para inverter a direção de X. 

Link para aplicação rodando em WebAssembly: [link](https://thainasmk.github.io/CG-UFABC-abcg/pong/)

------------------------------------------
### Terceira entrega: "Solar System"

A implementação seguiu os exemplos da aula de lookat, utilizando um arquivo .obj feito no Blender para o sistema inteiro. A ideia é na pŕoxima entrega utilizar um classe Model para fazer um modelo para cada planeta e com isso acrescentar as texturas do planeta em .mtl e adicionar uma animação de rotação para cada um deles e uma iluminação partindo do Sol. 
Para se movimentar pelo sistema utiliza os botões W e A para andar para frente e trás, A e D para mover a câmera para os lados e Q e E para andar lateralmente. Utilizando R você pode voltar para a posição original.

Link para aplicação rodando em WebAssembly: [link](https://thainasmk.github.io/CG-UFABC-abcg/solar_system/)
