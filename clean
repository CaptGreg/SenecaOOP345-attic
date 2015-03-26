for f in *.cpp  *.c
do
  # http://stackoverflow.com/questions/965053/extract-filename-and-extension-in-bash
  filename=$(basename "$f")
  # extension="${filename##*.}"
  rootname="${filename%.*}"

  # echo $f:  $rootname
  if test -f $rootname ; then 
    rm $rootname
  fi
done

