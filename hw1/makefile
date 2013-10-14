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

p:
	g++ counters.cpp pointer2.cpp -o p-out

mem_b:
	g++ mem_b.cpp -o mem-out

graph:
	g++ -O2 graph.cpp -o test-out

matrix:
	g++ counters.cpp mmultiply.cpp square_sgemm.o -o matrix-out

clean:
	rm -rf *-out
	rm -rf *~
	rm -rf ptr_chase.csv
