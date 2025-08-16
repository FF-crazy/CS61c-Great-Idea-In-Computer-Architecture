PY = python
UTFLAGS = -m unittest -v

testmain: ../src/write_matrix.s ../src/classify.s ../src/argmax.s ../src/dot.s ../src/matmul.s ../src/read_matrix.s ../src/relu.s ../src/write_matrix.s 
	$(PY) $(UTFLAGS) unittests.TestMain

testclassify: ../src/write_matrix.s ../src/classify.s ../src/argmax.s ../src/dot.s ../src/matmul.s ../src/read_matrix.s ../src/relu.s ../src/write_matrix.s 
	$(PY) $(UTFLAGS) unittests.TestClassify

testwritematrix: ../src/write_matrix.s unittests.py
	$(PY) $(UTFLAGS) unittests.TestWriteMatrix

testreadmatrix: ../src/read_matrix.s unittests.py
	$(PY) $(UTFLAGS) unittests.TestReadMatrix

testmatmul: ../src/matmul.s unittests.py ../src/dot.s
	$(PY) $(UTFLAGS) unittests.TestMatmul

testdot: ../src/dot.s unittests.py
	$(PY) $(UTFLAGS) unittests.TestDot

testrelu: ../src/relu.s unittests.py
	$(PY) $(UTFLAGS) unittests.TestRelu

testargmax: ../src/argmax.s unittests.py
	$(PY) $(UTFLAGS) unittests.TestArgmax

clean:
	rm -rf assembly

