echo IGNORE ./clean.sh: line 13: test: too many arguments
echo "(script file looking for .c files and there are not any)"
echo

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

for f in hexdump.dat
do
  if ( test -f $f ) ; then 
    rm $f
  fi
done
