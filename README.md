# EEZYbotARM Mk2

Base colaborativa para controlar o EEZYbotARM Mk2 com ESP32, PCA9685 e um Digital Twin.

Responsáveis: [Ryhan Schutz](https://github.com/ryhanschutz) (firmware e backend) e [Victor Gabriel Kostetzer](https://github.com/V651202) (Digital Twin, cinemática e modelo 3D).

O repositório é um monorepo: o firmware controla com segurança o hardware; backend e Digital Twin evoluem de forma independente e só se integram pelo contrato em [`docs/protocol.md`](docs/protocol.md).

## Estrutura

```text
firmware/       PlatformIO/ESP32 — Ryhan
backend/        adaptador serial e estado do robô — Ryhan
digital-twin/   modelo 3D, UI e cinemática — Vitor
docs/           contrato, segurança, hardware, calibração e decisões
ref/            PDFs de referência local (não são código-fonte)
```

## Primeiro ciclo de trabalho

1. Ryhan implementa `PING`, `GET_STATE` e `SET_JOINTS` no firmware, sem conectar os servos antes da calibração.
2. Vitor implementa cinemática e Digital Twin usando o simulador/adaptador de protocolo, sem depender do ESP32.
3. Ambos validam mensagens contra `docs/protocol.md`; só então conectam os dois lados.

## Ambiente

Firmware: abra [`firmware/`](firmware/) no PlatformIO e execute `pio run` a partir dessa pasta. A compilação inicial não movimenta servos.

Antes de alimentar os servos, leia [`docs/hardware.md`](docs/hardware.md) e preencha uma cópia local de [`docs/calibration.template.md`](docs/calibration.template.md) como `docs/calibration.local.md`.

As decisões derivadas das referências técnicas e o estágio atual do firmware estão em [`docs/firmware-design.md`](docs/firmware-design.md).

## Colaboração

- `main` deve estar sempre compilável e integrado.
- Trabalhe em `ryhan/<assunto>` ou `vitor/<assunto>`; abra Pull Request para `main`.
- Não altere o protocolo sem PR que atualize firmware, backend, Digital Twin e testes de contrato quando aplicável.
- Use commits convencionais, por exemplo `feat(firmware): adiciona parser de PING`.

As regras completas estão em [`CONTRIBUTING.md`](CONTRIBUTING.md) e o plano inicial em [`docs/roadmap.md`](docs/roadmap.md).

## Licença

O código deste repositório é distribuído sob [GPL-3.0-or-later](LICENSE). PDFs, modelos e outros materiais de terceiros mantêm suas licenças originais.
