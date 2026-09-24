# Referências técnicas

Referências de implementação, não código a copiar sem verificação de licença:

- [easyEEZYbotARM](https://github.com/meisben/easyEEZYbotARM): separa Python (cinemática/simulação) de Arduino (controle), documenta FK/IK para Mk1/Mk2, PCA9685 e necessidade de calibração individual.
- [eezy_arm_ros](https://github.com/contagon/eezy_arm_ros): referência de FK/IK e geração de trajetórias em arquitetura ROS.
- [eezybotarm-mk2-software](https://github.com/justbuchanan/eezybotarm-mk2-software), [Cartesian Coordinates](https://github.com/IdreesInc/EEZYbotARM-MK2-Cartesian-Coordinates) e [eezybotarm ROS](https://github.com/inaciose/eezybotarm): implementações comparativas; validar convenções de eixos e juntas antes de reutilizar ideias.
- [Corke, Robotics, Vision and Control](https://petercorke.com/books/robotics-vision-control-all-versions/): base para modelagem e cinemática.
- [Framework de Digital Twin com sincronização em tempo real](https://link.springer.com/article/10.1007/s10846-025-02346-w): referência acadêmica para sincronização e processamento de dados.

Os PDFs já presentes em `ref/` foram mantidos como material local. As demais URLs acadêmicas fornecidas devem ser citadas no relatório/TCC com metadados bibliográficos completos após leitura e validação de acesso.

## Engenharia de software e colaboração

- Bipp, Lepper e Schmedding (2008), [Pair programming in software development teams](https://doi.org/10.1016/j.infsof.2007.05.006): fundamenta pares em tarefas de interface e maior qualidade por revisão contínua.
- Kalliamvakou et al. (2016), [Work practices and challenges in pull-based development](https://doi.org/10.1145/2884781.2884826): fundamenta Pull Requests como ponto de integração e revisão.
- Patkar et al. (2026), [What do feature branches tell us about feature implementation](https://doi.org/10.1007/s10664-026-10941-0): fundamenta branches pequenas, orientadas a uma entrega.
- Arvanitou et al. (2023), [Quality attributes and maintainability in embedded systems](https://doi.org/10.1016/j.jss.2023.111678); [A Simple and Practical Embedded Software System Architecture](https://doi.org/10.1016/j.procs.2020.02.021); [Refactoring embedded software](https://doi.org/10.1016/j.infsof.2021.106760): fundamentam módulos de baixo acoplamento e refactors isolados.
- Gebhard, Cullmann e Heckmann (2011), [Software Structure and WCET Predictability](https://doi.org/10.4230/OASIcs.PPES.2011.1): fundamenta parser limitado, sem alocação dinâmica e comportamento previsível no firmware.
