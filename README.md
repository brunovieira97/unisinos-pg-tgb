# TGB de Processamento Gráfico - UNISINOS
O trabalho consiste no desenvolvimento de um jogo com itens avançados, baseado na evolução do projeto realizado no Grau A.

## Para executar
Foram utilizadas libraries compiladas especificamente para o uso com o G++ do MinGW. Para gerar o executável deste projeto, pode ser utilizado o seguinte comando:
	
	g++ -Wall ./Source/*.cpp ./Source/*.c -I. -g -lglfw3 -lopengl32 -lglu32 -lgdi32

## Construído com
* C++
* OpenGL (GLFW + GLAD)
* STB

## Ambiente
* [MinGW](http://mingw.org/)
* [VS Code](https://code.visualstudio.com/)

## Objetivos
* Visão isoétrica (slide, staggered ou diamond)
* Arquivos de configuração para mapas
* Detecção de tiles caminháveis
* Personagem animado
* Movimentação de 8 direções
* Detecção de clique para interação com tile

* Mapa multicamada
* Controle de NPCs (path finding)
* Menus e interface
* Som
* Pontuação, fases

## Autores
* [brunovieira97](https://www.github.com/brunovieira97) - Bruno Vieira
* [juliorenner](https://www.github.com/juliorenner) - Julio Renner