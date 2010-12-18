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

// 20: Jump Subroutine
void cCpu_Mos_6502::a_Jump_Subroutine() {

}

// 60: Return From Subroutine
void cCpu_Mos_6502::a_Return_From_Subroutine() {

}

// 78: Disable Interrupt Flag
void cCpu_Mos_6502::a_Flag_Interrupt_Disable_Set() {

}

// 85: 
void cCpu_Mos_6502::a_Store_Accumulator_ZeroPage() {

}

// 8D:
void cCpu_Mos_6502::a_Store_Accumulator_Absolute() {

}

// 8E: Store X Absolute
void cCpu_Mos_6502::a_Store_Index_X_Absolute() {

}

// 9A: Transfer X to StackPtr
void cCpu_Mos_6502::a_Transfer_X_to_StackPtr() {
	
}

// A2: Load X Register with Immediate Value
void cCpu_Mos_6502::a_Load_Index_X_Immediate() {
	
}

// A9: Load Accumulator with Immediate Value
void cCpu_Mos_6502::a_Load_Accumulator_Immediate() {

}

// BD: Load A from Absolute_X
void cCpu_Mos_6502::a_Load_A_Absolute_X() {

}

// CA: 
void cCpu_Mos_6502::a_Decrease_X() {

}

// D0: Branch not Equal
void cCpu_Mos_6502::a_Branch_Not_Equal() {

}

// D8: Clear Decimal Flag
void cCpu_Mos_6502::a_Flag_Decimal_Clear() {
	
}

// DD:
void cCpu_Mos_6502::a_Compare_Absolute_X() {
	
}
