CXX=g++ -g
LIBS=-lgtest -lgtest_main -lpthread

OUTPUT_DIR=exe

test: test_vector test_map test_list

test_%: makedir
	$(CXX) $@.cxx -o $(OUTPUT_DIR)/$@ $(LIBS)
	$(OUTPUT_DIR)/$@ --gtest_output=xml:$@.test.xml

makedir:
	mkdir -p $(OUTPUT_DIR)

clean:
	rm $(OUTPUT_DIR)/*
	rm *.test.xml
