// RUN: %clang_cc1 -O2 -triple=powerpc-unknown-aix -emit-llvm %s -o - | \
// RUN: FileCheck %s
// RUN: %clang_cc1 -O2 -triple=powerpc64-unknown-aix -emit-llvm %s -o - | \
// RUN: FileCheck %s
// RUN: %clang_cc1 -O2 -triple=powerpc64le-unknown-unknown -emit-llvm %s \
// RUN:  -o - | FileCheck %s
// RUN: %clang_cc1 -O2 -triple=powerpc64-unknown-unknown -emit-llvm %s \
// RUN:  -o - | FileCheck %s

int test_lwarx(volatile int* a) {
  // CHECK-LABEL: @test_lwarx
  // CHECK: %0 = tail call i32 asm sideeffect "lwarx $0, ${1:y}", "=r,*Z,~{memory}"(i32* %a)
  return __lwarx(a);
}

short test_lharx(volatile short* a) {
  // CHECK-LABEL: @test_lharx
  // CHECK: %0 = bitcast i16* %a to i8*
  // CHECK: %1 = tail call i32 @llvm.ppc.lharx(i8* %0)
  return __lharx(a);
}

char test_lbarx(volatile unsigned char* a) {
 // CHECK-LABEL: @test_lbarx
 // CHECK:  %0 = tail call i32 @llvm.ppc.lbarx(i8* %a)
  return __lbarx(a);
}

int test_stwcx(volatile int* a, int val) {
  // CHECK-LABEL: @test_stwcx
  // CHECK: %0 = bitcast i32* %a to i8*
  // CHECK: %1 = tail call i32 @llvm.ppc.stwcx(i8* %0, i32 %val)
  return __stwcx(a, val);
}

int test_sthcx(volatile short* a, short val) {
  // CHECK-LABEL: @test_sthcx
  // CHECK:   %0 = bitcast i16* %a to i8*
  // CHECK: %1 = sext i16 %val to i32
  // CHECK: %2 = tail call i32 @llvm.ppc.sthcx(i8* %0, i32 %1)
  return __sthcx(a, val);
}
