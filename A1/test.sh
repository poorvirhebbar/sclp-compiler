testdir="tests"



if [ ! -d "$testdir" ]; 
then
	echo "No directory named $testdir"
	exit
fi

make

for filename in "$testdir"/*.c; do
	./mysclp -tokens -parse "$filename"
	mv "$filename.toks" "$filename.mytoks"
	./sclp -tokens -parse "$filename"
	DIFF1=$(diff "$filename.toks" "$filename.mytoks")


	./mysclp -eval "$filename" 2> /dev/null
	mv "$filename.eval" "$filename.myeval"
	./sclp -eval "$filename" 2> /dev/null
	DIFF2=$(diff "$filename.eval" "$filename.myeval")

	./mysclp "$filename" 2>"$filename.myerr"
	mv "$filename.spim" "$filename.myspim"
	./sclp "$filename" 2>"$filename.err"
	DIFF3=$(diff "$filename.spim" "$filename.myspim")
	DIFF4=$(diff "$filename.err" "$filename.myerr")

	if [ "$DIFF1" != "" ]
	then
		echo "-parse -tokens failed on $filename. Did not run further tests (lexicographic ordering)."
		echo "Showing diff:"
		echo "$DIFF1"
		exit
	fi

	if [ "$DIFF2" != "" ]
	then
		echo "-eval failed on $filename. Did not run further tests (lexicographic ordering)."
		echo "Showing diff:"
		echo "$DIFF2"
		exit
	fi

	if [ "$DIFF3" != "" ]
	then
		echo "spimcode failed on $filename. Did not run further tests (lexicographic ordering)."
		echo "Showing diff:"
		echo "$DIFF3"
		exit
	fi

	# if [ "$DIFF4" != "" ]
	# then
	# 	echo "spim error message failed on $filename. Did not run further tests (lexicographic ordering)."
	# 	echo "Showing diff:"
	# 	echo "$DIFF4"
	# 	exit
	# fi
done
echo "All tests passed!"
