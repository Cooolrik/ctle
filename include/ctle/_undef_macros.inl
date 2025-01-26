// ctle Copyright (c) 2022 Ulrik Lindahl
// Licensed under the MIT license https://github.com/Cooolrik/ctle/blob/main/LICENSE
//
// _macros.inl & _undef_macros.inl are used to define convenience macros 
// for implementation files. 
//
// usage: include _macros.inl in implementation, *after* any other header file, so 
// as to not pollute any of those files with the macros defined. 
//
// If you *really* need to include in a header file (not recommended), include the
// _undef_macros.inl after the macros are used in the same file. This undefines/removes the
// macros from the rest of files used, and does not pollute the other parts of compilation.
//
// The _macros.inl and _undef_macros.inl can be safely included multiple times, 
// but pair each inclusion of _macros.inl with an inclusion of _undef_macros.inl, 
// before the next inclusion of _macros.inl
//

// Remove the marker that _macros.inl has been included. Also make sure that _macros.inl actually was included before the
// inclusion of _undef_macros.inl
#ifndef _CTLE_MACROS_INCLUDED
#error The _macros.inl has not been included before including the _undef_macros.inl file. 
#endif//_CTLE_MACROS_INCLUDED
#undef _CTLE_MACROS_INCLUDED

#ifndef _CTLE_FUNCTION_SIGNATURE
#error The expected macro _CTLE_FUNCTION_SIGNATURE does not exist.
#endif//_CTLE_FUNCTION_SIGNATURE
#undef _CTLE_FUNCTION_SIGNATURE

#ifndef _CTLE_STRINGIZE_DETAIL
#error The expected macro _CTLE_STRINGIZE_DETAIL does not exist.
#endif//_CTLE_STRINGIZE_DETAIL
#undef _CTLE_STRINGIZE_DETAIL

#ifndef _CTLE_STRINGIZE
#error The expected macro _CTLE_STRINGIZE does not exist.
#endif//_CTLE_STRINGIZE
#undef _CTLE_STRINGIZE

#ifndef ctLogLevel
#error The expected macro ctLogLevel does not exist.
#endif//ctLogLevel
#undef ctLogLevel

#ifndef ctLogError
#error The expected macro ctLogError does not exist.
#endif//ctLogError
#undef ctLogError

#ifndef ctLogWarning
#error The expected macro ctLogWarning does not exist.
#endif//ctLogWarning
#undef ctLogWarning

#ifndef ctLogInfo
#error The expected macro ctLogInfo does not exist.
#endif//ctLogInfo
#undef ctLogInfo

#ifndef ctLogDebug
#error The expected macro ctLogDebug does not exist.
#endif//ctLogDebug
#undef ctLogDebug

#ifndef ctLogVerbose
#error The expected macro ctLogVerbose does not exist.
#endif//ctLogVerbose
#undef ctLogVerbose

#ifndef ctLogEnd
#error The expected macro ctLogEnd does not exist.
#endif//ctLogEnd
#undef ctLogEnd

#ifndef ctLogThis
#error The expected macro ctLogThis does not exist.
#endif//ctLogThis
#undef ctLogThis

#ifndef ctValidate
#error The expected macro ctValidate does not exist.
#endif//ctValidate
#undef ctValidate

#ifndef ctValidateEnd
#error The expected macro ctValidateEnd does not exist.
#endif//ctValidateEnd
#undef ctValidateEnd

#ifndef ctSanityCheck
#error The expected macro ctSanityCheck does not exist.
#endif//ctSanityCheck
#undef ctSanityCheck

#ifndef ctStatusCall
#error The expected macro ctStatusCall does not exist.
#endif//ctStatusCall
#undef ctStatusCall

#ifndef ctStatusReturnCall
#error The expected macro ctStatusReturnCall does not exist.
#endif//ctStatusReturnCall
#undef ctStatusReturnCall

#ifndef ctStatusAutoReturnCall
#error The expected macro ctStatusAutoReturnCall does not exist.
#endif//ctStatusAutoReturnCall
#undef ctStatusAutoReturnCall

#ifndef ctStatusCallThrow
#error The expected macro ctStatusCallThrow does not exist.
#endif//ctStatusCallThrow
#undef ctStatusCallThrow

