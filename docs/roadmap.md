# Roadmap v0.1 — vertical slice segura

- [ ] **#1** Confirmar pinagem, fonte e montagem; preencher calibração local de J1.
- [ ] **#2** Firmware: driver PCA9685 e teste seguro de J1.
- [ ] **#3** Firmware/backend: `PING`, `GET_STATE` e parser de `SET_JOINTS` com testes.
- [ ] **#4** Digital Twin: modelo mínimo e controle de J1 usando adaptador simulado.
- [ ] **#5** Integração: Digital Twin → Backend → ESP32 → J1.
- [ ] **#6** Repetir juntas J2–J4 após calibração individual.
- [ ] **#7** FK, IK, verificação de workspace e testes de pontos alcançáveis.

Cada item acima deve virar uma issue no GitHub, com branch e PR próprios.
