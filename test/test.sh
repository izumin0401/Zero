../compiler/zero test.ze > test_.result 2>&1
../compiler/zero array.ze > array_.result 2>&1
../compiler/zero class01.ze > class01_.result 2>&1
../compiler/zero class02.ze > class02_.result 2>&1
../compiler/zero class03.ze > class03_.result 2>&1
../compiler/zero method.ze > method_.result 2>&1
../compiler/zero cast.ze > cast_.result 2>&1
../compiler/zero classmain.ze > classmain_.result 2>&1
../compiler/zero downcast.ze > downcast_.result 2>&1
../compiler/zero instanceof.ze > instanceof_.result 2>&1
../compiler/zero super.ze > super_.result 2>&1
../compiler/zero exception.ze > exception_.result 2>&1
../compiler/zero shapemain.ze > shapemain_.result 2>&1
../compiler/zero throws.ze > throws_.result 2>&1
../compiler/zero nullpointer.ze > nullpointer_.result 2>&1
../compiler/zero array_ex.ze > array_ex_.result 2>&1
../compiler/zero else_ex.ze > else_ex_.result 2>&1
../compiler/zero native_pointer.ze
../compiler/zero switch.ze > switch_.result 2>&1
../compiler/zero final.ze > final_.result 2>&1
../compiler/zero do_while.ze > do_while_.result 2>&1
../compiler/zero enum.ze > enum_.result 2>&1
../compiler/zero delegate.ze > delegate_.result 2>&1
../compiler/zero rename.ze > rename_.result 2>&1

diff test.result test_.result
diff array.result array_.result
diff class01.result class01_.result
diff class02.result class02_.result
diff class03.result class03_.result
diff method.result method_.result
diff cast.result cast_.result
diff classmain.result classmain_.result
diff downcast.result downcast_.result
diff instanceof.result instanceof_.result
diff super.result super_.result
diff exception.result exception_.result
diff shapemain.result shapemain_.result
diff throws.result throws_.result
diff nullpointer.result nullpointer_.result
diff array_ex.result array_ex_.result
diff else_ex.result else_ex_.result
diff test.ze test.copy
diff switch.result switch_.result
diff final.result final_.result
diff do_while.result do_while_.result
diff enum.result enum_.result
diff delegate.result delegate_.result
diff rename.result rename_.result