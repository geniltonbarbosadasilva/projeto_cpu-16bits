# Projeto CPU 16 bits

<blockquote>
<p><em>Projeto de caminho de dados de uma CPU 16 bits monociclo ao nível de portas lógicas, realizado utilizando o simulador de lógica digital Logisim 2.7.1</em></p>
</blockquote>  

<p align="center">
    <img src="print-screen1.png" width=500>
    <img src="print-screen2.png" width=500>
</p>

## Execução
Para a execução deste projeto utilize o programa <a href="http://www.cburch.com/logisim/">Logisim 2.7.1</a>.

## Como executar ?
Abra o arquivo .circ no Logsim e divirta-se!

## Conjunto de instruções:

<blockquote>
<p><em>Esse projeto de CPU dispõe de 8 registradores com 3 bits para endereçamento para acessa-los</em></p>
</blockquote>

```
    - RS registrador de operação
    - RT registrador de operação    
    - RD registrador de destino
```

### Tipo R:
    Formato:  Opcode RS RT RD Funct

    <table>
    <tr>
        <th>Instrunção</th>
        <th>Opcode</th>
        <th>Funct</th>
    </tr>
    </table>
    if(op == "lw"):     # Load Word
        return "0001"
    if(op == "sw"):     # Store Word
        return "0010"
    if(op == "addi"):   # Add immediate
        return "0011"
    if(op == "subi"):   # Subtract immediate
        return "0100"
    if(op == "beq"):    # Branch Equal
        return "0101"
    if(op == "jump"):   # Jump
        return "0110"
    if(op == "jr"):     # Jump Register
        return "0111"
    if(op == "spc"):    # Store Program Counter
        return "1000" 
    if(op == "sg"):     # Store Graphics
        return "1001" 
    if(op == "lb"):     # Load buffer Keyboad
        return "1010" 
```
        - Ajuste a resolução da imagem para a resolução do display
        - A imagem deve ser um PNG
        - Utilize o programa convert.py para converter a imagem    
        - Agora basta carreaga os dados para ROM usando o formato v3.0 hex
```
