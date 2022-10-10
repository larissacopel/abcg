# Larissa Copel Rothman, RA: 11201721367


## Descrição

Para o desenvolvimento do projeto, eu resolvi utilizar um cenário que observei ser bem recorrente durante meus anos na UFABC e durante minha experiência em empresas, que é formulários. Nesse caso específico, optei por criar um formulário destinado ao cadastro de clientes, fazendo uso de algumas funcionalidade do ImGui.

Como essa primeira atividade teve como objetivo explorar o ImGui, eu optei por focar na implementação gráfica ao invés de um desenvolvimento que requeresse uma lógica de algoritmo mais complexo.

---

## Organização do projeto

```
abcg
│
└───examples
│   │
│   └───atividade1
│       │   CMakeLists.txt
│       │   main.cpp
│       │   README.md
│       │   window.cpp
│       │   window.hpp
|       └───code
│       │   │   atividade-1.html
│       │   │   atvidade-1.js
│       │   │   atividade-1.wasm
```

A atividade foi desenvolvida na pasta examples/atividade-1, no repositório abdg (forkeado).

Dentro da pasta da atividade, há primeiro os arquivos utilizados para o desenvolvimento e documentação da aplicação. Além disso, na pasta code, encontram-se os arquivos compilados para a criação da página web, dispnonível em https://larissacopel.github.io/abcg/examples/atividade-1/code/.


---

## Desenvolvimento

O desenvolvimento teve início com a criação da estrutura básica do projeto, que consistiu na criação dos arquivos main.cpp, window.cpp e window.hpp. Além da criação de tais arquivos, também realizei a inclusão da referência dessa aplicação em CMakeLists.txt.

A partir disso, a maior parte do desenvolvimento ficou centralizada no arquivo window.cpp. 

Dentre os elementos do formulário, inclui:

- Nome
- CPF
- Gênero
- Data de Nascimento

Dentre os elementos ImGui utilizados, tem-se: 

- TextWrapped
- Text
- InputText
- InputTextWithHint
- Combo
- InputInt3
- Button

---

## Deployment

Após o desenvolvimento, realizei a compilação dos arquivos conforme orientado e com a geração dos arquivos javascript e wasm, centralizei-os na pasta examples/atividade-1/code. Além disso, realizei a configuração de um GitHub Page em meu repositório e com os workflows já existentes, o deploy foi realizado de forma automática.