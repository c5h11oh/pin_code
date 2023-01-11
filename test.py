"""
test that duplicate pin codes are not emitted until all other alternatives are exhausted.
persistent data should not exist before running this test.
"""

# how many valid pin codes depends on the "good pin code criterias" 
VALID_PIN_CODE_COUNT = 9540

from subprocess import Popen, PIPE

passed = True
for round in range(3):
    outputs = set()
    for i in range(VALID_PIN_CODE_COUNT):
        process = Popen(["./pin_codes"], stdout=PIPE)
        (output, err) = process.communicate()
        exit_code = process.wait()
        output = int(output)
        if output in outputs:
            print(f'duplicate output seen in {i + 1}-th pin code: {output}')
            passed = False
        else:
            outputs.add(output)
    if not passed:
        print("Test failed")
        exit(0)

print("Test passed")