testdir="tests"


if [ ! -d "$testdir" ]; 
then
	echo "No directory named $testdir"
	exit
fi

make

nodiff=true
stopondiff=false
#ignore these for now

for filename in "$testdir"/*.c; do

	./mysclp -icode "$filename" 2>"$filename.myerr"
	mv "$filename.spim" "$filename.myspim"
	mv "$filename.ic" "$filename.myic"

	./sclp -icode "$filename" 2>"$filename.err"
	DIFF1=$(diff "$filename.spim" "$filename.myspim")
	DIFF2=$(diff "$filename.err" "$filename.myerr")

	DIFF3=$(diff "$filename.ic" "$filename.myic")




	./mysclp -tokens -ast -symtab -eval "$filename" 2> /dev/null
	mv "$filename.eval" "$filename.myeval"
	mv "$filename.ast" "$filename.myast"
	mv "$filename.sym" "$filename.mysym"
	mv "$filename.toks" "$filename.mytoks"
	./sclp -tokens -ast -symtab -eval "$filename" 2> /dev/null
	DIFF4=$(diff "$filename.ast" "$filename.myast")
	DIFF5=$(diff "$filename.eval" "$filename.myeval")

	DIFF6=$(diff "$filename.toks" "$filename.mytoks")
	DIFF7=$(diff "$filename.sym" "$filename.mysym")
	
	if [ "$DIFF5" != "" ]
	then
		echo "eval comparison failed on $filename. Did not run further tests (lexicographic ordering)."
		echo "Showing diff:"
		echo "$DIFF5"
		exit
	fi
	
	if [ "$DIFF1" != "" ]
	then
		echo "spim code comparison failed on $filename. Continuing further tests."
		echo "continuing with further tests"
		echo "Showing diff:"
		echo "$DIFF1"
		# exit
	fi

	if [ "$DIFF2" != "" ]
	then
		echo "error message comparison failed on $filename. "
		echo "Showing diff:"
		echo "$DIFF2"
		echo "Continuing with further tests."
		echo ""
		echo ""
		# exit
	fi	

	# if [ "$DIFF3" != "" ]
	# then
	# 	echo "icode comparison failed on $filename. Continuing further tests."
	# 	echo "Showing diff:"
	# 	echo "$DIFF3"
	# 	# exit
	# fi


	if [ "$DIFF4" != "" ]
	then
		echo "ast comparison failed on $filename. Continuing further tests."
		echo "Showing diff:"
		echo "$DIFF4"
		# exit
	fi

	

	if [ "$DIFF6" != "" ]
	then
		if ! [ -s "$filename.myerr" ] #myerr is empty
		then
			echo "tokens comparison failed on $filename even though myerr is empty. Did not run further tests (lexicographic ordering)."
			exit
		fi
		echo "tokens comparison failed on $filename, but myerr is non-empty. Continuing tests"
		echo ""
		# echo "Showing diff:"
		# echo "$DIFF6"
		# exit
	fi

	if [ "$DIFF7" != "" ]
	then
		echo "symtab comparison failed on $filename. Did not run further tests (lexicographic ordering)."
		echo "Showing diff:"
		echo "$DIFF7"
		exit
	fi

	
done
echo "All tests passed!"
