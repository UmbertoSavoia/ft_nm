SO=/usr/lib/wsl/drivers/nvdm.inf_amd64_aced13c2084a2857/libnvidia-ml_loader.so
OBJ=./src/main.o
BIN1=./test/test1.out
BIN2=./test/test2.out
BIN3=./test/test1_32.out
BIN4=./test/test2_32.out

FILE_TEST1=./test/my.txt
FILE_TEST2=./test/org.txt

run_with_option() {
  echo -n "      "
  if [ "$2" = "" ]; then
    echo "\"\""
  else
    echo $2
  fi
  ./ft_nm $2 $1 > $FILE_TEST1
  RET=$(($?))
  nm $2 $1 > $FILE_TEST2
  RET=$(($RET + $?))
  diff -U 3 $FILE_TEST1 $FILE_TEST2
  RET=$(($RET + $?))
  if [ $RET -eq 0 ]; then
    echo "        OK"
  else
    echo "        KO"
  fi
}

do_test() {
  echo "- TEST " $1
  run_with_option $1 ""
  run_with_option $1 "-g"
  run_with_option $1 "-u"
  run_with_option $1 "-a"
  run_with_option $1 "-p"
}

rm $FILE_TEST1 $FILE_TEST2

do_test $BIN1
do_test $BIN2
do_test $BIN3
do_test $BIN4
do_test $OBJ
do_test $SO

rm $FILE_TEST1 $FILE_TEST2
