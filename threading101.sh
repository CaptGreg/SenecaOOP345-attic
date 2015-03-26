for i in threading101*.cpp
 do 
  echo
  echo $i:
  b=`basename $i .cpp`
  rm $b
  make $b && ./$b 
  echo $i: "done"
  echo
 done
