for i in threading301*.cpp
 do 
  echo
  echo $i:
  b=`basename $i .cpp`
  make $b && ./$b 
  echo $i: "done"
  echo
 done
