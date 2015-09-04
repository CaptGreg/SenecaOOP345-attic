# convert complains if executed form this script
# pipe script into bash 

SRC_DIR="what-dir?"
SRC_DIR="frames"
if test $# -ge 1; then
  SRC_DIR=$1
fi

geometry=1920x1080
geometry=320x240
if test $# -ge 2; then
  geometry=$2
fi

if ! test -d $SRC_DIR ; then
  echo convert folder .JPG files to $geometry resolution
  echo usage: $0 folder
  echo example: $0 2012-09-12
  exit
fi

DST_DIR=${SRC_DIR}-$geometry

if ! test -d  ${DST_DIR} ; then
  echo mkdir ${DST_DIR}
  mkdir ${DST_DIR}
fi

for f in ${SRC_DIR}/*.png
  do
    if test -f $f ; then
      b=`basename $f .png`
      # 'geometry', filter
      # 'scale', no filter
      echo  convert -filter Lanczos -set filter:support 7 -resize $geometry -extent $geometry $f ${DST_DIR}/$b.png
      convert -filter Lanczos -set filter:support 7 -resize $geometry -extent $geometry $f ${DST_DIR}/$b.png
    fi
  done
