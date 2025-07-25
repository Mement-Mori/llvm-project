// RUN: llvm-tblgen %s -gen-global-isel -optimize-match-table=false -I %p/../../../include -I %p/../Common | FileCheck %s

// Verify that all MI predicates are enumerated.
//
// CHECK: // PatFrag predicates.
// CHECK-NEXT: enum {
// CHECK-NEXT:   GICXXPred_MI_Predicate_and_or_pat = GICXXPred_Invalid + 1,
// CHECK-NEXT:   GICXXPred_MI_Predicate_mul_pat,
// CHECK-NEXT:   GICXXPred_MI_Predicate_or_disjoint,
// CHECK-NEXT:   GICXXPred_MI_Predicate_or_oneuse,
// CHECK-NEXT:   GICXXPred_MI_Predicate_patfrags_test_pat,
// CHECK-NEXT:   GICXXPred_MI_Predicate_sub3_pat,
// CHECK-NEXT: };

// Verify that we emit cases for all MI predicates.
//
// CHECK: bool MyTargetInstructionSelector::testMIPredicate_MI(
// CHECK:    case GICXXPred_MI_Predicate_and_or_pat: {
// CHECK:      return doesComplexCheck(MI);
// CHECK:    case GICXXPred_MI_Predicate_mul_pat: {
// CHECK:      return doesComplexCheck(MI);
// CHECK:    case GICXXPred_MI_Predicate_or_oneuse: {
// CHECK:      return MRI.hasOneNonDBGUse(MI.getOperand(0).getReg());
// CHECK:    case GICXXPred_MI_Predicate_patfrags_test_pat: {
// CHECK:      return doesComplexCheck(MI);
// CHECK:    case GICXXPred_MI_Predicate_sub3_pat: {
// CHECK:      return doesComplexCheck(MI);

include "llvm/Target/Target.td"
include "GlobalISelEmitterCommon.td"

// Boilerplate code for setting up some registers with subregs.
class MyReg<string n, list<Register> subregs = []>
  : Register<n> {
  let SubRegs = subregs;
}

class MyClass<int size, list<ValueType> types, dag registers>
  : RegisterClass<"Test", types, size, registers> {
  let Size = size;
}

def sub0 : SubRegIndex<16>;
def sub1 : SubRegIndex<16, 16>;
def S0 : MyReg<"s0">;
def S1 : MyReg<"s1">;
def SRegs : MyClass<16, [i16], (sequence "S%u", 0, 1)>;

let SubRegIndices = [sub0, sub1] in {
def D0 : MyReg<"d0", [S0, S1]>;
}

def DRegs : MyClass<32, [i32], (sequence "D%u", 0, 0)>;
def DOP : RegisterOperand<DRegs>;
def AND_OR : I<(outs DRegs:$dst), (ins DOP:$src0, DOP:$src1, DOP:$src2), []>;
def MUL_OR : I<(outs DRegs:$dst), (ins DOP:$src0, DOP:$src1, DOP:$src2), []>;

def or_oneuse : PatFrag<
  (ops node:$x, node:$y),
  (or node:$x, node:$y), [{ return foo(); }]> {
  let GISelPredicateCode = [{
    return MRI.hasOneNonDBGUse(MI.getOperand(0).getReg());
  }];
}


// FIXME: GISelPredicateCode ignored if DAG predicate not set.
def and_or_pat : PatFrag<
  (ops node:$x, node:$y, node:$z),
  (and (or node:$x, node:$y), node:$z), [{ return foo(); }]> {
  let GISelPredicateCode = [{
    return doesComplexCheck(MI);
  }];
  let PredicateCodeUsesOperands = 1;
}

// CHECK:      GIM_Try, /*On fail goto*//*Label 0*/ GIMT_Encode4(97), // Rule ID 7 //
// CHECK-NEXT:   GIM_CheckNumOperands, /*MI*/0, /*Expected*/3,
// CHECK-NEXT:   GIM_CheckOpcode, /*MI*/0, GIMT_Encode2(TargetOpcode::G_AND),
// CHECK-NEXT:   // MIs[0] DstI[dst]
// CHECK-NEXT:   GIM_RootCheckType, /*Op*/0, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_RootCheckRegBankForClass, /*Op*/0, /*RC*/GIMT_Encode2(Test::DRegsRegClassID),
// CHECK-NEXT:   // MIs[0] src2
// CHECK-NEXT:   GIM_RootCheckType, /*Op*/1, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_RecordNamedOperand, /*MI*/0, /*Op*/1, /*StoreIdx*/2, // Name : pred:3:z
// CHECK-NEXT:   GIM_RootCheckRegBankForClass, /*Op*/1, /*RC*/GIMT_Encode2(Test::DRegsRegClassID),
// CHECK-NEXT:   // MIs[0] Operand 2
// CHECK-NEXT:   GIM_RootCheckType, /*Op*/2, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_RecordInsn, /*DefineMI*/1, /*MI*/0, /*OpIdx*/2, // MIs[1]
// CHECK-NEXT:   GIM_CheckNumOperands, /*MI*/1, /*Expected*/3,
// CHECK-NEXT:   GIM_CheckOpcode, /*MI*/1, GIMT_Encode2(TargetOpcode::G_OR),
// CHECK-NEXT:   // MIs[1] Operand 0
// CHECK-NEXT:   GIM_CheckType, /*MI*/1, /*Op*/0, /*Type*/GILLT_s32,
// CHECK-NEXT:   // MIs[1] src0
// CHECK-NEXT:   GIM_CheckType, /*MI*/1, /*Op*/1, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_RecordNamedOperand, /*MI*/1, /*Op*/1, /*StoreIdx*/0, // Name : pred:3:x
// CHECK-NEXT:   GIM_CheckRegBankForClass, /*MI*/1, /*Op*/1, /*RC*/GIMT_Encode2(Test::DRegsRegClassID),
// CHECK-NEXT:   // MIs[1] src1
// CHECK-NEXT:   GIM_CheckType, /*MI*/1, /*Op*/2, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_RecordNamedOperand, /*MI*/1, /*Op*/2, /*StoreIdx*/1, // Name : pred:3:y
// CHECK-NEXT:   GIM_CheckRegBankForClass, /*MI*/1, /*Op*/2, /*RC*/GIMT_Encode2(Test::DRegsRegClassID),
// CHECK-NEXT:   GIM_CheckCxxInsnPredicate, /*MI*/0, /*FnId*/GIMT_Encode2(GICXXPred_MI_Predicate_and_or_pat),
// CHECK-NEXT:   GIM_CheckIsSafeToFold, /*NumInsns*/1,
// CHECK-NEXT:   // (and:{ *:[i32] } DOP:{ *:[i32] }:$src2:$pred:3:z, (or:{ *:[i32] } DOP:{ *:[i32] }:$src0:$pred:3:x, DOP:{ *:[i32] }:$src1:$pred:3:y))<<P:3:Predicate_and_or_pat>>  =>  (AND_OR:{ *:[i32] } DOP:{ *:[i32] }:$src0, DOP:{ *:[i32] }:$src1, DOP:{ *:[i32] }:$src2)
// CHECK-NEXT:   GIR_BuildRootMI, /*Opcode*/GIMT_Encode2(MyTarget::AND_OR),

// CHECK:      GIM_Try, /*On fail goto*//*Label 1*/ GIMT_Encode4(194), // Rule ID 3 //
// CHECK-NEXT:   GIM_CheckNumOperands, /*MI*/0, /*Expected*/3,
// CHECK-NEXT:   GIM_CheckOpcode, /*MI*/0, GIMT_Encode2(TargetOpcode::G_AND),
// CHECK-NEXT:   // MIs[0] DstI[dst]
// CHECK-NEXT:   GIM_RootCheckType, /*Op*/0, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_RootCheckRegBankForClass, /*Op*/0, /*RC*/GIMT_Encode2(Test::DRegsRegClassID),
// CHECK-NEXT:   // MIs[0] Operand 1
// CHECK-NEXT:   GIM_RootCheckType, /*Op*/1, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_RecordInsn, /*DefineMI*/1, /*MI*/0, /*OpIdx*/1, // MIs[1]
// CHECK-NEXT:   GIM_CheckNumOperands, /*MI*/1, /*Expected*/3,
// CHECK-NEXT:   GIM_CheckOpcode, /*MI*/1, GIMT_Encode2(TargetOpcode::G_OR),
// CHECK-NEXT:   // MIs[1] Operand 0
// CHECK-NEXT:   GIM_CheckType, /*MI*/1, /*Op*/0, /*Type*/GILLT_s32,
// CHECK-NEXT:   // MIs[1] src0
// CHECK-NEXT:   GIM_CheckType, /*MI*/1, /*Op*/1, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_RecordNamedOperand, /*MI*/1, /*Op*/1, /*StoreIdx*/0, // Name : pred:3:x
// CHECK-NEXT:   GIM_CheckRegBankForClass, /*MI*/1, /*Op*/1, /*RC*/GIMT_Encode2(Test::DRegsRegClassID),
// CHECK-NEXT:   // MIs[1] src1
// CHECK-NEXT:   GIM_CheckType, /*MI*/1, /*Op*/2, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_RecordNamedOperand, /*MI*/1, /*Op*/2, /*StoreIdx*/1, // Name : pred:3:y
// CHECK-NEXT:   GIM_CheckRegBankForClass, /*MI*/1, /*Op*/2, /*RC*/GIMT_Encode2(Test::DRegsRegClassID),
// CHECK-NEXT:   // MIs[0] src2
// CHECK-NEXT:   GIM_RootCheckType, /*Op*/2, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_RecordNamedOperand, /*MI*/0, /*Op*/2, /*StoreIdx*/2, // Name : pred:3:z
// CHECK-NEXT:   GIM_RootCheckRegBankForClass, /*Op*/2, /*RC*/GIMT_Encode2(Test::DRegsRegClassID),
// CHECK-NEXT:   GIM_CheckCxxInsnPredicate, /*MI*/0, /*FnId*/GIMT_Encode2(GICXXPred_MI_Predicate_and_or_pat),
// CHECK-NEXT:   GIM_CheckIsSafeToFold, /*NumInsns*/1,
// CHECK-NEXT:   // (and:{ *:[i32] } (or:{ *:[i32] } DOP:{ *:[i32] }:$src0:$pred:3:x, DOP:{ *:[i32] }:$src1:$pred:3:y), DOP:{ *:[i32] }:$src2:$pred:3:z)<<P:3:Predicate_and_or_pat>>  =>  (AND_OR:{ *:[i32] } DOP:{ *:[i32] }:$src0, DOP:{ *:[i32] }:$src1, DOP:{ *:[i32] }:$src2)
// CHECK-NEXT:   GIR_BuildRootMI, /*Opcode*/GIMT_Encode2(MyTarget::AND_OR),

// Test commutative, standalone pattern.
def : Pat<
  (i32 (and_or_pat DOP:$src0, DOP:$src1, DOP:$src2)),
  (AND_OR DOP:$src0, DOP:$src1, DOP:$src2)
>;

def mul_pat : PatFrag<
  (ops node:$x, node:$y),
  (mul node:$x, node:$y), [{ return foo(); }]> {
  let GISelPredicateCode = [{
    return doesComplexCheck(MI);
  }];
  let PredicateCodeUsesOperands = 1;
}

// CHECK:      GIM_Try, /*On fail goto*//*Label 2*/ GIMT_Encode4(287), // Rule ID 4 //
// CHECK-NEXT:   GIM_CheckNumOperands, /*MI*/0, /*Expected*/3,
// CHECK-NEXT:   GIM_CheckOpcode, /*MI*/0, GIMT_Encode2(TargetOpcode::G_MUL),
// CHECK-NEXT:   // MIs[0] DstI[dst]
// CHECK-NEXT:   GIM_RootCheckType, /*Op*/0, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_RootCheckRegBankForClass, /*Op*/0, /*RC*/GIMT_Encode2(Test::DRegsRegClassID),
// CHECK-NEXT:   // MIs[0] Operand 1
// CHECK-NEXT:   GIM_RootCheckType, /*Op*/1, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_RecordNamedOperand, /*MI*/0, /*Op*/1, /*StoreIdx*/0, // Name : pred:4:x
// CHECK-NEXT:   GIM_RecordInsn, /*DefineMI*/1, /*MI*/0, /*OpIdx*/1, // MIs[1]
// CHECK-NEXT:   GIM_CheckNumOperands, /*MI*/1, /*Expected*/3,
// CHECK-NEXT:   GIM_CheckOpcode, /*MI*/1, GIMT_Encode2(TargetOpcode::G_OR),
// CHECK-NEXT:   // MIs[1] Operand 0
// CHECK-NEXT:   GIM_CheckType, /*MI*/1, /*Op*/0, /*Type*/GILLT_s32,
// CHECK-NEXT:   // MIs[1] src0
// CHECK-NEXT:   GIM_CheckType, /*MI*/1, /*Op*/1, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_CheckRegBankForClass, /*MI*/1, /*Op*/1, /*RC*/GIMT_Encode2(Test::DRegsRegClassID),
// CHECK-NEXT:   // MIs[1] src1
// CHECK-NEXT:   GIM_CheckType, /*MI*/1, /*Op*/2, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_CheckRegBankForClass, /*MI*/1, /*Op*/2, /*RC*/GIMT_Encode2(Test::DRegsRegClassID),
// CHECK-NEXT:   // MIs[0] src2
// CHECK-NEXT:   GIM_RootCheckType, /*Op*/2, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_RecordNamedOperand, /*MI*/0, /*Op*/2, /*StoreIdx*/1, // Name : pred:4:y
// CHECK-NEXT:   GIM_RootCheckRegBankForClass, /*Op*/2, /*RC*/GIMT_Encode2(Test::DRegsRegClassID),
// CHECK-NEXT:   GIM_CheckCxxInsnPredicate, /*MI*/0, /*FnId*/GIMT_Encode2(GICXXPred_MI_Predicate_mul_pat),
// CHECK-NEXT:   GIM_CheckIsSafeToFold, /*NumInsns*/1,
// CHECK-NEXT:   // (mul:{ *:[i32] } (or:{ *:[i32] } DOP:{ *:[i32] }:$src0, DOP:{ *:[i32] }:$src1):$pred:4:x, DOP:{ *:[i32] }:$src2:$pred:4:y)<<P:4:Predicate_mul_pat>>  =>  (MUL_OR:{ *:[i32] } DOP:{ *:[i32] }:$src0, DOP:{ *:[i32] }:$src1, DOP:{ *:[i32] }:$src2)
// CHECK-NEXT:   GIR_BuildRootMI, /*Opcode*/GIMT_Encode2(MyTarget::MUL_OR),

// CHECK:      GIM_Try, /*On fail goto*//*Label 3*/ GIMT_Encode4(380), // Rule ID 8 //
// CHECK-NEXT:   GIM_CheckNumOperands, /*MI*/0, /*Expected*/3,
// CHECK-NEXT:   GIM_CheckOpcode, /*MI*/0, GIMT_Encode2(TargetOpcode::G_MUL),
// CHECK-NEXT:   // MIs[0] DstI[dst]
// CHECK-NEXT:   GIM_RootCheckType, /*Op*/0, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_RootCheckRegBankForClass, /*Op*/0, /*RC*/GIMT_Encode2(Test::DRegsRegClassID),
// CHECK-NEXT:   // MIs[0] src2
// CHECK-NEXT:   GIM_RootCheckType, /*Op*/1, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_RecordNamedOperand, /*MI*/0, /*Op*/1, /*StoreIdx*/1, // Name : pred:4:y
// CHECK-NEXT:   GIM_RootCheckRegBankForClass, /*Op*/1, /*RC*/GIMT_Encode2(Test::DRegsRegClassID),
// CHECK-NEXT:   // MIs[0] Operand 2
// CHECK-NEXT:   GIM_RootCheckType, /*Op*/2, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_RecordNamedOperand, /*MI*/0, /*Op*/2, /*StoreIdx*/0, // Name : pred:4:x
// CHECK-NEXT:   GIM_RecordInsn, /*DefineMI*/1, /*MI*/0, /*OpIdx*/2, // MIs[1]
// CHECK-NEXT:   GIM_CheckNumOperands, /*MI*/1, /*Expected*/3,
// CHECK-NEXT:   GIM_CheckOpcode, /*MI*/1, GIMT_Encode2(TargetOpcode::G_OR),
// CHECK-NEXT:   // MIs[1] Operand 0
// CHECK-NEXT:   GIM_CheckType, /*MI*/1, /*Op*/0, /*Type*/GILLT_s32,
// CHECK-NEXT:   // MIs[1] src0
// CHECK-NEXT:   GIM_CheckType, /*MI*/1, /*Op*/1, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_CheckRegBankForClass, /*MI*/1, /*Op*/1, /*RC*/GIMT_Encode2(Test::DRegsRegClassID),
// CHECK-NEXT:   // MIs[1] src1
// CHECK-NEXT:   GIM_CheckType, /*MI*/1, /*Op*/2, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_CheckRegBankForClass, /*MI*/1, /*Op*/2, /*RC*/GIMT_Encode2(Test::DRegsRegClassID),
// CHECK-NEXT:   GIM_CheckCxxInsnPredicate, /*MI*/0, /*FnId*/GIMT_Encode2(GICXXPred_MI_Predicate_mul_pat),
// CHECK-NEXT:   GIM_CheckIsSafeToFold, /*NumInsns*/1,
// CHECK-NEXT:   // (mul:{ *:[i32] } DOP:{ *:[i32] }:$src2:$pred:4:y, (or:{ *:[i32] } DOP:{ *:[i32] }:$src0, DOP:{ *:[i32] }:$src1):$pred:4:x)<<P:4:Predicate_mul_pat>>  =>  (MUL_OR:{ *:[i32] } DOP:{ *:[i32] }:$src0, DOP:{ *:[i32] }:$src1, DOP:{ *:[i32] }:$src2)
// CHECK-NEXT:   GIR_BuildRootMI, /*Opcode*/GIMT_Encode2(MyTarget::MUL_OR),

// Test commutative patterns where named operands in the source pattern are not
// directly bound to PatFrag's operands.
def : Pat<
  (i32 (mul_pat (or DOP:$src0, DOP:$src1), DOP:$src2)),
  (MUL_OR DOP:$src0, DOP:$src1, DOP:$src2)
>;

def sub3_pat : PatFrag<
  (ops node:$x, node:$y, node:$z),
  (sub (sub node:$x, node:$y), node:$z), [{ return foo(); }]> {
  let GISelPredicateCode = [{
    return doesComplexCheck(MI);
  }];

  let PredicateCodeUsesOperands = 1;
}

// CHECK:      GIM_Try, /*On fail goto*//*Label 4*/ GIMT_Encode4(463), // Rule ID 0 //
// CHECK-NEXT:   GIM_CheckNumOperands, /*MI*/0, /*Expected*/3,
// CHECK-NEXT:   GIM_CheckOpcode, /*MI*/0, GIMT_Encode2(TargetOpcode::G_SUB),
// CHECK-NEXT:   // MIs[0] DstI[dst]
// CHECK-NEXT:   GIM_RootCheckType, /*Op*/0, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_RootCheckRegBankForClass, /*Op*/0, /*RC*/GIMT_Encode2(Test::DRegsRegClassID),
// CHECK-NEXT:   // MIs[0] Operand 1
// CHECK-NEXT:   GIM_RootCheckType, /*Op*/1, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_RecordInsn, /*DefineMI*/1, /*MI*/0, /*OpIdx*/1, // MIs[1]
// CHECK-NEXT:   GIM_CheckNumOperands, /*MI*/1, /*Expected*/3,
// CHECK-NEXT:   GIM_CheckOpcode, /*MI*/1, GIMT_Encode2(TargetOpcode::G_SUB),
// CHECK-NEXT:   // MIs[1] Operand 0
// CHECK-NEXT:   GIM_CheckType, /*MI*/1, /*Op*/0, /*Type*/GILLT_s32,
// CHECK-NEXT:   // MIs[1] src0
// CHECK-NEXT:   GIM_CheckType, /*MI*/1, /*Op*/1, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_RecordNamedOperand, /*MI*/1, /*Op*/1, /*StoreIdx*/0, // Name : pred:1:x
// CHECK-NEXT:   // MIs[1] src1
// CHECK-NEXT:   GIM_CheckType, /*MI*/1, /*Op*/2, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_RecordNamedOperand, /*MI*/1, /*Op*/2, /*StoreIdx*/1, // Name : pred:1:y
// CHECK-NEXT:   // MIs[0] src2
// CHECK-NEXT:   GIM_RootCheckType, /*Op*/2, /*Type*/GILLT_s32,
// CHECK-NEXT:   GIM_RecordNamedOperand, /*MI*/0, /*Op*/2, /*StoreIdx*/2, // Name : pred:1:z
// CHECK-NEXT:   GIM_CheckCxxInsnPredicate, /*MI*/0, /*FnId*/GIMT_Encode2(GICXXPred_MI_Predicate_sub3_pat),
// CHECK-NEXT:   GIM_CheckIsSafeToFold, /*NumInsns*/1,
// CHECK-NEXT:   // (sub:{ *:[i32] } (sub:{ *:[i32] } i32:{ *:[i32] }:$src0:$pred:1:x, i32:{ *:[i32] }:$src1:$pred:1:y), i32:{ *:[i32] }:$src2:$pred:1:z)<<P:1:Predicate_sub3_pat>>  =>  (SUB3:{ *:[i32] } i32:{ *:[i32] }:$src0, i32:{ *:[i32] }:$src1, i32:{ *:[i32] }:$src2)
// CHECK-NEXT:   GIR_BuildRootMI, /*Opcode*/GIMT_Encode2(MyTarget::SUB3)

// Test a non-commutative pattern.
def SUB3 : I<(outs DRegs:$dst),
  (ins DOP:$src0, DOP:$src1, DOP:$src2),
  [(set DRegs:$dst, (sub3_pat i32:$src0, i32:$src1, i32:$src2))]
>;


def patfrags_test_pat : PatFrags<
  (ops node:$x, node:$y, node:$z),
  [ (xor (add node:$x, node:$y), node:$z),
    (xor (sub node:$x, node:$y), node:$z)
  ], [{ return foo(); }]> {
  let GISelPredicateCode = [{
    return doesComplexCheck(MI);
  }];

  let PredicateCodeUsesOperands = 1;
}

// CHECK: GIM_Try, /*On fail goto*//*Label 5*/ GIMT_Encode4(546), // Rule ID 1 //
// CHECK: // (xor:{ *:[i32] } (add:{ *:[i32] } i32:{ *:[i32] }:$src0:$pred:2:x, i32:{ *:[i32] }:$src1:$pred:2:y), i32:{ *:[i32] }:$src2:$pred:2:z)<<P:2:Predicate_patfrags_test_pat>>  =>  (PATFRAGS:{ *:[i32] } i32:{ *:[i32] }:$src0, i32:{ *:[i32] }:$src1, i32:{ *:[i32] }:$src2)

// CHECK: GIM_Try, /*On fail goto*//*Label 6*/ GIMT_Encode4(629), // Rule ID 2 //
// CHECK: // (xor:{ *:[i32] } (sub:{ *:[i32] } i32:{ *:[i32] }:$src0:$pred:2:x, i32:{ *:[i32] }:$src1:$pred:2:y), i32:{ *:[i32] }:$src2:$pred:2:z)<<P:2:Predicate_patfrags_test_pat>>  =>  (PATFRAGS:{ *:[i32] } i32:{ *:[i32] }:$src0, i32:{ *:[i32] }:$src1, i32:{ *:[i32] }:$src2)

// CHECK: GIM_Try, /*On fail goto*//*Label 7*/ GIMT_Encode4(712), // Rule ID 5 //
// CHECK: // (xor:{ *:[i32] } i32:{ *:[i32] }:$src2:$pred:2:z, (add:{ *:[i32] } i32:{ *:[i32] }:$src0:$pred:2:x, i32:{ *:[i32] }:$src1:$pred:2:y))<<P:2:Predicate_patfrags_test_pat>>  =>  (PATFRAGS:{ *:[i32] } i32:{ *:[i32] }:$src0, i32:{ *:[i32] }:$src1, i32:{ *:[i32] }:$src2)

// CHECK: GIM_Try, /*On fail goto*//*Label 8*/ GIMT_Encode4(795), // Rule ID 6 //
// CHECK: // (xor:{ *:[i32] } i32:{ *:[i32] }:$src2:$pred:2:z, (sub:{ *:[i32] } i32:{ *:[i32] }:$src0:$pred:2:x, i32:{ *:[i32] }:$src1:$pred:2:y))<<P:2:Predicate_patfrags_test_pat>>  =>  (PATFRAGS:{ *:[i32] } i32:{ *:[i32] }:$src0, i32:{ *:[i32] }:$src1, i32:{ *:[i32] }:$src2)


// Test a commutative pattern using multiple patterns using PatFrags.
def PATFRAGS : I<(outs DRegs:$dst),
  (ins DOP:$src0, DOP:$src1, DOP:$src2),
  [(set DRegs:$dst, (patfrags_test_pat i32:$src0, i32:$src1, i32:$src2))]
>;
