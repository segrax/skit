#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "chips/register.hpp"
#include "chips/opcode.hpp"
#include "chips/cpu/cpu.hpp"
#include "6502.hpp"
#include "systems/system.hpp"

void cCpu_Mos_6502::a_Nop() {

}

// 09: Or
void cCpu_Mos_6502::a_Or_Accumulator() {

}

// 18: 
void cCpu_Mos_6502::a_Flag_Carry_Clear() {

}

// 20: Jump Subroutine
void cCpu_Mos_6502::a_Jump_Subroutine() {

}

// 29: 
void cCpu_Mos_6502::a_And_Immediate() {

}

// 2A: 
void cCpu_Mos_6502::a_Roll_Accumulator_Left() {

}

// 4C: Jump
void cCpu_Mos_6502::a_Jump_Absolute() {

}

// 60: Return From Subroutine
void cCpu_Mos_6502::a_Return_From_Subroutine() {

}

// 78: Disable Interrupt Flag
void cCpu_Mos_6502::a_Flag_Interrupt_Disable_Set() {

}

// 84: 
void cCpu_Mos_6502::a_Store_Index_Y_ZeroPage() {

}

// 85: 
void cCpu_Mos_6502::a_Store_Accumulator_ZeroPage() {

}

// 86: 
void cCpu_Mos_6502::a_Store_Index_X_ZeroPage() {

}

// 8A: 
void cCpu_Mos_6502::a_Transfer_Index_X_To_A() {

}

// 8C: 
void cCpu_Mos_6502::a_Store_Index_Y_Absolute() {

}

// 8D:
void cCpu_Mos_6502::a_Store_Accumulator_Absolute() {

}

// 8E: Store X Absolute
void cCpu_Mos_6502::a_Store_Index_X_Absolute() {

}

// 91:
void cCpu_Mos_6502::a_Store_Accumulator_Indirect_Y() {

}

// 98:
void cCpu_Mos_6502::a_Transfer_Index_Y_To_A() {
	
}

// 99: 
void cCpu_Mos_6502::a_Store_Accumulator_Absolute_Y() {

}

// 9A: Transfer X to StackPtr
void cCpu_Mos_6502::a_Transfer_X_to_StackPtr() {
	
}

// A0: 
void cCpu_Mos_6502::a_Load_Index_Y_Immediate() {

}

// A2: Load X Register with Immediate Value
void cCpu_Mos_6502::a_Load_Index_X_Immediate() {
	
}

// A4: 
void cCpu_Mos_6502::a_Load_Index_Y_ZeroPage() {

}

// A8:
void cCpu_Mos_6502::a_Transfer_Accumulator_To_Y() {

}

// A9: Load Accumulator with Immediate Value
void cCpu_Mos_6502::a_Load_Accumulator_Immediate() {

}

// AA:
void cCpu_Mos_6502::a_Transfer_Accumulator_To_X() {

}

// AD: 
void cCpu_Mos_6502::a_Load_Accumulator_Absolute() {

}

// B1: 
void cCpu_Mos_6502::a_Load_Accumulator_Indirect_Y() {

}

// BD: Load A from Absolute_X
void cCpu_Mos_6502::a_Load_A_Absolute_X() {

}

// C8: 
void cCpu_Mos_6502::a_Increase_Y() {

}

// CA: 
void cCpu_Mos_6502::a_Decrease_X() {

}

// D0: Branch not Equal
void cCpu_Mos_6502::a_Branch_Not_Equal() {

}

// D1: 
void cCpu_Mos_6502::a_Compare_Indirect_Y() {

}

// D8: Clear Decimal Flag
void cCpu_Mos_6502::a_Flag_Decimal_Clear() {
	
}

// DD:
void cCpu_Mos_6502::a_Compare_Absolute_X() {
	
}

// E6: 
void cCpu_Mos_6502::a_Increment_Memory_ZeroPage() {
	
}

// F0: 
void cCpu_Mos_6502::a_Branch_Equal() {

}
