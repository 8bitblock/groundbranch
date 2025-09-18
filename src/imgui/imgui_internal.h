// dear imgui, v1.92.3 WIP
// (internal structures/api)

// You may use this file to debug, understand or extend Dear ImGui features but
we don't provide any guarantee of forward compatibility.

/*

Index of this file:

// [SECTION] Header mess
// [SECTION] Forward declarations
// [SECTION] Context pointer
// [SECTION] STB libraries includes
// [SECTION] Macros
// [SECTION] Generic helpers
// [SECTION] ImDrawList support
// [SECTION] Style support
// [SECTION] Data types support
// [SECTION] Widgets support: flags, enums, data structures
// [SECTION] Popup support
// [SECTION] Inputs support
// [SECTION] Clipper support
// [SECTION] Navigation support
// [SECTION] Typing-select support
// [SECTION] Columns support
// [SECTION] Box-select support
// [SECTION] Multi-select support
// [SECTION] Docking support
// [SECTION] Viewport support
// [SECTION] Settings support
// [SECTION] Localization support
// [SECTION] Error handling, State recovery support
// [SECTION] Metrics, Debug tools
// [SECTION] Generic context hooks
// [SECTION] ImGuiContext (main imgui context)
// [SECTION] ImGuiWindowTempData, ImGuiWindow
// [SECTION] Tab bar, Tab item support
// [SECTION] Table support
// [SECTION] ImGui internal API
// [SECTION] ImFontLoader
// [SECTION] ImFontAtlas internal API
// [SECTION] Test Engine specific hooks (imgui_test_engine)

*/

#pragma once
#ifndef IMGUI_DISABLE

//-----------------------------------------------------------------------------
// [SECTION] Header mess
//-----------------------------------------------------------------------------

#ifndef IMGUI_VERSION
#include "imgui.h"
#endif

#include <stdio.h>      // FILE*, sscanf
#include <stdlib.h>     // NULL, malloc, free, qsort, atoi, atof
#include <math.h>       // sqrtf, fabsf, fmodf, powf, floorf, ceilf, cosf, sinf
#include <limits.h>     // INT_MIN, INT_MAX

// Enable SSE intrinsics if available
#if (defined __SSE__ || defined __x86_64__ || defined _M_X64 || (defined(_M_IX86
_FP) && (_M_IX86_FP >= 1))) && !defined(IMGUI_DISABLE_SSE)
#define IMGUI_ENABLE_SSE
#include <immintrin.h>
#if (defined __AVX__ || defined __SSE4_2__)
#define IMGUI_ENABLE_SSE4_2
#include <nmmintrin.h>
#endif
#endif
// Emscripten has partial SSE 4.2 support where _mm_crc32_u32 is not available.
See https://emscripten.org/docs/porting/simd.html#id11 and #8213
#if defined(IMGUI_ENABLE_SSE4_2) && !defined(IMGUI_USE_LEGACY_CRC32_ADLER) && !d
efined(__EMSCRIPTEN__)
#define IMGUI_ENABLE_SSE4_2_CRC
#endif

// Visual Studio warnings
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4251)     // class 'xxx' needs to have dll-interface t
o be used by clients of struct 'xxx' // when IMGUI_API is set to__declspec(dllex
port)
#pragma warning (disable: 26495)    // [Static Analyzer] Variable 'XXX' is unini
tialized. Always initialize a member variable (type.6).
#pragma warning (disable: 26812)    // [Static Analyzer] The enum type 'xxx' is
unscoped. Prefer 'enum class' over 'enum' (Enum.3).
#if defined(_MSC_VER) && _MSC_VER >= 1922 // MSVC 2019 16.2 or later
#pragma warning (disable: 5054)     // operator '|': deprecated between enumerat
ions of different types
#endif
#endif

// Clang/GCC warnings with -Weverything
#if defined(__clang__)
#pragma clang diagnostic push
#if __has_warning("-Wunknown-warning-option")
#pragma clang diagnostic ignored "-Wunknown-warning-option"         // warning:
unknown warning group 'xxx'
#endif
#pragma clang diagnostic ignored "-Wunknown-pragmas"                // warning:
unknown warning group 'xxx'
#pragma clang diagnostic ignored "-Wfloat-equal"                    // warning:
comparing floating point with == or != is unsafe // storing and comparing agains
t same constants ok, for ImFloor()
#pragma clang diagnostic ignored "-Wold-style-cast"                 // warning:
use of old-style cast
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"  // warning:
zero as null pointer constant
#pragma clang diagnostic ignored "-Wdouble-promotion"               // warning:
implicit conversion from 'float' to 'double' when passing argument to function
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"  // warning:
implicit conversion from 'xxx' to 'float' may lose precision
#pragma clang diagnostic ignored "-Wmissing-noreturn"               // warning:
function 'xxx' could be declared with attribute 'noreturn'
#pragma clang diagnostic ignored "-Wdeprecated-enum-enum-conversion"// warning:
bitwise operation between different enumeration types ('XXXFlags_' and 'XXXFlags
Private_') is deprecated
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"            // warning:
'xxx' is an unsafe pointer used for buffer access
#pragma clang diagnostic ignored "-Wnontrivial-memaccess"           // warning:
first argument in call to 'memset' is a pointer to non-trivially copyable type
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"                          // warning:
unknown option after '#pragma GCC diagnostic' kind
#pragma GCC diagnostic ignored "-Wfloat-equal"                      // warning:
comparing floating-point with '==' or '!=' is unsafe
#pragma GCC diagnostic ignored "-Wclass-memaccess"                  // [__GNUC__
 >= 8] warning: 'memset/memcpy' clearing/writing an object of type 'xxxx' with n
o trivial copy-assignment; use assignment or value-initialization instead
#pragma GCC diagnostic ignored "-Wdeprecated-enum-enum-conversion"  // warning:
bitwise operation between different enumeration types ('XXXFlags_' and 'XXXFlags
Private_') is deprecated
#endif

// In 1.89.4, we moved the implementation of "courtesy maths operators" from img
ui_internal.h in imgui.h
// As they are frequently requested, we do not want to encourage to many people
using imgui_internal.h
#if defined(IMGUI_DEFINE_MATH_OPERATORS) && !defined(IMGUI_DEFINE_MATH_OPERATORS
_IMPLEMENTED)
#error Please '#define IMGUI_DEFINE_MATH_OPERATORS' _BEFORE_ including imgui.h!
#endif

// Legacy defines
#ifdef IMGUI_DISABLE_FORMAT_STRING_FUNCTIONS            // Renamed in 1.74
#error Use IMGUI_DISABLE_DEFAULT_FORMAT_FUNCTIONS
#endif
#ifdef IMGUI_DISABLE_MATH_FUNCTIONS                     // Renamed in 1.74
#error Use IMGUI_DISABLE_DEFAULT_MATH_FUNCTIONS
#endif

// Enable stb_truetype by default unless FreeType is enabled.
// You can compile with both by defining both IMGUI_ENABLE_FREETYPE and IMGUI_EN
ABLE_STB_TRUETYPE together.
#ifndef IMGUI_ENABLE_FREETYPE
#define IMGUI_ENABLE_STB_TRUETYPE
#endif

//-----------------------------------------------------------------------------
// [SECTION] Forward declarations
//-----------------------------------------------------------------------------

// Utilities
// (other types which are not forwarded declared are: ImBitArray<>, ImSpan<>, Im
SpanAllocator<>, ImStableVector<>, ImPool<>, ImChunkStream<>)
struct ImBitVector;                 // Store 1-bit per value
struct ImRect;                      // An axis-aligned rectangle (2 points)
struct ImGuiTextIndex;              // Maintain a line index for a text buffer.

// ImDrawList/ImFontAtlas
struct ImDrawDataBuilder;           // Helper to build a ImDrawData instance
struct ImDrawListSharedData;        // Data shared between all ImDrawList instan
ces
struct ImFontAtlasBuilder;          // Internal storage for incrementally packin
g and building a ImFontAtlas
struct ImFontAtlasPostProcessData;  // Data available to potential texture post-
processing functions
struct ImFontAtlasRectEntry;        // Packed rectangle lookup entry

// ImGui
struct ImGuiBoxSelectState;         // Box-selection state (currently used by mu
lti-selection, could potentially be used by others)
struct ImGuiColorMod;               // Stacked color modifier, backup of modifie
d data so we can restore it
struct ImGuiContext;                // Main Dear ImGui context
struct ImGuiContextHook;            // Hook for extensions like ImGuiTestEngine
struct ImGuiDataTypeInfo;           // Type information associated to a ImGuiDat
aType enum
struct ImGuiDeactivatedItemData;    // Data for IsItemDeactivated()/IsItemDeacti
vatedAfterEdit() function.
struct ImGuiErrorRecoveryState;     // Storage of stack sizes for error handling
 and recovery
struct ImGuiGroupData;              // Stacked storage data for BeginGroup()/End
Group()
struct ImGuiInputTextState;         // Internal state of the currently focused/e
dited text input box
struct ImGuiInputTextDeactivateData;// Short term storage to backup text of a de
activating InputText() while another is stealing active id
struct ImGuiLastItemData;           // Status storage for last submitted items
struct ImGuiLocEntry;               // A localization entry.
struct ImGuiMenuColumns;            // Simple column measurement, currently used
 for MenuItem() only
struct ImGuiMultiSelectState;       // Multi-selection persistent state (for foc
used selection).
struct ImGuiMultiSelectTempData;    // Multi-selection temporary state (while tr
aversing).
struct ImGuiNavItemData;            // Result of a keyboard/gamepad directional
navigation move query result
struct ImGuiMetricsConfig;          // Storage for ShowMetricsWindow() and Debug
NodeXXX() functions
struct ImGuiNextWindowData;         // Storage for SetNextWindow** functions
struct ImGuiNextItemData;           // Storage for SetNextItem** functions
struct ImGuiOldColumnData;          // Storage data for a single column for lega
cy Columns() api
struct ImGuiOldColumns;             // Storage data for a columns set for legacy
 Columns() api
struct ImGuiPopupData;              // Storage for current popup stack
struct ImGuiSettingsHandler;        // Storage for one type registered in the .i
ni file
struct ImGuiStyleMod;               // Stacked style modifier, backup of modifie
d data so we can restore it
struct ImGuiStyleVarInfo;           // Style variable information (e.g. to acces
s style variables from an enum)
struct ImGuiTabBar;                 // Storage for a tab bar
struct ImGuiTabItem;                // Storage for a tab item (within a tab bar)
struct ImGuiTable;                  // Storage for a table
struct ImGuiTableHeaderData;        // Storage for TableAngledHeadersRow()
struct ImGuiTableColumn;            // Storage for one column of a table
struct ImGuiTableInstanceData;      // Storage for one instance of a same table
struct ImGuiTableTempData;          // Temporary storage for one table (one per
table in the stack), shared between tables.
struct ImGuiTableSettings;          // Storage for a table .ini settings
struct ImGuiTableColumnsSettings;   // Storage for a column .ini settings
struct ImGuiTreeNodeStackData;      // Temporary storage for TreeNode().
struct ImGuiTypingSelectState;      // Storage for GetTypingSelectRequest()
struct ImGuiTypingSelectRequest;    // Storage for GetTypingSelectRequest() (aim
ed to be public)
struct ImGuiWindow;                 // Storage for one window
struct ImGuiWindowTempData;         // Temporary storage for one window (that's
the data which in theory we could ditch at the end of the frame, in practice we
currently keep it for each window)
struct ImGuiWindowSettings;         // Storage for a window .ini settings (we ke
ep one of those even if the actual window wasn't instanced during this session)

// Enumerations
// Use your programming IDE "Go to definition" facility on the names of the cent
er columns to find the actual flags/enum lists.
enum ImGuiLocKey : int;                 // -> enum ImGuiLocKey              // E
num: a localization entry for translation.
typedef int ImGuiLayoutType;            // -> enum ImGuiLayoutType_         // E
num: Horizontal or vertical

// Flags
typedef int ImDrawTextFlags;            // -> enum ImDrawTextFlags_         // F
lags: for ImTextCalcWordWrapPositionEx()
typedef int ImGuiActivateFlags;         // -> enum ImGuiActivateFlags_      // F
lags: for navigation/focus function (will be for ActivateItem() later)
typedef int ImGuiDebugLogFlags;         // -> enum ImGuiDebugLogFlags_      // F
lags: for ShowDebugLogWindow(), g.DebugLogFlags
typedef int ImGuiFocusRequestFlags;     // -> enum ImGuiFocusRequestFlags_  // F
lags: for FocusWindow()
typedef int ImGuiItemStatusFlags;       // -> enum ImGuiItemStatusFlags_    // F
lags: for g.LastItemData.StatusFlags
typedef int ImGuiOldColumnFlags;        // -> enum ImGuiOldColumnFlags_     // F
lags: for BeginColumns()
typedef int ImGuiLogFlags;              // -> enum ImGuiLogFlags_           // F
lags: for LogBegin() text capturing function
typedef int ImGuiNavRenderCursorFlags;  // -> enum ImGuiNavRenderCursorFlags_//F
lags: for RenderNavCursor()
typedef int ImGuiNavMoveFlags;          // -> enum ImGuiNavMoveFlags_       // F
lags: for navigation requests
typedef int ImGuiNextItemDataFlags;     // -> enum ImGuiNextItemDataFlags_  // F
lags: for SetNextItemXXX() functions
typedef int ImGuiNextWindowDataFlags;   // -> enum ImGuiNextWindowDataFlags_// F
lags: for SetNextWindowXXX() functions
typedef int ImGuiScrollFlags;           // -> enum ImGuiScrollFlags_        // F
lags: for ScrollToItem() and navigation requests
typedef int ImGuiSeparatorFlags;        // -> enum ImGuiSeparatorFlags_     // F
lags: for SeparatorEx()
typedef int ImGuiTextFlags;             // -> enum ImGuiTextFlags_          // F
lags: for TextEx()
typedef int ImGuiTooltipFlags;          // -> enum ImGuiTooltipFlags_       // F
lags: for BeginTooltipEx()
typedef int ImGuiTypingSelectFlags;     // -> enum ImGuiTypingSelectFlags_  // F
lags: for GetTypingSelectRequest()
typedef int ImGuiWindowRefreshFlags;    // -> enum ImGuiWindowRefreshFlags_ // F
lags: for SetNextWindowRefreshPolicy()

// Table column indexing
typedef ImS16 ImGuiTableColumnIdx;
typedef ImU16 ImGuiTableDrawChannelIdx;

//-----------------------------------------------------------------------------
// [SECTION] Context pointer
// See implementation of this variable in imgui.cpp for comments and details.
//-----------------------------------------------------------------------------

#ifndef GImGui
extern IMGUI_API ImGuiContext* GImGui;  // Current implicit context pointer
#endif

//-----------------------------------------------------------------------------
// [SECTION] STB libraries includes
//-----------------------------------------------------------------------------

namespace ImStb
{
#undef STB_TEXTEDIT_STRING
#undef STB_TEXTEDIT_CHARTYPE
#define STB_TEXTEDIT_STRING             ImGuiInputTextState
#define STB_TEXTEDIT_CHARTYPE           char
#define STB_TEXTEDIT_GETWIDTH_NEWLINE   (-1.0f)
#define STB_TEXTEDIT_UNDOSTATECOUNT     99
#define STB_TEXTEDIT_UNDOCHARCOUNT      999
#include "imstb_textedit.h"
} // namespace ImStb

//-----------------------------------------------------------------------------
// [SECTION] Macros
//-----------------------------------------------------------------------------

// Debug Printing Into TTY
// (since IMGUI_VERSION_NUM >= 18729: IMGUI_DEBUG_LOG was reworked into IMGUI_DE
BUG_PRINTF (and removed framecount from it). If you were using a #define IMGUI_D
EBUG_LOG please rename)
#ifndef IMGUI_DEBUG_PRINTF
#ifndef IMGUI_DISABLE_DEFAULT_FORMAT_FUNCTIONS
#define IMGUI_DEBUG_PRINTF(_FMT,...)    printf(_FMT, __VA_ARGS__)
#else
#define IMGUI_DEBUG_PRINTF(_FMT,...)    ((void)0)
#endif
#endif

// Debug Logging for ShowDebugLogWindow(). This is designed for relatively rare
events so please don't spam.
#define IMGUI_DEBUG_LOG_ERROR(...)      do { if (g.DebugLogFlags & ImGuiDebugLog
Flags_EventError)       IMGUI_DEBUG_LOG(__VA_ARGS__); else g.DebugLogSkippedErro
rs++; } while (0)
#define IMGUI_DEBUG_LOG_ACTIVEID(...)   do { if (g.DebugLogFlags & ImGuiDebugLog
Flags_EventActiveId)    IMGUI_DEBUG_LOG(__VA_ARGS__); } while (0)
#define IMGUI_DEBUG_LOG_FOCUS(...)      do { if (g.DebugLogFlags & ImGuiDebugLog
Flags_EventFocus)       IMGUI_DEBUG_LOG(__VA_ARGS__); } while (0)
#define IMGUI_DEBUG_LOG_POPUP(...)      do { if (g.DebugLogFlags & ImGuiDebugLog
Flags_EventPopup)       IMGUI_DEBUG_LOG(__VA_ARGS__); } while (0)
#define IMGUI_DEBUG_LOG_NAV(...)        do { if (g.DebugLogFlags & ImGuiDebugLog
Flags_EventNav)         IMGUI_DEBUG_LOG(__VA_ARGS__); } while (0)
#define IMGUI_DEBUG_LOG_SELECTION(...)  do { if (g.DebugLogFlags & ImGuiDebugLog
Flags_EventSelection)   IMGUI_DEBUG_LOG(__VA_ARGS__); } while (0)
#define IMGUI_DEBUG_LOG_CLIPPER(...)    do { if (g.DebugLogFlags & ImGuiDebugLog
Flags_EventClipper)     IMGUI_DEBUG_LOG(__VA_ARGS__); } while (0)
#define IMGUI_DEBUG_LOG_IO(...)         do { if (g.DebugLogFlags & ImGuiDebugLog
Flags_EventIO)          IMGUI_DEBUG_LOG(__VA_ARGS__); } while (0)
#define IMGUI_DEBUG_LOG_FONT(...)       do { ImGuiContext* g2 = GImGui; if (g2 &
& g2->DebugLogFlags & ImGuiDebugLogFlags_EventFont) IMGUI_DEBUG_LOG(__VA_ARGS__)
; } while (0) // Called from ImFontAtlas function which may operate without a co
ntext.
#define IMGUI_DEBUG_LOG_INPUTROUTING(...) do{if (g.DebugLogFlags & ImGuiDebugLog
Flags_EventInputRouting)IMGUI_DEBUG_LOG(__VA_ARGS__); } while (0)

// Static Asserts
#define IM_STATIC_ASSERT(_COND)         static_assert(_COND, "")

// "Paranoid" Debug Asserts are meant to only be enabled during specific debuggi
ng/work, otherwise would slow down the code too much.
// We currently don't have many of those so the effect is currently negligible,
but onward intent to add more aggressive ones in the code.
//#define IMGUI_DEBUG_PARANOID
#ifdef IMGUI_DEBUG_PARANOID
#define IM_ASSERT_PARANOID(_EXPR)       IM_ASSERT(_EXPR)
#else
#define IM_ASSERT_PARANOID(_EXPR)
#endif

// Misc Macros
#define IM_PI                           3.14159265358979323846f
#ifdef _WIN32
#define IM_NEWLINE                      "\r\n"   // Play it nice with Windows us
ers (Update: since 2018-05, Notepad finally appears to support Unix-style carria
ge returns!)
#else
#define IM_NEWLINE                      "\n"
#endif
#ifndef IM_TABSIZE                      // Until we move this to runtime and/or
add proper tab support, at least allow users to compile-time override
#define IM_TABSIZE                      (4)
#endif
#define IM_MEMALIGN(_OFF,_ALIGN)        (((_OFF) + ((_ALIGN) - 1)) & ~((_ALIGN)
- 1))           // Memory align e.g. IM_ALIGN(0,4)=0, IM_ALIGN(1,4)=4, IM_ALIGN(
4,4)=4, IM_ALIGN(5,4)=8
#define IM_F32_TO_INT8_UNBOUND(_VAL)    ((int)((_VAL) * 255.0f + ((_VAL)>=0 ? 0.
5f : -0.5f)))   // Unsaturated, for display purpose
#define IM_F32_TO_INT8_SAT(_VAL)        ((int)(ImSaturate(_VAL) * 255.0f + 0.5f)
)               // Saturated, always output 0..255
#define IM_TRUNC(_VAL)                  ((float)(int)(_VAL))
                // ImTrunc() is not inlined in MSVC debug builds
#define IM_ROUND(_VAL)                  ((float)(int)((_VAL) + 0.5f))
                //
#define IM_STRINGIFY_HELPER(_X)         #_X
#define IM_STRINGIFY(_X)                IM_STRINGIFY_HELPER(_X)
                // Preprocessor idiom to stringify e.g. an integer.
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#define IM_FLOOR IM_TRUNC
#endif

// Hint for branch prediction
#if (defined(__cplusplus) && (__cplusplus >= 202002L)) || (defined(_MSVC_LANG) &
& (_MSVC_LANG >= 202002L))
#define IM_LIKELY   [[likely]]
#define IM_UNLIKELY [[unlikely]]
#else
#define IM_LIKELY
#define IM_UNLIKELY
#endif

// Enforce cdecl calling convention for functions called by the standard library
, in case compilation settings changed the default to e.g. __vectorcall
#ifdef _MSC_VER
#define IMGUI_CDECL __cdecl
#else
#define IMGUI_CDECL
#endif

// Warnings
#if defined(_MSC_VER) && !defined(__clang__)
#define IM_MSVC_WARNING_SUPPRESS(XXXX)  __pragma(warning(suppress: XXXX))
#else
#define IM_MSVC_WARNING_SUPPRESS(XXXX)
#endif

// Debug Tools
// Use 'Metrics/Debugger->Tools->Item Picker' to break into the call-stack of a
specific item.
// This will call IM_DEBUG_BREAK() which you may redefine yourself. See https://
github.com/scottt/debugbreak for more reference.
#ifndef IM_DEBUG_BREAK
#if defined (_MSC_VER)
#define IM_DEBUG_BREAK()    __debugbreak()
#elif defined(__clang__)
#define IM_DEBUG_BREAK()    __builtin_debugtrap()
#elif defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
#define IM_DEBUG_BREAK()    __asm__ volatile("int3;nop")
#elif defined(__GNUC__) && defined(__thumb__)
#define IM_DEBUG_BREAK()    __asm__ volatile(".inst 0xde01")
#elif defined(__GNUC__) && defined(__arm__) && !defined(__thumb__)
#define IM_DEBUG_BREAK()    __asm__ volatile(".inst 0xe7f001f0")
#else
#define IM_DEBUG_BREAK()    IM_ASSERT(0)    // It is expected that you define IM
_DEBUG_BREAK() into something that will break nicely in a debugger!
#endif
#endif // #ifndef IM_DEBUG_BREAK

// Format specifiers, printing 64-bit hasn't been decently standardized...
// In a real application you should be using PRId64 and PRIu64 from <inttypes.h>
 (non-windows) and on Windows define them yourself.
#if defined(_MSC_VER) && !defined(__clang__)
#define IM_PRId64   "I64d"
#define IM_PRIu64   "I64u"
#define IM_PRIX64   "I64X"
#else
#define IM_PRId64   "lld"
#define IM_PRIu64   "llu"
#define IM_PRIX64   "llX"
#endif
#define IM_TEXTUREID_TO_U64(_TEXID) ((ImU64)(intptr_t)(_TEXID))

//-----------------------------------------------------------------------------
// [SECTION] Generic helpers
// Note that the ImXXX helpers functions are lower-level than ImGui functions.
// ImGui functions or the ImGui context are never called/used from other ImXXX f
unctions.
//-----------------------------------------------------------------------------
// - Helpers: Hashing
// - Helpers: Sorting
// - Helpers: Bit manipulation
// - Helpers: String
// - Helpers: Formatting
// - Helpers: UTF-8 <> wchar conversions
// - Helpers: ImVec2/ImVec4 operators
// - Helpers: Maths
// - Helpers: Geometry
// - Helper: ImVec1
// - Helper: ImVec2ih
// - Helper: ImRect
// - Helper: ImBitArray
// - Helper: ImBitVector
// - Helper: ImSpan<>, ImSpanAllocator<>
// - Helper: ImStableVector<>
// - Helper: ImPool<>
// - Helper: ImChunkStream<>
// - Helper: ImGuiTextIndex
// - Helper: ImGuiStorage
//-----------------------------------------------------------------------------

// Helpers: Hashing
IMGUI_API ImGuiID       ImHashData(const void* data, size_t data_size, ImGuiID s
eed = 0);
IMGUI_API ImGuiID       ImHashStr(const char* data, size_t data_size = 0, ImGuiI
D seed = 0);
IMGUI_API const char*   ImHashSkipUncontributingPrefix(const char* label);

// Helpers: Sorting
#ifndef ImQsort
inline void             ImQsort(void* base, size_t count, size_t size_of_element
, int(IMGUI_CDECL *compare_func)(void const*, void const*)) { if (count > 1) qso
rt(base, count, size_of_element, compare_func); }
#endif

// Helpers: Color Blending
IMGUI_API ImU32         ImAlphaBlendColors(ImU32 col_a, ImU32 col_b);

// Helpers: Bit manipulation
inline bool             ImIsPowerOfTwo(int v)               { return v != 0 && (
v & (v - 1)) == 0; }
inline bool             ImIsPowerOfTwo(ImU64 v)             { return v != 0 && (
v & (v - 1)) == 0; }
inline int              ImUpperPowerOfTwo(int v)            { v--; v |= v >> 1;
v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16; v++; return v; }
inline unsigned int     ImCountSetBits(unsigned int v)      { unsigned int count
 = 0; while (v > 0) { v = v & (v - 1); count++; } return count; }

// Helpers: String
#define ImStrlen strlen
#define ImMemchr memchr
IMGUI_API int           ImStricmp(const char* str1, const char* str2);
            // Case insensitive compare.
IMGUI_API int           ImStrnicmp(const char* str1, const char* str2, size_t co
unt);       // Case insensitive compare to a certain count.
IMGUI_API void          ImStrncpy(char* dst, const char* src, size_t count);
            // Copy to a certain count and always zero terminate (strncpy doesn'
t).
IMGUI_API char*         ImStrdup(const char* str);
            // Duplicate a string.
IMGUI_API void*         ImMemdup(const void* src, size_t size);
            // Duplicate a chunk of memory.
IMGUI_API char*         ImStrdupcpy(char* dst, size_t* p_dst_size, const char* s
tr);        // Copy in provided buffer, recreate buffer if needed.
IMGUI_API const char*   ImStrchrRange(const char* str_begin, const char* str_end
, char c);  // Find first occurrence of 'c' in string range.
IMGUI_API const char*   ImStreolRange(const char* str, const char* str_end);
            // End end-of-line
IMGUI_API const char*   ImStristr(const char* haystack, const char* haystack_end
, const char* needle, const char* needle_end);  // Find a substring in a string
range.
IMGUI_API void          ImStrTrimBlanks(char* str);
            // Remove leading and trailing blanks from a buffer.
IMGUI_API const char*   ImStrSkipBlank(const char* str);
            // Find first non-blank character.
IMGUI_API int           ImStrlenW(const ImWchar* str);
            // Computer string length (ImWchar string)
IMGUI_API const char*   ImStrbol(const char* buf_mid_line, const char* buf_begin
);          // Find beginning-of-line
IM_MSVC_RUNTIME_CHECKS_OFF
inline char             ImToUpper(char c)               { return (c >= 'a' && c
<= 'z') ? c &= ~32 : c; }
inline bool             ImCharIsBlankA(char c)          { return c == ' ' || c =
= '\t'; }
inline bool             ImCharIsBlankW(unsigned int c)  { return c == ' ' || c =
= '\t' || c == 0x3000; }
inline bool             ImCharIsXdigitA(char c)         { return (c >= '0' && c
<= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'); }
IM_MSVC_RUNTIME_CHECKS_RESTORE

// Helpers: Formatting
IMGUI_API int           ImFormatString(char* buf, size_t buf_size, const char* f
mt, ...) IM_FMTARGS(3);
IMGUI_API int           ImFormatStringV(char* buf, size_t buf_size, const char*
fmt, va_list args) IM_FMTLIST(3);
IMGUI_API void          ImFormatStringToTempBuffer(const char** out_buf, const c
har** out_buf_end, const char* fmt, ...) IM_FMTARGS(3);
IMGUI_API void          ImFormatStringToTempBufferV(const char** out_buf, const
char** out_buf_end, const char* fmt, va_list args) IM_FMTLIST(3);
IMGUI_API const char*   ImParseFormatFindStart(const char* format);
IMGUI_API const char*   ImParseFormatFindEnd(const char* format);
IMGUI_API const char*   ImParseFormatTrimDecorations(const char* format, char* b
uf, size_t buf_size);
IMGUI_API void          ImParseFormatSanitizeForPrinting(const char* fmt_in, cha
r* fmt_out, size_t fmt_out_size);
IMGUI_API const char*   ImParseFormatSanitizeForScanning(const char* fmt_in, cha
r* fmt_out, size_t fmt_out_size);
IMGUI_API int           ImParseFormatPrecision(const char* format, int default_v
alue);

// Helpers: UTF-8 <> wchar conversions
IMGUI_API int           ImTextCharToUtf8(char out_buf[5], unsigned int c);
                                                // return output UTF-8 bytes cou
nt
IMGUI_API int           ImTextStrToUtf8(char* out_buf, int out_buf_size, const I
mWchar* in_text, const ImWchar* in_text_end);   // return output UTF-8 bytes cou
nt
IMGUI_API int           ImTextCharFromUtf8(unsigned int* out_char, const char* i
n_text, const char* in_text_end);               // read one character. return in
put UTF-8 bytes count
IMGUI_API int           ImTextStrFromUtf8(ImWchar* out_buf, int out_buf_size, co
nst char* in_text, const char* in_text_end, const char** in_remaining = NULL);
 // return input UTF-8 bytes count
IMGUI_API int           ImTextCountCharsFromUtf8(const char* in_text, const char
* in_text_end);                                 // return number of UTF-8 code-p
oints (NOT bytes count)
IMGUI_API int           ImTextCountUtf8BytesFromChar(const char* in_text, const
char* in_text_end);                             // return number of bytes to exp
ress one char in UTF-8
IMGUI_API int           ImTextCountUtf8BytesFromStr(const ImWchar* in_text, cons
t ImWchar* in_text_end);                        // return number of bytes to exp
ress string in UTF-8
IMGUI_API const char*   ImTextFindPreviousUtf8Codepoint(const char* in_text_star
t, const char* in_text_curr);                   // return previous UTF-8 code-po
int.
IMGUI_API int           ImTextCountLines(const char* in_text, const char* in_tex
t_end);                                         // return number of lines taken
by text. trailing carriage return doesn't count as an extra line.

// Helpers: High-level text functions (DO NOT USE!!! THIS IS A MINIMAL SUBSET OF
 LARGER UPCOMING CHANGES)
enum ImDrawTextFlags_
{
    ImDrawTextFlags_None                = 0,
    ImDrawTextFlags_CpuFineClip         = 1 << 0,    // Must be == 1/true for le
gacy with 'bool cpu_fine_clip' arg to RenderText()
    ImDrawTextFlags_WrapKeepBlanks      = 1 << 1,
    ImDrawTextFlags_StopOnNewLine       = 1 << 2,
};
IMGUI_API ImVec2        ImFontCalcTextSizeEx(ImFont* font, float size, float max
_width, float wrap_width, const char* text_begin, const char* text_end_display,
const char* text_end, const char** out_remaining, ImVec2* out_offset, ImDrawText
Flags flags);
IMGUI_API const char*   ImFontCalcWordWrapPositionEx(ImFont* font, float size, c
onst char* text, const char* text_end, float wrap_width, ImDrawTextFlags flags =
 0);
IMGUI_API const char*   ImTextCalcWordWrapNextLineStart(const char* text, const
char* text_end, ImDrawTextFlags flags = 0); // trim trailing space and find begi
nning of next line

// Helpers: File System
#ifdef IMGUI_DISABLE_FILE_FUNCTIONS
#define IMGUI_DISABLE_DEFAULT_FILE_FUNCTIONS
typedef void* ImFileHandle;
inline ImFileHandle         ImFileOpen(const char*, const char*)
    { return NULL; }
inline bool                 ImFileClose(ImFileHandle)
    { return false; }
inline ImU64                ImFileGetSize(ImFileHandle)
    { return (ImU64)-1; }
inline ImU64                ImFileRead(void*, ImU64, ImU64, ImFileHandle)
    { return 0; }
inline ImU64                ImFileWrite(const void*, ImU64, ImU64, ImFileHandle)
    { return 0; }
#endif
#ifndef IMGUI_DISABLE_DEFAULT_FILE_FUNCTIONS
typedef FILE* ImFileHandle;
IMGUI_API ImFileHandle      ImFileOpen(const char* filename, const char* mode);
IMGUI_API bool              ImFileClose(ImFileHandle file);
IMGUI_API ImU64             ImFileGetSize(ImFileHandle file);
IMGUI_API ImU64             ImFileRead(void* data, ImU64 size, ImU64 count, ImFi
leHandle file);
IMGUI_API ImU64             ImFileWrite(const void* data, ImU64 size, ImU64 coun
t, ImFileHandle file);
#else
#define IMGUI_DISABLE_TTY_FUNCTIONS // Can't use stdout, fflush if we are not us
ing default file functions
#endif
IMGUI_API void*             ImFileLoadToMemory(const char* filename, const char*
 mode, size_t* out_file_size = NULL, int padding_bytes = 0);

// Helpers: Maths
IM_MSVC_RUNTIME_CHECKS_OFF
// - Wrapper for standard libs functions. (Note that imgui_demo.cpp does _not_ u
se them to keep the code easy to copy)
#ifndef IMGUI_DISABLE_DEFAULT_MATH_FUNCTIONS
#define ImFabs(X)           fabsf(X)
#define ImSqrt(X)           sqrtf(X)
#define ImFmod(X, Y)        fmodf((X), (Y))
#define ImCos(X)            cosf(X)
#define ImSin(X)            sinf(X)
#define ImAcos(X)           acosf(X)
#define ImAtan2(Y, X)       atan2f((Y), (X))
#define ImAtof(STR)         atof(STR)
#define ImCeil(X)           ceilf(X)
inline float  ImPow(float x, float y)    { return powf(x, y); }          // Drag
BehaviorT/SliderBehaviorT uses ImPow with either float/double and need the preci
sion
inline double ImPow(double x, double y)  { return pow(x, y); }
inline float  ImLog(float x)             { return logf(x); }             // Drag
BehaviorT/SliderBehaviorT uses ImLog with either float/double and need the preci
sion
inline double ImLog(double x)            { return log(x); }
inline int    ImAbs(int x)               { return x < 0 ? -x : x; }
inline float  ImAbs(float x)             { return fabsf(x); }
inline double ImAbs(double x)            { return fabs(x); }
inline float  ImSign(float x)            { return (x < 0.0f) ? -1.0f : (x > 0.0f
) ? 1.0f : 0.0f; } // Sign operator - returns -1, 0 or 1 based on sign of argume
nt
inline double ImSign(double x)           { return (x < 0.0) ? -1.0 : (x > 0.0) ?
 1.0 : 0.0; }
#ifdef IMGUI_ENABLE_SSE
inline float  ImRsqrt(float x)           { return _mm_cvtss_f32(_mm_rsqrt_ss(_mm
_set_ss(x))); }
#else
inline float  ImRsqrt(float x)           { return 1.0f / sqrtf(x); }
#endif
inline double ImRsqrt(double x)          { return 1.0 / sqrt(x); }
#endif
// - ImMin/ImMax/ImClamp/ImLerp/ImSwap are used by widgets which support variety
of types: signed/unsigned int/long long float/double
// (Exceptionally using templates here but we could also redefine them for those
 types)
template<typename T> T ImMin(T lhs, T rhs)                              { return
 lhs < rhs ? lhs : rhs; }
template<typename T> T ImMax(T lhs, T rhs)                              { return
 lhs >= rhs ? lhs : rhs; }
template<typename T> T ImClamp(T v, T mn, T mx)                         { return
 (v < mn) ? mn : (v > mx) ? mx : v; }
template<typename T> T ImLerp(T a, T b, float t)                        { return
 (T)(a + (b - a) * t); }
template<typename T> void ImSwap(T& a, T& b)                            { T tmp
= a; a = b; b = tmp; }
template<typename T> T ImAddClampOverflow(T a, T b, T mn, T mx)         { if (b
< 0 && (a < mn - b)) return mn; if (b > 0 && (a > mx - b)) return mx; return a +
 b; }
template<typename T> T ImSubClampOverflow(T a, T b, T mn, T mx)         { if (b
> 0 && (a < mn + b)) return mn; if (b < 0 && (a > mx + b)) return mx; return a -
 b; }
// - Misc maths helpers
inline ImVec2 ImMin(const ImVec2& lhs, const ImVec2& rhs)               { return
 ImVec2(lhs.x < rhs.x ? lhs.x : rhs.x, lhs.y < rhs.y ? lhs.y : rhs.y); }
inline ImVec2 ImMax(const ImVec2& lhs, const ImVec2& rhs)               { return
 ImVec2(lhs.x >= rhs.x ? lhs.x : rhs.x, lhs.y >= rhs.y ? lhs.y : rhs.y); }
inline ImVec2 ImClamp(const ImVec2& v, const ImVec2&mn, const ImVec2&mx){ return
 ImVec2((v.x < mn.x) ? mn.x : (v.x > mx.x) ? mx.x : v.x, (v.y < mn.y) ? mn.y : (
v.y > mx.y) ? mx.y : v.y); }
inline ImVec2 ImLerp(const ImVec2& a, const ImVec2& b, float t)         { return
 ImVec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t); }
inline ImVec2 ImLerp(const ImVec2& a, const ImVec2& b, const ImVec2& t) { return
 ImVec2(a.x + (b.x - a.x) * t.x, a.y + (b.y - a.y) * t.y); }
inline ImVec4 ImLerp(const ImVec4& a, const ImVec4& b, float t)         { return
 ImVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w
 + (b.w - a.w) * t); }
inline float  ImSaturate(float f)                                       { return
 (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f; }
inline float  ImLengthSqr(const ImVec2& lhs)                            { return
 (lhs.x * lhs.x) + (lhs.y * lhs.y); }
inline float  ImLengthSqr(const ImVec4& lhs)                            { return
 (lhs.x * lhs.x) + (lhs.y * lhs.y) + (lhs.z * lhs.z) + (lhs.w * lhs.w); }
inline float  ImInvLength(const ImVec2& lhs, float fail_value)          { float
d = (lhs.x * lhs.x) + (lhs.y * lhs.y); if (d > 0.0f) return ImRsqrt(d); return f
ail_value; }
inline float  ImTrunc(float f)                                          { return
 (float)(int)(f); }
inline ImVec2 ImTrunc(const ImVec2& v)                                  { return
 ImVec2((float)(int)(v.x), (float)(int)(v.y)); }
inline float  ImFloor(float f)                                          { return
 (float)((f >= 0 || (float)(int)f == f) ? (int)f : (int)f - 1); } // Decent repl
acement for floorf()
inline ImVec2 ImFloor(const ImVec2& v)                                  { return
 ImVec2(ImFloor(v.x), ImFloor(v.y)); }
inline float  ImTrunc64(float f)                                        { return
 (float)(ImS64)(f); }
inline float  ImRound64(float f)                                        { return
 (float)(ImS64)(f + 0.5f); }
inline int    ImModPositive(int a, int b)                               { return
 (a + b) % b; }
inline float  ImDot(const ImVec2& a, const ImVec2& b)                   { return
 a.x * b.x + a.y * b.y; }
inline ImVec2 ImRotate(const ImVec2& v, float cos_a, float sin_a)       { return
 ImVec2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a); }
inline float  ImLinearSweep(float current, float target, float speed)   { if (cu
rrent < target) return ImMin(current + speed, target); if (current > target) ret
urn ImMax(current - speed, target); return current; }
inline float  ImLinearRemapClamp(float s0, float s1, float d0, float d1, float x
) { return ImSaturate((x - s0) / (s1 - s0)) * (d1 - d0) + d0; }
inline ImVec2 ImMul(const ImVec2& lhs, const ImVec2& rhs)               { return
 ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
inline bool   ImIsFloatAboveGuaranteedIntegerPrecision(float f)         { return
 f <= -16777216 || f >= 16777216; }
inline float  ImExponentialMovingAverage(float avg, float sample, int n){ avg -=
 avg / n; avg += sample / n; return avg; }
IM_MSVC_RUNTIME_CHECKS_RESTORE

// Helpers: Geometry
IMGUI_API ImVec2     ImBezierCubicCalc(const ImVec2& p1, const ImVec2& p2, const
 ImVec2& p3, const ImVec2& p4, float t);
IMGUI_API ImVec2     ImBezierCubicClosestPoint(const ImVec2& p1, const ImVec2& p
2, const ImVec2& p3, const ImVec2& p4, const ImVec2& p, int num_segments);
 // For curves with explicit number of segments
IMGUI_API ImVec2     ImBezierCubicClosestPointCasteljau(const ImVec2& p1, const
ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImVec2& p, float tess_tol)
;// For auto-tessellated curves you can use tess_tol = style.CurveTessellationTo
l
IMGUI_API ImVec2     ImBezierQuadraticCalc(const ImVec2& p1, const ImVec2& p2, c
onst ImVec2& p3, float t);
IMGUI_API ImVec2     ImLineClosestPoint(const ImVec2& a, const ImVec2& b, const
ImVec2& p);
IMGUI_API bool       ImTriangleContainsPoint(const ImVec2& a, const ImVec2& b, c
onst ImVec2& c, const ImVec2& p);
IMGUI_API ImVec2     ImTriangleClosestPoint(const ImVec2& a, const ImVec2& b, co
nst ImVec2& c, const ImVec2& p);
IMGUI_API void       ImTriangleBarycentricCoords(const ImVec2& a, const ImVec2&
b, const ImVec2& c, const ImVec2& p, float& out_u, float& out_v, float& out_w);
inline float         ImTriangleArea(const ImVec2& a, const ImVec2& b, const ImVe
c2& c)          { return ImFabs((a.x * (b.y - c.y)) + (b.x * (c.y - a.y)) + (c.x
 * (a.y - b.y))) * 0.5f; }
inline bool          ImTriangleIsClockwise(const ImVec2& a, const ImVec2& b, con
st ImVec2& c)   { return ((b.x - a.x) * (c.y - b.y)) - ((c.x - b.x) * (b.y - a.y
)) > 0.0f; }

// Helper: ImVec1 (1D vector)
// (this odd construct is used to facilitate the transition between 1D and 2D, a
nd the maintenance of some branches/patches)
IM_MSVC_RUNTIME_CHECKS_OFF
struct ImVec1
{
    float   x;
    constexpr ImVec1()         : x(0.0f) { }
    constexpr ImVec1(float _x) : x(_x) { }
};

// Helper: ImVec2i (2D vector, integer)
struct ImVec2i
{
    int         x, y;
    constexpr ImVec2i()                             : x(0), y(0) {}
    constexpr ImVec2i(int _x, int _y)               : x(_x), y(_y) {}
};

// Helper: ImVec2ih (2D vector, half-size integer, for long-term packed storage)
struct ImVec2ih
{
    short   x, y;
    constexpr ImVec2ih()                           : x(0), y(0) {}
    constexpr ImVec2ih(short _x, short _y)         : x(_x), y(_y) {}
    constexpr explicit ImVec2ih(const ImVec2& rhs) : x((short)rhs.x), y((short)r
hs.y) {}
};

// Helper: ImRect (2D axis aligned bounding-box)
// NB: we can't rely on ImVec2 math operators being available here!
struct IMGUI_API ImRect
{
    ImVec2      Min;    // Upper-left
    ImVec2      Max;    // Lower-right

    constexpr ImRect()                                        : Min(0.0f, 0.0f),
 Max(0.0f, 0.0f)  {}
    constexpr ImRect(const ImVec2& min, const ImVec2& max)    : Min(min), Max(ma
x)                {}
    constexpr ImRect(const ImVec4& v)                         : Min(v.x, v.y), M
ax(v.z, v.w)      {}
    constexpr ImRect(float x1, float y1, float x2, float y2)  : Min(x1, y1), Max
(x2, y2)          {}

    ImVec2      GetCenter() const                   { return ImVec2((Min.x + Max
.x) * 0.5f, (Min.y + Max.y) * 0.5f); }
    ImVec2      GetSize() const                     { return ImVec2(Max.x - Min.
x, Max.y - Min.y); }
    float       GetWidth() const                    { return Max.x - Min.x; }
    float       GetHeight() const                   { return Max.y - Min.y; }
    float       GetArea() const                     { return (Max.x - Min.x) * (
Max.y - Min.y); }
    ImVec2      GetTL() const                       { return Min; }
      // Top-left
    ImVec2      GetTR() const                       { return ImVec2(Max.x, Min.y
); }  // Top-right
    ImVec2      GetBL() const                       { return ImVec2(Min.x, Max.y
); }  // Bottom-left
    ImVec2      GetBR() const                       { return Max; }
      // Bottom-right
    bool        Contains(const ImVec2& p) const     { return p.x     >= Min.x &&
 p.y     >= Min.y && p.x     <  Max.x && p.y     <  Max.y; }
    bool        Contains(const ImRect& r) const     { return r.Min.x >= Min.x &&
 r.Min.y >= Min.y && r.Max.x <= Max.x && r.Max.y <= Max.y; }
    bool        ContainsWithPad(const ImVec2& p, const ImVec2& pad) const { retu
rn p.x >= Min.x - pad.x && p.y >= Min.y - pad.y && p.x < Max.x + pad.x && p.y <
Max.y + pad.y; }
    bool        Overlaps(const ImRect& r) const     { return r.Min.y <  Max.y &&
 r.Max.y >  Min.y && r.Min.x <  Max.x && r.Max.x >  Min.x; }
    void        Add(const ImVec2& p)                { if (Min.x > p.x)     Min.x
 = p.x;     if (Min.y > p.y)     Min.y = p.y;     if (Max.x < p.x)     Max.x = p
.x;     if (Max.y < p.y)     Max.y = p.y; }
    void        Add(const ImRect& r)                { if (Min.x > r.Min.x) Min.x
 = r.Min.x; if (Min.y > r.Min.y) Min.y = r.Min.y; if (Max.x < r.Max.x) Max.x = r
.Max.x; if (Max.y < r.Max.y) Max.y = r.Max.y; }
    void        Expand(const float amount)          { Min.x -= amount;   Min.y -
= amount;   Max.x += amount;   Max.y += amount; }
    void        Expand(const ImVec2& amount)        { Min.x -= amount.x; Min.y -
= amount.y; Max.x += amount.x; Max.y += amount.y; }
    void        Translate(const ImVec2& d)          { Min.x += d.x; Min.y += d.y
; Max.x += d.x; Max.y += d.y; }
    void        TranslateX(float dx)                { Min.x += dx; Max.x += dx;
}
    void        TranslateY(float dy)                { Min.y += dy; Max.y += dy;
}
    void        ClipWith(const ImRect& r)           { Min = ImMax(Min, r.Min); M
ax = ImMin(Max, r.Max); }                   // Simple version, may lead to an in
verted rectangle, which is fine for Contains/Overlaps test but not for display.
    void        ClipWithFull(const ImRect& r)       { Min = ImClamp(Min, r.Min,
r.Max); Max = ImClamp(Max, r.Min, r.Max); } // Full version, ensure both points
are fully clipped.
    void        Floor()                             { Min.x = IM_TRUNC(Min.x); M
in.y = IM_TRUNC(Min.y); Max.x = IM_TRUNC(Max.x); Max.y = IM_TRUNC(Max.y); }
    bool        IsInverted() const                  { return Min.x > Max.x || Mi
n.y > Max.y; }
    ImVec4      ToVec4() const                      { return ImVec4(Min.x, Min.y
, Max.x, Max.y); }
    const ImVec4& AsVec4() const                    { return *(const ImVec4*)&Mi
n.x; }
};

// Helper: ImBitArray
#define         IM_BITARRAY_TESTBIT(_ARRAY, _N)                 ((_ARRAY[(_N) >>
 5] & ((ImU32)1 << ((_N) & 31))) != 0) // Macro version of ImBitArrayTestBit():
ensure args have side-effect or are costly!
#define         IM_BITARRAY_CLEARBIT(_ARRAY, _N)                ((_ARRAY[(_N) >>
 5] &= ~((ImU32)1 << ((_N) & 31))))    // Macro version of ImBitArrayClearBit():
 ensure args have side-effect or are costly!
inline size_t   ImBitArrayGetStorageSizeInBytes(int bitcount)   { return (size_t
)((bitcount + 31) >> 5) << 2; }
inline void     ImBitArrayClearAllBits(ImU32* arr, int bitcount){ memset(arr, 0,
 ImBitArrayGetStorageSizeInBytes(bitcount)); }
inline bool     ImBitArrayTestBit(const ImU32* arr, int n)      { ImU32 mask = (
ImU32)1 << (n & 31); return (arr[n >> 5] & mask) != 0; }
inline void     ImBitArrayClearBit(ImU32* arr, int n)           { ImU32 mask = (
ImU32)1 << (n & 31); arr[n >> 5] &= ~mask; }
inline void     ImBitArraySetBit(ImU32* arr, int n)             { ImU32 mask = (
ImU32)1 << (n & 31); arr[n >> 5] |= mask; }
inline void     ImBitArraySetBitRange(ImU32* arr, int n, int n2) // Works on ran
ge [n..n2)
{
    n2--;
    while (n <= n2)
    {
        int a_mod = (n & 31);
        int b_mod = (n2 > (n | 31) ? 31 : (n2 & 31)) + 1;
        ImU32 mask = (ImU32)(((ImU64)1 << b_mod) - 1) & ~(ImU32)(((ImU64)1 << a_
mod) - 1);
        arr[n >> 5] |= mask;
        n = (n + 32) & ~31;
    }
}

typedef ImU32* ImBitArrayPtr; // Name for use in structs

// Helper: ImBitArray class (wrapper over ImBitArray functions)
// Store 1-bit per value.
template<int BITCOUNT, int OFFSET = 0>
struct ImBitArray
{
    ImU32           Storage[(BITCOUNT + 31) >> 5];
    ImBitArray()                                { ClearAllBits(); }
    void            ClearAllBits()              { memset(Storage, 0, sizeof(Stor
age)); }
    void            SetAllBits()                { memset(Storage, 255, sizeof(St
orage)); }
    bool            TestBit(int n) const        { n += OFFSET; IM_ASSERT(n >= 0
&& n < BITCOUNT); return IM_BITARRAY_TESTBIT(Storage, n); }
    void            SetBit(int n)               { n += OFFSET; IM_ASSERT(n >= 0
&& n < BITCOUNT); ImBitArraySetBit(Storage, n); }
    void            ClearBit(int n)             { n += OFFSET; IM_ASSERT(n >= 0
&& n < BITCOUNT); ImBitArrayClearBit(Storage, n); }
    void            SetBitRange(int n, int n2)  { n += OFFSET; n2 += OFFSET; IM_
ASSERT(n >= 0 && n < BITCOUNT && n2 > n && n2 <= BITCOUNT); ImBitArraySetBitRang
e(Storage, n, n2); } // Works on range [n..n2)
    bool            operator[](int n) const     { n += OFFSET; IM_ASSERT(n >= 0
&& n < BITCOUNT); return IM_BITARRAY_TESTBIT(Storage, n); }
};

// Helper: ImBitVector
// Store 1-bit per value.
struct IMGUI_API ImBitVector
{
    ImVector<ImU32> Storage;
    void            Create(int sz)              { Storage.resize((sz + 31) >> 5)
; memset(Storage.Data, 0, (size_t)Storage.Size * sizeof(Storage.Data[0])); }
    void            Clear()                     { Storage.clear(); }
    bool            TestBit(int n) const        { IM_ASSERT(n < (Storage.Size <<
 5)); return IM_BITARRAY_TESTBIT(Storage.Data, n); }
    void            SetBit(int n)               { IM_ASSERT(n < (Storage.Size <<
 5)); ImBitArraySetBit(Storage.Data, n); }
    void            ClearBit(int n)             { IM_ASSERT(n < (Storage.Size <<
 5)); ImBitArrayClearBit(Storage.Data, n); }
};
IM_MSVC_RUNTIME_CHECKS_RESTORE

// Helper: ImSpan<>
// Pointing to a span of data we don't own.
template<typename T>
struct ImSpan
{
    T*                  Data;
    T*                  DataEnd;

    // Constructors, destructor
    inline ImSpan()                                 { Data = DataEnd = NULL; }
    inline ImSpan(T* data, int size)                { Data = data; DataEnd = dat
a + size; }
    inline ImSpan(T* data, T* data_end)             { Data = data; DataEnd = dat
a_end; }

    inline void         set(T* data, int size)      { Data = data; DataEnd = dat
a + size; }
    inline void         set(T* data, T* data_end)   { Data = data; DataEnd = dat
a_end; }
    inline int          size() const                { return (int)(ptrdiff_t)(Da
taEnd - Data); }
    inline int          size_in_bytes() const       { return (int)(ptrdiff_t)(Da
taEnd - Data) * (int)sizeof(T); }
    inline T&           operator[](int i)           { T* p = Data + i; IM_ASSERT
(p >= Data && p < DataEnd); return *p; }
    inline const T&     operator[](int i) const     { const T* p = Data + i; IM_
ASSERT(p >= Data && p < DataEnd); return *p; }

    inline T*           begin()                     { return Data; }
    inline const T*     begin() const               { return Data; }
    inline T*           end()                       { return DataEnd; }
    inline const T*     end() const                 { return DataEnd; }

    // Utilities
    inline int  index_from_ptr(const T* it) const   { IM_ASSERT(it >= Data && it
 < DataEnd); const ptrdiff_t off = it - Data; return (int)off; }
};
...
// The file is too long, it will be truncated. The rest of the file will be included.
