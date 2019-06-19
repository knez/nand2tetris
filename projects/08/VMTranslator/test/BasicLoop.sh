../VMTranslator ../../ProgramFlow/BasicLoop/BasicLoop.vm
mv BasicLoop.asm ../../ProgramFlow/BasicLoop
../../../../tools/CPUEmulator.sh ../../ProgramFlow/BasicLoop/BasicLoop.tst >/dev/null 2>&1
