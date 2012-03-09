#!/bin/bash

PREBUILT_IMAGE_URL="https://ci.lille.inria.fr/pharo/view/VM-dev/job/Cog%20Git%20Tracker%20(cog-osx)/lastSuccessfulBuild/artifact/vmmaker-image.zip"

VERSION="Pharo-1.4"

NO_COLOR='\033[0m' #disable any colors
YELLOW='\033[0;33m'
# ----------------------------------------------------------------------------

openImage() {
    IMAGE=$1
    SCRIPT=$2

    echo -e $YELLOW 'OPENING IMAGE' $NO_COLOR
    echo "    $IMAGE"
    echo "    $SCRIPT"
   
    for vm in CogVM cog pharo squeak StackVM stackVM; do
        hash $vm 2>&-      && echo "using " `which ${vm}` \
            && $vm  "$IMAGE" "$SCRIPT"        && exit 0
    done  
   
    hash open 2>&-       && echo "using " `which open` \
        && open "$IMAGE" --args "$SCRIPT"   && exit 0
    
    hash gnome-open 2>&- && echo "using " `which gnome-open` \
        && gnome-open "$IMAGE"              && exit 0
}

# ----------------------------------------------------------------------------
echo -e $YELLOW "LOADING PREBUILT IMAGE" $NO_COLOR
echo "    $PREBUILT_IMAGE_URL"

curl "$PREBUILT_IMAGE_URL" -o "image.zip" && \
unzip image.zip && \ 
rm image.zip  && \
openImage "$PWD/generator.image" && exit 1


# ----------------------------------------------------------------------------
echo -e $YELLOW "FETCHING FRESH IMAGE" $NO_COLOR
echo "   $URL$VERSION.zip"

# using curl since that's installed by default on mac os x :/
curl "$URL$VERSION.zip" -o "image.zip"

# ----------------------------------------------------------------------------
echo -e $YELLOW "UNZIPPING" $NO_COLOR
echo "    $PWD/image.zip"

unzip image.zip && \
rm image.zip    && \
mv $VERSION/*  .   && \
rm -rf $VERSION    || exit 1


# ----------------------------------------------------------------------------
# try to open the image...
openImage "$PWD/$VERSION.image" "$PWD/ImageConfiguration.st"

