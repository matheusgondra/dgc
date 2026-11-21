# dgc - Gerador de Projetos em C23

O `dgc` é uma ferramenta de linha de comando (CLI) moderna e leve escrita em **C (C23)**, projetada para criar (*scaffold*) rapidamente novos projetos em C configurados com **CMake**, **Ninja** e **vcpkg**.

## Funcionalidades

- **Padrão C23 Moderno**: Desenvolvido utilizando recursos contemporâneos da linguagem, incluindo a diretiva `#embed` para embutir templates de texto diretamente no binário em tempo de compilação (sem dependências de arquivos externos em tempo de execução).
- **Arquitetura Modular de Comandos**: Registro centralizado de comandos e parsing dedicado de opções, facilitando a adição de novos comandos e flags.
- **Pronto para Uso**: Gera uma estrutura completa de projeto contendo códigos-fonte, cabeçalhos, presets de build e regras de ignore.
- **CMake Presets**: Configurações de compilação padronizadas e prontas para uso.
- **Integração com vcpkg**: Preparado para gerenciamento de dependências usando o modo manifesto (`vcpkg.json`).
- **Interface Visual Limpa**: Saídas no terminal estilizadas com cores ANSI e uma visualização em árvore da estrutura criada.

## Pré-requisitos

Certifique-se de ter as seguintes ferramentas instaladas no seu sistema:
- **CMake** (versão 3.20 ou superior)
- **Ninja** (sistema de build)
- **vcpkg** (com a variável de ambiente `VCPKG_ROOT` configurada)
- Um compilador compatível com C23 (como **GCC 14+** ou **Clang 18+**)

## Compilação e Instalação

### 1. Clonar e Configurar
```bash
git clone https://github.com/matheusgondra/dgc.git
cd dgc
cmake --preset default
```

### 2. Compilar o Binário
```bash
cmake --build --preset default
```

### 3. Instalar Globalmente (Opcional)
Para disponibilizar o `dgc` globalmente no `PATH` do seu sistema:
```bash
sudo cmake --install build
```

## Como Usar

Gere um novo projeto C23 usando o subcomando `new`:
```bash
dgc new [OPÇÕES] <nome_do_projeto>
```

Exemplo:
```bash
dgc new meu_projeto_c23
```

### Comandos
- `new [OPÇÕES] <nome_do_projeto>`: Cria um novo projeto em C23.

### Opções do Comando `new`
- `-f`, `--force`: Permite usar um diretório de projeto já existente.
- `-h`, `--help`: Exibe a mensagem de ajuda do comando `new` e sai.

### Opções Globais
- `-h`, `--help`: Exibe a mensagem de ajuda geral e sai.
- `-v`, `--version`: Exibe as informações de versão e sai.

## Licença
Este projeto é de código aberto e está disponível sob a Licença MIT.