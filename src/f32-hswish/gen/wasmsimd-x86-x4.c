// Auto-generated file. Do not edit!
//   Template: src/f32-hswish/wasmsimd.c.in
//   Generator: tools/xngen
//
// Copyright 2020 Google LLC
//
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree.

#include <assert.h>

#include <wasm_simd128.h>

#include <xnnpack/common.h>
#include <xnnpack/hswish.h>


void xnn_f32_hswish_ukernel__wasmsimd_x86_x4(
    size_t n,
    const float* x,
    float* y,
    const union xnn_f32_hswish_params params[restrict XNN_MIN_ELEMENTS(1)]) XNN_DISABLE_TSAN
{
  assert(n != 0);
  assert(n % sizeof(float) == 0);

  const v128_t vsixth = wasm_v32x4_load_splat(&params->scalar.sixth);
  const v128_t vhalf = wasm_v32x4_load_splat(&params->scalar.half);
  const v128_t vone = wasm_v32x4_load_splat(&params->scalar.one);
  const v128_t vzero = wasm_f32x4_splat(0.0f);

  for (; n >= 4 * sizeof(float); n -= 4 * sizeof(float)) {
    const v128_t vx = wasm_v128_load(x);
    x += 4;
    v128_t vacc = wasm_f32x4_add(vhalf, wasm_f32x4_mul(vx, vsixth));

    const v128_t vmasklt = wasm_f32x4_lt(vacc, vzero);
    vacc = wasm_v128_andnot(vacc, vmasklt);
    const v128_t vmaskge = wasm_f32x4_ge(vacc, vone);
    vacc = wasm_f32x4_mul(vacc, vx);
    vacc = wasm_v128_bitselect(vx, vacc, vmaskge);

    wasm_v128_store(y, vacc);
    y += 4;
  }
  if XNN_UNLIKELY(n != 0) {
    const v128_t vx = wasm_v128_load(x);
    v128_t vacc = wasm_f32x4_add(vhalf, wasm_f32x4_mul(vx, vsixth));

    const v128_t vmasklt = wasm_f32x4_lt(vacc, vzero);
    vacc = wasm_v128_andnot(vacc, vmasklt);
    const v128_t vmaskge = wasm_f32x4_ge(vacc, vone);
    vacc = wasm_f32x4_mul(vacc, vx);
    vacc = wasm_v128_bitselect(vx, vacc, vmaskge);

    if (n & (2 * sizeof(float))) {
      *((double*) y) = wasm_f64x2_extract_lane(vacc, 0);
      vacc = wasm_v32x4_shuffle(vacc, vacc, 2, 3, 2, 3);
      y += 2;
    }
    if (n & (1 * sizeof(float))) {
      *y = wasm_f32x4_extract_lane(vacc, 0);
    }
  }
}
