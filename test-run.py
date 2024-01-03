import subprocess
import os
import csv
import itertools
import sys
import re

TEST_FILE = 'test-gen.csv'
RESULT_FILE = 'test_result.csv'
TIMEOUT = 60
HYPERFINE_WARMUP = '1000'

def write_header():
    with open(f'test_result.csv', 'w') as file:
        file.write('seed,lattice_type,dimension,bit_level,'
                   'lattice,expected_answer,actual_answer,'
                   'mean,sd,user,system,min,max,runs\n')

def check_tests_exist():
    if not os.path.exists(TEST_FILE):
        print("No test cases found.")
        sys.exit(1)

def make(command):
    try:
        subprocess.run(['make', command], check=True)
    except subprocess.CalledProcessError as e:
        print(f"An error occurred: {e}")

def extract_hyperfine(stdout):
    lines = stdout.strip().split('\n')

    if len(lines) >= 3:
        decimals = re.findall(r'[\d.]+ (?:\w|run)?s', lines[1] + lines[2])

        for i, decimal in enumerate(decimals):
            value = float(re.match(r'([\d.]+)', decimal).group())
            if 'µs' in decimal:
                decimals[i] = f'{round(value / 1_000_000, 7):.7f}'
            elif 'ms' in decimal:
                decimals[i] = str(round(value / 1_000, 4))
            elif 'runs' in decimal:
                decimals[i] = str(int(value))
        
        return decimals

def run_hyperfine(lattice):
    try:
        result = subprocess.run(['hyperfine', '--warmup', HYPERFINE_WARMUP, '-N', f'./runme {lattice}'], check=True, capture_output=True, text=True, timeout=TIMEOUT)
        with open('result.txt', 'r') as result_file:
            program_result = result_file.read()

        return (program_result, extract_hyperfine(result.stdout))
    except subprocess.TimeoutExpired:
        print(f"Subprocess timed out after {TIMEOUT} seconds.")
    except subprocess.CalledProcessError as e:
        print(f"An error occurred: {e}")

def run(lattice):
    try:
        subprocess.run(['./runme', *lattice.split(' ')], check=True, timeout=TIMEOUT)
        with open('result.txt', 'r') as result_file:
            program_result = result_file.read()



        return program_result
    except subprocess.TimeoutExpired:
        print(f"Subprocess timed out after {TIMEOUT} seconds.")
        return '-1.0'
    except subprocess.CalledProcessError as e:
        print(f"An error occurred: {e}")
        return '-2.0'


if __name__ == '__main__':
    check_tests_exist()
    make('all')
    write_header()

    # Open the file
    with open(TEST_FILE, newline='') as csvfile:
        # Create a CSV reader object
        csvreader = csv.reader(csvfile)

        # Iterate over each row in the CSV file
        for test_case in csvreader:
            # answer, hyperfine_stats = run_hyperfine(test_case[4])
            answer = run(test_case[4])

            # data = list(test_case) + [answer] + hyperfine_stats
            data = list(test_case) + [answer]

            with open(RESULT_FILE, 'a') as file:
                file.write(','.join(data) + "\n")

    make('clean')
