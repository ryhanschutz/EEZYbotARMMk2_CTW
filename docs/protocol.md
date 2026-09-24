# Protocolo serial — v0

Este arquivo é o contrato normativo da integração. Transporte: serial USB, **115200 8N1**, UTF-8/ASCII, uma mensagem por linha (`\n`), máximo de 96 bytes. Vírgula é separador; não use espaços. O firmware deve descartar linhas excedentes, nunca movimentar servos após uma mensagem inválida e responder com um único `ERR,...`.

## Convenções

- Ângulos são graus decimais com ponto e referem-se a juntas lógicas: `J1` base, `J2` ombro, `J3` cotovelo, `J4` garra/punho. A associação física final fica em `hardware.md`.
- `seq` é inteiro decimal de 0 a 65535, gerado pelo cliente; permite correlacionar respostas.
- A confirmação indica alvo aceito, não posição medida. O Mk2 com servos hobby não possui realimentação de posição.

## Comandos do host

| Mensagem | Efeito | Resposta de sucesso |
| --- | --- | --- |
| `PING,<seq>` | verifica conexão | `PONG,<seq>,v0` |
| `GET_STATE,<seq>` | consulta o último estado conhecido | `STATE,<seq>,<moving>,<j1>,<j2>,<j3>,<j4>` |
| `SET_JOINTS,<seq>,<j1>,<j2>,<j3>,<j4>` | valida e define alvos de todas as juntas atomicamente | `ACK,<seq>` |
| `ESTOP,<seq>` | desabilita movimento pendente e desliga saídas PWM | `ACK,<seq>` |
| `CAL_PULSE,<seq>,<ch>,<pulse_us>` | envia pulso bruto em microssegundos (450..2550) ao canal do PCA9685 para calibração | `ACK,<seq>` |
| `CAL_DISABLE,<seq>,<ch>` | desativa PWM do canal específico (solta o torque do servo) | `ACK,<seq>` |

`moving` é `0` ou `1`. Na versão vertical slice, movimentos podem ser imediatos e então retornar `0`; ao adicionar trajetória, atualize esta especificação.

## Erros

`ERR,<seq>,<code>` onde `code` é `MALFORMED`, `UNKNOWN_COMMAND`, `OUT_OF_RANGE`, `ESTOP_ACTIVE`, `NOT_CALIBRATED`, `HARDWARE_ERROR` ou `BUSY`.

Exemplos:

```text
PING,17
PONG,17,v0
SET_JOINTS,18,90,90,90,90
ACK,18
GET_STATE,19
STATE,19,0,90,90,90,90
```

Alterações incompatíveis exigem nova versão de protocolo e revisão dos dois responsáveis.
