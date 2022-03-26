CC = /opt/homebrew/opt/llvm/bin/clang -fopenmp -L /opt/homebrew/lib 
exe_file = run



$(exe_file): main.cpp 
	$(CC) main.cpp -o $(exe_file)

clean:
	rm -f *.o $(exe_file)
