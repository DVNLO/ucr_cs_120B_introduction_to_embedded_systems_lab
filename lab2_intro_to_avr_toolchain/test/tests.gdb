# Test file for "lab2_intro_to_avr_toolchain"


# commands.gdb provides the following functions for ease:
#   test "<message>"
#       Where <message> is the message to print. Must call this at the beginning of every test
#       Example: test "PINA: 0x00 => expect PORTC: 0x01"
#   checkResult
#       Verify if the test passed or failed. Prints "passed." or "failed." accordingly, 
#       Must call this at the end of every test.
#   expectPORTx <val>
#       With x as the port (A,B,C,D)
#       The value the port is epected to have. If not it will print the erroneous actual value
#   setPINx <val>
#       With x as the port or pin (A,B,C,D)
#       The value to set the pin to (can be decimal or hexidecimal
#       Example: setPINA 0x01
#   printPORTx f OR printPINx f 
#       With x as the port or pin (A,B,C,D)
#       With f as a format option which can be: [d] decimal, [x] hexadecmial (default), [t] binary 
#       Example: printPORTC d
#   printDDRx
#       With x as the DDR (A,B,C,D)
#       Example: printDDRB

echo ======================================================\n
echo Running all tests..."\n\n

test "PINA: 0xFF, PINB: 0xFF, PINC: 0xFF => PORTD: 0xBD"
setPINA 0xFF
setPINB 0xFF
setPINC 0xFF
continue 5
expectPORTD 0xFD
checkResult

test "PINA: 0x8C, PINB: 0x8C, PINC: 0x00 => PORTD: 0xFF"
setPINA 0x8C
setPINB 0x8C
setPINC 0x00
continue 5
expectPORTD 0xFF
checkResult

test "PINA: 0x00, PINB: 0x8C, PINC: 0x00 => PORTD: 0x8C"
setPINA 0x00
setPINB 0x8C
setPINC 0x00
continue 5
expectPORTD 0x8C
checkResult

test "PINA: 0x30, PINB: 0x30, PINC: 0x30 => PORTD: 0x91"
setPINA 0x30
setPINB 0x30
setPINC 0x30
continue 5
expectPORTD 0x91
checkResult

test "PINA: 0x55, PINB: 0x02, PINC: 0x03 => PORTD: 0x5A"
setPINA 0x55
setPINB 0x02
setPINC 0x03
continue 5
expectPORTD 0x5A
checkResult

test "PINA: 0x00, PINB: 0x00, PINC: 0x00 => PORTD: 0x00"
setPINA 0x00
setPINB 0x00
setPINC 0x00
continue 5
expectPORTD 0x00
checkResult

test "PINA: 0x04, PINB: 0x04, PINC: 0x04 => PORTD: 0x0C"
setPINA 0x04
setPINB 0x04
setPINC 0x04
continue 5
expectPORTD 0x0C
checkResult


# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
