import subprocess
import sys
import os

def errCheck(test):
    result = subprocess.run(test[0],capture_output=True, text=True)
    if result.stdout == test[1]:
        return True
    else:
        return False

def main():
    os.system('g++ -o lc3sim lc3sim.cpp')
    testList = [(['./lc3sim', 'tests/code.obj'], 'ABCDE\n'), (['./lc3sim', 'tests/code2.obj'], 'A\n'),
                (['./lc3sim', 'tests/code3.obj'], 'A\n'), (['./lc3sim', 'tests/code4.obj'], 'B\n')]
    for i in range(len(testList)):
        if errCheck(testList[i]):
            print("test:", i, "passed")
        else:
            print("test", i, "failed")

if __name__ == "__main__":
    main()