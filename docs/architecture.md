# Arquitetura

```text
Digital Twin (UI + FK/IK) -> Backend (validação + serial) -> ESP32 -> PCA9685 -> servos
          Vitor                    Ryhan                    Ryhan
```

O único contrato entre as camadas é o protocolo de linha UTF-8 em `protocol.md`. O Digital Twin calcula juntas lógicas; o firmware aplica mapeamento, limites mecânicos e pulsos calibrados. O firmware não é fonte de verdade para posição física: sem encoder, ele só conhece o último alvo aceito.

## Limites de responsabilidade

- **Digital Twin:** FK/IK, visualização, verificação de alcançabilidade e geração de comandos.
- **Backend:** transporte serial, reconexão, correlação de comando/resposta e estado de comunicação.
- **Firmware:** parser defensivo, parada segura, limites e PWM. Nenhum movimento deve ser emitido durante boot.

As decisões que modificarem essas fronteiras devem ser registradas em `docs/adr/`.
