all: 
	g++ --std=c++17 -o pin_codes pin_codes.cpp 

clean_persistent_data:
	rm data.db

clean: clean_persistent_data
	rm pin_codes