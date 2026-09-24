# Como contribuir

## Donos e fronteiras

| Área | Responsável inicial | Contrato público |
| --- | --- | --- |
| `firmware/` e `backend/` | [Ryhan Schutz](https://github.com/ryhanschutz) | `docs/protocol.md` |
| `digital-twin/` | [Victor Gabriel Kostetzer](https://github.com/V651202) | `docs/protocol.md` |
| `docs/` | ambos | revisão obrigatória para contrato e segurança |

Não há dependência direta entre `digital-twin/` e detalhes internos de PWM. O backend não reproduz cinemática: ele transporta e valida comandos.

## Fluxo

1. Crie uma issue pequena e uma branch `ryhan/<assunto>` ou `vitor/<assunto>`.
2. Faça commits pequenos usando `feat`, `fix`, `docs`, `test`, `refactor` ou `chore`.
3. Execute os testes que sua alteração alcança e documente o resultado no PR.
4. Solicite revisão antes de mesclar em `main`.

Mudanças em `docs/protocol.md`, limites mecânicos, pinagem, alimentação ou parada de emergência exigem concordância dos dois desenvolvedores. Nunca adivinhe valores de calibração.

## Pareamento e integração

Para entregas de fronteira (protocolo, integração, cinemática e segurança), trabalhem em pares: quem dirige implementa; quem navega confere contrato, limites, testes e documentação. Alternem os papéis ao concluir uma tarefa pequena. O PR é a unidade de integração: deve ter objetivo, teste e revisão do outro responsável; evitem branches longas e misturas de refactor com mudança funcional.

O guia operacional completo do Victor está em `docs/onboarding-victor.md`; ele também estabelece exatamente o que o Digital Twin pode e não pode alterar.

## Definition of Done

- Código e testes relevantes passam.
- O contrato/documentação foi atualizado quando necessário.
- O PR declara como foi testado e se exigiu hardware.
- Nenhum segredo, porta serial local ou calibração física privada foi incluído.
