# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: Result.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='Result.proto',
  package='Persistance',
  syntax='proto3',
  serialized_options=None,
  serialized_pb=_b('\n\x0cResult.proto\x12\x0bPersistance\"7\n\x06Result\x12\x17\n\x0flight_intensity\x18\x01 \x01(\x02\x12\x14\n\x0cis_converged\x18\x02 \x01(\x08\x62\x06proto3')
)




_RESULT = _descriptor.Descriptor(
  name='Result',
  full_name='Persistance.Result',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='light_intensity', full_name='Persistance.Result.light_intensity', index=0,
      number=1, type=2, cpp_type=6, label=1,
      has_default_value=False, default_value=float(0),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='is_converged', full_name='Persistance.Result.is_converged', index=1,
      number=2, type=8, cpp_type=7, label=1,
      has_default_value=False, default_value=False,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=29,
  serialized_end=84,
)

DESCRIPTOR.message_types_by_name['Result'] = _RESULT
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

Result = _reflection.GeneratedProtocolMessageType('Result', (_message.Message,), dict(
  DESCRIPTOR = _RESULT,
  __module__ = 'Result_pb2'
  # @@protoc_insertion_point(class_scope:Persistance.Result)
  ))
_sym_db.RegisterMessage(Result)


# @@protoc_insertion_point(module_scope)
