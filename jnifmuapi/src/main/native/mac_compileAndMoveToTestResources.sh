#!/bin/bash

case "$(uname -s)" in

   Darwin)
     echo 'Running on Darwin'
     ;;

   *)
     echo 'This script is meant for Darwin. Cannot continue'
     exit 1
     ;;
esac

current_dir=$(pwd)
# https://stackoverflow.com/questions/59895/how-can-i-get-the-source-directory-of-a-bash-script-from-within-the-script-itsel/246128#246128
script_dir="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"


compile_and_move () {
  cmake -DCMAKE_BUILD_TYPE=Debug  . && make && cp libfmuapi.dylib ../../../src/test/resources/lib/Mac-x86_64
}

if [ "$current_dir" == "$script_dir" ]; then
    compile_and_move
else
    echo "Changing directory to $script_dir"
    cd $script_dir
    compile_and_move
    echo "Changing directory back to $current_dir"
    cd $current_dir
fi