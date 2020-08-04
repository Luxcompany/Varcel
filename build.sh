#INTENDED TO BE RUN VIA BASH - USE CYGWIN TO RUN ON WINDOWS

INPUT_DIRECTORIES="./src"

COMPILER="gcc"
OPTIONS="-Wall -Wextra -g" #release should contain -s

#########################################################
INPUT=""
OUTPUT="./bin/$(basename $(pwd)).exe"

#recursive search                   Recurse if directory is folder          Give back name of file
rSearch() { for i in "$1"/*;do      if [ -d "$i" ]; then rSearch "$i";      elif [ -f "$i" ] && [ ${i: -2} == ".c" ]; then printf "$i ";fi      done }

for i in $INPUT_DIRECTORIES; do
    INPUT="$INPUT $(rSearch $i)"
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