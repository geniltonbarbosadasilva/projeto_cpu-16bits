def printf(x):
    print(x, end=' ')

def treat(line):
    line = line.replace('(', ' ')
    line = line.replace(')', ' ')
    line = line.replace(',', ' ')
    line = line.replace('  ', ' ')
    line = line.strip()
    line = line.lower()
    return line

def addOne( number, digit):
    number = number[::-1]
    overFlow = 1
    result = ""
    for i in range(digit):
        if ( number[i] == '1' and overFlow == 1):
            result = "0" + result
            continue
        if ( number[i] == '1' or overFlow == 1):
            result = "1" + result
            overFlow = 0
            continue
        result = "0" + result

    return result
            

def twoComplement( number, digit):
    bin = convertBin( number*-1, digit)
    bin = bin.replace( '0', 'i')
    bin = bin.replace( '1', '0')
    bin = bin.replace( 'i', '1')

    return addOne( bin, digit)

def convertBin(number, digit):    
    bin = ""

    if number < 0:
        return twoComplement( number, digit)

    while number >= 1:
        bin = bin + "{}".format(number%2)
        number = int(number/2)

    while len(bin) < digit:
        bin = bin + "0"
    
    return bin[::-1];

def convertHexa(bin):    
    hexadecimal = [ bin[0:4], bin[4:8], bin[8:12], bin[12:16]]
    result = ""
    
    for hexa in hexadecimal:
        sum = 0    
        hexa = hexa[::-1]
        for i in range( 0, 4):            
            if(int(hexa[i]) == 1):
                sum = sum + 2**i                
        result = result + " {}".format(sum)

    result = result.replace(" 10", "a")
    result = result.replace(" 11", "b")
    result = result.replace(" 12", "c")
    result = result.replace(" 13", "d")
    result = result.replace(" 14", "e")
    result = result.replace(" 15", "f")
    result = result.replace(" ", "")
    return result;

def opCode(op):
    if(op == "add" or op == "sub" or op == "and" or op ==  "or" or op ==  "not" or op == "slt"):
        return "0000"
    if(op == "lw"):
        return "0001"
    if(op == "sw"):
        return "0010"
    if(op == "addi"):
        return "0011"
    if(op == "subi"):
        return "0100"
    if(op == "beq"):
        return "0101"
    if(op == "bnq"):
        return "0110"
    if(op == "jump"):
        return "0111"
    if(op == "jr"):
        return "1000"
    if(op == "jal"):
        return "1001"


def funct(op):
    if(op == "add"): 
        return "000"
    if(op == "sub"):
        return "001"
    if(op == "and"):  
        return "010"
    if(op ==  "or"):  
        return "011"
    if(op ==  "not"):  
        return "100"
    if(op == "slt"):
        return "101"

def code(instruct):
    op = opCode(instruct[0])

    if(op == "0000"):
        rd = int(instruct[1].replace('$', ''))
        rs = int(instruct[2].replace('$', ''))
        rt = int(instruct[3].replace('$', ''))

        return op + convertBin( rs, 3) + convertBin( rt, 3) + convertBin( rd, 3) + funct(instruct[0])
    
    if(op == "0011"):        
        rs = int(instruct[2].replace('$', ''))
        rt = int(instruct[1].replace('$', ''))
        imme = int(instruct[3].replace('$', ''))
        return op + convertBin( rs, 3) + convertBin( rt, 3) + convertBin( imme, 6)

    if(op == "0101"):        
        rs = int(instruct[1].replace('$', ''))
        rt = int(instruct[2].replace('$', ''))
        imme = int(instruct[3].replace('$', ''))
        return op + convertBin( rs, 3) + convertBin( rt, 3) + convertBin( imme, 6)

    rt = int(instruct[1].replace('$', ''))
    imme = int(instruct[2].replace('$', ''))
    rs = int(instruct[3].replace('$', ''))
    
    return op + convertBin( rs, 3) + convertBin( rt, 3) + convertBin( imme, 6)

file = open("assembly", "r")

lines = file.readlines()

for line in lines:
    if(len(line)==1 or '#' in line):
        continue
    instruct = treat(line).split()
    # print(code(instruct))
    printf(convertHexa(code(instruct)))
    # print(line.strip() + " = " + code(instruct))

print("f000")
