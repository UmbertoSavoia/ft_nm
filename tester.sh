SO=/usr/lib/wsl/drivers/nvdm.inf_amd64_aced13c2084a2857/libnvidia-ml_loader.so
OBJ=./src/main.o
BIN1=./test/test1.out
BIN2=./test/test2.out
BIN3=./test/test1_32.out

FILE_TEST1=./test/my.txt
FILE_TEST2=./test/org.txt

do_test() {
  echo "- TEST " $1
  ./ft_nm $1 > $FILE_TEST1
  nm $1 > $FILE_TEST2
  diff -U 3 $FILE_TEST1 $FILE_TEST2
}

rm $FILE_TEST1 $FILE_TEST2

do_test $BIN1
do_test $BIN2
do_test $BIN3
do_test $OBJ
do_test $SO

rm $FILE_TEST1 $FILE_TEST2
