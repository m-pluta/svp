import subprocess
import os
import csv
import sys
import re

TEST_DIR = 'tests/'
TEST_FILE = os.path.join(TEST_DIR, 'test-gen.csv')
RESULT_FILE = os.path.join(TEST_DIR, 'test-result.csv')
TIMEOUT = 60
HYPERFINE_WARMUP = '1000'

def setup():
    if not os.path.exists(TEST_FILE):
        print("No test cases found.")
        sys.exit(1)

    with open(RESULT_FILE, 'w') as file:
        file.write('seed,lattice_type,dimension,bit_level,'
                   'lattice,expected_answer,actual_answer,'
                   'mean,sd,user,system,min,max,runs\n')

def make(command: str):
    try:
        subprocess.run(['make', command], check=True)
    except subprocess.CalledProcessError as e:
        print(f"An error occurred: {e}")

def extract_hyperfine(stdout: str):
    lines = stdout.strip().split('\n')

    if len(lines) >= 3:
        time_values = re.findall(r'[\d.]+ (?:\w|run)?s', lines[1] + lines[2])

        for i, time_value in enumerate(time_values):
            value = float(re.match(r'([\d.]+)', time_value).group())
            if 'µs' in time_value:
                time_values[i] = f'{round(value / 1_000_000, 7):.7f}'
            elif 'ms' in time_value:
                time_values[i] = str(round(value / 1_000, 4))
            elif 'runs' in time_value:
                time_values[i] = str(int(value))
        
        return time_values

def run_hyperfine(lattice: str):
    try:
        hyperfine = subprocess.run(['hyperfine', '--warmup', HYPERFINE_WARMUP, '-N', f'./runme {lattice}'], check=True, capture_output=True, text=True, timeout=TIMEOUT)
        with open('result.txt', 'r') as result_file:
            answer = result_file.read()

        return (answer, extract_hyperfine(hyperfine.stdout))
    except subprocess.TimeoutExpired:
        print(f"Subprocess timed out after {TIMEOUT} seconds.")
        return ('-1.0', [])
    except subprocess.CalledProcessError as e:
        print(f"An error occurred: {e}")
        return ('-2.0', [])

def run(lattice: str):
    try:
        subprocess.run(['./runme', *lattice.split(' ')], check=True, timeout=TIMEOUT)
        with open('result.txt', 'r') as result_file:
            return result_file.read()
    except subprocess.TimeoutExpired:
        print(f"Subprocess timed out after {TIMEOUT} seconds.")
        return '-1.0'
    except subprocess.CalledProcessError as e:
        print(f"An error occurred: {e}")
        return '-2.0'
if __name__ == '__main__':
    setup()
    make('all')

    with open(TEST_FILE, newline='') as test_file, open(RESULT_FILE, 'a', newline='') as result_file:
        csv_reader = csv.reader(test_file)
        csv_writer = csv.writer(result_file)

        # Iterate over each test case
        for i, test_case in enumerate(csv_reader):
            # answer, hyperfine_stats = run_hyperfine(test_case[4])
            answer = run(test_case[4])

            # data = list(test_case) + [answer] + hyperfine_stats
            data = list(test_case) + [answer]

            csv_writer.writerow(data)
            print(i)

    make('clean')
