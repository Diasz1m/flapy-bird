# Flappy Bird em C++

Clone simples do Flappy Bird feito em C++17 com [raylib](https://www.raylib.com/). Os gráficos e os sons são gerados no código, então o projeto não depende de imagens nem de arquivos de áudio.

## Controles

- **Espaço**, **seta para cima** ou **clique do mouse**: voar
- **Esc**: sair

## Como compilar (Windows + Visual Studio)

No terminal, na pasta do projeto:

```bash
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

Na primeira vez o CMake baixa e compila a raylib automaticamente (precisa de internet). Depois rode:

```bash
./build/Release/flappy_bird.exe
```

## Estrutura

```
src/
  main.cpp        ponto de entrada
  Game.cpp/.hpp   loop, estados, pontuação e desenho
  Bird.cpp/.hpp   física e desenho do pássaro
  Pipe.cpp/.hpp   canos, colisão e pontuação
  Constants.hpp   tamanho da tela, gravidade, velocidades
```

O recorde fica salvo em `highscore.txt` na pasta de onde o jogo foi executado.
