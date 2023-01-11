# test if all 
VALID_PIN_CODE_COUNT = 9900

from subprocess import Popen, PIPE

outputs = set()
for i in range(10000):
    process = Popen(["./pin_codes"], stdout=PIPE)
    (output, err) = process.communicate()
    exit_code = process.wait()
    output = int(output)
    if output in outputs:
        print(f'duplicate output first seen in {i + 1}-th number: {output}')
    else:
        outputs.add(output)
