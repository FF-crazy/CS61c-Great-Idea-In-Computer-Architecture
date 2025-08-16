from unittest import TestCase
from framework import AssemblyTest, print_coverage


class TestAbs(TestCase):
    def test_zero(self):
        t = AssemblyTest(self, "abs.s")
        # load 0 into register a0
        t.input_scalar("a0", 0)
        # call the abs function
        t.call("abs")
        # check that after calling abs, a0 is equal to 0 (abs(0) = 0)
        t.check_scalar("a0", 0)
        # generate the `assembly/TestAbs_test_zero.s` file and run it through venus
        t.execute()

    def test_one(self):
        # same as test_zero, but with input 1
        t = AssemblyTest(self, "abs.s")
        t.input_scalar("a0", 1)
        t.call("abs")
        t.check_scalar("a0", 1)
        t.execute()
    
    def test_minus_one(self):
        t = AssemblyTest(self, "abs.s")
        t.input_scalar("a0", -1)
        t.call("abs")
        t.check_scalar("a0", 1)
        t.execute()

    @classmethod
    def tearDownClass(cls):
        print_coverage("abs.s", verbose=False)


class TestRelu(TestCase):
    def test_simple(self):
        t = AssemblyTest(self, "relu.s")
        # create an array in the data section
        array0 = t.array([1, -2, 3, -4, 5, -6, 7, -8, 9])
        # load address of `array0` into register a0
        t.input_array("a0", array0)
        # set a1 to the length of our array
        t.input_scalar("a1", len(array0))
        # call the relu function
        t.call("relu")
        # check that the array0 was changed appropriately
        t.check_array(array0, [1, 0, 3, 0, 5, 0, 7, 0, 9])
        # generate the `assembly/TestRelu_test_simple.s` file and run it through venus
        t.execute()

    def test_all_zero(self):
        t = AssemblyTest(self, "relu.s")
        array0 = t.array([0, 0, 0, 0, 0])
        t.input_array("a0", array0)
        t.input_scalar("a1", len(array0))
        t.call("relu")
        t.check_array(array0, [0, 0, 0, 0, 0])
        t.execute()
    
    def test_one_and_minus_one(self):
        t = AssemblyTest(self, "relu.s")
        array0 = t.array([1, -1])
        t.input_array("a0", array0)
        t.input_scalar("a1", len(array0))
        t.call("relu")
        t.check_array(array0, [1, 0])
        t.execute()

    def test_minus_one(self):
        t = AssemblyTest(self, "relu.s")
        array0 = t.array([-1])
        t.input_array("a0", array0)
        t.input_scalar("a1", len(array0))
        t.call("relu")
        t.check_array(array0, [0])
        t.execute()

    def test_exit_code(self):
        t = AssemblyTest(self, "relu.s")
        array0 = t.array([1, 0, 1])
        t.input_array("a0", array0)
        t.input_scalar("a1", 0)
        t.call("relu")
        # t.check_array(array0, [0])
        t.execute(code=78)

    @classmethod
    def tearDownClass(cls):
        print_coverage("relu.s", verbose=False)


class TestArgmax(TestCase):
    def test_simple(self):
        t = AssemblyTest(self, "argmax.s")
        # create an array in the data section
        array0 = t.array([1, -2, 3, -4, 5, -6, 7, -8, 9])
        # load address of the array into register a0
        t.input_array("a0", array0)
        # set a1 to the length of the array
        t.input_scalar("a1", len(array0))
        # call the `argmax` function
        t.call("argmax")
        # check that the register a0 contains the correct output
        t.check_scalar("a0", 8)
        # generate the `assembly/TestArgmax_test_simple.s` file and run it through venus
        t.execute()
    
    def test_exit(self):
        t = AssemblyTest(self, "argmax.s")
        array0 = t.array([1, -2, 3, -4, 5, -6, 7, -8, 9])
        t.input_array("a0", array0)
        t.input_scalar("a1", -1)
        t.call("argmax")
        # t.check_scalar("a0", 8)
        t.execute(code=77)

    def test_one_length_array(self):
        t = AssemblyTest(self, "argmax.s")
        array0 = t.array([3])
        t.input_array("a0", array0)
        t.input_scalar("a1", len(array0))
        t.call("argmax")
        t.check_scalar("a0", 0)
        t.execute()

    def test_first_max_array(self):
        t = AssemblyTest(self, "argmax.s")
        array0 = t.array([3, 1, 2])
        t.input_array("a0", array0)
        t.input_scalar("a1", len(array0))
        t.call("argmax")
        t.check_scalar("a0", 0)
        t.execute()

    def test_multi_max_array(self):
        t = AssemblyTest(self, "argmax.s")
        array0 = t.array([3, 4, 2, 3, 4])
        t.input_array("a0", array0)
        t.input_scalar("a1", len(array0))
        t.call("argmax")
        t.check_scalar("a0", 1)
        t.execute()

    @classmethod
    def tearDownClass(cls):
        print_coverage("argmax.s", verbose=False)


class TestDot(TestCase):
    def test_simple(self):
        t = AssemblyTest(self, "dot.s")
        # create arrays in the data section
        array0 = t.array([1, 2, 3, 4, 5])
        array1 = t.array([1, 2, 3, 4, 5])
        # raise NotImplementedError("TODO")
        # TODO
        # load array addresses into argument registers
        t.input_array("a0", array0)
        t.input_array("a1", array1)
        # TODO
        # load array attributes into argument registers
        t.input_scalar("a2", len(array0))
        t.input_scalar("a3", 1)
        t.input_scalar("a4", 1)
        # TODO
        # call the `dot` function
        t.call("dot")
        # check the return value
        t.check_scalar("a0", 55) 
        # TODO
        t.execute()
    
    def test_stride3(self):
        t = AssemblyTest(self, "dot.s")
        array0 = t.array([1, 2, 3, 4, 5])
        array1 = t.array([1, 2, 3, 4, 5])
        t.input_array("a0", array0)
        t.input_array("a1", array1)
        t.input_scalar("a2", 3)
        t.input_scalar("a3", 1)
        t.input_scalar("a4", 2)
        t.call("dot")
        t.check_scalar("a0", 22) 
        t.execute()
    

    def test_stride3(self):
        t = AssemblyTest(self, "dot.s")
        array0 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        array1 = t.array([1, 2, 3, 4, 5, 6, 7, 8, 9])
        t.input_array("a0", array0)
        t.input_array("a1", array1)
        t.input_scalar("a2", 5)
        t.input_scalar("a3", 1)
        t.input_scalar("a4", 2)
        t.call("dot")
        t.check_scalar("a0", 95) 
        t.execute()

    def test_check_len_exit(self):
        t = AssemblyTest(self, "dot.s")
        array0 = t.array([1, 2, 3, 4, 5])
        array1 = t.array([1, 2, 3, 4, 5])
        t.input_array("a0", array0)
        t.input_array("a1", array1)
        t.input_scalar("a2", 0)
        t.input_scalar("a3", 1)
        t.input_scalar("a4", 1)
        t.call("dot")
        t.execute(code=75)
    
    def test_check_stride_exit1(self):
        t = AssemblyTest(self, "dot.s")
        array0 = t.array([1, 2, 3, 4, 5])
        array1 = t.array([1, 2, 3, 4, 5])
        t.input_array("a0", array0)
        t.input_array("a1", array1)
        t.input_scalar("a2", len(array0))
        t.input_scalar("a3", 0)
        t.input_scalar("a4", 1)
        t.call("dot")
        t.execute(code=76)

    def test_check_stride_exit2(self):
        t = AssemblyTest(self, "dot.s")
        array0 = t.array([1, 2, 3, 4, 5])
        array1 = t.array([1, 2, 3, 4, 5])
        t.input_array("a0", array0)
        t.input_array("a1", array1)
        t.input_scalar("a2", len(array0))
        t.input_scalar("a3", 1)
        t.input_scalar("a4", 0)
        t.call("dot")
        t.execute(code=76)
    
    def test_check_stride_len_exit(self):
        t = AssemblyTest(self, "dot.s")
        array0 = t.array([1, 2, 3, 4, 5])
        array1 = t.array([1, 2, 3, 4, 5])
        t.input_array("a0", array0)
        t.input_array("a1", array1)
        t.input_scalar("a2", 0)
        t.input_scalar("a3", 0)
        t.input_scalar("a4", 0)
        t.call("dot") 
        t.execute(code=75)

    @classmethod
    def tearDownClass(cls):
        print_coverage("dot.s", verbose=False)


class TestMatmul(TestCase):

    def do_matmul(self, m0, m0_rows, m0_cols, m1, m1_rows, m1_cols, result, code=0):
        t = AssemblyTest(self, "matmul.s")
        # we need to include (aka import) the dot.s file since it is used by matmul.s
        t.include("dot.s")

        # create arrays for the arguments and to store the result
        array0 = t.array(m0)
        array1 = t.array(m1)
        array_out = t.array([0] * len(result))

        # load address of input matrices and set their dimensions
        # raise NotImplementedError("TODO")
        t.input_array("a0", array0)
        t.input_scalar("a1", m0_rows)
        t.input_scalar("a2", m0_cols)
        t.input_array("a3", array1)
        t.input_scalar("a4", m1_rows)
        t.input_scalar("a5", m1_cols)
        # TODO
        # load address of output array        
        # TODO
        t.input_array("a6", array_out)

        # call the matmul function
        t.call("matmul")

        # check the content of the output array
        # TODO

        if code == 0:
            t.check_array(array_out, result)

        # generate the assembly file and run it through venus, we expect the simulation to exit with code `code`
        t.execute(code=code)

    def test_simple(self):
        self.do_matmul(
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 3, 3,
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 3, 3,
            [30, 36, 42, 66, 81, 96, 102, 126, 150]
        )
    
    def test_case1(self):
        self.do_matmul(
            [5], 1, 1,
            [13], 1, 1,
            [65]
        )
    
    def test_case2(self):
        self.do_matmul(
            [1, 2, 3], 1, 3,
            [4, 5, 6], 3, 1,
            [32]
        )

    def test_case3(self):
        self.do_matmul(
            [4], 1, 1,
            [2, 3, 4], 1, 3,
            [8, 12, 16]
        )

    def test_case4(self):
        self.do_matmul(
            [2, 3, 4], 3, 1,
            [4], 1, 1,
            [8, 12, 16]
        )

    def test_case5(self):
        self.do_matmul(
            [1, 3, 5, 2, 4, 6], 2, 3,
            [1, 2, 3, 4, 5, 6, 7, 8,9, 10, 11, 12], 3, 4,
            [61, 70, 79, 88, 76, 88, 100, 112]
        )
    
    def test_exit_1(self):
        self.do_matmul(
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 0, 3,
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 3, 3,
            [30, 36, 42, 66, 81, 96, 102, 126, 150],
            code=72
        )
    
    def test_exit_2(self):
        self.do_matmul(
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 3, 0,
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 3, 3,
            [30, 36, 42, 66, 81, 96, 102, 126, 150],
            code=72
        )
    
    def test_exit_3(self):
        self.do_matmul(
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 3, 3,
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 0, 3,
            [30, 36, 42, 66, 81, 96, 102, 126, 150],
            code=73
        )
    
    def test_exit_4(self):
        self.do_matmul(
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 3, 3,
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 3, 0,
            [30, 36, 42, 66, 81, 96, 102, 126, 150],
            code=73
        )

    def test_exit_5(self):
        self.do_matmul(
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 3, 5,
            [1, 2, 3, 4, 5, 6, 7, 8, 9], 4, 3,
            [30, 36, 42, 66, 81, 96, 102, 126, 150],
            code=74
        )

    @classmethod
    def tearDownClass(cls):
        print_coverage("matmul.s", verbose=False)


class TestReadMatrix(TestCase):

    # def do_read_matrix(self, fail='', code=0):
    #     t = AssemblyTest(self, "read_matrix.s")
    #     # load address to the name of the input file into register a0
    #     t.input_read_filename("a0", "inputs/test_read_matrix/test_input.bin")

    #     # allocate space to hold the rows and cols output parameters
    #     rows = t.array([-1])
    #     cols = t.array([-1])

    #     # load the addresses to the output parameters into the argument registers
    #     raise NotImplementedError("TODO")
    #     # TODO

    #     # call the read_matrix function
    #     t.call("read_matrix")

    #     # check the output from the function
    #     # TODO

    #     # generate assembly and run it through venus
    #     t.execute(fail=fail, code=code)
    
    def do_read_matrix_fopen_error(self, fail='', code=90):
        t = AssemblyTest(self, "read_matrix.s")
        # load address to the name of the input file into register a0
        t.input_read_filename("a0", "inputs/test_read_matrix/test_input.bin")

        # allocate space to hold the rows and cols output parameters
        rows = t.array([-1])
        cols = t.array([-1]) # this is already the address

        # load the addresses to the output parameters into the argument registers
        t.input_array("a1", rows)
        t.input_array("a2", cols)

        # call the read_matrix function
        t.call("read_matrix")

        # check the output from the function

        # generate assembly and run it through venus
        t.execute(fail=fail, code=code)
    
    def do_read_matrix_fread_error(self, fail='', code=91):
        t = AssemblyTest(self, "read_matrix.s")
        t.input_read_filename("a0", "inputs/test_read_matrix/test_input.bin")
        rows = t.array([-1])
        cols = t.array([-1])
        t.input_array("a1", rows)
        t.input_array("a2", cols)
        t.call("read_matrix")
        t.execute(fail=fail, code=code)


    def do_read_matrix(self, fail='', code=0):
        t = AssemblyTest(self, "read_matrix.s")
        t.input_read_filename("a0", "inputs/test_read_matrix/test_input.bin")
        rows = t.array([-1])
        cols = t.array([-1])
        t.input_array("a1", rows)
        t.input_array("a2", cols)
        t.call("read_matrix")
        t.check_array(rows, [3])
        t.check_array(cols, [3])
        t.check_array_pointer("a0", [1, 2, 3, 4, 5, 6, 7, 8, 9])
        t.execute(fail=fail, code=code)
    

    def do_read_stu_matrix1(self, fail='', code=0):
        t = AssemblyTest(self, "read_matrix.s")
        t.input_read_filename("a0", "inputs/student_input/student_input1.bin")
        rows = t.array([-1])
        cols = t.array([-1])
        t.input_array("a1", rows)
        t.input_array("a2", cols)
        t.call("read_matrix")
        t.check_array(rows, [4])
        t.check_array(cols, [5])
        t.check_array_pointer("a0", [1, 3, 5, 7, 9, 2, 4, 6, 8, 10, -1, -3, -5, -7, -9, -2, -4, -6, -8, -10])
        t.execute(fail=fail, code=code)
    
    def do_read_stu_matrix2(self, fail='', code=0):
        t = AssemblyTest(self, "read_matrix.s")
        t.input_read_filename("a0", "inputs/student_input/student_input2.bin")
        rows = t.array([-1])
        cols = t.array([-1])
        t.input_array("a1", rows)
        t.input_array("a2", cols)
        t.call("read_matrix")
        t.check_array(rows, [1])
        t.check_array(cols, [1])
        t.check_array_pointer("a0", [1234])
        t.execute(fail=fail, code=code)

    def do_read_stu_matrix_one_row(self, fail='', code=0):
        t = AssemblyTest(self, "read_matrix.s")
        t.input_read_filename("a0", "inputs/student_input/student_input_one_row.bin")
        rows = t.array([-1])
        cols = t.array([-1])
        t.input_array("a1", rows)
        t.input_array("a2", cols)
        t.call("read_matrix")
        t.check_array(rows, [1])
        t.check_array(cols, [5])
        t.check_array_pointer("a0", [1, 2, 3, 4, 5])
        t.execute(fail=fail, code=code) 
    
    def do_read_stu_matrix_one_col(self, fail='', code=0):
        t = AssemblyTest(self, "read_matrix.s")
        t.input_read_filename("a0", "inputs/student_input/student_input_one_col.bin")
        rows = t.array([-1])
        cols = t.array([-1])
        t.input_array("a1", rows)
        t.input_array("a2", cols)
        t.call("read_matrix")
        t.check_array(rows, [5])
        t.check_array(cols, [1])
        t.check_array_pointer("a0", [1, 2, 3, 4, 5])
        t.execute(fail=fail, code=code) 
    

    # def test_simple(self):
        # self.do_read_matrix()
    
    def test_do_read_matrix(self):
        self.do_read_matrix()
    
    # def test_do_read_matrix_fread_error(self):
    #     self.do_read_matrix_fread_error()

    def test_do_read_matrix_fopen_error(self):
        self.do_read_matrix_fopen_error(fail='fopen')
    
    def test_do_read_stu_matrix1(self):
        self.do_read_stu_matrix1()
    
    def test_do_read_stu_matrix2(self):
        self.do_read_stu_matrix2()
    
    def test_do_read_stu_matrix_one_col(self):
        self.do_read_stu_matrix_one_col()

    def test_do_read_stu_matrix_one_row(self):
        self.do_read_stu_matrix_one_row()

    @classmethod
    def tearDownClass(cls):
        print_coverage("read_matrix.s", verbose=False)


class TestWriteMatrix(TestCase):

    def do_write_matrix(self, fail='', code=0):
        t = AssemblyTest(self, "write_matrix.s")
        outfile = "outputs/test_write_matrix/student.bin"
        # outfile = "student.bin"
        # load output file name into a0 register
        t.input_write_filename("a0", outfile)
        # load input array and other arguments
        t.input_array("a1", t.array([1, 2, 3, 4, 5, 6, 7, 8, 9]))
        t.input_scalar("a2", 3)
        t.input_scalar("a3", 3)
        # call `write_matrix` function
        t.call("write_matrix")
        # generate assembly and run it through venus
        t.execute(fail=fail, code=code)
        # compare the output file against the reference
        if code == 0:
            t.check_file_output(outfile, "outputs/test_write_matrix/reference.bin")

    def do_custom_write_matrix(self, outfile, matrix, row, col, targetfile, fail='', code=0):
        t = AssemblyTest(self, "write_matrix.s")

        outputs_dir = 'outputs/test_write_matrix/student_test/'
        outfile = outputs_dir + outfile
        targetfile = outputs_dir + targetfile

        t.input_write_filename("a0", outfile)
        t.input_array("a1", t.array(matrix))
        t.input_scalar("a2", row)
        t.input_scalar("a3", col)
        t.call("write_matrix")
        t.execute(fail=fail, code=code)
        if code == 0:
            t.check_file_output(outfile, targetfile)

    def test_simple(self):
        self.do_write_matrix()

    def test_fopen_error(self):
        self.do_write_matrix(fail='fopen', code=93)
    
    def test_malloc_error(self):
        self.do_write_matrix(fail='malloc', code=88)
    
    def test_fwrite_error(self):
        self.do_write_matrix(fail='fwrite', code=94)
    
    def test_fclose_error(self):
        self.do_write_matrix(fail='fclose', code=95)
    
    def test_write_custom_matrix(self):
        self.do_custom_write_matrix('one_matrix.bin', [1, 3, 5, 7, 9, 2, 4, 6, 8, 10, -1, -3, -5, -7, -9], 3, 5, 'matrix_answer.bin')
    
    def test_write_one_elem(self):
        self.do_custom_write_matrix('one_elem.bin', [1234], 1, 1, 'one_elem_answer.bin')
    
    def test_write_one_elem(self):
        self.do_custom_write_matrix('one_elem.bin', [1234], 1, 1, 'one_elem_answer.bin')
    
    def test_write_one_row(self):
        self.do_custom_write_matrix('one_row.bin', [1, 3, 5, 7, 9], 1, 5, 'one_row_answer.bin')

    def test_write_one_col(self):
        self.do_custom_write_matrix('one_col.bin', [2, 4, 6, 8, 10], 5, 1, 'one_col_answer.bin')


    @classmethod
    def tearDownClass(cls):
        print_coverage("write_matrix.s", verbose=False)


class TestClassify(TestCase):

    def make_test(self):
        t = AssemblyTest(self, "classify.s")
        t.include("argmax.s")
        t.include("dot.s")
        t.include("matmul.s")
        t.include("read_matrix.s")
        t.include("relu.s")
        t.include("write_matrix.s")
        return t
    
    def do_simple(self, simple_id, input_id):
        t = self.make_test()
        simple_bin_dir = f"inputs/simple{simple_id}/bin"
        m0_file = simple_bin_dir + "/m0.bin"
        m1_file = simple_bin_dir + "/m1.bin"
        input_file = simple_bin_dir + f"/inputs/input{input_id}.bin"
        out_file = f"outputs/test_basic_main/simple{simple_id}/output{input_id}.bin"

        args = [m0_file, m1_file, input_file, out_file]
        t.input_scalar("a2", 1)
        # call classify function
        t.call("classify")
        # generate assembly and pass program arguments directly to venus
        t.execute(args=args, code=1)
        # compare the output file and
        # t.check_file_output(out_file, ref_file)
        # compare the classification output with `check_stdout`
        # t.check_stdout(expected="2")  
    
    def test_arg_num_error(self):
        t = self.make_test()
        args = ["inputs/simple0/bin/m0.bin", "inputs/simple0/bin/m1.bin",
                "inputs/simple0/bin/inputs/input0.bin"]
        t.input_scalar("a2", 1)
        # call classify function
        t.call("classify")
        # generate assembly and pass program arguments directly to venus
        t.execute(args=args, code=89)
    
    def test_malloc_error(self):
        t = self.make_test()
        out_file = "outputs/test_basic_main/student0.bin"
        args = ["inputs/simple0/bin/m0.bin", "inputs/simple0/bin/m1.bin",
                "inputs/simple0/bin/inputs/input0.bin", out_file]
        t.input_scalar("a2", 1)
        # call classify function
        t.call("classify")
        # generate assembly and pass program arguments directly to venus
        t.execute(args=args, fail='malloc', code=88)

    # def test_simple0_input0(self):
        # self.do_simple(2, 2)
    
    # You Can't test mnist by python scripts.
    def test_mnist_input0(self):
        self.do_mnist(0, "6")

    def do_mnist(self, id, label):
        t = self.make_test()
        out_file = f"outputs/test_mnist_main/student_mnist_outputs{id}.bin"
        args = ["inputs/mnist/bin/m0.bin", "inputs/mnist/bin/m1.bin",
                f"inputs/mnist/bin/inputs/mnist_input{label}.bin", out_file]
        t.input_scalar("a2", 0)
        t.call("classify")
        t.execute(args=args)
        # compare the classification output with `check_stdout`
        t.check_stdout(expected=label)
        


    @classmethod
    def tearDownClass(cls):
        print_coverage("classify.s", verbose=False)


class TestMain(TestCase):

    def run_main(self, inputs, output_id, label):
        args = [f"{inputs}/m0.bin", f"{inputs}/m1.bin", f"{inputs}/inputs/input0.bin",
                f"outputs/test_basic_main/student{output_id}.bin"]
        reference = f"outputs/test_basic_main/reference{output_id}.bin"
        t = AssemblyTest(self, "main.s", no_utils=True)
        t.call("main")
        t.execute(args=args, verbose=False)
        t.check_stdout(label)
        t.check_file_output(args[-1], reference)

    def test0(self):
        self.run_main("inputs/simple0/bin", "0", "2")

    def test1(self):
        self.run_main("inputs/simple1/bin", "1", "1")