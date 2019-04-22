def parse(gcode):
    lines = gcode.split('\n')
    output = []

    for line in lines:
        cmd = line.split(' ')
        instr = parseCmd(cmd)
        if instr != None:
            output.append(instr)

    return output


def valueCode(cmd, code):
    return list(filter(lambda x: x[0] == code, cmd[1:]))[0][1:]


def parseCmd(cmd):
    operation = cmd[0]

    if operation in ['G0', 'G1']:  # movimento sugli assi
        x = valueCode(cmd, 'X')
        y = valueCode(cmd, 'Y')
        return {
            'type': 'movement',
            'move_type': 'linear',
            'toX': x,
            'toY': y
        }
    elif operation == 'G2':  # movimento controllato ad arco in senso orario
        x = valueCode(cmd, 'X')
        y = valueCode(cmd, 'Y')
        instr = {
            'type': 'movement',
            'move_type': 'arc',
            'clockwise': True,
            'toX': x,
            'toY': y
        }

        try:
            offsetX = valueCode(cmd, 'I')
            offsetY = valueCode(cmd, 'J')
            instr['offsetX'] = offsetX,
            instr['offsetY'] = offsetY
        except:
            radius = valueCode(cmd, 'R')
            instr['radius'] = radius

        return instr
    elif operation == 'G3':  # movimento controllato ad arco in senso antiorario
        x = valueCode(cmd, 'X')
        y = valueCode(cmd, 'Y')
        instr = {
            'type': 'movement',
            'move_type': 'arc',
            'clockwise': False,
            'toX': x,
            'toY': y
        }

        try:
            offsetX = valueCode(cmd, 'I')
            offsetY = valueCode(cmd, 'J')
            instr['offsetX'] = offsetX,
            instr['offsetY'] = offsetY
        except:
            radius = valueCode(cmd, 'R')
            instr['radius'] = radius

        return instr
    elif operation == 'M03':  # abbassa penna
        return {
            'type': 'pen',
            'down': True
        }
    elif operation == 'M05':  # alza penna
        return {
            'type': 'pen',
            'down': False
        }

    return None
