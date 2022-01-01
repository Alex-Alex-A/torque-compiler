// Copyright 2018 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_OBJECTS_CALL_SITE_INFO_H_
#define V8_OBJECTS_CALL_SITE_INFO_H_

#include "src/objects/struct.h"
#include "torque-generated/bit-fields.h"

// Has to be the last include (doesn't have include guards):
#include "src/objects/object-macros.h"

namespace v8 {
namespace internal {

class MessageLocation;
class WasmInstanceObject;
class StructBodyDescriptor;

#include "torque-generated/src/objects/call-site-info-tq.inc"

class CallSiteInfo : public TorqueGeneratedCallSiteInfo<CallSiteInfo, Struct> {
 public:
  NEVER_READ_ONLY_SPACE
  DEFINE_TORQUE_GENERATED_CALL_SITE_INFO_FLAGS()

#if V8_ENABLE_WEBASSEMBLY
  inline bool IsWasm() const;
  inline bool IsAsmJsWasm() const;
  inline bool IsAsmJsAtNumberConversion() const;
#endif  // V8_ENABLE_WEBASSEMBLY

  inline bool IsStrict() const;
  inline bool IsConstructor() const;
  inline bool IsAsync() const;
  bool IsEval() const;
  bool IsUserJavaScript() const;
  bool IsMethodCall() const;
  bool IsToplevel() const;
  bool IsPromiseAll() const;
  bool IsPromiseAny() const;
  bool IsNative() const;

  DECL_ACCESSORS(code_object, HeapObject)

  // Dispatched behavior.
  DECL_VERIFIER(CallSiteInfo)

  // Used to signal that the requested field is unknown.
  static constexpr int kUnknown = kNoSourcePosition;

  V8_EXPORT_PRIVATE static int GetLineNumber(Handle<CallSiteInfo> info);
  V8_EXPORT_PRIVATE static int GetColumnNumber(Handle<CallSiteInfo> info);

  static int GetEnclosingLineNumber(Handle<CallSiteInfo> info);
  static int GetEnclosingColumnNumber(Handle<CallSiteInfo> info);

  // Returns the script ID if one is attached,
  // Message::kNoScriptIdInfo otherwise.
  int GetScriptId() const;
  Object GetScriptName() const;
  Object GetScriptNameOrSourceURL() const;
  Object GetScriptSource() const;
  Object GetScriptSourceMappingURL() const;

  static Handle<PrimitiveHeapObject> GetEvalOrigin(Handle<CallSiteInfo> info);
  V8_EXPORT_PRIVATE static Handle<Object> GetFunctionName(
      Handle<CallSiteInfo> info);
  static Handle<Object> GetMethodName(Handle<CallSiteInfo> info);
  static Handle<Object> GetTypeName(Handle<CallSiteInfo> info);

#if V8_ENABLE_WEBASSEMBLY
  // These methods are only valid for Wasm and asm.js Wasm frames.
  uint32_t GetWasmFunctionIndex() const;
  WasmInstanceObject GetWasmInstance() const;
  static Handle<Object> GetWasmModuleName(Handle<CallSiteInfo> info);
#endif  // V8_ENABLE_WEBASSEMBLY

  // Returns the 0-based source position, which is the offset into the
  // Script in case of JavaScript and Asm.js, and the bytecode offset
  // in the module in case of actual Wasm. In case of async promise
  // combinator frames, this returns the index of the promise.
  static int GetSourcePosition(Handle<CallSiteInfo> info);

  // Attempts to fill the |location| based on the |info|, and avoids
  // triggering source position table building for JavaScript frames.
  static bool ComputeLocation(Handle<CallSiteInfo> info,
                              MessageLocation* location);

  using BodyDescriptor = StructBodyDescriptor;

 private:
  static int ComputeSourcePosition(Handle<CallSiteInfo> info, int offset);

  base::Optional<Script> GetScript() const;
  SharedFunctionInfo GetSharedFunctionInfo() const;

  static MaybeHandle<Script> GetScript(Isolate* isolate,
                                       Handle<CallSiteInfo> info);

  TQ_OBJECT_CONSTRUCTORS(CallSiteInfo)
};

class IncrementalStringBuilder;
void SerializeCallSiteInfo(Isolate* isolate, Handle<CallSiteInfo> frame,
                           IncrementalStringBuilder* builder);
V8_EXPORT_PRIVATE
MaybeHandle<String> SerializeCallSiteInfo(Isolate* isolate,
                                          Handle<CallSiteInfo> frame);

}  // namespace internal
}  // namespace v8

#include "src/objects/object-macros-undef.h"

#endif  // V8_OBJECTS_CALL_SITE_INFO_H_
