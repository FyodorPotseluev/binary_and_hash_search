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

# Add maximum size entry name
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

# "merge" command tests

input+=("merge file1 file2")
expected+=( "Error: Incorrect number of arguments" )

input+=("merge file1 file2 file3 file4")
expected+=( "Error: Incorrect number of arguments" )

input+=("merge file1 file2 file3")
expected+=( "file1: No such file or directory" )

input+=("dst.bin add pear")
expected+=( "" )

input+=("dst.bin add carrot")
expected+=( "" )

input+=("dst.bin add pumpkin")
expected+=( "" )

input+=("dst.bin add carrot")
expected+=( "" )

input+=("dst.bin add cucumber")
expected+=( "" )

input+=("dst.bin add cabbage")
expected+=( "" )

input+=("dst.bin add pineapple")
expected+=( "" )

input+=("dst.bin add tomato")
expected+=( "" )

input+=("dst.bin list")
expected+=( $'pear - 1\ncarrot - 2\ncabbage - 1\ntomato - 1\npineapple - 1\ncucumber - 1\npumpkin - 1' )

input+=("src.bin add tomato")
expected+=( "" )

input+=("src.bin add banana")
expected+=( "" )

input+=("src.bin add onion")
expected+=( "" )

input+=("src.bin add potato")
expected+=( "" )

input+=("src.bin add an_entry_which_consist_of_exactly_59_characters_long_string")
expected+=( "" )

input+=("src.bin add kiwi")
expected+=( "" )

# input+=("src.bin list")
# expected+=( $'tomato - 1\nbanana - 1\nan_entry_which_consist_of_exactly_59_characters_long_string - 1\nkiwi - 1\nonion - 1\npotato - 1' )

# input+=("merge dst.bin src.bin res.bin")
# expected+=( "" )

# input+=("res.bin list")
# expected+=( $'cabbage - 1\ntomato - 2\npineapple - 1\npear - 1\npumpkin - 1\nbanana - 1\npotato - 1\ncucumber - 1\ncarrot - 2\nan_entry_which_consist_of_exactly_59_characters_long_string - 1\nonion - 1\nkiwi - 1' )

input+=("src.bin add cherry")
expected+=( "" )

input+=("src.bin add pumpkin")
expected+=( "" )

input+=("src.bin list")
expected+=( $'tomato - 1\nbanana - 1\nan_entry_which_consist_of_exactly_59_characters_long_string - 1\nkiwi - 1\nonion - 1\npotato - 1\ncherry - 1\npumpkin - 1' )

input+=("merge dst.bin src.bin res.bin")
expected+=( "" )

input+=("res.bin list")
expected+=( $'cucumber - 1\nkiwi - 1\ncarrot - 2\npotato - 1\ntomato - 2\npineapple - 1\ncabbage - 1\nan_entry_which_consist_of_exactly_59_characters_long_string - 1\nbanana - 1\npear - 1\npumpkin - 2\ncherry - 1\nonion - 1' )

input+=("src.bin add g")
expected+=( "" )

input+=("merge src.bin res.bin new_res.bin")
expected+=( "" )

input+=("new_res.bin list")
expected+=( $'cucumber - 1\nkiwi - 1\ncarrot - 2\npotato - 1\ntomato - 2\npineapple - 1\ncabbage - 1\nan_entry_which_consist_of_exactly_59_characters_long_string - 1\nbanana - 1\npear - 1\npumpkin - 2\ncherry - 1\ng - 1\nonion - 1' )

input+=("src.bin add pear")
expected+=( "" )

input+=("src.bin add carrot")
expected+=( "" )

input+=("src.bin add pumpkin")
expected+=( "" )

input+=("src.bin add carrot")
expected+=( "" )

input+=("src.bin add cucumber")
expected+=( "" )

input+=("src.bin add cabbage")
expected+=( "" )

input+=("src.bin add pineapple")
expected+=( "" )

input+=("src.bin add tomato")
expected+=( "" )

input+=("src.bin add banana")
expected+=( "" )

input+=("src.bin add onion")
expected+=( "" )

input+=("src.bin add potato")
expected+=( "" )

input+=("src.bin list")
expected+=( $'cabbage - 1\ntomato - 1\npineapple - 1\npear - 1\npumpkin - 1\nbanana - 1\npotato - 1\ncucumber - 1\ncarrot - 2\nonion - 1' )

input+=("dst.bin add an_entry_which_consist_of_exactly_59_characters_long_string")
expected+=( "" )

input+=("dst.bin add kiwi")
expected+=( "" )

input+=("dst.bin add cherry")
expected+=( "" )

input+=("dst.bin add g")
expected+=( "" )

input+=("dst.bin list")
expected+=( $'an_entry_which_consist_of_exactly_59_characters_long_string - 1\nkiwi - 1\ng - 1\ncherry - 1' )

input+=("merge src.bin dst.bin final_res.bin")
expected+=( "" )

input+=("final_res.bin list")
expected+=( $'cucumber - 1\nkiwi - 1\ncarrot - 2\npotato - 1\ntomato - 1\npineapple - 1\ncabbage - 1\nan_entry_which_consist_of_exactly_59_characters_long_string - 1\nbanana - 1\npear - 1\npumpkin - 1\ng - 1\ncherry - 1\nonion - 1' )

i=0
arr_len=${#input[@]}
printf '\n'
for idx in "${!input[@]}"; do
    actual=$($executable ${input[idx]} 2>&1)
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
rm new_res.bin
rm final_res.bin
if [[ "$i" = "$arr_len" ]]; then
    echo OK
fi
