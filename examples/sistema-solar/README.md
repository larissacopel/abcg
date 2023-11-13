# Atividade 2

- **Larissa Copel Rothman, RA: 11201721367**
- **Vinícius Teixeira Fantaccini, RA: 11087316**

https://github.com/larissacopel/abcg/tree/main/examples/sistema-solar

## Descrição

Esse projeto tem como objetivo implementar uma representação do sistema solar.

O código desenvolvido pode ser encontrado em: https://github.com/larissacopel/abcg/tree/main/examples/sistema-solar.

---

## Organização do projeto

O projeto segue a organização dos exemplos mostrados em aula, tendo a seguinte estrutura:

```
abcg
│
└───examples
    │
    └───sistema-solar
        │   CMakeLists.txt 
        │   main.cpp
        │   README.mdp
        │   camera.cpp
        │   camera.hpp
        │   window.cpp
        │   window.hpp
        └───code
        │       sistema-solar.html
        │       sistema-solar.js
        │       sistema-solar.wasm
        └───assets
                esferas.obj
                lookat.frag
                lookat.vert
```

Os arquivos `camera.cpp` e `camera.hpp` são utilizados para a definição da câmera e sua posição. Os arquivos `window.cpp` e `window.hpp` são utilizados para a definição da janela em que a renderização ocorrerá. O arquivo `main.cpp` é o arquivo principal da aplicação, responsável por inicializar a janela e executar a aplicação. Dentre os `assets`, o arquivo `esferas.obj` define a plotagem de uma esfera, utilizada para representação dos elementos do sistema solar, o arquivo `lookat.frag` contém o *fragment shader* e o `lookat.vert` contém o *vertex shader*.

---

## Desenvolvimento

O desenvolvimento foi baseado no caso `lookat` e o primeiro ponto a ser implementado foi a alteração do objeto a ser renderizado para uma esfera (`assets/esfera.obj`). Para isso, utilizamos o arquivo disponível nas notas de aula.

Em sequência, foi necessário realizar a criação das 9 esferas que compõem o sistema solar, no arquivo `window.cpp`: 


```
// Mercúrio

float mercuZ = 0.387f * semiMajorAxis * cos(m_earthRotation); // Calcula a posição x com base no ângulo
float mercuX = 0.378f * semiMinorAxis * sin(m_earthRotation); // Calcula a posição y com base no ângulo

model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(mercuX, 0.0f, mercuZ));
model = glm::scale(model, glm::vec3(0.001f * globalScale));

abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
abcg::glUniform4f(m_colorLocation, 0.788f, 0.788f, 0.788f, 1.0f);
abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
```

Pode-se observar que para cada esfera, utiliza-se uma transformação de translação e uma transformação de escala.

A transformação de **translação** é responsável por alterar a posição do elemento no espaço, sendo que nesse caso, os elementos são todos alinhados no eixo z e y, sendo variado apenas o eixo x.

`model = glm::translate(model, glm::vec3(mercuX, 0.0f, mercuZ));`

A transformação de **escala** tem como objetivo alterar o tamanho (proporcionalmente) do elemento. Ela é utilizada para deixar cada elemento em um tamanho, simulando o cenário real.

`model = glm::scale(model, glm::vec3(0.001f * globalScale));`

Além disso, cada esfera foi renderizada de um cor, exemplo:

`abcg::glUniform4f(m_colorLocation, 0.788f, 0.788f, 0.788f, 1.0f);`


Com as teclas `up`, `down`, `left` e `right` é possível alterar a posição da câmera. Além disso, com a tecla `d` ou a tecla `a`, é possível realizar o movimento de translação dos planetas em volta do Sol.

Para a implementação da translação, adicionamos na função `onEvent` uma ação ao pressionar as teclas `d` ou `a`:

```
if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_LEFT)
        m_earthRotation -= 0.1f;
    if (event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT)
        m_earthRotation += 0.1f;
}
```