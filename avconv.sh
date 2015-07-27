# defaults
FOLDER=frames
FPS=30
VBR=2400
QSCALE=2

if test $# -ge 1; then
  echo arg 1: $1
  FOLDER=$1
fi

if ! test -d $FOLDER ; then
  echo usage $0 folder fps bit-rate qscale
  echo example $0 2012-09-12 24 2400 2
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

if test $# -ge 4; then
  echo arg 4: $4
  QSCALE=$4
fi


echo only works if file name numbers start at 1
echo folder=$FOLDER fps=$FPS vbitrate=$VBR qscale=${QSCALE}
#
# avconv -r 10 -b 1800 -i %03d.JPG test1800.mp4
echo avconv -i ${FOLDER}/f%06d.png -r ${FPS} -qscale ${QSCALE} -b ${VBR}k  ${FOLDER}-avconv-fps${FPS}-vbr${VBR}-qscale${QSCALE}.mp4
avconv -i ${FOLDER}/f%06d.png -r ${FPS} -qscale ${QSCALE} -b ${VBR}k  ${FOLDER}-avconv-fps${FPS}-vbr${VBR}-qscale${QSCALE}.mp4
#
