def parse(gcode):
    lines = gcode.split('\n')
    output = []

    for line in lines:
        cmd = line.split(' ')
        instr = parseCmd(cmd)
        if instr != None:
            output.append(instr)

    return output


def parseCmd(cmd):
    operation = cmd[0]

    if operation in ['G0', 'G1']: #movimento sugli assi
        return {}
    elif operation == 'G2': #movimento controllato ad arco in senso orario
        return {}
    elif operation == 'G3': #movimento controllato ad arco in senso antiorario
        return {}
    elif operation == 'M03': #abbassa penna
        return {}
    elif operation == 'M05': #alza penna
        return {}

    return None