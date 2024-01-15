import glob
import subprocess
import os
import csv
import sys
import re

NAME = 'SE_double'
TEST_DIR = 'tests/'
TEST_FILE = os.path.join(TEST_DIR, 'test-gen.csv')
RESULT_FILE = os.path.join(TEST_DIR, f'{NAME}.csv')

SCRIPT = f'./{NAME}'
TIMEOUT = 300
HYPERFINE_WARMUP = 0
RUNS = 5
HYPERFINE_FILE = os.path.join(TEST_DIR, 'hyperfine.csv')
VALGRIND_FILE = os.path.join(TEST_DIR, 'valgrind.csv')

def setup():
    if not os.path.exists(TEST_FILE):
        print("No test cases found.")
        sys.exit(1)

    with open(RESULT_FILE, 'w') as file:
        file.write('seed,lattice_type,dimension,bit_level,'
                   'lattice,expected_answer,actual_answer,'
                   'mean,stddev,median,user,system,min,max,'
                   'total_bytes,total_blocks,bytes_at_tgmax,'
                   'blocks_at_tgmax,bytes_at_tend,'
                   'blocks_at_tend,reads, writes\n')


def make(command: str):
    try:
        subprocess.run(['make', command], check=True)
    except subprocess.CalledProcessError as e:
        print(f"An error occurred: {e}")
        
def remove_dhat_files():
    for filename in glob.glob('dhat.out.*'):
        os.remove(filename)
        
def remove_hyperfine_file():
    if os.path.exists(HYPERFINE_FILE):
        os.remove(HYPERFINE_FILE)

def run_hyperfine(lattice: str):
    command = ['hyperfine',
               '--warmup', str(HYPERFINE_WARMUP),
               '--runs', str(RUNS),
               '-u', 'second',
               '--export-csv', HYPERFINE_FILE,
               f'{SCRIPT} {lattice}']

    try:
        subprocess.run(command, check=True, capture_output=True,
                       text=True, timeout=TIMEOUT)

        with open('result.txt', 'r') as result_file:
            answer = result_file.read()

        with open(HYPERFINE_FILE, 'r') as hyperfine_file:
            csv_reader = csv.reader(hyperfine_file)
            
            next(csv_reader)
            timings = list(next(csv_reader))[1:]
        
        remove_hyperfine_file()

        return [answer] + timings
    except subprocess.TimeoutExpired:
        print(f"Subprocess timed out after {TIMEOUT} seconds.")
        return ['TIMEOUT']
    except subprocess.CalledProcessError as e:
        print(f"An error occurred: {e}")
        return ['ERROR']
    
def run_valgrind(lattice: str):
    command = ['valgrind', '--tool=dhat', SCRIPT, *lattice.split()]

    try:
        valgrind = subprocess.run(command, capture_output=True,
                                  text=True, timeout=TIMEOUT)
        
        remove_dhat_files()
        
        pattern = re.compile(r"(\d{1,3}(?:,\d{3})*) (?:blocks|bytes)")

        if os.path.exists('dhat.out.*'):
            os.remove('dhat.out.*')

        # Extract the values
        values = pattern.findall(valgrind.stderr)
        for i, value in enumerate(values):
            values[i] = value.replace(',', '')

        return values
    except subprocess.TimeoutExpired:
        print(f"Subprocess timed out after {TIMEOUT} seconds.")
        return ['TIMEOUT']
    except subprocess.CalledProcessError as e:
        print(f"An error occurred: {e}")
        return ['ERROR']


def run(lattice: str):
    try:
        subprocess.run([SCRIPT, *lattice.split(' ')],
                       check=True, timeout=TIMEOUT)

        with open('result.txt', 'r') as result_file:
            return [result_file.read()]

    except subprocess.TimeoutExpired:
        print(f"Subprocess timed out after {TIMEOUT} seconds.")
        return ['TIMEOUT']
    except subprocess.CalledProcessError as e:
        print(f"An error occurred: {e}")
        return ['ERROR']


if __name__ == '__main__':
    setup()
    make('all')

    with open(TEST_FILE, newline='') as test_file, open(RESULT_FILE, 'a', newline='') as result_file:
        csv_reader = csv.reader(test_file)
        csv_writer = csv.writer(result_file)

        # Iterate over each test case
        for i, test_case in enumerate(csv_reader):
            timing_stats = run_hyperfine(test_case[4])
            memory_stats = run_valgrind(test_case[4])
            csv_writer.writerow(test_case + timing_stats + memory_stats)
            print(i)
            
    remove_hyperfine_file()
    remove_dhat_files()

    make('clean')
