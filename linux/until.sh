#! /bin/bash
echo "Multiple program"
echo ""
echo -n "Input num: "
	read _num
echo ""
echo "========="
echo "$_num dan"
echo ""
x=1
until [ $x -gt 9 ]
	do
		result=`expr $_num \* $x`
		echo "$_num x $x = $result"
		x=`expr $x \+ 1`
	done
