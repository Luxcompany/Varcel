#INTENDED TO BE RUN VIA BASH - USE CYGWIN TO RUN ON WINDOWS

INPUT_DIRECTORIES="./src"

COMPILER="g++"
OPTIONS="-Wall -Wextra -g -std=c++17 -DVARCEL_LOG_SHOW_DEBUG_" #release should contain -s

#########################################################
OUTPUT="./bin/$(basename $(pwd)).exe"

INPUT=""
#recursive search files                 Recurse if directory is folder              Give back name of directory if it's a c file
rSearchFile() { for i in "$1"/*;do      if [ -d "$i" ]; then rSearchFile "$i";      elif [ -f "$i" ] && [ ${i: -4} == ".cpp" ]; then printf "$i ";fi      done }
for i in $INPUT_DIRECTORIES; do
    INPUT="$INPUT $(rSearchFile $i)"
done

INCLUDES=""
for i in $INPUT_DIRECTORIES;do
    INCLUDES="$INCLUDES -I$i"
done

########################################################
COMPILE_COMMAND=$(echo "$COMPILER $OPTIONS $INCLUDES $INPUT -o $OUTPUT" | xargs)

PURPLE='\033[0;35m'
NC='\033[0m' # No Color

echo -e "$PURPLE--------BUILDING--------$NC"
printf "$COMPILE_COMMAND\n\n"
eval $COMPILE_COMMAND
echo -e "$PURPLE----------DONE----------$NC"