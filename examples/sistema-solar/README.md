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
        │   ground.cpp
        │   ground.hpp
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

Os arquivos `camera.cpp` e `camera.hpp` são utilizados para a definição da câmera e sua posição. Os arquivos `ground.cpp` e `ground.hpp` tem como objetivo definir o chão que é exibido. Os arquivos `window.cpp` e `window.hpp` são utilizados para a definição da janela em que a renderização ocorrerá. O arquivo `main.cpp` é o arquivo principal da aplicação, responsável por inicializar a janela e executar a aplicação. Dentre os `assets`, o arquivo `esferas.obj` define a plotagem de uma esfera, utilizada para representação dos elementos do sistema solar, o arquivo `lookat.frag` contém o *fragment shader* e o `lookat.vert` contém o *vertex shader*.

---

## Desenvolvimento

O desenvolvimento foi baseado no caso `lookat` e o primeiro ponto a ser implementado foi a alteração do objeto a ser renderizado para uma esfera (`assets/esfera.obj`). Para isso, utilizamos o arquivo disponível nas notas de aula.

Em sequência, foi necessário realizar a criação das 9 esferas que compõem o sistema solar, no arquivo `window.cpp`: 


```
glm::mat4 model{1.0f};

// Sun
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(-2.7f, 0.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.6f));

abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
abcg::glUniform4f(m_colorLocation, 1.0f, 0.722f, 0.25f, 1.0f);
abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                    nullptr);

// Mercúrio
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(-0.7f, 0.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.1f));

abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
abcg::glUniform4f(m_colorLocation, 0.788f, 0.788f, 0.788f, 1.0f);
abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                    nullptr);

// Vênus
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(0.3f, 0.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f));

abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
abcg::glUniform4f(m_colorLocation, 0.878f, 0.6f, 0.122f, 1.0f);
abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                    nullptr);

// Terra
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.2f));

abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
abcg::glUniform4f(m_colorLocation, 0.122f, 0.369f, 0.878f, 1.0f);
abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                    nullptr);

// Marte
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(2.5f, 0.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.1f));

abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
abcg::glUniform4f(m_colorLocation, 0.839f, 0.251f, 0.0f, 1.0f);
abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                    nullptr);

// Júpiter
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(3.5f, 0.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.3f));

abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
abcg::glUniform4f(m_colorLocation, 0.78f, 0.616f, 0.451f, 1.0f);
abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                    nullptr);
                
// Saturno
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.1f));

abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
abcg::glUniform4f(m_colorLocation, 0.878f, 0.729f, 0.514f, 1.0f);
abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                    nullptr);

// Urano
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(4.5f, 0.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.1f));

abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
abcg::glUniform4f(m_colorLocation, 0.435f, 0.596f, 0.89f, 1.0f);
abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                    nullptr);

// Netuno
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
model = glm::scale(model, glm::vec3(0.1f));

abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
abcg::glUniform4f(m_colorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                    nullptr);
```

Pode-se observar que para cada esfera, utiliza-se uma transformação de translação e uma transformação de escala.

A transformação de **translação** é responsável por alterar a posição do elemento no espaço, sendo que nesse caso, os elementos são todos alinhados no eixo z e y, sendo variado apenas o eixo x.

`model = glm::translate(model, glm::vec3(-2.7f, 0.0f, 0.0f));`

A transformação de **escala** tem como objetivo alterar o tamanho (proporcionalmente) do elemento. Ela é utilizada para deixar cada elemento em um tamanho, simulando o cenário real.

`model = glm::scale(model, glm::vec3(0.6f));`

Além disso, cada esfera foi renderizada de um cor, exemplo:

`abcg::glUniform4f(m_colorLocation, 1.0f, 0.722f, 0.25f, 1.0f);`

---

## Deployment

