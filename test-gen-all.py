import subprocess
import random
import os

TEST_FILE = 'test-gen.csv'
BASH_SCRIPT = 'test-gen.sh'

def reset_output_csv():
    try:
        os.remove(TEST_FILE)
        print(f"File {TEST_FILE} has been removed.")
    except FileNotFoundError:
        print(f"No action taken. The file {TEST_FILE} does not exist.")

def generate_tests():
    tests = []

    for dimension in range(1, 51):
        for bit_level in (8, 16, 32):
            for _ in range(100 if dimension <= 10 else 10):
                tests.append((str(random.getrandbits(32)), 'u', str(dimension), str(bit_level)))

    for dimension in range(2, 51):
        for bit_level in (8, 16, 32):
            for _ in range(100 if dimension <= 10 else 10):
                tests.append((str(random.getrandbits(32)), 'r', str(dimension), str(bit_level)))

    return tests


if __name__ == '__main__':
    reset_output_csv()
    tests = generate_tests()

    assert len(tests) == len(set(tests)), "Duplicate tests were generated."

    random.shuffle(tests)

    for t in tests:
        try:
            subprocess.run(['bash', BASH_SCRIPT, t[0], t[1], t[2], t[3]], check=True)
        except subprocess.CalledProcessError as e:
            print(f"An error occurred: {e}")
