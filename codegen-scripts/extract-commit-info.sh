# run this script from build/ subdirectory, so vmVersionInfo.h will be placed in build dir

URL=`git config --get remote.origin.url`
COMMIT=`git show HEAD --pretty="Commit: %H Date: %ci By: %cn <%cE>" | head -n 1`

echo "#define REVISION_STRING \"$URL $COMMIT\"" > vmVersionInfo.h 

