#!/usr/bin/env bash

# valgrind exitcode can't match program error codes: this causes the test to
# react in some cases when program returns error exitcode as to a test failure
valgrind_command="valgrind --error-exitcode=42 --tool=memcheck --leak-check=full --errors-for-leak-kinds=definite,indirect,possible --show-leak-kinds=definite,indirect,possible"

input=(
    "test.bin add apple pear"
    "test.bin add this_is_too_long_string_that_surpasses_the_60_characters_limit_and_causes_Too_Long_ID_Error"
    "test.bin query apple pear"
    "test.bin list add query"
    "test.bin"
    "test.bin badd apple"
    "test.bin add apple"
    "test.bin add pear"
    "test.bin add tomato"
    "test.bin add pear"
    "test.bin query this_is_too_long_string_that_surpasses_the_60_characters_limit_and_causes_Too_Long_ID_Error"
    "test.bin query apple"
    "test.bin query pear"
    "test.bin query tomato"
    "test.bin query pineapple"
    "test.bin list this_is_too_long_string_that_surpasses_the_60_characters_limit_and_causes_Too_Long_ID_Error"
    "test.bin list"
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
