testdir="tests"


for filename in "$testdir"/*.c; do
	./mysclp "$filename"
	mv "$filename.spim" "$filename.myspim"
	./sclp "$filename"
	spim -file "$filename.spim" > reference.out
	spim -file "$filename.myspim" > mine.out
	DIFF=$(diff "reference.out"  "mine.out")
	echo "=================================================="
	echo "$filename"
	echo "$DIFF"
	echo "=================================================="
	echo ""
done

echo "All tests done"