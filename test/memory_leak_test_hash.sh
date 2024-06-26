#!/usr/bin/env bash

# valgrind exitcode can't match program error codes: this causes the test to
# react in some cases when program returns error exitcode as to a test failure
valgrind_command="valgrind --error-exitcode=42 --tool=memcheck --leak-check=full --errors-for-leak-kinds=definite,indirect,possible --show-leak-kinds=definite,indirect,possible"

project=database_binary_and_hash_search
build_dir=./build
bin_dir=$build_dir/bin
executable=$bin_dir/$project

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
    "test.bin add carrot"
    "test.bin query pear"
    "test.bin add carrot"
    "test.bin query carrot"
    "test.bin query pumpkin"
    "test.bin list"
    "test.bin add cucumber"
    "test.bin add tomato"
    "test.bin add cabbage"
    "test.bin add tomato"
    "test.bin add pineapple"
    "test.bin add tomato"
    "test.bin add banana"
    "test.bin query tomato"
    "test.bin query mango"
    "test.bin list"
    "test.bin add onion"
    "test.bin list"
    "test.bin add potato"
    "test.bin add an_entry_which_consist_of_exactly_59_characters_long_string"
    "test.bin add potato"
    "test.bin add pumpkin"
    "test.bin add potato"
    "test.bin add an_entry_which_consist_of_exactly_59_characters_long_string"
    "test.bin add kiwi"
    "test.bin query potato"
    "test.bin query pumpkin"
    "test.bin query an_entry_which_consist_of_exactly_59_characters_long_string"
    "test.bin list"
    "test.bin query string_which_consist_of_60_chars_which_surpas_the_limit_by_1"
    "test.bin add cherry"
    "test.bin query kiwi"
    "test.bin query pineapple"
    "test.bin query cherry"
    "test.bin list string_which_consist_of_60_chars_which_surpas_the_limit_by_1"
    "test.bin query"
    "merge file1 file2"
    "merge file1 file2 file3 file4"
    "merge file1 file2 file3"
    "dst.bin add pear"
    "dst.bin add carrot"
    "dst.bin add carrot"
    "dst.bin add cucumber"
    "dst.bin add cabbage"
    "dst.bin add pineapple"
    "dst.bin add tomato"
    "dst.bin list"
    "src.bin add tomato"
    "src.bin add banana"
    "src.bin add onion"
    "src.bin add potato"
    "src.bin add an_entry_which_consist_of_exactly_59_characters_long_string"
    "src.bin add kiwi"
    "src.bin add cherry"
    "src.bin add pumpkin"
    "src.bin list"
    "merge dst.bin src.bin res.bin"
    "res.bin list"
    "src.bin add g"
    "merge src.bin res.bin new_res.bin"
    "new_res.bin list"
    "src.bin add pear"
    "src.bin add carrot"
    "src.bin add pumpkin"
    "src.bin add carrot"
    "src.bin add cucumber"
    "src.bin add cabbage"
    "src.bin add pineapple"
    "src.bin add tomato"
    "src.bin add banana"
    "src.bin add onion"
    "src.bin add potato"
    "src.bin list"
    "dst.bin add an_entry_which_consist_of_exactly_59_characters_long_string"
    "dst.bin add kiwi"
    "dst.bin add cherry"
    "dst.bin add g"
    "dst.bin list"
    "merge src.bin dst.bin final_res.bin"
    "final_res.bin list"
)

i=0
fail_test_flag=0
arr_len=${#input[@]}
for arg in "${input[@]}"; do
    output=$($valgrind_command $executable $arg 2>&1)
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
rm new_res.bin
rm final_res.bin
if [[ "$fail_test_flag" = 0 ]]; then
    echo OK
fi
