for f in *.cpp *.c
do
  if grep main $f >/dev/null ; then
    # http://stackoverflow.com/questions/965053/extract-filename-and-extension-in-bash
    filename=$(basename "$f")
    # extension="${filename##*.}"
    rootname="${filename%.*}"

    # echo $f  make $rootname
    make $rootname
  # else
    # echo no main in $f 
  fi
done

echo
echo "IGNORE grep: *.c: No such file or directory"
echo "(script file looking for .c files and there are not any)"
echo
