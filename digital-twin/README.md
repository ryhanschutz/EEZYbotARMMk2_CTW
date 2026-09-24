# Digital Twin

Responsável por modelo, interface, cinemática e adaptador de comunicação. Escolham o motor/stack (por exemplo, Python, web ou Unity) em uma ADR antes de criar dependências.

```text
src/kinematics/      FK, IK e testes numéricos
src/robot/           estado e convenções geométricas
src/communication/   cliente do protocolo serial/simulador
models/              arquivos 3D com licença e origem documentadas
assets/              texturas e imagens
```

O cliente deve funcionar contra um simulador do protocolo e nunca acessar PWM ou detalhes de servo.
