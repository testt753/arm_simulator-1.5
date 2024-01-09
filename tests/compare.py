import re

def parse_regs(text):
    regs = {}
    for line in text.split('\n'):
        if line.startswith('R'):
            parts = re.split(r'\s=\s', line)
            regs[parts[0]] = int(parts[1], 16)
    return regs

def compare_simulators(file1, file2):
    with open(file1) as f1, open(file2) as f2:
        regs1 = parse_regs(f1.read())
        regs2 = parse_regs(f2.read())

    match = True
    for reg, val1 in regs1.items():
        val2 = regs2.get(reg)
        if val1 != val2:
            print(f'{reg} mismatch: {val1:08x} != {val2:08x}') 
            match = False

    if match:
        print('Registers match!')
    else:
        print('Registers do NOT match!')

compare_simulators('sim1.txt', 'sim2.txt')