import subprocess
import random
import os

try:
    os.remove('test-gen.csv')
    print(f"File {'test-gen.svp'} has been removed.")
except FileNotFoundError:
    print(f"No action taken. The file {'test-gen.svp'} does not exist.")

tests = []

for dimension in range(1, 51):
    for bit_level in (8, 16, 32):
        for _ in range(100 if dimension <= 10 else 10):
            tests.append((str(random.getrandbits(32)), 'u', str(dimension), str(bit_level)))

for dimension in range(2, 51):
    for bit_level in (8, 16, 32):
        for _ in range(100 if dimension <= 10 else 10):
            tests.append((str(random.getrandbits(32)), 'r', str(dimension), str(bit_level)))


print(len(tests))
assert len(tests) == len(set(tests)), "Duplicate tests were generated."
random.shuffle(tests)
bash_script = 'test-gen.sh'

for t in tests:
    try:
        subprocess.run(['bash', bash_script, t[0], t[1], t[2], t[3]], check=True)
    except subprocess.CalledProcessError as e:
        print(f"An error occurred: {e}")
