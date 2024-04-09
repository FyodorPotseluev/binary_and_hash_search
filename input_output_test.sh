#!/usr/bin/env bash

input=()
expected=()

input+=("test.bin add apple pear")
expected+=( "Error: Incorrect number of arguments" )

input+=("test.bin add this_is_too_long_string_that_surpasses_the_60_characters_limit_and_causes_Too_Long_ID_Error")
expected+=( "Error: Too long entry name" )

input+=("test.bin query apple pear")
expected+=( "Error: Incorrect number of arguments" )

input+=("test.bin list add query")
expected+=( "Error: Incorrect number of arguments" )

input+=("test.bin")
expected+=( "Error: Incorrect number of arguments" )

input+=("test.bin badd apple")
expected+=( "Error: Incorrect command" )

input+=("test.bin add apple")
expected+=( "" )

input+=("test.bin add pear")
expected+=( "" )

input+=("test.bin add tomato")
expected+=( "" )

input+=("test.bin add pear")
expected+=( "" )

input+=("test.bin query this_is_too_long_string_that_surpasses_the_60_characters_limit_and_causes_Too_Long_ID_Error")
expected+=( "Error: Too long entry name" )

input+=("test.bin query apple")
expected+=( "apple - 1" )

input+=("test.bin query pear")
expected+=( "pear - 2" )

input+=("test.bin query tomato")
expected+=( "tomato - 1" )

input+=("test.bin query pineapple")
expected+=( "pineapple - 0" )

input+=("test.bin list this_is_too_long_string_that_surpasses_the_60_characters_limit_and_causes_Too_Long_ID_Error")
expected+=( $'apple - 1\npear - 2\ntomato - 1' )

input+=("test.bin list")
expected+=( $'apple - 1\npear - 2\ntomato - 1' )

input+=("test.bin query")
expected+=( "Error: Item name has not been specified" )

i=0
arr_len=${#input[@]}
printf '\n'
for idx in "${!input[@]}"; do
    actual=$(../database_binary_and_hash_search ${input[idx]})
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
