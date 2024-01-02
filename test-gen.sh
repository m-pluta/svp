#!/bin/bash

# Check if three arguments are provided
if [ $# -ne 4 ]; then
    echo "Usage: $0 [seed] [lattice-type] [dimension] [bit level]"
    exit 1
fi

# Function to calculate L2 norm
calculate_l2_norm() {
    local vector_string=$1
    # Replace brackets and split the string into an array
    local vector=(${vector_string//[\[\],]/})
    local sum=""

    for element in "${vector[@]}"; do
        sum+="($element)^2+"
    done

    # Remove the trailing '+'
    sum=${sum%+}

    # Calculate the square root of the sum of squares
    echo "sqrt($sum)" | bc -l
}


OUTPUT_PATH='test-gen.svp'
seed=$1
type=$2
dimension=$3
bit_level=$4
filename=$seed","$type","$dimension","$bit_level

# Generate uniform basis test case
gen_uniform() {
    local dimension=$1
    local bit_level=$2

    # Check if dimension is less than 1
    if [ $dimension -lt 1 ]; then
        echo "Invalid dimension: $dimension. Dimension must be greater than or equal to 1 for uniform"
        exit 1
    fi

    # Temp file
    touch $OUTPUT_PATH temp.txt

    # Generate the lattice
    lattice=$(latticegen -randseed $seed u $dimension $bit_level)
    echo $lattice > temp.txt

    # Format lattice according to coursework spec
    formatted_lattice=$(echo "$lattice" | tr -d '\n' | sed 's/^\[\(.*\)\]$/\1/' | sed 's/\]\[/\] \[/g')

    # Use fplll to find shortest vector an norm
    shortest_vector=$(fplll -a svp temp.txt)
    l2_norm=$(calculate_l2_norm "$shortest_vector")

    # Output test case to file
    echo $filename","$formatted_lattice","$l2_norm >> $OUTPUT_PATH
    rm temp.txt
}

# Generate knapsack-like basis test case
gen_knapsack() {
    # Check if dimension is less than 2
    if [ $dimension -lt 2 ]; then
        echo "Invalid dimension: $dimension. Dimension must be greater than or equal to 2 for knapsack"
        exit 1
    fi

    # Knapsack generation is weird and generates an n by n+1 matrix so 1D not possible, therefore adjust
    local dimension=$(($1-1))
    local bit_level=$2


    # Temp file
    touch $OUTPUT_PATH temp.txt

    # Generate most of the lattice
    lattice=$(latticegen -randseed $seed r $dimension $bit_level | sed 's/\[\[/[/g' | sed 's/\]\]/]/g')

    # Generate missing row in lattice
    seed_plus_one=$((seed+1))
    vector=$(latticegen -randseed $seed_plus_one r $dimension $bit_level | grep -o -E '^\[\[([0-9]+)' | grep -o -E '[0-9]+')
    for (( i=0; i<$dimension; i++ )); do
        vector+=" 0"
    done

    # Add it to the lattice
    lattice="[$lattice [$vector]]"
    echo $lattice > temp.txt

    # Format lattice according to coursework spec
    formatted_lattice=$(echo "$lattice" | tr -d '\n' | sed 's/^\[\(.*\)\]$/\1/' | sed 's/\]\[/\] \[/g')

    # Use fplll to find shortest vector an norm
    shortest_vector=$(fplll -a svp temp.txt)
    l2_norm=$(calculate_l2_norm "$shortest_vector")

    # Output test case to file
    echo $filename","$formatted_lattice","$l2_norm >> $OUTPUT_PATH
    rm temp.txt
}

# Main execution
case "$type" in
    u)
        gen_uniform "$dimension" "$bit_level"
        ;;
    r)
        gen_knapsack "$dimension" "$bit_level"
        ;;
    *)
        echo "Invalid type: $type. Please use 'u' or 'r'."
        exit 1
        ;;
esac
