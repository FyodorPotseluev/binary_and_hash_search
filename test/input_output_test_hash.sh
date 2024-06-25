#!/usr/bin/env bash

project=database_binary_and_hash_search
build_dir=./build
bin_dir=$build_dir/bin
executable=$bin_dir/$project

input=()
expected=()

input+=("test.bin add apple pear")
expected+=( "Error: Incorrect number of arguments" )

input+=("test.bin add string_which_consist_of_60_chars_which_surpas_the_limit_by_1")
expected+=( "Error: Too long entry name" )

input+=("test.bin query apple pear")
expected+=( "Error: Incorrect number of arguments" )

input+=("test.bin list add query")
expected+=( "Error: Incorrect number of arguments" )

input+=("test.bin")
expected+=( "Error: Incorrect number of arguments" )

input+=("test.bin badd apple")
expected+=( "Error: Incorrect command" )

input+=("test.bin add pear")
expected+=( "" )

input+=("test.bin add carrot")
expected+=( "" )

input+=("test.bin add pumpkin")
expected+=( "" )

input+=("test.bin add carrot")
expected+=( "" )

input+=("test.bin query pear")
expected+=( "pear - 1" )

input+=("test.bin add carrot")
expected+=( "" )

input+=("test.bin query carrot")
expected+=( "carrot - 3" )

input+=("test.bin query pumpkin")
expected+=( "pumpkin - 1" )

input+=("test.bin list")
expected+=( $'pear - 1\ncarrot - 3\npumpkin - 1' )

input+=("test.bin add cucumber")
expected+=( "" )

input+=("test.bin add tomato")
expected+=( "" )

input+=("test.bin add cabbage")
expected+=( "" )

input+=("test.bin add tomato")
expected+=( "" )

input+=("test.bin add pineapple")
expected+=( "" )

input+=("test.bin add tomato")
expected+=( "" )

input+=("test.bin add banana")
expected+=( "" )

input+=("test.bin query tomato")
expected+=( "tomato - 3" )

# Query non existing item
input+=("test.bin query mango")
expected+=( "mango - 0" )

input+=("test.bin list")
expected+=( $'pear - 1\ncarrot - 3\ntomato - 3\ncabbage - 1\nbanana - 1\npineapple - 1\ncucumber - 1\npumpkin - 1' )

input+=("test.bin add onion")
expected+=( "" )

input+=("test.bin list")
expected+=( $'cabbage - 1\ntomato - 3\npineapple - 1\npear - 1\npumpkin - 1\nbanana - 1\ncucumber - 1\ncarrot - 3\nonion - 1' )

input+=("test.bin add potato")
expected+=( "" )

# add maximum size entry name
input+=("test.bin add an_entry_which_consist_of_exactly_59_characters_long_string")
expected+=( "" )

input+=("test.bin add potato")
expected+=( "" )

input+=("test.bin add pumpkin")
expected+=( "" )

input+=("test.bin add potato")
expected+=( "" )

input+=("test.bin add an_entry_which_consist_of_exactly_59_characters_long_string")
expected+=( "" )

input+=("test.bin add kiwi")
expected+=( "" )

input+=("test.bin query potato")
expected+=( "potato - 3" )

input+=("test.bin query pumpkin")
expected+=( "pumpkin - 2" )

input+=("test.bin query an_entry_which_consist_of_exactly_59_characters_long_string")
expected+=( "an_entry_which_consist_of_exactly_59_characters_long_string - 2" )

input+=("test.bin list")
expected+=( $'cabbage - 1\ntomato - 3\npineapple - 1\npear - 1\npumpkin - 2\nbanana - 1\npotato - 3\ncucumber - 1\ncarrot - 3\nonion - 1\nan_entry_which_consist_of_exactly_59_characters_long_string - 2\nkiwi - 1' )

input+=("test.bin query string_which_consist_of_60_chars_which_surpas_the_limit_by_1")
expected+=( "Error: Too long entry name" )

input+=("test.bin add cherry")
expected+=( "" )

input+=("test.bin query kiwi")
expected+=( "kiwi - 1" )

input+=("test.bin query pineapple")
expected+=( "pineapple - 1" )

input+=("test.bin query cherry")
expected+=( "cherry - 1" )

input+=("test.bin list string_which_consist_of_60_chars_which_surpas_the_limit_by_1")
expected+=( $'cucumber - 1\nkiwi - 1\ncarrot - 3\npotato - 3\ntomato - 3\npineapple - 1\ncabbage - 1\nan_entry_which_consist_of_exactly_59_characters_long_string - 2\nbanana - 1\npear - 1\npumpkin - 2\ncherry - 1\nonion - 1' )

input+=("test.bin query")
expected+=( "Error: Item name has not been specified" )

i=0
arr_len=${#input[@]}
printf '\n'
for idx in "${!input[@]}"; do
    actual=$($executable ${input[idx]})
    if [[ "${expected[idx]}" != "$actual" ]]; then
        printf '%s\n\n' \
            '---------------------------------------------------------------'
        printf 'TEST "%b" FAILED: \n\nexpected: \n%b\n\ngot: \n%b\n\n' \
          "${input[idx]}" \
          "${expected[idx]}" \
          "$actual"
# wdiff is diff command analogue that shows the exact place with difference
        wdiff <(echo "${expected[idx]}" ) <(echo "$actual")
        printf '\n'
        printf '%s\n' \
            '---------------------------------------------------------------'
    else
        i=$((i+1))
    fi
done
rm test.bin
if [[ "$i" = "$arr_len" ]]; then
    echo OK
fi
