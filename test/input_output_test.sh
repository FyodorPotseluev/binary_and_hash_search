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

input+=("test.bin add pear")
expected+=( "" )

input+=("test.bin query pear")
expected+=( "pear - 2" )

input+=("test.bin query carrot")
expected+=( "carrot - 1" )

input+=("test.bin query pumpkin")
expected+=( "pumpkin - 1" )

input+=("test.bin list")
expected+=( $'carrot - 1\npear - 2\npumpkin - 1' )

input+=("test.bin add pear")
expected+=( "" )

input+=("test.bin add cucumber")
expected+=( "" )

input+=("test.bin add cabbage")
expected+=( "" )

input+=("test.bin add cucumber")
expected+=( "" )

input+=("test.bin add pineapple")
expected+=( "" )

input+=("test.bin add pineapple")
expected+=( "" )

input+=("test.bin add pumpkin")
expected+=( "" )

input+=("test.bin add tomato")
expected+=( "" )

input+=("test.bin query cucumber")
expected+=( "cucumber - 2" )

input+=("test.bin add banana")
expected+=( "" )

input+=("test.bin query pumpkin")
expected+=( "pumpkin - 2" )

input+=("test.bin query cabbage")
expected+=( "cabbage - 1" )

input+=("test.bin query pear")
expected+=( "pear - 3" )

input+=("test.bin query carrot")
expected+=( "carrot - 1" )

input+=("test.bin query tomato")
expected+=( "tomato - 1" )

# Query non existing item
input+=("test.bin query mango")
expected+=( "mango - 0" )

input+=("test.bin list")
expected+=( $'banana - 1\ncabbage - 1\ncarrot - 1\ncucumber - 2\npear - 3\npineapple - 2\npumpkin - 2\ntomato - 1' )

input+=("test.bin add onion")
expected+=( "" )

input+=("test.bin add pear")
expected+=( "" )

input+=("test.bin query pineapple")
expected+=( "pineapple - 2" )

input+=("test.bin add onion")
expected+=( "" )

input+=("test.bin add pineapple")
expected+=( "" )

input+=("test.bin add potato")
expected+=( "" )

# add maximum size entry name
input+=("test.bin add an_entry_which_consist_of_exactly_59_characters_long_string")
expected+=( "" )

input+=("test.bin query pineapple")
expected+=( "pineapple - 3" )

input+=("test.bin query an_entry_which_consist_of_exactly_59_characters_long_string")
expected+=( "an_entry_which_consist_of_exactly_59_characters_long_string - 1" );

input+=("test.bin query onion")
expected+=( "onion - 2" )

input+=("test.bin query pear")
expected+=( "pear - 4" )

input+=("test.bin query cucumber")
expected+=( "cucumber - 2" )

input+=("test.bin query cabbage")
expected+=( "cabbage - 1" )

input+=("test.bin query pumpkin")
expected+=( "pumpkin - 2" )

input+=("test.bin query potato")
expected+=( "potato - 1" )

input+=("test.bin query banana")
expected+=( "banana - 1" )

input+=("test.bin query tomato")
expected+=( "tomato - 1" )

input+=("test.bin query carrot")
expected+=( "carrot - 1" )

input+=("test.bin list")
expected+=( $'an_entry_which_consist_of_exactly_59_characters_long_string - 1\nbanana - 1\ncabbage - 1\ncarrot - 1\ncucumber - 2\nonion - 2\npear - 4\npineapple - 3\npotato - 1\npumpkin - 2\ntomato - 1' )

input+=("test.bin query string_which_consist_of_60_chars_which_surpas_the_limit_by_1")
expected+=( "Error: Too long entry name" )

input+=("test.bin list string_which_consist_of_60_chars_which_surpas_the_limit_by_1")
expected+=( $'an_entry_which_consist_of_exactly_59_characters_long_string - 1\nbanana - 1\ncabbage - 1\ncarrot - 1\ncucumber - 2\nonion - 2\npear - 4\npineapple - 3\npotato - 1\npumpkin - 2\ntomato - 1' )

input+=("test.bin add 51")
expected+=( "" )

input+=("test.bin add 72")
expected+=( "" )

input+=("test.bin add 15")
expected+=( "" )

input+=("test.bin add 36")
expected+=( "" )

input+=("test.bin add 68")
expected+=( "" )

input+=("test.bin add 10")
expected+=( "" )

input+=("test.bin add 56")
expected+=( "" )

input+=("test.bin add 76")
expected+=( "" )

input+=("test.bin add 45")
expected+=( "" )

input+=("test.bin add 57")
expected+=( "" )

input+=("test.bin add 94")
expected+=( "" )

input+=("test.bin add 96")
expected+=( "" )

input+=("test.bin add 74")
expected+=( "" )

input+=("test.bin add 16")
expected+=( "" )

input+=("test.bin add 90")
expected+=( "" )

input+=("test.bin add 36")
expected+=( "" )

input+=("test.bin add 83")
expected+=( "" )

input+=("test.bin add 15")
expected+=( "" )

input+=("test.bin add 62")
expected+=( "" )

input+=("test.bin add 90")
expected+=( "" )

input+=("test.bin add 31")
expected+=( "" )

input+=("test.bin add 55")
expected+=( "" )

input+=("test.bin add 91")
expected+=( "" )

input+=("test.bin add 18")
expected+=( "" )

input+=("test.bin add 94")
expected+=( "" )

input+=("test.bin add 17")
expected+=( "" )

input+=("test.bin add 70")
expected+=( "" )

input+=("test.bin add 79")
expected+=( "" )

input+=("test.bin add 56")
expected+=( "" )

input+=("test.bin add 90")
expected+=( "" )

input+=("test.bin query pear")
expected+=( "pear - 4" )

input+=("test.bin query cucumber")
expected+=( "cucumber - 2" )

input+=("test.bin query pumpkin")
expected+=( "pumpkin - 2" )

input+=("test.bin query potato")
expected+=( "potato - 1" )

input+=("test.bin query 90")
expected+=( "90 - 3" )

input+=("test.bin query 44")
expected+=( "44 - 0" )

input+=("test.bin query 15")
expected+=( "15 - 2" )

input+=("test.bin query 79")
expected+=( "79 - 1" )

input+=("test.bin query 56")
expected+=( "56 - 2" )

input+=("test.bin list")
expected+=( $'10 - 1\n15 - 2\n16 - 1\n17 - 1\n18 - 1\n31 - 1\n36 - 2\n45 - 1\n51 - 1\n55 - 1\n56 - 2\n57 - 1\n62 - 1\n68 - 1\n70 - 1\n72 - 1\n74 - 1\n76 - 1\n79 - 1\n83 - 1\n90 - 3\n91 - 1\n94 - 2\n96 - 1\nan_entry_which_consist_of_exactly_59_characters_long_string - 1\nbanana - 1\ncabbage - 1\ncarrot - 1\ncucumber - 2\nonion - 2\npear - 4\npineapple - 3\npotato - 1\npumpkin - 2\ntomato - 1' )

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
