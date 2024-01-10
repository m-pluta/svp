import subprocess
import random
import os

# Constants
TEST_DIR = 'tests/'
TEST_FILE = os.path.join(TEST_DIR, 'test-gen.csv')
BASH_SCRIPT = os.path.join(TEST_DIR, 'test-gen.sh')

# Reset the csv
def reset_output_csv():
    if os.path.exists(TEST_FILE):
        os.remove(TEST_FILE)

# Generate tuples for all the test-cases
def generate_tests():
    tests = []

    for dim in range(1, 51):
        for bit_level in (32,):
            for _ in range(30 if dim <= 20 else 10):
                tests.append((str(random.getrandbits(32)), 'u', str(dim), str(bit_level)))

    for dim in range(2, 51):
        for bit_level in (32,):
            for _ in range(30 if dim <= 20 else 10 if dim <= 40 else 5):
                tests.append((str(random.getrandbits(32)), 'r', str(dim), str(bit_level)))

    return tests


if __name__ == '__main__':
    reset_output_csv()
    tests = generate_tests()

    assert len(tests) == len(set(tests)), "Duplicate tests were generated."

    # random.shuffle(tests)

    # Iterate through each tuple and run the bash script to generate that test-case
    for t in tests:
        try:
            print(f"Generating {'_'.join(list(t)[1:])}")
            subprocess.run(['bash', BASH_SCRIPT, *t], check=True)
        except subprocess.CalledProcessError as e:
            print(f"An error occurred: {e}")
