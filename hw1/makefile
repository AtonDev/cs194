default:
	echo 'default'

skeleton:
	g++ -msse4 -O2 counters.cpp skeleton.cpp -o skeleton-out

sum:
	g++ -msse4 -O2 counters.cpp sum.cpp -o sum-out
	./sum-out
	g++ -msse4 counters.cpp sum.cpp -o sum-out
	./sum-out

sum_n:
	g++ -msse4 -O2 counters.cpp sum.cpp -o sum-out
	./sum-out 10000
	g++ -msse4 counters.cpp sum.cpp -o sum-out
	./sum-out 10000

ptr_chase:
	g++ counters.cpp pointer_chase.cpp -o ptr-out

clean:
	rm -rf *-out
	rm -rf *~
