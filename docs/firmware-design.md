# Fundamentação do firmware

## Decisões adotadas

1. **O firmware é uma camada de execução segura, não de cinemática.** FK/IK e planejamento de trajetória pertencem ao Digital Twin; o ESP32 recebe alvos articulares. Essa divisão permite testar matemática no ambiente virtual e manter o microcontrolador determinístico.
2. **O estado publicado é “alvo comandado”.** MG996/MG90 não fornecem leitura de posição ao ESP32. Por isso `STATE` não declara posição física nem conclusão de movimento; essa distinção evita uma falsa sincronização do gêmeo digital.
3. **Segurança antes de movimento.** O parser limita cada linha a 96 bytes, descarta a linha inteira quando ultrapassada, valida todos os quatro ângulos antes de aceitar o comando e começa descalibrado. `SET_JOINTS` retorna `NOT_CALIBRATED` até existir configuração física validada. `ESTOP` é prioritário e bloqueia novos alvos.
4. **Comandos articulares são atômicos.** `SET_JOINTS` envia J1–J4 juntos; não há meio estado devido a uma mensagem parcialmente válida.
5. **O driver PWM é uma fase posterior.** PCA9685 e servo só serão adicionados após registrar canal, pulsos e limites reais por junta. Não há valores inventados de µs, I²C ou pinos no código atual.

## Evidência para o desenho

O estudo de Sheshu et al. usa ESP32, PCA9685 e servos de alto torque num gêmeo digital e mede sincronização média de 32 ms em Wi-Fi de consumo. Isso apoia uma arquitetura modular e a medição explícita de comunicação, mas não autoriza assumir a mesma latência em USB serial ou a mesma calibração mecânica. O TCC de Souza modela juntas e valida trajetórias/controle primeiro no ambiente virtual, reforçando o fluxo simulação → teste físico controlado. As implementações EEZYbotARM de referência também separam cinemática de alto nível do controle de servo e salientam a calibração experimental individual.

## Próxima fase de firmware

1. Implementar `JointConfig` por junta (canal, inversão, faixa lógica, pulso mínimo/máximo), preenchido a partir da calibração real.
2. Criar adaptador PCA9685 isolado do parser e testar uma junta sem carga.
3. Adicionar trajetória limitada por velocidade e `moving=1`; só então a interface pode representar movimento temporal.
4. Para sincronização física real, adicionar feedback externo (encoder/sensor) e um estado de falha de comunicação.

## Fontes

- Sheshu et al., [Digital Twin-Based Interactive Robotic Arm System](https://www.atlantis-press.com/proceedings/issf-26/126025145), 2026.
- Souza, *Desenvolvimento de um Gêmeo Digital para um Braço Robótico com Três Graus de Liberdade*, 2025 (cópia local em `ref/`).
- [easyEEZYbotARM](https://github.com/meisben/easyEEZYbotARM), especialmente suas notas de calibração e separação Python/Arduino.
