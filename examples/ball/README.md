# Atividade 1

- **Larissa Copel Rothman, RA: 11201721367**
- **Vinícius Teixeira Fantaccini, RA: 11087316**

https://github.com/larissacopel/abcg/tree/main/examples/ball

## Descrição

Esse projeto tem como objetivo implementar um jogo em que tem-se uma bolinha e uma base e intuito é não deixar a bolinha tocar na parte inferior da tela. A base pode ser movimentada com o mouse.

O código desenvolvido pode ser encontrado em: https://github.com/larissacopel/abcg/tree/main/examples/ball.

---

## Organização do projeto

O projeto segue a organização dos exemplos mostrados em aula, tendo a seguinte estrutura:

```
abcg
│
└───examples
    │
    └───ball
        │   CMakeLists.txt
        │   main.cpp
        │   README.md
        │   window.cpp
        │   window.hpp
        │   base.cpp
        │   base.hpp
        │   ball.cpp
        │   ball.hpp
        │   gamedata.hpp
        └───code
        │       ball.html
        │       ball.js
        │       ball.wasm
        └───assets
                Inconsolata-Medium.ttf
                objects.frag
                objects.vert
```

A aplicação é composta de dois elementos, a bolinha e a base. A definições de cada objeto, assim como suas funcionalidades, estão centralizadas em arquivos específicos:

- base.cpp
- base.hpp
- ball.cpp
- ball.hpp

Além das definições dos objetos, há também a definição da janela:

- window.cpp
- window.hpp

Para o controle do jogo, foi criado o arquivo gamedata.hpp.

Por fim, tem-se a pasta `assets` com a fonte que será usada para os textos exibidos, com o vertex shader e com o fragment shader.

---

## Desenvolvimento

O desenvolvimento teve início com a criação da estrutura básica do projeto mencionada acima. Em sequência, realizamos a criação da base noss arquivos `base.cpp` e `base.hpp`.

### Base

No arquivo `base.hpp`, definimos a classe que representa uma base, tendo alguns atributos públicos que a caracterizam:

- cor
- rotação
- escala
- translação
- velocidade

Ela possui também alguns métodos públicos:

- create
- paint
- destroy
- update

A lógica dos métodos é definida no arquivo `base.cpp`.

É interessante frisar que o método `create` recebe como argumento a posição do mouse, para que a base seja criada verticamente alinhada com o mouse do usuário.

Para a base, a lógica mais relevante está na sua criação, que é feita definindo a posição dos triângulos que a compõem. A posição da base depende da posição do mouse, estando sempre alinhada.

Ao criar a janela, a base é criada e ao mover o mouse, ela é recriada na nova posição.

### Bola

A bola possui alguns métodos privados:

- create
- paint
- destroy
- update

E alguns atributos privados:

- velocidade angular
- cor
- quantidade de lados
- rotação
- escala
- translação
- velocidade
- indicativo se bateu

A bola é criada como um polígno de 50 lados (visivelmente uma bola) com velocidade angular e velocidade. Para garantir que sua direção mude ao bater em um dos lados (exceto o chão), implementamos uma lógica de atualização da translação e da velocidade da bola (update).

---

## Deployment

Houverem problema com a criação da aplicação Web, mesmo tentando em dois sistemas operacionais diferentes. Por isso, realizamos a gravação da aplicação executando e disponibilizamos em https://drive.google.com/file/d/15f_aBi_0IosZ4GK3vQZaGQATR5mCOfbT/view?usp=share_link.

## Próximos passos

O projeto ainda tem pontos a ser trabalhados que serão implementados como uma evolução na próxima etapa do projeto:

- gerar o impacto da bola com a base
- exibir o game over ao bater com o chão

