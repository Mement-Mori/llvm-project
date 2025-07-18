// NOTE: Assertions have been autogenerated by utils/update_cc_test_checks.py UTC_ARGS: --version 5

// RUN: %clang_cc1 -triple aarch64 -target-feature +sme2  -target-feature +sme  -target-feature +sme-lutv2 -disable-O0-optnone -Werror -Wall -emit-llvm -o - %s | opt -S -p mem2reg,instcombine,tailcallelim | FileCheck %s
// RUN: %clang_cc1 -triple aarch64 -target-feature +sme2  -target-feature +sme  -target-feature +sme-lutv2 -disable-O0-optnone -Werror -Wall -emit-llvm -o - -x c++ %s | opt -S -p mem2reg,instcombine,tailcallelim | FileCheck %s -check-prefix=CPP-CHECK
// RUN: %clang_cc1 -DSVE_OVERLOADED_FORMS -triple aarch64 -target-feature +sme  -target-feature +sme2  -target-feature +sme-lutv2 -disable-O0-optnone -Werror -Wall -emit-llvm -o - %s | opt -S -p mem2reg,instcombine,tailcallelim | FileCheck %s
// RUN: %clang_cc1 -DSVE_OVERLOADED_FORMS -triple aarch64 -target-feature +sme  -target-feature +sme2 -target-feature +sme-lutv2 -disable-O0-optnone -Werror -Wall -emit-llvm -o - -x c++ %s | opt -S -p mem2reg,instcombine,tailcallelim | FileCheck %s -check-prefix=CPP-CHECK
// RUN: %clang_cc1 -triple aarch64 -target-feature +sme  -target-feature +sme2 -target-feature +sme-lutv2 -S -disable-O0-optnone -Werror -Wall -o /dev/null %s

// REQUIRES: aarch64-registered-target

#ifdef SVE_OVERLOADED_FORMS
#define SVE_ACLE_FUNC(A1,A2_UNUSED) A1
#else
#define SVE_ACLE_FUNC(A1,A2) A1##A2
#endif

#include <arm_sme.h>

// CHECK-LABEL: define dso_local void @test_write_lane_zt_u8_1(
// CHECK-SAME: <vscale x 16 x i8> [[V:%.*]]) #[[ATTR0:[0-9]+]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv16i8(i32 0, <vscale x 16 x i8> [[V]], i32 1)
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z23test_write_lane_zt_u8_1u11__SVUint8_t(
// CPP-CHECK-SAME: <vscale x 16 x i8> [[V:%.*]]) #[[ATTR0:[0-9]+]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv16i8(i32 0, <vscale x 16 x i8> [[V]], i32 1)
// CPP-CHECK-NEXT:    ret void
//
void test_write_lane_zt_u8_1(svuint8_t v) __arm_streaming __arm_inout("zt0") {
  SVE_ACLE_FUNC(svwrite_lane_zt, _u8)(0, v, 1);
}

// CHECK-LABEL: define dso_local void @test_write_lane_zt_s8_2(
// CHECK-SAME: <vscale x 16 x i8> [[V:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv16i8(i32 0, <vscale x 16 x i8> [[V]], i32 2)
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z23test_write_lane_zt_s8_2u10__SVInt8_t(
// CPP-CHECK-SAME: <vscale x 16 x i8> [[V:%.*]]) #[[ATTR0]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv16i8(i32 0, <vscale x 16 x i8> [[V]], i32 2)
// CPP-CHECK-NEXT:    ret void
//
void test_write_lane_zt_s8_2(svint8_t v) __arm_streaming __arm_inout("zt0") {
  SVE_ACLE_FUNC(svwrite_lane_zt, _s8)(0, v, 2);
}

// CHECK-LABEL: define dso_local void @test_write_lane_zt_u16_3(
// CHECK-SAME: <vscale x 8 x i16> [[V:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv8i16(i32 0, <vscale x 8 x i16> [[V]], i32 1)
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z24test_write_lane_zt_u16_3u12__SVUint16_t(
// CPP-CHECK-SAME: <vscale x 8 x i16> [[V:%.*]]) #[[ATTR0]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv8i16(i32 0, <vscale x 8 x i16> [[V]], i32 1)
// CPP-CHECK-NEXT:    ret void
//
void test_write_lane_zt_u16_3(svuint16_t v) __arm_streaming __arm_inout("zt0") {
  SVE_ACLE_FUNC(svwrite_lane_zt, _u16)(0, v, 1);
}

// CHECK-LABEL: define dso_local void @test_write_lane_zt_s16_1(
// CHECK-SAME: <vscale x 8 x i16> [[V:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv8i16(i32 0, <vscale x 8 x i16> [[V]], i32 1)
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z24test_write_lane_zt_s16_1u11__SVInt16_t(
// CPP-CHECK-SAME: <vscale x 8 x i16> [[V:%.*]]) #[[ATTR0]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv8i16(i32 0, <vscale x 8 x i16> [[V]], i32 1)
// CPP-CHECK-NEXT:    ret void
//
void test_write_lane_zt_s16_1(svint16_t v) __arm_streaming __arm_inout("zt0") {
  SVE_ACLE_FUNC(svwrite_lane_zt, _s16)(0, v, 1);
}

// CHECK-LABEL: define dso_local void @test_write_lane_zt_u32_2(
// CHECK-SAME: <vscale x 4 x i32> [[V:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv4i32(i32 0, <vscale x 4 x i32> [[V]], i32 2)
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z24test_write_lane_zt_u32_2u12__SVUint32_t(
// CPP-CHECK-SAME: <vscale x 4 x i32> [[V:%.*]]) #[[ATTR0]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv4i32(i32 0, <vscale x 4 x i32> [[V]], i32 2)
// CPP-CHECK-NEXT:    ret void
//
void test_write_lane_zt_u32_2(svuint32_t v) __arm_streaming __arm_inout("zt0") {
  SVE_ACLE_FUNC(svwrite_lane_zt, _u32)(0, v, 2);
}

// CHECK-LABEL: define dso_local void @test_write_lane_zt_s32_3(
// CHECK-SAME: <vscale x 4 x i32> [[V:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv4i32(i32 0, <vscale x 4 x i32> [[V]], i32 3)
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z24test_write_lane_zt_s32_3u11__SVInt32_t(
// CPP-CHECK-SAME: <vscale x 4 x i32> [[V:%.*]]) #[[ATTR0]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv4i32(i32 0, <vscale x 4 x i32> [[V]], i32 3)
// CPP-CHECK-NEXT:    ret void
//
void test_write_lane_zt_s32_3(svint32_t v) __arm_streaming __arm_inout("zt0") {
  SVE_ACLE_FUNC(svwrite_lane_zt, _s32)(0, v, 3);
}

// CHECK-LABEL: define dso_local void @test_write_lane_zt_u64_0(
// CHECK-SAME: <vscale x 2 x i64> [[V:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv2i64(i32 0, <vscale x 2 x i64> [[V]], i32 1)
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z24test_write_lane_zt_u64_0u12__SVUint64_t(
// CPP-CHECK-SAME: <vscale x 2 x i64> [[V:%.*]]) #[[ATTR0]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv2i64(i32 0, <vscale x 2 x i64> [[V]], i32 1)
// CPP-CHECK-NEXT:    ret void
//
void test_write_lane_zt_u64_0(svuint64_t v) __arm_streaming __arm_inout("zt0") {
  SVE_ACLE_FUNC(svwrite_lane_zt, _u64)(0, v, 1);
}

// CHECK-LABEL: define dso_local void @test_write_lane_zt_s64_1(
// CHECK-SAME: <vscale x 2 x i64> [[V:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv2i64(i32 0, <vscale x 2 x i64> [[V]], i32 1)
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z24test_write_lane_zt_s64_1u11__SVInt64_t(
// CPP-CHECK-SAME: <vscale x 2 x i64> [[V:%.*]]) #[[ATTR0]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv2i64(i32 0, <vscale x 2 x i64> [[V]], i32 1)
// CPP-CHECK-NEXT:    ret void
//
void test_write_lane_zt_s64_1(svint64_t v) __arm_streaming __arm_inout("zt0") {
  SVE_ACLE_FUNC(svwrite_lane_zt, _s64)(0, v, 1);
}

// CHECK-LABEL: define dso_local void @test_write_lane_zt_f16_2(
// CHECK-SAME: <vscale x 8 x half> [[V:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv8f16(i32 0, <vscale x 8 x half> [[V]], i32 2)
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z24test_write_lane_zt_f16_2u13__SVFloat16_t(
// CPP-CHECK-SAME: <vscale x 8 x half> [[V:%.*]]) #[[ATTR0]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv8f16(i32 0, <vscale x 8 x half> [[V]], i32 2)
// CPP-CHECK-NEXT:    ret void
//
void test_write_lane_zt_f16_2(svfloat16_t v) __arm_streaming __arm_inout("zt0") {
  SVE_ACLE_FUNC(svwrite_lane_zt, _f16)(0, v, 2);
}

// CHECK-LABEL: define dso_local void @test_write_lane_zt_bf16_3(
// CHECK-SAME: <vscale x 8 x bfloat> [[V:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv8bf16(i32 0, <vscale x 8 x bfloat> [[V]], i32 3)
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z25test_write_lane_zt_bf16_3u14__SVBfloat16_t(
// CPP-CHECK-SAME: <vscale x 8 x bfloat> [[V:%.*]]) #[[ATTR0]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv8bf16(i32 0, <vscale x 8 x bfloat> [[V]], i32 3)
// CPP-CHECK-NEXT:    ret void
//
void test_write_lane_zt_bf16_3(svbfloat16_t v) __arm_streaming __arm_inout("zt0") {
  SVE_ACLE_FUNC(svwrite_lane_zt, _bf16)(0, v, 3);
}

// CHECK-LABEL: define dso_local void @test_write_lane_zt_f32_0(
// CHECK-SAME: <vscale x 4 x float> [[V:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv4f32(i32 0, <vscale x 4 x float> [[V]], i32 2)
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z24test_write_lane_zt_f32_0u13__SVFloat32_t(
// CPP-CHECK-SAME: <vscale x 4 x float> [[V:%.*]]) #[[ATTR0]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv4f32(i32 0, <vscale x 4 x float> [[V]], i32 2)
// CPP-CHECK-NEXT:    ret void
//
void test_write_lane_zt_f32_0(svfloat32_t v) __arm_streaming __arm_inout("zt0") {
  SVE_ACLE_FUNC(svwrite_lane_zt, _f32)(0, v, 2);
}

// CHECK-LABEL: define dso_local void @test_write_lane_zt_f64_1(
// CHECK-SAME: <vscale x 2 x double> [[V:%.*]]) #[[ATTR0]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv2f64(i32 0, <vscale x 2 x double> [[V]], i32 1)
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z24test_write_lane_zt_f64_1u13__SVFloat64_t(
// CPP-CHECK-SAME: <vscale x 2 x double> [[V:%.*]]) #[[ATTR0]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.lane.zt.nxv2f64(i32 0, <vscale x 2 x double> [[V]], i32 1)
// CPP-CHECK-NEXT:    ret void
//
void test_write_lane_zt_f64_1(svfloat64_t v) __arm_streaming __arm_inout("zt0") {
  SVE_ACLE_FUNC(svwrite_lane_zt, _f64)(0, v, 1);
}

//ALIAS
// CHECK-LABEL: define dso_local void @test_write_zt_u8(
// CHECK-SAME: <vscale x 16 x i8> [[V:%.*]]) #[[ATTR2:[0-9]+]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv16i8(i32 0, <vscale x 16 x i8> [[V]])
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z16test_write_zt_u8u11__SVUint8_t(
// CPP-CHECK-SAME: <vscale x 16 x i8> [[V:%.*]]) #[[ATTR2:[0-9]+]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv16i8(i32 0, <vscale x 16 x i8> [[V]])
// CPP-CHECK-NEXT:    ret void
//
void test_write_zt_u8(svuint8_t v) __arm_streaming __arm_out("zt0") {
  SVE_ACLE_FUNC(svwrite_zt, _u8)(0, v);
}

// CHECK-LABEL: define dso_local void @test_write_zt_s8(
// CHECK-SAME: <vscale x 16 x i8> [[V:%.*]]) #[[ATTR2]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv16i8(i32 0, <vscale x 16 x i8> [[V]])
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z16test_write_zt_s8u10__SVInt8_t(
// CPP-CHECK-SAME: <vscale x 16 x i8> [[V:%.*]]) #[[ATTR2]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv16i8(i32 0, <vscale x 16 x i8> [[V]])
// CPP-CHECK-NEXT:    ret void
//
void test_write_zt_s8(svint8_t v) __arm_streaming __arm_out("zt0") {
  SVE_ACLE_FUNC(svwrite_zt, _s8)(0, v);
}

// CHECK-LABEL: define dso_local void @test_write_zt_u16(
// CHECK-SAME: <vscale x 8 x i16> [[V:%.*]]) #[[ATTR2]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv8i16(i32 0, <vscale x 8 x i16> [[V]])
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z17test_write_zt_u16u12__SVUint16_t(
// CPP-CHECK-SAME: <vscale x 8 x i16> [[V:%.*]]) #[[ATTR2]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv8i16(i32 0, <vscale x 8 x i16> [[V]])
// CPP-CHECK-NEXT:    ret void
//
void test_write_zt_u16(svuint16_t v) __arm_streaming __arm_out("zt0") {
  SVE_ACLE_FUNC(svwrite_zt, _u16)(0, v);
}

// CHECK-LABEL: define dso_local void @test_write_zt_s16(
// CHECK-SAME: <vscale x 8 x i16> [[V:%.*]]) #[[ATTR2]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv8i16(i32 0, <vscale x 8 x i16> [[V]])
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z17test_write_zt_s16u11__SVInt16_t(
// CPP-CHECK-SAME: <vscale x 8 x i16> [[V:%.*]]) #[[ATTR2]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv8i16(i32 0, <vscale x 8 x i16> [[V]])
// CPP-CHECK-NEXT:    ret void
//
void test_write_zt_s16(svint16_t v) __arm_streaming __arm_out("zt0") {
  SVE_ACLE_FUNC(svwrite_zt, _s16)(0, v);
}

// CHECK-LABEL: define dso_local void @test_write_zt_u32(
// CHECK-SAME: <vscale x 4 x i32> [[V:%.*]]) #[[ATTR2]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv4i32(i32 0, <vscale x 4 x i32> [[V]])
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z17test_write_zt_u32u12__SVUint32_t(
// CPP-CHECK-SAME: <vscale x 4 x i32> [[V:%.*]]) #[[ATTR2]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv4i32(i32 0, <vscale x 4 x i32> [[V]])
// CPP-CHECK-NEXT:    ret void
//
void test_write_zt_u32(svuint32_t v) __arm_streaming __arm_out("zt0") {
  SVE_ACLE_FUNC(svwrite_zt, _u32)(0, v);
}

// CHECK-LABEL: define dso_local void @test_write_zt_s32(
// CHECK-SAME: <vscale x 4 x i32> [[V:%.*]]) #[[ATTR2]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv4i32(i32 0, <vscale x 4 x i32> [[V]])
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z17test_write_zt_s32u11__SVInt32_t(
// CPP-CHECK-SAME: <vscale x 4 x i32> [[V:%.*]]) #[[ATTR2]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv4i32(i32 0, <vscale x 4 x i32> [[V]])
// CPP-CHECK-NEXT:    ret void
//
void test_write_zt_s32(svint32_t v) __arm_streaming __arm_out("zt0") {
  SVE_ACLE_FUNC(svwrite_zt, _s32)(0, v);
}

// CHECK-LABEL: define dso_local void @test_write_zt_u64(
// CHECK-SAME: <vscale x 2 x i64> [[V:%.*]]) #[[ATTR2]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv2i64(i32 0, <vscale x 2 x i64> [[V]])
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z17test_write_zt_u64u12__SVUint64_t(
// CPP-CHECK-SAME: <vscale x 2 x i64> [[V:%.*]]) #[[ATTR2]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv2i64(i32 0, <vscale x 2 x i64> [[V]])
// CPP-CHECK-NEXT:    ret void
//
void test_write_zt_u64(svuint64_t v) __arm_streaming __arm_out("zt0") {
  SVE_ACLE_FUNC(svwrite_zt, _u64)(0, v);
}

// CHECK-LABEL: define dso_local void @test_write_zt_s64(
// CHECK-SAME: <vscale x 2 x i64> [[V:%.*]]) #[[ATTR2]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv2i64(i32 0, <vscale x 2 x i64> [[V]])
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z17test_write_zt_s64u11__SVInt64_t(
// CPP-CHECK-SAME: <vscale x 2 x i64> [[V:%.*]]) #[[ATTR2]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv2i64(i32 0, <vscale x 2 x i64> [[V]])
// CPP-CHECK-NEXT:    ret void
//
void test_write_zt_s64(svint64_t v) __arm_streaming __arm_out("zt0") {
  SVE_ACLE_FUNC(svwrite_zt, _s64)(0, v);
}

// CHECK-LABEL: define dso_local void @test_write_zt_f16(
// CHECK-SAME: <vscale x 8 x half> [[V:%.*]]) #[[ATTR2]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv8f16(i32 0, <vscale x 8 x half> [[V]])
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z17test_write_zt_f16u13__SVFloat16_t(
// CPP-CHECK-SAME: <vscale x 8 x half> [[V:%.*]]) #[[ATTR2]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv8f16(i32 0, <vscale x 8 x half> [[V]])
// CPP-CHECK-NEXT:    ret void
//
void test_write_zt_f16(svfloat16_t v) __arm_streaming __arm_out("zt0") {
  SVE_ACLE_FUNC(svwrite_zt, _f16)(0, v);
}

// CHECK-LABEL: define dso_local void @test_write_zt_bf16(
// CHECK-SAME: <vscale x 8 x bfloat> [[V:%.*]]) #[[ATTR2]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv8bf16(i32 0, <vscale x 8 x bfloat> [[V]])
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z18test_write_zt_bf16u14__SVBfloat16_t(
// CPP-CHECK-SAME: <vscale x 8 x bfloat> [[V:%.*]]) #[[ATTR2]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv8bf16(i32 0, <vscale x 8 x bfloat> [[V]])
// CPP-CHECK-NEXT:    ret void
//
void test_write_zt_bf16(svbfloat16_t v) __arm_streaming __arm_out("zt0") {
  SVE_ACLE_FUNC(svwrite_zt, _bf16)(0, v);
}

// CHECK-LABEL: define dso_local void @test_write_zt_f32(
// CHECK-SAME: <vscale x 4 x float> [[V:%.*]]) #[[ATTR2]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv4f32(i32 0, <vscale x 4 x float> [[V]])
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z17test_write_zt_f32u13__SVFloat32_t(
// CPP-CHECK-SAME: <vscale x 4 x float> [[V:%.*]]) #[[ATTR2]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv4f32(i32 0, <vscale x 4 x float> [[V]])
// CPP-CHECK-NEXT:    ret void
//
void test_write_zt_f32(svfloat32_t v) __arm_streaming __arm_out("zt0") {
  SVE_ACLE_FUNC(svwrite_zt, _f32)(0, v);
}

// CHECK-LABEL: define dso_local void @test_write__zt_f64(
// CHECK-SAME: <vscale x 2 x double> [[V:%.*]]) #[[ATTR2]] {
// CHECK-NEXT:  [[ENTRY:.*:]]
// CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv2f64(i32 0, <vscale x 2 x double> [[V]])
// CHECK-NEXT:    ret void
//
// CPP-CHECK-LABEL: define dso_local void @_Z18test_write__zt_f64u13__SVFloat64_t(
// CPP-CHECK-SAME: <vscale x 2 x double> [[V:%.*]]) #[[ATTR2]] {
// CPP-CHECK-NEXT:  [[ENTRY:.*:]]
// CPP-CHECK-NEXT:    tail call void @llvm.aarch64.sme.write.zt.nxv2f64(i32 0, <vscale x 2 x double> [[V]])
// CPP-CHECK-NEXT:    ret void
//
void test_write__zt_f64(svfloat64_t v) __arm_streaming __arm_out("zt0") {
  SVE_ACLE_FUNC(svwrite_zt, _f64)(0, v);
}
