#!/usr/bin/env bash

# valgrind exitcode can't match program error codes: this causes the test to
# react in some cases when program returns error exitcode as to a test failure
valgrind_command="valgrind --error-exitcode=42 --tool=memcheck --leak-check=full --errors-for-leak-kinds=definite,indirect,possible --show-leak-kinds=definite,indirect,possible"

input=(
    "test.bin add apple pear"
    "test.bin add string_which_consist_of_60_chars_which_surpas_the_limit_by_1"
    "test.bin query apple pear"
    "test.bin list add query"
    "test.bin"
    "test.bin badd apple"
    "test.bin add pear"
    "test.bin add carrot"
    "test.bin add pumpkin"
    "test.bin add pear"
    "test.bin query pear"
    "test.bin query carrot"
    "test.bin query pumpkin"
    "test.bin list"
    "test.bin add pear"
    "test.bin add cucumber"
    "test.bin add cabbage"
    "test.bin add cucumber"
    "test.bin add pineapple"
    "test.bin add pineapple"
    "test.bin add pumpkin"
    "test.bin add tomato"
    "test.bin query cucumber"
    "test.bin add banana"
    "test.bin query pumpkin"
    "test.bin query cabbage"
    "test.bin query pear"
    "test.bin query carrot"
    "test.bin query tomato"
    "test.bin query mango"
    "test.bin list"
    "test.bin add onion"
    "test.bin add pear"
    "test.bin query pineapple"
    "test.bin add onion"
    "test.bin add pineapple"
    "test.bin add potato"
    "test.bin add an_entry_which_consist_of_exactly_59_characters_long_string"
    "test.bin query pineapple"
    "test.bin query apple"
    "test.bin query onion"
    "test.bin query pear"
    "test.bin query cucumber"
    "test.bin query cabbage"
    "test.bin query pumpkin"
    "test.bin query potato"
    "test.bin query banana"
    "test.bin query tomato"
    "test.bin query carrot"
    "test.bin list"
    "test.bin query string_which_consist_of_60_chars_which_surpas_the_limit_by_1"
    "test.bin list string_which_consist_of_60_chars_which_surpas_the_limit_by_1"
    "test.bin query"
)

i=0
fail_test_flag=0
arr_len=${#input[@]}
for arg in "${input[@]}"; do
    output=$($valgrind_command ./database_binary_and_hash_search $arg 2>&1)
    if [ $? -eq 42 ]; then
        printf '%s\n' \
            '------------------------------------------------------------------------------'
        printf '%s\n' "$arg"
        printf '%s\n' \
            '------------------------------------------------------------------------------'
        echo "$output"
        printf '%s\n' \
            '------------------------------------------------------------------------------'
        fail_test_flag=1
    fi
    i=$((i+1))
    printf '[%d/%d]\n' "$i" "$arr_len"
done
rm test.bin
if [[ "$fail_test_flag" = 0 ]]; then
    echo OK
fi
