# Atividade 3

- **Larissa Copel Rothman, RA: 11201721367**
- **Vinícius Teixeira Fantaccini, RA: 11087316**

https://github.com/larissacopel/abcg/tree/main/examples/sun

## Descrição

Esse projeto tem como objetivo tratar o sistema solar agora na visão de textura e iluminação

O código desenvolvido pode ser encontrado em https://github.com/larissacopel/abcg/tree/main/examples/sun e a aplicação compilada está disponível em `/public/sun/`.

---

## Organização do projeto

O projeto segue a organização dos exemplos mostrados em aula, tendo a seguinte estrutura:

```
└───sun
    │   CMakeLists.txt
    │   main.cpp
    │   model.cpp
    │   model.hpp
    │   trackball.cpp
    │   trackball.hpp
    │   window.cpp
    │   window.hpp
    └───assets
        │   esphere.mtl
        │   esphere.obj
        ├───maps
        │       sun.jpg
        └───shaders
                blinnphong.frag
                blinnphong.vert
                depth.frag
                depth.vert
                gouraud.frag
                gouraud.vert
                normal.frag
                normal.vert
                normalmapping.frag
                normalmapping.vert
                phong.frag
                phong.vert
                texture.frag
                texture.vert
```

Os arquivos `window.cpp` e `window.hpp` são utilizados para a definição da janela em que a renderização ocorrerá. O arquivo `main.cpp` é o arquivo principal da aplicação, responsável por inicializar a janela e executar a aplicação. Dentre os `assets`, o arquivo `esferas.obj` define a plotagem de uma esfera, utilizada para representação dos elementos do sistema solar, o arquivo `lookat.frag` contém o *fragment shader* e o `lookat.vert` contém o *vertex shader*. Os arquivos `model.cpp` e `model.hpp` definem o modelo padrão a ser apresentado com os shaders. Já o nosso planeta é criado em `trackball.cpp` e `trackball.hpp`

---

## Desenvolvimento

O desenvolvimento foi baseado no caso `TrackBall Virtual` e o primeiro ponto a ser implementado foi a alteração do objeto a ser renderizado para uma esfera (`assets/esfera.obj`). Para isso, utilizamos o arquivo disponível nas notas de aula.

Em sequência, criar a textura do sistema solar, começando pelo sol, instanciado no arquivo `window.cpp`: 

Com o mouse é possível alterar a posição da câmera. Girando o planeta e dando zoom in e zoom out.

A atualização da esfera de acordo com o movimento do mouse é feito no arquivo window.cpp:

```
if (event.type == SDL_MOUSEMOTION) {
    m_trackBallModel.mouseMove(mousePosition);
    m_trackBallLight.mouseMove(mousePosition);
}
```

A criação dos programas e do modelo é feita também no window.cpp ao criar a janela:

```
// Create programs
for (auto const &name : m_shaderNames) {
    auto const path{assetsPath + "shaders/" + name};
    auto const program{abcg::createOpenGLProgram(
        {{.source = path + ".vert", .stage = abcg::ShaderStage::Vertex},
            {.source = path + ".frag", .stage = abcg::ShaderStage::Fragment}})};
    m_programs.push_back(program);
}

// Load default model
loadModel(assetsPath + "esphere.obj");
m_mappingMode = 2;
```

Para a adição da texturização, foi utilizado o spherical mapping.

