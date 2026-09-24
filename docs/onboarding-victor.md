# Guia de entrada - Victor

Este documento permite iniciar o Digital Twin sem acesso ao ESP32 e sem depender do desenvolvimento do Ryhan.

## Sua área

Você é responsável por `digital-twin/`:

- modelo 3D e hierarquia das juntas;
- UI para visualizar e solicitar movimento;
- cinemática direta (FK) e inversa (IK);
- verificação de alcançabilidade e limites lógicos;
- adaptador de comunicação que produz/consome `docs/protocol.md`.

Ryhan é responsável por `firmware/` e `backend/`: porta serial, parser, PCA9685, limites mecânicos, calibração e segurança física. Não altere esses diretórios sem alinhamento prévio.

## Leia nesta ordem

1. `README.md` - mapa do projeto e licença.
2. `docs/architecture.md` - fronteiras das camadas.
3. `docs/protocol.md` - contrato que seu adaptador deve cumprir.
4. `docs/firmware-design.md` - por que `STATE` não é posição física.
5. `docs/hardware.md` - limites que ainda dependem de medição; não codifique valores provisórios como definitivos.
6. `docs/roadmap.md` - sequência de entregas.

## Contrato que você pode assumir

Envie uma linha por comando, terminada por `\n`, no formato UTF-8/ASCII e com no máximo 96 bytes.

```text
PING,1             -> PONG,1,v0
GET_STATE,2        -> STATE,2,0,90.00,90.00,90.00,90.00
SET_JOINTS,3,90,90,90,90 -> ACK,3 | ERR,3,<codigo>
ESTOP,4            -> ACK,4
```

Gere `seq` incremental de 0 a 65535 e associe a resposta ao comando correspondente. Trate timeout, desconexão e `ERR` como falha de comunicação; a UI não deve assumir que o robô físico alcançou a posição. Até a calibração, `SET_JOINTS` corretamente retorna `NOT_CALIBRATED`.

## Plano de implementação do Digital Twin

### Entrega V1 - estado e simulador

Crie `src/robot/RobotState` com quatro ângulos lógicos; crie um adaptador simulado em `src/communication/` que implemente o mesmo contrato e permita testar a UI sem serial. Teste que a UI representa o alvo aceito e mostra erros.

### Entrega V2 - FK e modelo visual

Defina, em uma ADR, origem, unidades, orientação dos eixos, sentido positivo de cada junta e referências de zero. Faça o modelo renderizar os quatro ângulos a partir de um único estado. Teste FK com poses de referência documentadas.

### Entrega V3 - IK e validação

Implemente IK fora da UI. A entrada é pose XYZ (e orientação apenas se definida); a saída deve ser quatro ângulos ou erro de ponto inalcançável. Valide limites lógicos antes de oferecer `SET_JOINTS`.

### Entrega V4 - integração

Substitua o simulador por transporte real mantendo a mesma interface. A UI só envia os quatro alvos em uma mensagem atômica e atualiza o estado após `ACK`/`STATE` correspondente. Façam esta etapa em pareamento com Ryhan.

## Regras que evitam retrabalho

- Não calcule cinemática no firmware e não controle PWM pelo Digital Twin.
- Não use ângulo de servo como ângulo cinemático sem uma camada de mapeamento calibrada.
- Não implemente trajetória contínua como confirmação de posição: o hardware atual não tem encoder.
- Não altere `protocol.md` sozinho. Proponha a mudança em issue/PR e atualize simulador, firmware e testes de contrato juntos.
- Não use arquivos de calibração local como dados do repositório.

## Git diário

```text
git switch main
git pull --ff-only
git switch -c vitor/<assunto>
# desenvolver, testar e documentar
git commit -m "feat(digital-twin): <resultado>"
git push -u origin vitor/<assunto>
```

Abra um PR pequeno para `main`, atribua o Ryhan como revisor quando tocar em contrato/integração e só mescle depois da revisão. Em tarefas de fronteira, alternem driver e navigator; isso cria revisão contínua antes do PR.
