                THUMB

                AREA    |.text|, CODE, READONLY


measure_pulse   PROC
                EXPORT  measure_pulse

				push {r1-r5, lr}
wait_low
				LDRH     r2,[r0]
				TST      r2, r1
				BNE      wait_low

wait_high
				LDRH     r2, [r0]
				TST      r2, r1
				BEQ      wait_high
			
				LDR      r2, =0xE000E010
				LDR      r3, [r2]
				MOVS     r4, #1
				ORRS     r3,r3,r4
				STR      r3, [r2]
				

while_high
				LDRH     r2,[r0]
				TST      r2, r1
				BNE      while_high
				
				LDR      r2, =0xE000E018
				LDR      r0, [r2]
				
				pop {r1-r5, pc}

                ENDP

                END
