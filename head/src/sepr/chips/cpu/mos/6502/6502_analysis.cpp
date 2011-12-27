#include "sepr.hpp"
#include "device/deviceConnection.hpp"
#include "device/device.hpp"
#include "chips/register.hpp"
#include "chips/opcode.hpp"
#include "chips/cpu/cpu.hpp"
#include "6502.hpp"
#include "systems/system.hpp"

#include "analyse/analyse.hpp"

void cCpu_Mos_6502::a_Nop() {

}

// 05:
void cCpu_Mos_6502::a_Or_Accumulator_ZeroPage() {

}

// 06: 
void cCpu_Mos_6502::a_Arith_Shift_Left_ZeroPage() {

}

// 08: 
void cCpu_Mos_6502::a_Push_Flags() {

}

// 09: Or
void cCpu_Mos_6502::a_Or_Accumulator() {

}

// 0A: Asl
void cCpu_Mos_6502::a_Arithmetic_Shift_Left() {

}

// 0D: Or
void cCpu_Mos_6502::a_Or_Accumulator_Absolute() {

}

// 10: 
void cCpu_Mos_6502::a_Branch_If_Negative_Clear() {

}

// 16: 
void cCpu_Mos_6502::a_Arith_Shift_Left_ZeroPage_X() {

}

// 18: 
void cCpu_Mos_6502::a_Flag_Carry_Clear() {

}

// 20: Jump Subroutine
void cCpu_Mos_6502::a_Jump_Subroutine() {

}

// 24: 
void cCpu_Mos_6502::a_Bit_ZeroPage() {
	
}

// 26:
void cCpu_Mos_6502::a_Rotate_Left_ZeroPage() {

}

// 28: 
void cCpu_Mos_6502::a_Pull_Flags() {

}

// 29: 
void cCpu_Mos_6502::a_And_Immediate() {

}

// 2A: 
void cCpu_Mos_6502::a_Rotate_Accumulator_Left() {

}

// 2C: 
void cCpu_Mos_6502::a_Bit_Absolute() {

}

// 30: 
void cCpu_Mos_6502::a_Branch_If_Negative_Set() {

}

// 38: 
void cCpu_Mos_6502::a_Flag_Carry_Set() {

}

void cCpu_Mos_6502::a_Return_From_Interrupt() {

}

// 45:
void cCpu_Mos_6502::a_Exclusive_Or_ZeroPage() {

}

// 46: 
void cCpu_Mos_6502::a_Logical_Shift_Right_ZeroPage() {

}

// 48: 
void cCpu_Mos_6502::a_Push_Accumulator() {

}

// 49
void cCpu_Mos_6502::a_Exclusive_Or() {

}

// 4A: 
void cCpu_Mos_6502::a_Logical_Shift_Right() {

}

// 4C: Jump
void cCpu_Mos_6502::a_Jump_Absolute() {

}

// 50:
void cCpu_Mos_6502::a_Branch_If_Overflow_Clear() {

}

// 51: 
void cCpu_Mos_6502::a_Exclusive_Or_Indirect_Y() {

}

// 56:
void cCpu_Mos_6502::a_Logical_Shift_Right_ZeroPage_X() {

}

// 58: 
void cCpu_Mos_6502::a_Flag_Interrupt_Clear() {

}

// 60: Return From Subroutine
void cCpu_Mos_6502::a_Return_From_Subroutine() {

}

// 65:
void cCpu_Mos_6502::a_Add_With_Carry_ZeroPage() {

}

// 66:
void cCpu_Mos_6502::a_Rotate_Right_ZeroPage() {

}

// 68: 
void cCpu_Mos_6502::a_Pull_Accumulator() {

}

// 69: 
void cCpu_Mos_6502::a_Add_With_Carry_Immediate() {

}

// 6A: 
void cCpu_Mos_6502::a_Rotate_Right_Accumulator() {

}

// 6C:
void cCpu_Mos_6502::a_Jump_Indirect() {

}

// 70: 
void cCpu_Mos_6502::a_Branch_If_Overflow() {

}

// 71:
void cCpu_Mos_6502::a_Add_With_Carry_IndirectY() {

}

// 76: 
void cCpu_Mos_6502::a_Rotate_Right_ZeroPage_X() {

}

// 78: Disable Interrupt Flag
void cCpu_Mos_6502::a_Flag_Interrupt_Disable_Set() {

}

// 79: 
void cCpu_Mos_6502::a_Add_With_Carry_Absolute_Y() {

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

// 88: 
void cCpu_Mos_6502::a_Decrease_Y() {

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

// 90: 
void cCpu_Mos_6502::a_Branch_If_Carry_Clear() {

}

// 91:
void cCpu_Mos_6502::a_Store_Accumulator_Indirect_Y() {

}

// 94: 
void cCpu_Mos_6502::a_Store_Index_Y_ZeroPage_X() {

}

// 95: 
void cCpu_Mos_6502::a_Store_Accumulator_ZeroPage_X() {

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

// 9D: 
void cCpu_Mos_6502::a_Store_Accumulator_Absolute_X() {

}

// A0: 
void cCpu_Mos_6502::a_Load_Index_Y_Immediate() {

}

// A1: 
void cCpu_Mos_6502::a_Load_Accumulator_Indirect_X() {

}

// A2: Load X Register with Immediate Value
void cCpu_Mos_6502::a_Load_Index_X_Immediate() {
   
   mOpcodeAnalysis->ParameterAdd( new cAnalysis_Opcode_Parameter_Value_Byte( readImmediate() ) );

   mOpcodeAnalysis->CommandAdd( new cAnalysis_Opcode_Command( OP_COMMAND_SET, CMD_OPT_REG, "X", CMD_OPT_PARAM, 0 ) );

}

// A4: 
void cCpu_Mos_6502::a_Load_Index_Y_ZeroPage() {

}

// A5: 
void cCpu_Mos_6502::a_Load_Accumulator_ZeroPage() {

}

// A6: 
void cCpu_Mos_6502::a_Load_Index_X_ZeroPage() {

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

// AE:
void cCpu_Mos_6502::a_Load_Index_X_Absolute() {

}

// AD: 
void cCpu_Mos_6502::a_Load_Accumulator_Absolute() {

}

// B0: 
void cCpu_Mos_6502::a_Branch_If_CarrySet() {


}
// B1: 
void cCpu_Mos_6502::a_Load_Accumulator_Indirect_Y() {

}

// B4: 
void cCpu_Mos_6502::a_Load_Index_Y_ZeroPage_X() {

}

// B5: 
void cCpu_Mos_6502::a_Load_Accumulator_ZeroPage_X() {

}

// B9: 
void cCpu_Mos_6502::a_Load_Accumulator_Absolute_Y() {

}

// BA: 
void cCpu_Mos_6502::a_Transfer_S_To_IndexX() {

}

// BD: Load A from Absolute_X
void cCpu_Mos_6502::a_Load_A_Absolute_X() {

}

// C0:
void cCpu_Mos_6502::a_Compare_Index_Y_Immediate() {

}

// C1:
void cCpu_Mos_6502::a_Compare_Indirect_X() {

}

// C4:
void cCpu_Mos_6502::a_Compare_Index_Y_ZeroPage() {

}

// C5:
void cCpu_Mos_6502::a_Compare_Accumulator_ZeroPage() {

}

// C6:
void cCpu_Mos_6502::a_Decrease_ZeroPage() {

}

// C8: 
void cCpu_Mos_6502::a_Increase_Y() {

}

// C9:
void cCpu_Mos_6502::a_Compare_Accumulator_Immediate() {

}

// CA: 
void cCpu_Mos_6502::a_Decrease_X() {

}

// CD: 
void cCpu_Mos_6502::a_Compare_Accumulator_Absolute() {

}

// CE:
void cCpu_Mos_6502::a_Decrease_Memory_Absolute() {

}

// D0: Branch not Equal
void cCpu_Mos_6502::a_Branch_Not_Equal() {

}

// D1: 
void cCpu_Mos_6502::a_Compare_Indirect_Y() {

}

// D5:
void cCpu_Mos_6502::a_Compare_Zeropage_X() {

}

// D8: Clear Decimal Flag
void cCpu_Mos_6502::a_Flag_Decimal_Clear() {
	
}

// DD:
void cCpu_Mos_6502::a_Compare_Absolute_X() {
	
}

// E0:
void cCpu_Mos_6502::a_Compare_Index_X_Immediate() {

}

// E4:
void cCpu_Mos_6502::a_Compare_Index_X_ZeroPage() {

}

// E5:
void cCpu_Mos_6502::a_Subtract_With_Carry_ZeroPage() {

}

// E6: 
void cCpu_Mos_6502::a_Increment_Memory_ZeroPage() {
	
}

// E8: 
void cCpu_Mos_6502::a_Increase_X(){						

}

// E9:
void cCpu_Mos_6502::a_Subtract_With_Carry_Immediate() {

}

// EC:
void cCpu_Mos_6502::a_Compare_Index_X_Absolute() {

}


// EE:
void cCpu_Mos_6502::a_Increase_Memory_Absolute() {

}

// F0: 
void cCpu_Mos_6502::a_Branch_Equal() {

}

// F6: 
void cCpu_Mos_6502::a_Increase_Memory_ZeroPage_X() {

}

// F8:
void cCpu_Mos_6502::a_Flag_Decimal_Set() {
	
}

// F9:
void cCpu_Mos_6502::a_Subtract_With_Carry_Absolute_Y() {

}
