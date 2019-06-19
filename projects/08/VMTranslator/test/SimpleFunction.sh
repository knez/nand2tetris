../VMTranslator ../../FunctionCalls/SimpleFunction/SimpleFunction.vm
mv SimpleFunction.asm ../../FunctionCalls/SimpleFunction
../../../../tools/CPUEmulator.sh ../../FunctionCalls/SimpleFunction/SimpleFunction.tst >/dev/null 2>&1
