def parse(gcode):
    lines = gcode.split('\n')
    output = []

    for line in lines:
        cmd = list(filter(None, line.split(' ')))
        if(len(cmd) == 0):
            continue
        instr = parseCmd(cmd)
        if instr != None:
            output.append(instr)

    return output


def valueCode(cmd, code):
    try:
        return list(filter(lambda x: x[0] == code, cmd[1:]))[0][1:]
    except:
        raise Exception("Error parsing cmd: ", cmd)


def parseCmd(cmd):
    operation = cmd[0]

    if operation in ['G0', 'G1']:  # linear movement
        if(len(cmd) <= 2):
            return None
        x = valueCode(cmd, 'X')
        y = valueCode(cmd, 'Y')
        return {
            'type': 'movement',
            'move_type': 'linear',
            'toX': float(x),
            'toY': float(y)
        }
    elif operation in ['G2', 'G3']:  # G2 = arc movement clockwise; G3 = arc movement counter-clockwise
        x = valueCode(cmd, 'X')
        y = valueCode(cmd, 'Y')
        instr = {
            'type': 'movement',
            'move_type': 'arc',
            'clockwise': operation == 'G2',
            'toX': float(x),
            'toY': float(y)
        }

        try:
            offsetX = valueCode(cmd, 'I')
            offsetY = valueCode(cmd, 'J')
            instr['offsetX'] = float(offsetX)
            instr['offsetY'] = float(offsetY)
        except:
            radius = valueCode(cmd, 'R')
            instr['radius'] = radius

        return instr
    elif operation in ['M03', 'M05']: # M03 = put down the pen; M05 = put up the pen
        return {
            'type': 'pen',
            'down': operation == 'M03'
        }

    return None

def findLastPosition(lines):
    for line in reversed(lines):
        cmd = list(filter(None, line.split(' ')))
        if(len(cmd) == 0):
            continue
        instr = parseCmd(cmd)
        if instr != None and instr['type'] == 'movement':
            return instr['toX'], instr['toY']
    
    return 0, 0
