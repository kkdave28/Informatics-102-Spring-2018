nineteen:
	g++ -std=c++11 -fPIC -shared words1.cpp -o words1.so
	g++ -std=c++11 -fPIC -shared words2.cpp -o words2.so
	g++ -std=c++11 -fPIC -shared freq1.cpp -o freq1.so
	g++ nineteen.cpp words1.so words2.so freq1.so -std=c++11 -ldl -rdynamic -o nineteen

	

clean:
	rm -f core.*
	rm -f vgcore.*
	rm -f *.class
	rm -f nineteen
	rm -f words1.so
	rm -f words2.so
