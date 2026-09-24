# Hardware e segurança

## Premissas

- Controlador: ESP32 DevKit V1.
- Driver de servos: PCA9685 em I²C (a confirmar endereço e pinagem).
- Servos: MG996 para juntas de carga e MG90 para junta leve, conforme montagem física.

## Regras obrigatórias

1. Alimente servos com fonte externa adequada; **não** pelo pino 5 V/USB do ESP32.
2. Una o GND da fonte de servos ao GND do ESP32/PCA9685.
3. Teste uma junta por vez, sem carga e com botão/cabo de energia acessível.
4. Não fixe horns/braços antes de centralizar o servo e registrar a referência.
5. Pinagem, canal PCA9685, faixa angular e faixa de pulso só entram no código após medição em `calibration.local.md`.

| Junta lógica | Servo | Canal PCA9685 | Limite mecânico | Status |
| --- | --- | --- | --- | --- |
| J1 | a confirmar | a confirmar | medir | pendente |
| J2 | a confirmar | a confirmar | medir | pendente |
| J3 | a confirmar | a confirmar | medir | pendente |
| J4 | a confirmar | a confirmar | medir | pendente |
