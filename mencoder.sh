# defaults
FOLDER=frames
FPS=24
VBR=2400

if test $# -ge 1; then
  FOLDER=$1
fi

if ! test -d $FOLDER ; then
  echo usage $0 folder fps bit-rate
  echo example $0 2012-09-12 $FPS $VBR
  exit
fi

if test $# -ge 2; then
  echo arg 2: $2
  FPS=$2
fi

if test $# -ge 3; then
  echo arg 3: $3
  VBR=$3
fi

echo folder=$FOLDER fps=$FPS vbitrate=$VBR
echo mencoder mf://$FOLDER/*.png -mf fps=$FPS -o $FOLDER-mencode-fps$FPS-VBR$VBR.avi -ovc lavc -lavcopts vcodec=msmpeg4v2:vbitrate=$VBR
mencoder mf://$FOLDER/*.png -mf fps=$FPS -o $FOLDER-mencode-fps$FPS-VBR$VBR.avi -ovc lavc -lavcopts vcodec=msmpeg4v2:vbitrate=$VBR
