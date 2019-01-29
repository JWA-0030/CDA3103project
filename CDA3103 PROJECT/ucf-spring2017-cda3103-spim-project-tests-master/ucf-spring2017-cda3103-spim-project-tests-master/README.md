# ucf-spring2017-cda3103-spim-project-tests
bash shell script and test files to automate testing of spim project

This is a bash shell script that will perform automated testing for the UCF Spring 2017 CDA3103 spim project.

The shell script (`run-spim-tests.sh`) will run the spimcore executable with three test machine code files (`test-spim-rtype.asc, test-spim-itype.asc, and test-spim-memry.asc`), and compare the register and memory output to files that contain the expected output (`test-spim-rtype-output.txt, test-spim-itype-output.txt, and test-spim-memry-output.txt`).

Here's what it does:
1. Compiles the project.c and spimcore.c files.
2. Runs the resulting spimcore executable with the test machine code files.
3. Issues commands to output registers and memory to file.
4. Compares the output to the expected output, and prints a pass or fail message.

To use the shell script:
1. Copy the shell script, machine code files, and output files to the directory in Eustis where the project files exist.
2. Make the shell script executable by running the command `chmod 744 run-spim-tests.sh`.
3. Run the script by typing the command `./run-spim-tests.sh`.

When you run the script, you should see something like:
```
Compiling project files... ok
Checking test-spim-rtype.asc...PASS!
Checking test-spim-itype.asc...PASS!
Checking test-spim-memry.asc...PASS!
```
If you see a fail message (`fail (output does not match)`):
1. run the spimcore executable with the test machine code file, 
2. issue a `c` command to run to completion,
3. issue `r` and `m` commands to view register and memory, and
4. compare the output to the contents of the test output files to determine differences.

The shell script uses the `diff` command to compare the spimcore executable output to the expected output in the test files. If you are familiar with the `diff` command, you can use it to help find differences between your output and the expected output.

I believe that the test files produce the correct expected output, but there's always a chance that I've made an error. Please let me know if you believe there is an error in the files, or if you would like to add some tests of your own. You can post a comment in the course discussions, or if you have a GitHub account requests, you can submit a pull request.
