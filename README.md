# 

# 

# 

# **Documentação de Projeto – Parte 1** **CONOPS, Domínio do Problema, Especificação**

# 

# 

Projeto:   	Sistema Filtro Digital Parametrizado

Autores:	Diogo Levandoski e Letícia Walger Amaro	

**Versão:** 01-Nov-2025

# **Parte 1a – CONOPS**

1. ## **Introdução**

O Sistema Filtro Digital Parametrizado é um sistema embarcado para processamento de sinais. Sua principal funcionalidade é adquirir um sinal analógico, aplicar um filtro digital com características configuráveis pelo usuário e, em seguida, converter o sinal digital filtrado de volta para analógico. O usuário pode selecionar o tipo de filtro (Passa-Baixa, Passa-Alta, Passa-Faixa) e definir suas frequências de corte através de uma interface de usuário local. O sistema é aplicável em um contexto de laboratório acadêmico, permitindo a análise e visualização dos efeitos da filtragem digital em sinais com frequências de 1 Hz a 5 kHz. Outras aplicações incluem em equipamentos de áudio, como equalizadores ou *crossovers* de caixas acústicas, para separar e ajustar faixas de frequência específicas. Além disso, seria aplicável em instrumentação biomédica ou industrial para filtrar ruídos de sinais de sensores em tempo real, melhorando a precisão de medições.

2. ## **Descrição do Sistema**

O sistema a ser desenvolvido atua como um bloco de processamento entre dois equipamentos de laboratório existentes: um Gerador de Sinais e um Osciloscópio/Analisador de Espectro.

O diagrama estrutural do sistema e seu contexto operacional é composto por três interfaces principais:

* **Interface de Entrada (ADC):** Recebe um sinal analógico (0-3V, 1 Hz-5 kHz) de um Gerador de Sinal.  
* **Interface de Saída (DAC):** Envia o sinal analógico processado para um Osciloscópio ou Analisador de Espectro para visualização.  
* **Interface com o Usuário (IHM):** Uma "IHM de parametrização" que permite ao usuário configurar o comportamento do filtro.

Internamente, o sistema implementará filtros digitais de ordem 64\.

3. ## **Interface com o Usuário**

A interface com o usuário será implementada em um display LCD de um *boosterpack*. Esta interface é o único meio pelo qual o usuário irá parametrizar o sistema.

A descrição informal da interface planejada é:

* **Tela Principal/Menu:** Apresentará o status atual (filtro ativo, frequências) e permitirá a navegação para as telas de configuração. 

![Tela principal no LCD](LCD_principal.png)

* **Tela de Seleção de Filtro:** Permitirá ao usuário escolher entre os tipos de filtro disponíveis: FPB (Filtro Passa-Baixa), FPA (Filtro Passa-Alta) e FPF (Filtro Passa-Faixa).

![Tela de seleção de filtro no LCD](LCD_filtro.png)

* **Tela de Parametrização:** Permitirá ao usuário escolher os valores numéricos para as frequências de corte de acordo com o filtro selecionado. No caso do FPF (Filtro Passa-Faixa), o usuário escolherá primeiro a frequência de corte baixa e depois a alta.

![Tela de escolha da frequência no LCD](LCD_freq.png)

4. ## **Identificação dos Stakeholders**

Os seguintes papéis/pessoas têm interesse no sistema:

* **Cliente:** O professor da disciplina, que definiu os requisitos do projeto e avaliará a solução final.  
* **Usuário:** Alunos da disciplina de Sistemas Embarcados ou técnicos de laboratório que utilizarão o sistema para fins didáticos e de teste.  
* **Desenvolvedor:** Os autores do projeto, responsáveis por projetar, implementar e documentar o sistema.

5. ## **Necessidades de Stakeholders**

A lista informal de necessidades dos stakeholders é:

* **Usuário:**  
  * Precisa ser capaz de aplicar um filtro digital a um sinal de entrada vindo de um gerador.  
  * Precisa poder escolher entre um filtro Passa-Baixa, Passa-Alta e Passa-Faixa.  
  * Precisa poder definir as frequências de corte do filtro selecionado.  
  * Precisa que a interface no LCD seja clara e fácil de usar para fazer essas configurações.  
  * Precisa ver o resultado do sinal filtrado em um osciloscópio.  
  * Precisa que o sistema funcione para sinais de entrada na faixa de 1 Hz a 5 kHz e com amplitude de 0 a 3V.  
* **Cliente:**  
  * Precisa que o sistema implemente corretamente filtros de ordem 64\.  
  * Precisa que a solução utilize a biblioteca CMSIS-DSP.  
  * Precisa que a IHM seja feita no display LCD do boosterpack.  
* **Desenvolvedor:**  
  * Precisa de acesso aos equipamentos de laboratório (Gerador de Sinais, Osciloscópio) para testar o sistema.  
  * Precisa dos requisitos claros de operação e parametrização.

6. ## **Cenários de Operação**

**Cenário 1: Operação Normal (Filtro Passa-Baixa)**

1. O Usuário conecta um Gerador de Sinais à entrada ADC e um Osciloscópio à saída DAC.  
2. O Usuário liga o sistema. A IHM exibe o menu principal.  
3. O Usuário configura o Gerador para um sinal senoidal de 2V com frequência de 2 kHz.  
4. Na IHM, o Usuário seleciona o tipo de filtro "FPB" e define a frequência de corte como 1 kHz.  
5. O Usuário confirma a seleção. O sistema começa a processar.  
6. O Usuário observa no Osciloscópio que o sinal de 2 kHz na saída está visivelmente atenuado, conforme esperado para um filtro passa-baixa.

**Cenário 2: Reparametrização**

1. Seguindo o Cenário 1, o sistema está rodando com um FPB de 1 kHz.  
2. O Usuário, através da IHM , altera o tipo de filtro para "FPA" (Filtro Passa-Alta) e mantém a frequência de corte em 1 kHz.  
3. O sistema aplica a nova configuração.  
4. O Usuário observa no Osciloscópio que o sinal de 2 kHz agora passa sem atenuação (ou com pouca atenuação).

**Cenário 3: Situação Anormal (Sinal de Entrada Fora da Faixa)**

1. O Usuário configura um filtro FPA de 1 kHz.  
2. O Usuário ajusta o Gerador de Sinais para uma amplitude de 4V, que está acima do limite especificado de 3V.  
3. A entrada ADC do sistema satura.  
4. O Usuário observa no Osciloscópio um sinal de saída distorcido ("clipado"), independentemente da configuração do filtro. O sistema, no entanto, não deve travar e deve continuar operando.

# **Parte 1b – Domínio do Problema**

O objetivo desta seção é descrever os conceitos relevantes do domínio de Processamento Digital de Sinais (PDS) aplicados ao desenvolvimento do Sistema Filtro Digital Parametrizado.

1. ## **Processamento Digital de Sinais (PDS)**

O PDS é a área da engenharia que trata do processamento de sinais por meios digitais. Diferente do processamento analógico, no PDS o sinal é primeiro digitalizado (amostrado e quantizado), processado por um algoritmo em um processador digital (como um microcontrolador) e, frequentemente, convertido de volta para a forma analógica. Este projeto foca em uma das principais aplicações do PDS: a filtragem digital.

2. ## **Amostragem e Conversão Analógico-Digital (ADC)**

Para que um sinal analógico contínuo no tempo (como o proveniente do Gerador de Sinais) possa ser processado digitalmente, ele precisa ser convertido em uma sequência de números digitais. Esse processo é feito pelo Conversor Analógico-Digital (ADC).

* **Amostragem:** O sinal é medido em intervalos de tempo regulares, definidos pela "taxa de amostragem".  
* **Teorema de Nyquist:** Para evitar a perda de informação (aliasing), a taxa de amostragem deve ser, no mínimo, duas vezes maior que a maior frequência presente no sinal de entrada. Dado que o sinal de entrada pode chegar a 5 kHz, a taxa de amostragem do sistema deverá ser superior a 10 kHz.  
* **Quantização:** A amplitude de cada amostra é representada por um número digital finito, o que introduz um erro de quantização. A tensão de entrada do sistema é especificada como 0 a 3V.

3. ## **Filtros Digitais**

Um filtro digital é um algoritmo que opera sobre uma sequência de amostras digitais (o sinal de entrada) para produzir uma nova sequência (o sinal de saída), atenuando ou realçando bandas de frequência específicas. O projeto utilizará três tipos de filtros:

* **Filtro Passa-Baixa (FPB):** Permite a passagem de frequências abaixo de uma frequência de corte definida, atenuando frequências mais altas.  
* **Filtro Passa-Alta (FPA):** Permite a passagem de frequências acima de uma frequência de corte, atenuando frequências mais baixas.  
* **Filtro Passa-Faixa (FPF):** Permite a passagem de frequências dentro de uma faixa definida por duas frequências de corte, atenuando as demais.

A ordem do filtro (definida como 64 neste projeto) está relacionada à complexidade do filtro e à sua capacidade de atenuar frequências indesejadas. Filtros de ordem mais alta geralmente têm uma transição mais abrupta entre a banda de passagem e a banda de rejeição.

4. ## **Conversão Digital-Analógica (DAC)**

Após o processamento digital (filtragem), a sequência de amostras resultante é convertida de volta para um sinal analógico contínuo pelo Conversor Digital-Analógico (DAC). Isso permite que o sinal processado seja visualizado em equipamentos analógicos, como um osciloscópio. Uma alternativa mencionada é o uso de PWM (Modulação por Largura de Pulso) seguido de um filtro passa-baixa analógico (rede RC) para realizar essa conversão.

5. ## **Biblioteca CMSIS-DSP**

A CMSIS (Common Microcontroller Software Interface Standard) é um padrão da ARM para microcontroladores Cortex-M. A biblioteca CMSIS-DSP, um componente desse padrão, fornece uma coleção de funções otimizadas para tarefas de PDS, como operações matemáticas, transformadas (FFT) e implementação de filtros digitais. O uso desta biblioteca é um requisito e é fundamental para o domínio do problema, pois garante que os algoritmos de filtragem sejam executados de forma eficiente no hardware embarcado, permitindo o processamento necessário para a aplicação.

# **Parte 1c – Especificação**

1. ## **Introdução**

Esta seção detalha os requisitos funcionais, não funcionais e as restrições do sistema. Estes requisitos servem como contrato formal para o desenvolvimento e validação do projeto, fazendo referência às necessidades dos stakeholders identificadas na Parte 1a.

2. ## **Estrutura do Sistema**

![Diagrama UML](UML_CONOPS.png)

Componentes Principais do Sistema

1\. Interface ADC (Analog-to-Digital Converter)

* Função: Conversão do sinal analógico de entrada para formato digital  
* Entrada: Recebe sinal analógico do gerador de sinais (0-3V, 1Hz-5kHz)  
* Saída: Fornece amostras digitais para processamento  
* Característica: Interface de aquisição que digitaliza o sinal contínuo

2\. Processamento Digital

* Função: Núcleo de processamento do sistema  
* Capacidades:  
  * Implementa três tipos de filtros: Passa-Baixa (FPB), Passa-Alta (FPA), Passa-Faixa (FPF)  
  * Filtros de ordem 64 para resposta frequencial precisa  
  * Utiliza biblioteca CMSIS-DSP para eficiência computacional  
* Processo: Aplica algoritmos de filtragem digital às amostras adquiridas

3\. Interface DAC (Digital-to-Analog Converter)

* Função: Reconversão do sinal processado para formato analógico  
* Entrada: Recebe sinais digitais filtrados  
* Saída: Fornece sinal analógico processado para visualização  
* Implementação: Pode utilizar DAC hardware ou PWM com filtro RC

4\. IHM \- LCD BoosterPack

* Função: Interface primária de interação com o usuário  
* Display: LCD do boosterpack para visualização  
* Funcionalidades:  
  * Seleção do tipo de filtro (FPB, FPA, FPF)  
  * Configuração de parâmetros (frequências de corte)  
  * Visualização do status do sistema

Interfaces Externas do Sistema

Ator: Gerador de Sinais

* Papel: Fonte do sinal analógico a ser processado  
* Características do sinal: 0-3V de amplitude, frequência de 1Hz a 5kHz  
* Conexão: Interface ADC do sistema

Ator: Osciloscópio/Analisador

* Papel: Equipamento de visualização do resultado  
* Função: Permite análise do sinal processado no domínio do tempo e frequência  
* Conexão: Interface DAC do sistema

Ator: Usuário

* Papel: Operador e configurador do sistema  
* Interação: Através da IHM para parametrização dos filtros  
* Capacidades: Seleção de tipo de filtro e definição de frequências de corte

Fluxo de Operação Principal

Aquisição: Sinal analógico \-\> Interface ADC \-\> Amostras digitais

Configuração: Usuário \-\> IHM \-\> Parâmetros do processamento

Processamento: Amostras digitais \+ Parâmetros \-\> Filtro digital \-\> Sinais filtrados

Saída: Sinais filtrados \-\> Interface DAC \-\> Sinal analógico processado

Visualização: Sinal processado \-\> Osciloscópio para análise

3. ## **Especificação Funcional**

Esta seção detalha as funções que o sistema deve executar.

* **RF-01:** O sistema deve adquirir um sinal analógico através da sua interface de entrada (ADC).  
* **RF-02:** O sistema deve gerar um sinal analógico processado através da sua interface de saída (DAC).  
* **RF-03:** O sistema deve aplicar um filtro digital ao sinal adquirido antes de o enviar para a saída.  
* **RF-04:** O sistema deve fornecer a funcionalidade de Filtro Passa-Baixa (FPB).  
* **RF-05:** O sistema deve fornecer a funcionalidade de Filtro Passa-Alta (FPA).  
* **RF-06:** O sistema deve fornecer a funcionalidade de Filtro Passa-Faixa (FPF).  
* **RF-07:** O sistema deve permitir ao usuário definir as frequências de corte para o filtro selecionado.

  1. ## ***Especificação da Interface com o Usuário***

Esta seção detalha os requisitos da Interface Homem-Máquina (IHM).

* **RF-IHM-01:** A IHM deve ser implementada e exibida no display LCD do boosterpack.  
* **RF-IHM-02:** A IHM deve apresentar ao usuário uma opção para selecionar o tipo de filtro (FPB, FPA ou FPF).  
* **RF-IHM-03:** A IHM deve permitir ao usuário inserir valores numéricos para as frequências de corte.  
* **RF-IHM-04:** A IHM deve incluir todas as telas e métodos de entrada de dados necessários para a configuração completa dos parâmetros do filtro.

4. ## **Especificação Não Funcional**

Esta seção apresenta as características não funcionais e de desempenho do sistema.

* **RNF-01:** O sistema deve ser capaz de processar sinais de entrada na faixa de frequência de 1 Hz a 5 kHz.  
* **RNF-02:** O sistema deve ser capaz de processar sinais de entrada na faixa de amplitude de 0 a 3V.  
* **RNF-03:** Todos os filtros digitais implementados devem ser de ordem 64\.

5. ## **Restrições**

São requisitos que limitam as escolhas de implementação ou o processo de desenvolvimento.

* **RST-01:** A solução de software deve obrigatoriamente utilizar a biblioteca CMSIS-DSP para as operações de processamento digital de sinal.  
* **RST-02:** O escopo do projeto a ser desenvolvido inclui apenas o HW e SW do Sistema Filtro Digital Parametrizado.  
* **RST-03:** O gerador de sinais e o osciloscópio são equipamentos externos, fornecidos pelo laboratório, e não fazem parte do escopo de desenvolvimento.  
* **RST-04:** A interface de usuário deve ser implementada especificamente no display LCD do boosterpack.

[image1]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAARoAAAA2CAYAAAAYqlZDAAAFCElEQVR4Xu2d25GjMBAAHRcBOR6iIRl+NhPfChAMeo7WjC37uqvmyjYDEjrUC+J1ewAAGHNb/rndCIIgLo9INAAAV4JoAMAcRAMA5iAaADAH0QCAOS8UzRSNQqfiPq3Z9+17OO/O7H4bju9vwNexFi3592kWJfwy3aMcGcN9POcDdIjsBy8RjRdJG7FohuV7H6LRUsqf53lfx1G6ZhNN1Gy/+fdhE87fGhXgZSCaJyiJI0Utfx6HWBw50Wys7TKc5QTQGSrR+A4S7tW3gWg0+eF61kQzslcDH4BKNG43fe3YR0xzq3XaRFMao5H1kMv0n6f7MW3/S7+M6ZznczFEFdoOYYb62IdGHBJNvq/XTkU0fl0HdmmgY+Q2nReNYBrvJ+kMwz1MyXCdaBypPRqf42LpdrMv7JjfC3KexqfHODTikNTz/TiNaNOKaJ6pP8CrkNu9SjSSWZ4Rqe4BpPcoZMg/ys+IJux3pQ5emlbDz5uPc/1KZTnx+flO1eesE3wBcrv/DtH81iM8iCjWb147+F+OPP4qmlJE9aiIZonEOgP0hNtO98/hDym6P3RKdLrl92zB6+FKdnKB0h5Kitb8hcqhkxyPAugVnWg6Gwx2XCeatnpJWsXRmr9QEY1j/z8JJwB0gtzus6LxHeSVp7cvF03mVLi/dkVZrROt4mjNX6iK5rjQL1xvgF7QiWYYLtiI3yeaUgf308J5NJSWm6I1f6Emmm2MqXm5AC9EJZpruFY0KUHkRCPn7/v0doKCaNoG4wHex8eKZr9cX4jCd7hYNI/lmpo9X0RvF+xFKM46JdcXoCPkdm8sGgD4X0E0AGAOogEAcxANAJiDaADAHEQDAOYgGgAwB9EAgDmIBgDMQTQAYI5SNOnL98OIruY3xpVZLnJ+DNv9TEP1xlCrXInuNoy1PbXP+QHoH+mVjxKNvyEyW2TmHqFkHa1yIxAN/J+4bXr/HP5woOsgr0I+hCtXpVRn9fOFWOXG6NoxVQbAJ/NRopnHY29i3D6nqpR/mNV6Z7Z8NYlVbhpdOyIa+DbsRDNPx/Ne9gifctfydLhpfcyCf0Zx9jkt8/5StRRLeeIREJpcX7eW3DS6dlzb5BDN6ZEYiSiXCfB+ZL+5VDThM4Z9nJfRKBr5IPSsaDYhJdcj7sSaXF9GS24aXTuGdUQ08OnIfnOdaMSAqUQ/lqEgJ5p5TD7m0xNOC79L/DR/RNSSm0Y3qL5G+dDJv/1A/X8C8EZkv6+KphSpdzJFneCJdyhFVEWT7qhr3ULRlHPPotHlpqm34xHpcha8yKMGBugTt73un8MfDtr2aHxHUab/japoynseue+ScC+lJTeNrh3LovGyyk0H6A870VQHRp8kJ5rH943RSNZp6ToA9IqJaHR/3Z8kK5pj/CLF0lHFiljlptG1Y040SAY+FRPR+NPAccdrW06Rgmj8WFBUzjbPSYBWuUl0658SzSrvzPoCdI6JaORp6/11JuJ1J5dQEs3j6Jj3cc1w7wxfy4/HWKxyY3TtuC7vEI0Xd20+gF6R/f5C0TwyF+yF13y0XEcTUBGNw0vhiLwMrHLP6NpxXeYhmnNZiagtEODNuO10/xz+cKDrICnc63R9Z4zf3W0rGrd8f5f1rXqXtVWuRNeOa5sgGvge3Ha6fw5/AAC4AkQDAOYgGgAwB9EAgDmIBgDMOYnm5+cnPqNBEARxQTi/LI4RAgIAMAHRAIA5/wDLNjdZGxPDxAAAAABJRU5ErkJggg==>

[image2]: <data:image/png;base64,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>

[image3]: <data:image/png;base64,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>

[image4]: <data:image/png;base64,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>