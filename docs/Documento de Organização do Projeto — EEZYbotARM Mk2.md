# EEZYbotARM Mk2 — Organização e Desenvolvimento

## 1. Objetivo

Desenvolver um sistema robótico baseado no **EEZYbotARM Mk2**, utilizando:

- ESP32 como controlador;
- módulos PWM para acionamento dos servomotores;
- servomotores MG996 e MG90;
- comunicação serial entre computador e ESP32;
- Digital Twin para representação virtual do braço;
- cinemática para transformação entre posição cartesiana e ângulos das juntas;
- arquitetura modular para permitir desenvolvimento simultâneo por dois programadores.

O projeto será dividido em dois subsistemas principais:

**Ryhan**
- Backend;
- Firmware ESP32;
- Comunicação serial;
- Protocolo de comandos;
- Controle dos servomotores;
- Validação dos comandos recebidos.

**Vitor**
- Digital Twin;
- Interface gráfica;
- Modelo 3D;
- Cinemática;
- Controle da movimentação virtual;
- Conversão de posição para ângulos.

---

# 2. Arquitetura do sistema

```text
┌───────────────────────────────────────────────┐
│                 DIGITAL TWIN                  │
│                                               │
│  Modelo 3D                                   │
│  Interface                                   │
│  Cinemática                                  │
│  Controle de juntas                          │
│                                               │
│                     VITOR                     │
└───────────────────────┬───────────────────────┘
                        │
                        │ Serial
                        │
                        ▼
┌───────────────────────────────────────────────┐
│                    BACKEND                     │
│                                               │
│  Parser                                       │
│  Protocolo                                    │
│  Validação                                    │
│  Estado do robô                               │
│                                               │
│                    RYHAN                      │
└───────────────────────┬───────────────────────┘
                        │
                        ▼
┌───────────────────────────────────────────────┐
│                    ESP32                       │
│                                               │
│  Firmware                                     │
│  Controle das juntas                          │
│  Limites                                      │
│  PWM                                           │
│                                               │
└───────────────────────┬───────────────────────┘
                        │
                        ▼
                 ┌──────────────┐
                 │ PWM DRIVER   │
                 └──────┬───────┘
                        │
             ┌──────────┼──────────┐
             ▼          ▼          ▼
           MG996       MG996       MG90
```

---

# 3. Divisão de responsabilidades

## 3.1 Ryhan — Backend + Firmware

Responsável pela parte física e pela comunicação.

### Backend

Responsabilidades:

- comunicação serial;
- descoberta/configuração da porta;
- envio de comandos;
- recebimento de respostas;
- parser do protocolo;
- validação de dados;
- gerenciamento do estado do robô;
- tratamento de erros;
- interface entre Digital Twin e ESP32.

### Firmware

Responsabilidades:

- inicialização do ESP32;
- inicialização do controlador PWM;
- controle dos servomotores;
- definição dos limites das juntas;
- calibração;
- interpretação dos comandos;
- movimentação dos servos;
- resposta de estado;
- tratamento de comandos inválidos;
- parada segura.

---

# 4. Vitor — Digital Twin

Responsável pela representação virtual do robô.

### Digital Twin

Responsabilidades:

- modelo 3D do EEZYbotARM;
- representação das juntas;
- hierarquia dos componentes;
- movimentação virtual;
- visualização dos ângulos;
- visualização da posição XYZ;
- cinemática direta;
- cinemática inversa;
- controle da posição desejada;
- geração dos comandos para o backend.

O Digital Twin deve possuir uma separação clara entre:

```text
Interface
    ↓
Controle
    ↓
Cinemática
    ↓
Estado do robô
    ↓
Protocolo serial
```

A interface não deve controlar diretamente o ESP32.

---

# 5. Estrutura do repositório

```text
eezybotarm-mk2/
│
├── firmware/
│   ├── src/
│   │   ├── main.cpp
│   │   ├── servo_controller.cpp
│   │   ├── serial_protocol.cpp
│   │   └── robot_state.cpp
│   │
│   ├── include/
│   │   ├── servo_controller.h
│   │   ├── serial_protocol.h
│   │   └── robot_state.h
│   │
│   ├── lib/
│   ├── test/
│   └── platformio.ini
│
├── backend/
│   ├── src/
│   │   ├── serial/
│   │   ├── protocol/
│   │   ├── robot/
│   │   └── main/
│   │
│   └── tests/
│
├── digital-twin/
│   ├── src/
│   │   ├── graphics/
│   │   ├── kinematics/
│   │   ├── robot/
│   │   └── communication/
│   │
│   ├── models/
│   └── assets/
│
├── docs/
│   ├── architecture.md
│   ├── protocol.md
│   ├── kinematics.md
│   ├── hardware.md
│   └── calibration.md
│
├── .gitignore
├── README.md
└── LICENSE
```

A configuração do PlatformIO deve permanecer no repositório. O `platformio.ini` é justamente o arquivo utilizado para declarar o ambiente de desenvolvimento, placa, framework e dependências do projeto, permitindo que os dois desenvolvedores trabalhem com uma configuração reproduzível. (PlatformIO, 2026).

---

# 6. Regra mais importante: contrato entre os sistemas

Antes de desenvolver a integração, os dois lados devem concordar sobre o **protocolo serial**.

Essa é a fronteira entre o trabalho de Ryhan e Vitor.

Exemplo:

```text
SET J1 90
SET J2 45
SET J3 120
SET J4 30
```

ou, preferencialmente, uma mensagem estruturada:

```text
J,90,45,120,30
```

Resposta:

```text
OK
```

Estado:

```text
STATE,90,45,120,30
```

Erro:

```text
ERR,INVALID_ANGLE
```

O formato definitivo deve ser documentado em:

```text
docs/protocol.md
```

### Regra

**Vitor não deve precisar saber como o servo é controlado.**

E:

**Ryhan não deve precisar saber como o modelo 3D calcula a cinemática.**

Ambos conhecem apenas o contrato.

---

# 7. Estado do robô

O sistema deve possuir um estado centralizado.

Exemplo:

```text
RobotState

J1 = 90°
J2 = 45°
J3 = 120°
J4 = 30°

X = ...
Y = ...
Z = ...

connected = true
moving = false
error = false
```

O Digital Twin utiliza esse estado para representar o robô.

O ESP32 utiliza esse estado para representar a posição física.

---

# 8. Cinemática

A cinemática pertence ao Digital Twin.

### Cinemática direta

```text
J1 J2 J3 J4
     │
     ▼
Forward Kinematics
     │
     ▼
 X Y Z
```

### Cinemática inversa

```text
 X Y Z
   │
   ▼
Inverse Kinematics
   │
   ▼
J1 J2 J3 J4
```

A saída da cinemática inversa deve ser entregue ao sistema de comunicação:

```text
XYZ
 ↓
IK
 ↓
J1 J2 J3 J4
 ↓
Protocolo
 ↓
Serial
 ↓
ESP32
```

Não colocar equações de cinemática dentro do firmware sem uma necessidade específica.

Isso mantém o ESP32 responsável pelo controle físico, enquanto o computador possui a carga computacional e a lógica matemática.

---

# 9. Controle dos servos

O firmware deve receber ângulos e realizar a conversão:

```text
ângulo desejado
      ↓
validação
      ↓
limite mecânico
      ↓
conversão para PWM
      ↓
driver PWM
      ↓
servo
```

Nunca assumir:

```text
ângulo = PWM
```

A relação real depende do servo, controlador, frequência e calibração.

Portanto, cada junta deve possuir parâmetros próprios:

```cpp
JointConfig joint1;

joint1.minAngle = ...;
joint1.maxAngle = ...;
joint1.minPulse = ...;
joint1.maxPulse = ...;
```

Os valores devem ser obtidos durante a calibração física.

---

# 10. Calibração

A calibração deve ser tratada como uma etapa própria do projeto.

Para cada junta:

```text
J1
├── posição mínima
├── posição central
└── posição máxima

J2
├── posição mínima
├── posição central
└── posição máxima

...
```

Nunca assumir que:

```text
0° = posição mecânica mínima
180° = posição mecânica máxima
```

O braço físico possui limitações mecânicas próprias.

Os valores encontrados devem ser registrados em:

```text
docs/calibration.md
```

---

# 11. Git — modelo de trabalho

A branch `main` representa uma versão integrada e funcional.

Não desenvolver diretamente nela.

Estrutura:

```text
main
│
├── feature/firmware-servo-control
├── feature/serial-protocol
├── feature/backend-serial
├── feature/digital-twin
├── feature/kinematics
└── feature/robot-model
```

Cada desenvolvedor trabalha em sua própria branch.

### Ryhan

```text
ryhan/firmware
ryhan/backend
ryhan/serial-protocol
```

### Vitor

```text
vitor/digital-twin
vitor/kinematics
vitor/robot-model
```

Branches isolam alterações antes que elas afetem a base principal; Pull Requests permitem revisar e integrar essas alterações. (GitHub, 2026).

---

# 12. Commits

Os commits devem representar uma alteração lógica pequena.

Evitar:

```text
update
teste
mudanças
final
final2
agora vai
```

Preferir:

```text
feat: adiciona controle da junta J1

feat: implementa parser do protocolo serial

fix: corrige limite angular da junta J2

feat: adiciona modelo 3D da base

feat: implementa cinemática inversa

fix: corrige conversão de coordenadas
```

Um commit deve responder:

**O que mudou?**

---

# 13. Pull Request

Uma alteração importante não deve simplesmente entrar na `main`.

Fluxo:

```text
branch
   ↓
desenvolvimento
   ↓
teste
   ↓
commit
   ↓
push
   ↓
Pull Request
   ↓
revisão
   ↓
merge
   ↓
main
```

Para alterações maiores, utilizar Pull Request mesmo sendo apenas duas pessoas.

O objetivo não é burocracia.

É criar um ponto de controle.

---

# 14. Issues

Cada funcionalidade relevante deve virar uma Issue.

Exemplo:

```text
#01 Criar estrutura PlatformIO
#02 Controle individual dos servos
#03 Definir protocolo serial
#04 Implementar parser serial
#05 Criar modelo 3D
#06 Implementar cinemática direta
#07 Implementar cinemática inversa
#08 Integrar Digital Twin
#09 Calibrar juntas
#10 Implementar limites mecânicos
#11 Testar movimentação individual
#12 Testar movimentação integrada
```

Cada Issue pode gerar uma branch.

---

# 15. Vibe Coding — regras para o projeto

Vibe coding será utilizado como **ferramenta de desenvolvimento**, não como substituto da arquitetura.

A IA pode:

- gerar código inicial;
- sugerir estruturas;
- explicar erros;
- refatorar;
- criar testes;
- documentar;
- revisar código;
- sugerir melhorias;
- investigar bugs.

A IA não deve decidir sozinha:

- arquitetura do sistema;
- protocolo de comunicação;
- limites mecânicos;
- parâmetros de segurança;
- comportamento físico do robô;
- valores de calibração.

Esses elementos devem ser definidos pelos desenvolvedores.

---

# 16. Regra para utilização de IA

Antes de pedir código:

```text
1. Definir problema
2. Definir entrada
3. Definir saída
4. Definir restrições
5. Pedir implementação
6. Testar
7. Revisar
8. Commitar
```

Exemplo ruim:

```text
"faz o código do braço robótico"
```

Exemplo adequado:

```text
"Tenho um ESP32 utilizando um controlador PWM.
Preciso criar uma classe ServoController.

Entrada:
ângulo de 0–180°.

Saída:
pulso PWM.

Requisitos:
- quatro juntas;
- limite individual por junta;
- nenhum movimento fora do limite;
- código compatível com PlatformIO;
- não alterar o protocolo serial existente.

Crie a implementação e os testes."
```

Isso reduz drasticamente a chance de a IA criar código incompatível com o restante do sistema.

---

# 17. Regra de integração

Nunca desenvolver tudo e tentar integrar no final.

A integração deve acontecer progressivamente:

```text
ETAPA 1
ESP32 → 1 servo

ETAPA 2
ESP32 → todos os servos

ETAPA 3
PC → ESP32 via serial

ETAPA 4
Digital Twin → protocolo

ETAPA 5
Digital Twin → ESP32

ETAPA 6
Cinemática → juntas

ETAPA 7
Cinemática → Digital Twin

ETAPA 8
Digital Twin → robô físico
```

Cada etapa deve possuir um teste.

---

# 18. Critério de conclusão

Uma funcionalidade só é considerada concluída quando:

```text
[ ] Código implementado
[ ] Código compilando
[ ] Teste realizado
[ ] Comportamento esperado confirmado
[ ] Documentação atualizada
[ ] Commit realizado
[ ] Pull Request revisado
```

---

# 19. Regra de ouro do projeto

A arquitetura deve permitir que:

```text
Vitor possa desenvolver o Digital Twin
SEM possuir o robô físico.

Ryhan possa desenvolver o firmware
SEM precisar do Digital Twin.
```

A integração ocorre através do protocolo:

```text
DIGITAL TWIN
      │
      │ protocolo
      ▼
   BACKEND
      │
      │ serial
      ▼
    ESP32
      │
      ▼
    ROBÔ
```

Essa separação reduz dependências entre os desenvolvedores e permite desenvolvimento paralelo.

---

# 20. Primeira versão do projeto

O primeiro objetivo não é fazer o robô completo.

É criar uma **Vertical Slice**:

```text
Digital Twin
     ↓
ângulo J1
     ↓
protocolo
     ↓
serial
     ↓
ESP32
     ↓
PWM
     ↓
MG996
     ↓
movimento físico
```

Quando essa cadeia funcionar, repetir para J2, J3 e J4.

Somente depois integrar a cinemática completa.

Isso cria uma prova de integração cedo e evita passar semanas desenvolvendo módulos que posteriormente não conseguem conversar entre si.

---

# 21. Estado esperado da versão 0.1

```text
[✓] Repositório criado
[✓] PlatformIO configurado
[✓] ESP32 compilando
[ ] Driver PWM funcionando
[ ] Servo J1 funcionando
[ ] Servo J2 funcionando
[ ] Servo J3 funcionando
[ ] Servo J4 funcionando
[ ] Protocolo serial definido
[ ] Backend comunicando
[ ] Digital Twin exibindo robô
[ ] Cinemática direta
[ ] Cinemática inversa
[ ] Integração completa
```

A versão `0.1` será considerada concluída quando o Digital Twin conseguir enviar uma posição/junta e o robô físico reproduzir o movimento correspondente de forma controlada.

---

# 22. Referências

- PlatformIO. **Project Configuration File — platformio.ini**. Documentação oficial. 2026.
- GitHub. **Writing code for a project / Pull Requests**. GitHub Docs. 2026.
- Franciscone, C. **EEZYbotARM Mk2 — 3D Printed Robot**. Instructables.
- meisben. **easyEEZYbotARM**. GitHub. Implementação de cinemática 3D e comunicação para EEZYbotARM Mk1/Mk2.