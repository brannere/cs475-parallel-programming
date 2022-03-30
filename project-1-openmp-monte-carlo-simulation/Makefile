#CC = /opt/homebrew/opt/llvm/bin/clang -lc++ -fopenmp -L /opt/homebrew/lib 
CC = g++ -fopenmp 
exe_file = run



$(exe_file): main.cpp 
	$(CC) main.cpp -o $(exe_file)

clean:
	rm -f $(exe_file)
