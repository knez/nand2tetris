../VMTranslator ../../ProgramFlow/FibonacciSeries/FibonacciSeries.vm
mv FibonacciSeries.asm ../../ProgramFlow/FibonacciSeries
../../../../tools/CPUEmulator.sh ../../ProgramFlow/FibonacciSeries/FibonacciSeries.tst >/dev/null 2>&1
