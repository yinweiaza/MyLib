#pragma once

/// If defined, indicates that Json use exception to report invalid type manipulation
/// instead of C assert macro.
# define JSON_USE_EXCEPTION 1

#ifdef NOUSE_JSON_API_IMPORT_EXPORT
#define JSON_API 
#else
#define JSON_API T9_EXT_CLASS
#endif
