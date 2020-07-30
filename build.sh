#INTENDED TO BE RUN VIA BASH - USE CYGWIN TO RUN ON WINDOWS

INPUT_DIRECTORIES="./src"

COMPILER="gcc"
OPTIONS="-Wall -Wextra -g" #release should contain -s

#########################################################
INPUT=""
OUTPUT="./bin/$(basename $(pwd)).exe"

#recursive search                   Recurse if directory is folder          Give back name of file
rSearch() { for i in "$1"/*;do      if [ -d "$i" ]; then rSearch "$i";      elif [ -f "$i" ]; then printf "$i ";fi      done }

for i in $INPUT_DIRECTORIES; do
    INPUT="$INPUT $(rSearch $i)"
done

########################################################
COMPILE_COMMAND=$(echo "$COMPILER $OPTIONS $INPUT -o $OUTPUT" | xargs)

PURPLE='\033[0;35m'
NC='\033[0m' # No Color

echo -e "$PURPLE--------BUILDING--------$NC"
eval $COMPILE_COMMAND
echo -e "$PURPLE----------DONE----------$NC"