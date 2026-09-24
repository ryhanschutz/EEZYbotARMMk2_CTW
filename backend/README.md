# Backend

Responsável pelo adaptador entre o Digital Twin e a porta serial. A implementação poderá ser Python, Node.js ou outra tecnologia definida pela equipe, desde que preserve `../docs/protocol.md`.

Estrutura reservada:

```text
src/eezybotarm_backend/  transporte serial, parser e estado
tests/                   testes de protocolo com porta simulada
```

Requisitos mínimos da primeira entrega: configuração local de porta, `PING`, timeout/reconexão e envio atômico de `SET_JOINTS`.
