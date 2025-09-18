// dear imgui, v1.92.3 WIP
// (headers)

// Help:
// - See links below.
// - Call and read ImGui::ShowDemoWindow() in imgui_demo.cpp. All applications i
n examples/ are doing that.
// - Read top of imgui.cpp for more details, links and comments.
// - Add '#define IMGUI_DEFINE_MATH_OPERATORS' before including this file (or in
 imconfig.h) to access courtesy maths operators for ImVec2 and ImVec4.

// Resources:
// - FAQ ........................ https://dearimgui.com/faq (in repository as do
cs/FAQ.md)
// - Homepage ................... https://github.com/ocornut/imgui
// - Releases & changelog ....... https://github.com/ocornut/imgui/releases
// - Gallery .................... https://github.com/ocornut/imgui/issues?q=labe
l%3Agallery (please post your screenshots/video there!)
// - Wiki ....................... https://github.com/ocornut/imgui/wiki (lots of
 good stuff there)
//   - Getting Started            https://github.com/ocornut/imgui/wiki/Getting-
Started (how to integrate in an existing app by adding ~25 lines of code)
//   - Third-party Extensions     https://github.com/ocornut/imgui/wiki/Useful-E
xtensions (ImPlot & many more)
//   - Bindings/Backends          https://github.com/ocornut/imgui/wiki/Bindings
 (language bindings, backends for various tech/engines)
//   - Glossary                   https://github.com/ocornut/imgui/wiki/Glossary
//   - Debug Tools                https://github.com/ocornut/imgui/wiki/Debug-To
ols
//   - Software using Dear ImGui  https://github.com/ocornut/imgui/wiki/Software
-using-dear-imgui
// - Issues & support ........... https://github.com/ocornut/imgui/issues
// - Test Engine & Automation ... https://github.com/ocornut/imgui_test_engine (
test suite, test engine to automate your apps)

// For first-time users having issues compiling/linking/running/loading fonts:
// please post in https://github.com/ocornut/imgui/discussions if you cannot fin
d a solution in resources above.
// Everything else should be asked in 'Issues'! We are building a database of cr
oss-linked knowledge there.

// Library Version
// (Integer encoded as XYYZZ for use in #if preprocessor conditionals, e.g. '#if
 IMGUI_VERSION_NUM >= 12345')
#define IMGUI_VERSION       "1.92.3 WIP"
#define IMGUI_VERSION_NUM   19227
#define IMGUI_HAS_TABLE             // Added BeginTable() - from IMGUI_VERSION_N
UM >= 18000
#define IMGUI_HAS_TEXTURES          // Added ImGuiBackendFlags_RendererHasTextur
es - from IMGUI_VERSION_NUM >= 19198

/*

Index of this file:
// [SECTION] Header mess
// [SECTION] Forward declarations and basic types
// [SECTION] Texture identifiers (ImTextureID, ImTextureRef)
// [SECTION] Dear ImGui end-user API functions
// [SECTION] Flags & Enumerations
// [SECTION] Tables API flags and structures (ImGuiTableFlags, ImGuiTableColumnF
lags, ImGuiTableRowFlags, ImGuiTableBgTarget, ImGuiTableSortSpecs, ImGuiTableCol
umnSortSpecs)
// [SECTION] Helpers: Debug log, Memory allocations macros, ImVector<>
// [SECTION] ImGuiStyle
// [SECTION] ImGuiIO
// [SECTION] Misc data structures (ImGuiInputTextCallbackData, ImGuiSizeCallback
Data, ImGuiPayload)
// [SECTION] Helpers (ImGuiOnceUponAFrame, ImGuiTextFilter, ImGuiTextBuffer, ImG
uiStorage, ImGuiListClipper, Math Operators, ImColor)
// [SECTION] Multi-Select API flags and structures (ImGuiMultiSelectFlags, ImGui
MultiSelectIO, ImGuiSelectionRequest, ImGuiSelectionBasicStorage, ImGuiSelection
ExternalStorage)
// [SECTION] Drawing API (ImDrawCallback, ImDrawCmd, ImDrawIdx, ImDrawVert, ImDr
awChannel, ImDrawListSplitter, ImDrawFlags, ImDrawListFlags, ImDrawList, ImDrawD
ata)
// [SECTION] Texture API (ImTextureFormat, ImTextureStatus, ImTextureRect, ImTex
tureData)
// [SECTION] Font API (ImFontConfig, ImFontGlyph, ImFontGlyphRangesBuilder, ImFo
ntAtlasFlags, ImFontAtlas, ImFontBaked, ImFont)
// [SECTION] Viewports (ImGuiViewportFlags, ImGuiViewport)
// [SECTION] ImGuiPlatformIO + other Platform Dependent Interfaces (ImGuiPlatfor
mImeData)
// [SECTION] Obsolete functions and types

*/

#pragma once

// Configuration file with compile-time options
// (edit imconfig.h or '#define IMGUI_USER_CONFIG "myfilename.h" from your build
 system)
#ifdef IMGUI_USER_CONFIG
#include IMGUI_USER_CONFIG
#endif
#include "imconfig.h"

#ifndef IMGUI_DISABLE

//-----------------------------------------------------------------------------
// [SECTION] Header mess
//-----------------------------------------------------------------------------

// Includes
#include <float.h>                  // FLT_MIN, FLT_MAX
#include <stdarg.h>                 // va_list, va_start, va_end
#include <stddef.h>                 // ptrdiff_t, NULL
#include <string.h>                 // memset, memmove, memcpy, strlen, strchr,
strcpy, strcmp

// Define attributes of all API symbols declarations (e.g. for DLL under Windows
)
// IMGUI_API is used for core imgui functions, IMGUI_IMPL_API is used for the de
fault backends files (imgui_impl_xxx.h)
// Using dear imgui via a shared library is not recommended: we don't guarantee
backward nor forward ABI compatibility + this is a call-heavy library and functi
on call overhead adds up.
#ifndef IMGUI_API
#define IMGUI_API
#endif
#ifndef IMGUI_IMPL_API
#define IMGUI_IMPL_API              IMGUI_API
#endif

// Helper Macros
#ifndef IM_ASSERT
#include <assert.h>
#define IM_ASSERT(_EXPR)            assert(_EXPR)
// You can override the default assert handler by editing imconfig.h
#endif
#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*(_ARR))))
// Size of a static C-style array. Don't use on pointers!
#define IM_UNUSED(_VAR)             ((void)(_VAR))
// Used to silence "unused variable warnings". Often useful as asserts may be st
ripped out from final builds.

// Check that version and structures layouts are matching between compiled imgui
 code and caller. Read comments above DebugCheckVersionAndDataLayout() for detai
ls.
#define IMGUI_CHECKVERSION()        ImGui::DebugCheckVersionAndDataLayout(IMGUI_
VERSION, sizeof(ImGuiIO), sizeof(ImGuiStyle), sizeof(ImVec2), sizeof(ImVec4), si
zeof(ImDrawVert), sizeof(ImDrawIdx))

// Helper Macros - IM_FMTARGS, IM_FMTLIST: Apply printf-style warnings to our fo
rmatting functions.
// (MSVC provides an equivalent mechanism via SAL Annotations but it requires th
e macros in a different
//  location. e.g. #include <sal.h> + void myprintf(_Printf_format_string_ const
 char* format, ...),
//  and only works when using Code Analysis, rather than just normal compiling).
// (see https://github.com/ocornut/imgui/issues/8871 for a patch to enable this
for MSVC's Code Analysis)
#if !defined(IMGUI_USE_STB_SPRINTF) && defined(__MINGW32__) && !defined(__clang_
_)
#define IM_FMTARGS(FMT)             __attribute__((format(gnu_printf, FMT, FMT+1
)))
#define IM_FMTLIST(FMT)             __attribute__((format(gnu_printf, FMT, 0)))
#elif !defined(IMGUI_USE_STB_SPRINTF) && (defined(__clang__) || defined(__GNUC__
))
#define IM_FMTARGS(FMT)             __attribute__((format(printf, FMT, FMT+1)))
#define IM_FMTLIST(FMT)             __attribute__((format(printf, FMT, 0)))
#else
#define IM_FMTARGS(FMT)
#define IM_FMTLIST(FMT)
#endif

// Disable some of MSVC most aggressive Debug runtime checks in function header/
footer (used in some simple/low-level functions)
#if defined(_MSC_VER) && !defined(__clang__)  && !defined(__INTEL_COMPILER) && !
defined(IMGUI_DEBUG_PARANOID)
#define IM_MSVC_RUNTIME_CHECKS_OFF      __pragma(runtime_checks("",off))     __p
ragma(check_stack(off)) __pragma(strict_gs_check(push,off))
#define IM_MSVC_RUNTIME_CHECKS_RESTORE  __pragma(runtime_checks("",restore)) __p
ragma(check_stack())    __pragma(strict_gs_check(pop))
#else
#define IM_MSVC_RUNTIME_CHECKS_OFF
#define IM_MSVC_RUNTIME_CHECKS_RESTORE
#endif

// Warnings
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 26495)    // [Static Analyzer] Variable 'XXX' is unini
tialized. Always initialize a member variable (type.6).
#endif
#if defined(__clang__)
#pragma clang diagnostic push
#if __has_warning("-Wunknown-warning-option")
#pragma clang diagnostic ignored "-Wunknown-warning-option"         // warning:
unknown warning group 'xxx'
#endif
#pragma clang diagnostic ignored "-Wunknown-pragmas"                // warning:
unknown warning group 'xxx'
#pragma clang diagnostic ignored "-Wold-style-cast"                 // warning:
use of old-style cast
#pragma clang diagnostic ignored "-Wfloat-equal"                    // warning:
comparing floating point with == or != is unsafe
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"  // warning:
zero as null pointer constant
#pragma clang diagnostic ignored "-Wreserved-identifier"            // warning:
identifier '_Xxx' is reserved because it starts with '_' followed by a capital l
etter
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
#endif

//-----------------------------------------------------------------------------
// [SECTION] Forward declarations and basic types
//-----------------------------------------------------------------------------

// Scalar data types
typedef unsigned int        ImGuiID;// A unique ID used by widgets (typically th
e result of hashing a stack of string)
typedef signed char         ImS8;   // 8-bit signed integer
typedef unsigned char       ImU8;   // 8-bit unsigned integer
typedef signed short        ImS16;  // 16-bit signed integer
typedef unsigned short      ImU16;  // 16-bit unsigned integer
typedef signed int          ImS32;  // 32-bit signed integer == int
typedef unsigned int        ImU32;  // 32-bit unsigned integer (often used to st
ore packed colors)
typedef signed   long long  ImS64;  // 64-bit signed integer
typedef unsigned long long  ImU64;  // 64-bit unsigned integer

// Forward declarations: ImDrawList, ImFontAtlas layer
struct ImDrawChannel;               // Temporary storage to output draw commands
 out of order, used by ImDrawListSplitter and ImDrawList::ChannelsSplit()
struct ImDrawCmd;                   // A single draw command within a parent ImD
rawList (generally maps to 1 GPU draw call, unless it is a callback)
struct ImDrawData;                  // All draw command lists required to render
 the frame + pos/size coordinates to use for the projection matrix.
struct ImDrawList;                  // A single draw command list (generally one
 per window, conceptually you may see this as a dynamic "mesh" builder)
struct ImDrawListSharedData;        // Data shared among multiple draw lists (ty
pically owned by parent ImGui context, but you may create one yourself)
struct ImDrawListSplitter;          // Helper to split a draw list into differen
t layers which can be drawn into out of order, then flattened back.
struct ImDrawVert;                  // A single vertex (pos + uv + col = 20 byte
s by default. Override layout with IMGUI_OVERRIDE_DRAWVERT_STRUCT_LAYOUT)
struct ImFont;                      // Runtime data for a single font within a p
arent ImFontAtlas
struct ImFontAtlas;                 // Runtime data for multiple fonts, bake mul
tiple fonts into a single texture, TTF/OTF font loader
struct ImFontAtlasBuilder;          // Opaque storage for building a ImFontAtlas
struct ImFontAtlasRect;             // Output of ImFontAtlas::GetCustomRect() wh
en using custom rectangles.
struct ImFontBaked;                 // Baked data for a ImFont at a given size.
struct ImFontConfig;                // Configuration data when adding a font or
merging fonts
struct ImFontGlyph;                 // A single font glyph (code point + coordin
ates within in ImFontAtlas + offset)
struct ImFontGlyphRangesBuilder;    // Helper to build glyph ranges from text/st
ring data
struct ImFontLoader;                // Opaque interface to a font loading backen
d (stb_truetype, FreeType etc.).
struct ImTextureData;               // Specs and pixel storage for a texture use
d by Dear ImGui.
struct ImTextureRect;               // Coordinates of a rectangle within a textu
re.
struct ImColor;                     // Helper functions to create a color that c
an be converted to either u32 or float4 (*OBSOLETE* please avoid using)

// Forward declarations: ImGui layer
struct ImGuiContext;                // Dear ImGui context (opaque structure, unl
ess including imgui_internal.h)
struct ImGuiIO;                     // Main configuration and I/O between your a
pplication and ImGui (also see: ImGuiPlatformIO)
struct ImGuiInputTextCallbackData;  // Shared state of InputText() when using cu
stom ImGuiInputTextCallback (rare/advanced use)
struct ImGuiKeyData;                // Storage for ImGuiIO and IsKeyDown(), IsKe
yPressed() etc functions.
struct ImGuiListClipper;            // Helper to manually clip large list of ite
ms
struct ImGuiMultiSelectIO;          // Structure to interact with a BeginMultiSe
lect()/EndMultiSelect() block
struct ImGuiOnceUponAFrame;         // Helper for running a block of code not mo
re than once a frame
struct ImGuiPayload;                // User data payload for drag and drop opera
tions
struct ImGuiPlatformIO;             // Interface between platform/renderer backe
nds and ImGui (e.g. Clipboard, IME hooks). Extends ImGuiIO. In docking branch, t
his gets extended to support multi-viewports.
struct ImGuiPlatformImeData;        // Platform IME data for io.PlatformSetImeDa
taFn() function.
struct ImGuiSelectionBasicStorage;  // Optional helper to store multi-selection
state + apply multi-selection requests.
struct ImGuiSelectionExternalStorage;//Optional helper to apply multi-selection
requests to existing randomly accessible storage.
struct ImGuiSelectionRequest;       // A selection request (stored in ImGuiMulti
SelectIO)
struct ImGuiSizeCallbackData;       // Callback data when using SetNextWindowSiz
eConstraints() (rare/advanced use)
struct ImGuiStorage;                // Helper for key->value storage (container
sorted by key)
struct ImGuiStoragePair;            // Helper for key->value storage (pair)
struct ImGuiStyle;                  // Runtime data for styling/colors
struct ImGuiTableSortSpecs;         // Sorting specifications for a table (often
 handling sort specs for a single column, occasionally more)
struct ImGuiTableColumnSortSpecs;   // Sorting specification for one column of a
 table
struct ImGuiTextBuffer;             // Helper to hold and append into a text buf
fer (~string builder)
struct ImGuiTextFilter;             // Helper to parse and apply text filters (e
.g. "aaaaa[,bbbbb][,ccccc]")
struct ImGuiViewport;               // A Platform Window (always only one in 'ma
ster' branch), in the future may represent Platform Monitor

// Enumerations
// - We don't use strongly typed enums much because they add constraints (can't
extend in private code, can't store typed in bit fields, extra casting on iterat
ion)
// - Tip: Use your programming IDE navigation facilities on the names in the _ce
ntral column_ below to find the actual flags/enum lists!
//   - In Visual Studio: CTRL+comma ("Edit.GoToAll") can follow symbols inside c
omments, whereas CTRL+F12 ("Edit.GoToImplementation") cannot.
//   - In Visual Studio w/ Visual Assist installed: ALT+G ("VAssistX.GoToImpleme
ntation") can also follow symbols inside comments.
//   - In VS Code, CLion, etc.: CTRL+click can follow symbols inside comments.
enum ImGuiDir : int;                // -> enum ImGuiDir              // Enum: A
cardinal direction (Left, Right, Up, Down)
enum ImGuiKey : int;                // -> enum ImGuiKey              // Enum: A
key identifier (ImGuiKey_XXX or ImGuiMod_XXX value)
enum ImGuiMouseSource : int;        // -> enum ImGuiMouseSource      // Enum; A
mouse input source identifier (Mouse, TouchScreen, Pen)
enum ImGuiSortDirection : ImU8;     // -> enum ImGuiSortDirection    // Enum: A
sorting direction (ascending or descending)
typedef int ImGuiCol;               // -> enum ImGuiCol_             // Enum: A
color identifier for styling
typedef int ImGuiCond;              // -> enum ImGuiCond_            // Enum: A
condition for many Set*() functions
typedef int ImGuiDataType;          // -> enum ImGuiDataType_        // Enum: A
primary data type
typedef int ImGuiMouseButton;       // -> enum ImGuiMouseButton_     // Enum: A
mouse button identifier (0=left, 1=right, 2=middle)
typedef int ImGuiMouseCursor;       // -> enum ImGuiMouseCursor_     // Enum: A
mouse cursor shape
typedef int ImGuiStyleVar;          // -> enum ImGuiStyleVar_        // Enum: A
variable identifier for styling
typedef int ImGuiTableBgTarget;     // -> enum ImGuiTableBgTarget_   // Enum: A
color target for TableSetBgColor()

// Flags (declared as int to allow using as flags without overhead, and to not p
ollute the top of this file)
// - Tip: Use your programming IDE navigation facilities on the names in the _ce
ntral column_ below to find the actual flags/enum lists!
//   - In Visual Studio: CTRL+comma ("Edit.GoToAll") can follow symbols inside c
omments, whereas CTRL+F12 ("Edit.GoToImplementation") cannot.
//   - In Visual Studio w/ Visual Assist installed: ALT+G ("VAssistX.GoToImpleme
ntation") can also follow symbols inside comments.
//   - In VS Code, CLion, etc.: CTRL+click can follow symbols inside comments.
typedef int ImDrawFlags;            // -> enum ImDrawFlags_          // Flags: f
or ImDrawList functions
typedef int ImDrawListFlags;        // -> enum ImDrawListFlags_      // Flags: f
or ImDrawList instance
typedef int ImDrawTextFlags;        // -> enum ImDrawTextFlags_      // Internal
, do not use!
typedef int ImFontFlags;            // -> enum ImFontFlags_          // Flags: f
or ImFont
typedef int ImFontAtlasFlags;       // -> enum ImFontAtlasFlags_     // Flags: f
or ImFontAtlas
typedef int ImGuiBackendFlags;      // -> enum ImGuiBackendFlags_    // Flags: f
or io.BackendFlags
typedef int ImGuiButtonFlags;       // -> enum ImGuiButtonFlags_     // Flags: f
or InvisibleButton()
typedef int ImGuiChildFlags;        // -> enum ImGuiChildFlags_      // Flags: f
or BeginChild()
typedef int ImGuiColorEditFlags;    // -> enum ImGuiColorEditFlags_  // Flags: f
or ColorEdit4(), ColorPicker4() etc.
typedef int ImGuiConfigFlags;       // -> enum ImGuiConfigFlags_     // Flags: f
or io.ConfigFlags
typedef int ImGuiComboFlags;        // -> enum ImGuiComboFlags_      // Flags: f
or BeginCombo()
typedef int ImGuiDragDropFlags;     // -> enum ImGuiDragDropFlags_   // Flags: f
or BeginDragDropSource(), AcceptDragDropPayload()
typedef int ImGuiFocusedFlags;      // -> enum ImGuiFocusedFlags_    // Flags: f
or IsWindowFocused()
typedef int ImGuiHoveredFlags;      // -> enum ImGuiHoveredFlags_    // Flags: f
or IsItemHovered(), IsWindowHovered() etc.
typedef int ImGuiInputFlags;        // -> enum ImGuiInputFlags_      // Flags: f
or Shortcut(), SetNextItemShortcut()
typedef int ImGuiInputTextFlags;    // -> enum ImGuiInputTextFlags_  // Flags: f
or InputText(), InputTextMultiline()
typedef int ImGuiItemFlags;         // -> enum ImGuiItemFlags_       // Flags: f
or PushItemFlag(), shared by all items
typedef int ImGuiKeyChord;          // -> ImGuiKey | ImGuiMod_XXX    // Flags: f
or IsKeyChordPressed(), Shortcut() etc. an ImGuiKey optionally OR-ed with one or
 more ImGuiMod_XXX values.
typedef int ImGuiListClipperFlags;  // -> enum ImGuiListClipperFlags_// Flags: f
or ImGuiListClipper
typedef int ImGuiPopupFlags;        // -> enum ImGuiPopupFlags_      // Flags: f
or OpenPopup*(), BeginPopupContext*(), IsPopupOpen()
typedef int ImGuiMultiSelectFlags;  // -> enum ImGuiMultiSelectFlags_// Flags: f
or BeginMultiSelect()
typedef int ImGuiSelectableFlags;   // -> enum ImGuiSelectableFlags_ // Flags: f
or Selectable()
typedef int ImGuiSliderFlags;       // -> enum ImGuiSliderFlags_     // Flags: f
or DragFloat(), DragInt(), SliderFloat(), SliderInt() etc.
typedef int ImGuiTabBarFlags;       // -> enum ImGuiTabBarFlags_     // Flags: f
or BeginTabBar()
typedef int ImGuiTabItemFlags;      // -> enum ImGuiTabItemFlags_    // Flags: f
or BeginTabItem()
typedef int ImGuiTableFlags;        // -> enum ImGuiTableFlags_      // Flags: F
or BeginTable()
typedef int ImGuiTableColumnFlags;  // -> enum ImGuiTableColumnFlags_// Flags: F
or TableSetupColumn()
typedef int ImGuiTableRowFlags;     // -> enum ImGuiTableRowFlags_   // Flags: F
or TableNextRow()
typedef int ImGuiTreeNodeFlags;     // -> enum ImGuiTreeNodeFlags_   // Flags: f
or TreeNode(), TreeNodeEx(), CollapsingHeader()
typedef int ImGuiViewportFlags;     // -> enum ImGuiViewportFlags_   // Flags: f
or ImGuiViewport
typedef int ImGuiWindowFlags;       // -> enum ImGuiWindowFlags_     // Flags: f
or Begin(), BeginChild()

// Character types
// (we generally use UTF-8 encoded string in the API. This is storage specifical
ly for a decoded character used for keyboard input and display)
typedef unsigned int ImWchar32;     // A single decoded U32 character/code point
. We encode them as multi bytes UTF-8 when used in strings.
typedef unsigned short ImWchar16;   // A single decoded U16 character/code point
. We encode them as multi bytes UTF-8 when used in strings.
#ifdef IMGUI_USE_WCHAR32            // ImWchar [configurable type: override in i
mconfig.h with '#define IMGUI_USE_WCHAR32' to support Unicode planes 1-16]
typedef ImWchar32 ImWchar;
#else
typedef ImWchar16 ImWchar;
#endif

// Multi-Selection item index or identifier when using BeginMultiSelect()
// - Used by SetNextItemSelectionUserData() + and inside ImGuiMultiSelectIO stru
cture.
// - Most users are likely to use this store an item INDEX but this may be used
to store a POINTER/ID as well. Read comments near ImGuiMultiSelectIO for details
.
typedef ImS64 ImGuiSelectionUserData;

// Callback and functions types
typedef int     (*ImGuiInputTextCallback)(ImGuiInputTextCallbackData* data);
// Callback function for ImGui::InputText()
typedef void    (*ImGuiSizeCallback)(ImGuiSizeCallbackData* data);
// Callback function for ImGui::SetNextWindowSizeConstraints()
typedef void*   (*ImGuiMemAllocFunc)(size_t sz, void* user_data);
// Function signature for ImGui::SetAllocatorFunctions()
typedef void    (*ImGuiMemFreeFunc)(void* ptr, void* user_data);
// Function signature for ImGui::SetAllocatorFunctions()

// ImVec2: 2D vector used to store positions, sizes etc. [Compile-time configura
ble type]
// - This is a frequently used type in the API. Consider using IM_VEC2_CLASS_EXT
RA to create implicit cast from/to our preferred type.
// - Add '#define IMGUI_DEFINE_MATH_OPERATORS' before including this file (or in
 imconfig.h) to access courtesy maths operators for ImVec2 and ImVec4.
IM_MSVC_RUNTIME_CHECKS_OFF
struct ImVec2
{
    float                                   x, y;
    constexpr ImVec2()                      : x(0.0f), y(0.0f) { }
    constexpr ImVec2(float _x, float _y)    : x(_x), y(_y) { }
    float& operator[] (size_t idx)          { IM_ASSERT(idx == 0 || idx == 1); r
eturn ((float*)(void*)(char*)this)[idx]; } // We very rarely use this [] operato
r, so the assert overhead is fine.
    float  operator[] (size_t idx) const    { IM_ASSERT(idx == 0 || idx == 1); r
eturn ((const float*)(const void*)(const char*)this)[idx]; }
#ifdef IM_VEC2_CLASS_EXTRA
    IM_VEC2_CLASS_EXTRA     // Define additional constructors and implicit cast
operators in imconfig.h to convert back and forth between your math types and Im
Vec2.
#endif
};

// ImVec4: 4D vector used to store clipping rectangles, colors etc. [Compile-tim
e configurable type]
struct ImVec4
{
    float                                                     x, y, z, w;
    constexpr ImVec4()                                        : x(0.0f), y(0.0f)
, z(0.0f), w(0.0f) { }
    constexpr ImVec4(float _x, float _y, float _z, float _w)  : x(_x), y(_y), z(
_z), w(_w) { }
#ifdef IM_VEC4_CLASS_EXTRA
    IM_VEC4_CLASS_EXTRA     // Define additional constructors and implicit cast
operators in imconfig.h to convert back and forth between your math types and Im
Vec4.
#endif
};
IM_MSVC_RUNTIME_CHECKS_RESTORE

//-----------------------------------------------------------------------------
// [SECTION] Texture identifiers (ImTextureID, ImTextureRef)
//-----------------------------------------------------------------------------

// ImTextureID = backend specific, low-level identifier for a texture uploaded i
n GPU/graphics system.
// [Compile-time configurable type]
// - When a Rendered Backend creates a texture, it store its native identifier i
nto a ImTextureID value.
//   (e.g. Used by DX11 backend to a `ID3D11ShaderResourceView*`; Used by OpenGL
 backends to store `GLuint`;
//         Used by SDLGPU backend to store a `SDL_GPUTextureSamplerBinding*`, et
c.).
// - User may submit their own textures to e.g. ImGui::Image() function by passi
ng this value.
// - During the rendering loop, the Renderer Backend retrieve the ImTextureID, w
hich stored inside a
//   ImTextureRef, which is stored inside a ImDrawCmd.
// - Compile-time type configuration:
//   - To use something other than a 64-bit value: add '#define ImTextureID MyTe
xtureType*' in your imconfig.h file.
//   - This can be whatever to you want it to be! read the FAQ entry about textu
res for details.
//   - You may decide to store a higher-level structure containing texture, samp
ler, shader etc. with various
//     constructors if you like. You will need to implement ==/!= operators.
// History:
// - In v1.91.4 (2024/10/08): the default type for ImTextureID was changed from
'void*' to 'ImU64'. This allowed backends requirig 64-bit worth of data to build
 on 32-bit architectures. Use intermediary intptr_t cast and read FAQ if you hav
e casting warnings.
// - In v1.92.0 (2025/06/11): added ImTextureRef which carry either a ImTextureI
D either a pointer to internal texture atlas. All user facing functions taking I
mTextureID changed to ImTextureRef
#ifndef ImTextureID
typedef ImU64 ImTextureID;      // Default: store up to 64-bits (any pointer or
integer). A majority of backends are ok with that.
#endif

// Define this if you need 0 to be a valid ImTextureID for your backend.
#ifndef ImTextureID_Invalid
#define ImTextureID_Invalid     ((ImTextureID)0)
#endif

// ImTextureRef = higher-level identifier for a texture. Store a ImTextureID _or
_ a ImTextureData*.
// The identifier is valid even before the texture has been uploaded to the GPU/
graphics system.
// This is what gets passed to functions such as `ImGui::Image()`, `ImDrawList::
AddImage()`.
// This is what gets stored in draw commands (`ImDrawCmd`) to identify a texture
 during rendering.
// - When a texture is created by user code (e.g. custom images), we directly st
ores the low-level ImTextureID.
//   Because of this, when displaying your own texture you are likely to ever on
ly manage ImTextureID values on your side.
// - When a texture is created by the backend, we stores a ImTextureData* which
becomes an indirection
//   to extract the ImTextureID value during rendering, after texture upload has
 happened.
// - To create a ImTextureRef from a ImTextureData you can use ImTextureData::Ge
tTexRef().
//   We intentionally do not provide an ImTextureRef constructor for this: we do
n't expect this
//   to be frequently useful to the end-user, and it would be erroneously called
 by many legacy code.
// - If you want to bind the current atlas when using custom rectangle, you can
use io.Fonts->TexRef.
// - Binding generators for languages such as C (which don't have constructors),
 should provide a helper, e.g.
//      inline ImTextureRef ImTextureRefFromID(ImTextureID tex_id) { ImTextureRe
f tex_ref = { ._TexData = NULL, .TexID = tex_id }; return tex_ref; }
// In 1.92 we changed most drawing functions using ImTextureID to use ImTextureR
ef.
// We intentionally do not provide an implicit ImTextureRef -> ImTextureID cast
operator because it is technically lossy to convert ImTextureRef to ImTextureID
before rendering.
IM_MSVC_RUNTIME_CHECKS_OFF
struct ImTextureRef
{
    ImTextureRef()                          { _TexData = NULL; _TexID = ImTextur
eID_Invalid; }
    ImTextureRef(ImTextureID tex_id)        { _TexData = NULL; _TexID = tex_id;
}
#if !defined(IMGUI_DISABLE_OBSOLETE_FUNCTIONS) && !defined(ImTextureID)
    ImTextureRef(void* tex_id)              { _TexData = NULL; _TexID = (ImTextu
reID)(size_t)tex_id; }  // For legacy backends casting to ImTextureID
#endif

    inline ImTextureID  GetTexID() const;   // == (_TexData ? _TexData->TexID :
_TexID) // Implemented below in the file.

    // Members (either are set, never both!)
    ImTextureData*      _TexData;           //      A texture, generally owned b
y a ImFontAtlas. Will convert to ImTextureID during render loop, after texture h
as been uploaded.
    ImTextureID         _TexID;             // _OR_ Low-level backend texture id
entifier, if already uploaded or created by user/app. Generally provided to e.g.
 ImGui::Image() calls.
};
IM_MSVC_RUNTIME_CHECKS_RESTORE

//-----------------------------------------------------------------------------
// [SECTION] Dear ImGui end-user API functions
// (Note that ImGui:: being a namespace, you can add extra ImGui:: functions in
your own separate file. Please don't modify imgui source files!)
//-----------------------------------------------------------------------------

namespace ImGui
{
    // Context creation and access
    // - Each context create its own ImFontAtlas by default. You may instance on
e yourself and pass it to CreateContext() to share a font atlas between contexts
.
    // - DLL users: heaps and globals are not shared across DLL boundaries! You
will need to call SetCurrentContext() + SetAllocatorFunctions()
    //   for each static/DLL boundary you are calling from. Read "Context and Me
mory Allocators" section of imgui.cpp for details.
    IMGUI_API ImGuiContext* CreateContext(ImFontAtlas* shared_font_atlas = NULL)
;
    IMGUI_API void          DestroyContext(ImGuiContext* ctx = NULL);   // NULL
= destroy current context
    IMGUI_API ImGuiContext* GetCurrentContext();
    IMGUI_API void          SetCurrentContext(ImGuiContext* ctx);

    // Main
    IMGUI_API ImGuiIO&      GetIO();                                    // acces
s the ImGuiIO structure (mouse/keyboard/gamepad inputs, time, various configurat
ion options/flags)
    IMGUI_API ImGuiPlatformIO& GetPlatformIO();                         // acces
s the ImGuiPlatformIO structure (mostly hooks/functions to connect to platform/r
enderer and OS Clipboard, IME etc.)
    IMGUI_API ImGuiStyle&   GetStyle();                                 // acces
s the Style structure (colors, sizes). Always use PushStyleColor(), PushStyleVar
() to modify style mid-frame!
    IMGUI_API void          NewFrame();                                 // start
 a new Dear ImGui frame, you can submit any command from this point until Render
()/EndFrame().
    IMGUI_API void          EndFrame();                                 // ends
the Dear ImGui frame. automatically called by Render(). If you don't need to ren
der data (skipping rendering) you may call EndFrame() without Render()... but yo
u'll have wasted CPU already! If you don't need to render, better to not create
any windows and not call NewFrame() at all!
    IMGUI_API void          Render();                                   // ends
the Dear ImGui frame, finalize the draw data. You can then get call GetDrawData(
).
    IMGUI_API ImDrawData*   GetDrawData();                              // valid
 after Render() and until the next call to NewFrame(). Call ImGui_ImplXXXX_Rende
rDrawData() function in your Renderer Backend to render.

    // Demo, Debug, Information
    IMGUI_API void          ShowDemoWindow(bool* p_open = NULL);        // creat
e Demo window. demonstrate most ImGui features. call this to learn about the lib
rary! try to make it always available in your application!
    IMGUI_API void          ShowMetricsWindow(bool* p_open = NULL);     // creat
e Metrics/Debugger window. display Dear ImGui internals: windows, draw commands,
 various internal state, etc.
    IMGUI_API void          ShowDebugLogWindow(bool* p_open = NULL);    // creat
e Debug Log window. display a simplified log of important dear imgui events.
    IMGUI_API void          ShowIDStackToolWindow(bool* p_open = NULL); // creat
e Stack Tool window. hover items with mouse to query information about the sourc
e of their unique ID.
    IMGUI_API void          ShowAboutWindow(bool* p_open = NULL);       // creat
e About window. display Dear ImGui version, credits and build/system information
.
    IMGUI_API void          ShowStyleEditor(ImGuiStyle* ref = NULL);    // add s
tyle editor block (not a window). you can pass in a reference ImGuiStyle structu
re to compare to, revert to and save to (else it uses the default style)
    IMGUI_API bool          ShowStyleSelector(const char* label);       // add s
tyle selector block (not a window), essentially a combo listing the default styl
es.
    IMGUI_API void          ShowFontSelector(const char* label);        // add f
ont selector block (not a window), essentially a combo listing the loaded fonts.
    IMGUI_API void          ShowUserGuide();                            // add b
asic help/info block (not a window): how to manipulate ImGui as an end-user (mou
se/keyboard controls).
    IMGUI_API const char*   GetVersion();                               // get t
he compiled version string e.g. "1.80 WIP" (essentially the value for IMGUI_VERS
ION from the compiled version of imgui.cpp)

    // Styles
    IMGUI_API void          StyleColorsDark(ImGuiStyle* dst = NULL);    // new,
recommended style (default)
    IMGUI_API void          StyleColorsLight(ImGuiStyle* dst = NULL);   // best
used with borders and a custom, thicker font
    IMGUI_API void          StyleColorsClassic(ImGuiStyle* dst = NULL); // class
ic imgui style

    // Windows
    // - Begin() = push window to the stack and start appending to it. End() = p
op window from the stack.
    // - Passing 'bool* p_open != NULL' shows a window-closing widget in the upp
er-right corner of the window,
    //   which clicking will set the boolean to false when clicked.
    // - You may append multiple times to the same window during the same frame
by calling Begin()/End() pairs multiple times.
    //   Some information such as 'flags' or 'p_open' will only be considered by
 the first call to Begin().
    // - Begin() return false to indicate the window is collapsed or fully clipp
ed, so you may early out and omit submitting
    //   anything to the window. Always call a matching End() for each Begin() c
all, regardless of its return value!
    //   [Important: due to legacy reason, Begin/End and BeginChild/EndChild are
 inconsistent with all other functions
    //    such as BeginMenu/EndMenu, BeginPopup/EndPopup, etc. where the EndXXX
call should only be called if the corresponding
    //    BeginXXX function returned true. Begin and BeginChild are the only odd
 ones out. Will be fixed in a future update.]
    // - Note that the bottom of window stack always contains a window called "D
ebug".
    IMGUI_API bool          Begin(const char* name, bool* p_open = NULL, ImGuiWi
ndowFlags flags = 0);
    IMGUI_API void          End();

    // Child Windows
    // - Use child windows to begin into a self-contained independent scrolling/
clipping regions within a host window. Child windows can embed their own child.
    // - Before 1.90 (November 2023), the "ImGuiChildFlags child_flags = 0" para
meter was "bool border = false".
    //   This API is backward compatible with old code, as we guarantee that ImG
uiChildFlags_Borders == true.
    //   Consider updating your old code:
    //      BeginChild("Name", size, false)   -> Begin("Name", size, 0); or Begi
n("Name", size, ImGuiChildFlags_None);
    //      BeginChild("Name", size, true)    -> Begin("Name", size, ImGuiChildF
lags_Borders);
    // - Manual sizing (each axis can use a different setting e.g. ImVec2(0.0f,
400.0f)):
    //     == 0.0f: use remaining parent window size for this axis.
    //      > 0.0f: use specified size for this axis.
    //      < 0.0f: right/bottom-align to specified distance from available cont
ent boundaries.
    // - Specifying ImGuiChildFlags_AutoResizeX or ImGuiChildFlags_AutoResizeY m
akes the sizing automatic based on child contents.
    //   Combining both ImGuiChildFlags_AutoResizeX _and_ ImGuiChildFlags_AutoRe
sizeY defeats purpose of a scrolling region and is NOT recommended.
    // - BeginChild() returns false to indicate the window is collapsed or fully
 clipped, so you may early out and omit submitting
    //   anything to the window. Always call a matching EndChild() for each Begi
nChild() call, regardless of its return value.
    //   [Important: due to legacy reason, Begin/End and BeginChild/EndChild are
 inconsistent with all other functions
    //    such as BeginMenu/EndMenu, BeginPopup/EndPopup, etc. where the EndXXX
call should only be called if the corresponding
    //    BeginXXX function returned true. Begin and BeginChild are the only odd
 ones out. Will be fixed in a future update.]
    IMGUI_API bool          BeginChild(const char* str_id, const ImVec2& size =
ImVec2(0, 0), ImGuiChildFlags child_flags = 0, ImGuiWindowFlags window_flags = 0
);
    IMGUI_API bool          BeginChild(ImGuiID id, const ImVec2& size = ImVec2(0
, 0), ImGuiChildFlags child_flags = 0, ImGuiWindowFlags window_flags = 0);
    IMGUI_API void          EndChild();

    // Windows Utilities
    // - 'current window' = the window we are appending into while inside a Begi
n()/End() block. 'next window' = next window we will Begin() into.
    IMGUI_API bool          IsWindowAppearing();
    IMGUI_API bool          IsWindowCollapsed();
    IMGUI_API bool          IsWindowFocused(ImGuiFocusedFlags flags=0); // is cu
rrent window focused? or its root/child, depending on flags. see flags for optio
ns.
    IMGUI_API bool          IsWindowHovered(ImGuiHoveredFlags flags=0); // is cu
rrent window hovered and hoverable (e.g. not blocked by a popup/modal)? See ImGu
iHoveredFlags_ for options. IMPORTANT: If you are trying to check whether your m
ouse should be dispatched to Dear ImGui or to your underlying app, you should no
t use this function! Use the 'io.WantCaptureMouse' boolean for that! Refer to FA
Q entry "How can I tell whether to dispatch mouse/keyboard to Dear ImGui or my a
pplication?" for details.
    IMGUI_API ImDrawList*   GetWindowDrawList();                        // get d
raw list associated to the current window, to append your own drawing primitives
    IMGUI_API ImVec2        GetWindowPos();                             // get c
urrent window position in screen space (IT IS UNLIKELY YOU EVER NEED TO USE THIS
. Consider always using GetCursorScreenPos() and GetContentRegionAvail() instead
)
    IMGUI_API ImVec2        GetWindowSize();                            // get c
urrent window size (IT IS UNLIKELY YOU EVER NEED TO USE THIS. Consider always us
ing GetCursorScreenPos() and GetContentRegionAvail() instead)
    IMGUI_API float         GetWindowWidth();                           // get c
urrent window width (IT IS UNLIKELY YOU EVER NEED TO USE THIS). Shortcut for Get
WindowSize().x.
    IMGUI_API float         GetWindowHeight();                          // get c
urrent window height (IT IS UNLIKELY YOU EVER NEED TO USE THIS). Shortcut for Ge
tWindowSize().y.

    // Window manipulation
    // - Prefer using SetNextXXX functions (before Begin) rather that SetXXX fun
ctions (after Begin).
    IMGUI_API void          SetNextWindowPos(const ImVec2& pos, ImGuiCond cond =
 0, const ImVec2& pivot = ImVec2(0, 0)); // set next window position. call befor
e Begin(). use pivot=(0.5f,0.5f) to center on given point, etc.
    IMGUI_API void          SetNextWindowSize(const ImVec2& size, ImGuiCond cond
 = 0);                  // set next window size. set axis to 0.0f to force an au
to-fit on this axis. call before Begin()
    IMGUI_API void          SetNextWindowSizeConstraints(const ImVec2& size_min,
 const ImVec2& size_max, ImGuiSizeCallback custom_callback = NULL, void* custom_
callback_data = NULL); // set next window size limits. use 0.0f or FLT_MAX if yo
u don't want limits. Use -1 for both min and max of same axis to preserve curren
t size (which itself is a constraint). Use callback to apply non-trivial program
matic constraints.
    IMGUI_API void          SetNextWindowContentSize(const ImVec2& size);
                        // set next window content size (~ scrollable client are
a, which enforce the range of scrollbars). Not including window decorations (tit
le bar, menu bar, etc.) nor WindowPadding. set an axis to 0.0f to leave it autom
atic. call before Begin()
    IMGUI_API void          SetNextWindowCollapsed(bool collapsed, ImGuiCond con
d = 0);                 // set next window collapsed state. call before Begin()
    IMGUI_API void          SetNextWindowFocus();
                        // set next window to be focused / top-most. call before
 Begin()
    IMGUI_API void          SetNextWindowScroll(const ImVec2& scroll);
                        // set next window scrolling value (use < 0.0f to not af
fect a given axis).
    IMGUI_API void          SetNextWindowBgAlpha(float alpha);
                        // set next window background color alpha. helper to eas
ily override the Alpha component of ImGuiCol_WindowBg/ChildBg/PopupBg. you may a
lso use ImGuiWindowFlags_NoBackground.
    IMGUI_API void          SetWindowPos(const ImVec2& pos, ImGuiCond cond = 0);
                        // (not recommended) set current window position - call
within Begin()/End(). prefer using SetNextWindowPos(), as this may incur tearing
 and side-effects.
    IMGUI_API void          SetWindowSize(const ImVec2& size, ImGuiCond cond = 0
);                      // (not recommended) set current window size - call with
in Begin()/End(). set to ImVec2(0, 0) to force an auto-fit. prefer using SetNext
WindowSize(), as this may incur tearing and minor side-effects.
    IMGUI_API void          SetWindowCollapsed(bool collapsed, ImGuiCond cond =
0);                     // (not recommended) set current window collapsed state.
 prefer using SetNextWindowCollapsed().
    IMGUI_API void          SetWindowFocus();
                        // (not recommended) set current window to be focused /
top-most. prefer using SetNextWindowFocus().
    IMGUI_API void          SetWindowPos(const char* name, const ImVec2& pos, Im
GuiCond cond = 0);      // set named window position.
    IMGUI_API void          SetWindowSize(const char* name, const ImVec2& size,
ImGuiCond cond = 0);    // set named window size. set axis to 0.0f to force an a
uto-fit on this axis.
    IMGUI_API void          SetWindowCollapsed(const char* name, bool collapsed,
 ImGuiCond cond = 0);   // set named window collapsed state
    IMGUI_API void          SetWindowFocus(const char* name);
                        // set named window to be focused / top-most. use NULL t
o remove focus.

    // Windows Scrolling
    // - Any change of Scroll will be applied at the beginning of next frame in
the first call to Begin().
    // - You may instead use SetNextWindowScroll() prior to calling Begin() to a
void this delay, as an alternative to using SetScrollX()/SetScrollY().
    IMGUI_API float         GetScrollX();
            // get scrolling amount [0 .. GetScrollMaxX()]
    IMGUI_API float         GetScrollY();
            // get scrolling amount [0 .. GetScrollMaxY()]
    IMGUI_API void          SetScrollX(float scroll_x);
            // set scrolling amount [0 .. GetScrollMaxX()]
    IMGUI_API void          SetScrollY(float scroll_y);
            // set scrolling amount [0 .. GetScrollMaxY()]
    IMGUI_API float         GetScrollMaxX();
            // get maximum scrolling amount ~~ ContentSize.x - WindowSize.x - De
corationsSize.x
    IMGUI_API float         GetScrollMaxY();
            // get maximum scrolling amount ~~ ContentSize.y - WindowSize.y - De
corationsSize.y
    IMGUI_API void          SetScrollHereX(float center_x_ratio = 0.5f);
            // adjust scrolling amount to make current cursor position visible.
center_x_ratio=0.0: left, 0.5: center, 1.0: right. When using to make a "default
/current item" visible, consider using SetItemDefaultFocus() instead.
    IMGUI_API void          SetScrollHereY(float center_y_ratio = 0.5f);
            // adjust scrolling amount to make current cursor position visible.
center_y_ratio=0.0: top, 0.5: center, 1.0: bottom. When using to make a "default
/current item" visible, consider using SetItemDefaultFocus() instead.
    IMGUI_API void          SetScrollFromPosX(float local_x, float center_x_rati
o = 0.5f);  // adjust scrolling amount to make given position visible. Generally
 GetCursorStartPos() + offset to compute a valid position.
    IMGUI_API void          SetScrollFromPosY(float local_y, float center_y_rati
o = 0.5f);  // adjust scrolling amount to make given position visible. Generally
 GetCursorStartPos() + offset to compute a valid position.

    // Parameters stacks (font)
    //  - PushFont(font, 0.0f)                       // Change font and keep cur
rent size
    //  - PushFont(NULL, 20.0f)                      // Keep font and change cur
rent size
    //  - PushFont(font, 20.0f)                      // Change font and set size
 to 20.0f
    //  - PushFont(font, style.FontSizeBase * 2.0f)  // Change font and set size
 to be twice bigger than current size.
    //  - PushFont(font, font->LegacySize)           // Change font and set size
 to size passed to AddFontXXX() function. Same as pre-1.92 behavior.
    // *IMPORTANT* before 1.92, fonts had a single size. They can now be dynamic
ally be adjusted.
    //  - In 1.92 we have REMOVED the single parameter version of PushFont() bec
ause it seems like the easiest way to provide an error-proof transition.
    //  - PushFont(font) before 1.92 = PushFont(font, font->LegacySize) after 1.
92          // Use default font size as passed to AddFontXXX() function.
    // *IMPORTANT* global scale factors are applied over the provided size.
    //  - Global scale factors are: 'style.FontScaleMain', 'style.FontScaleDpi'
and maybe more.
    // -  If you want to apply a factor to the _current_ font size:
    //  - CORRECT:   PushFont(NULL, style.FontSizeBase)         // use current u
nscaled size    == does nothing
    //  - CORRECT:   PushFont(NULL, style.FontSizeBase * 2.0f)  // use current u
nscaled size x2 == make text twice bigger
    //  - INCORRECT: PushFont(NULL, GetFontSize())              // INCORRECT! us
ing size after global factors already applied == GLOBAL SCALING FACTORS WILL APP
LY TWICE!
    //  - INCORRECT: PushFont(NULL, GetFontSize() * 2.0f)       // INCORRECT! us
ing size after global factors already applied == GLOBAL SCALING FACTORS WILL APP
LY TWICE!
    IMGUI_API void          PushFont(ImFont* font, float font_size_base_unscaled
);          // Use NULL as a shortcut to keep current font. Use 0.0f to keep cur
rent size.
    IMGUI_API void          PopFont();
    IMGUI_API ImFont*       GetFont();
            // get current font
    IMGUI_API float         GetFontSize();
            // get current scaled font size (= height in pixels). AFTER global s
cale factors applied. *IMPORTANT* DO NOT PASS THIS VALUE TO PushFont()! Use ImGu
i::GetStyle().FontSizeBase to get value before global scale factors.
    IMGUI_API ImFontBaked*  GetFontBaked();
            // get current font bound at current size // == GetFont()->GetFontBa
ked(GetFontSize())

    // Parameters stacks (shared)
    IMGUI_API void          PushStyleColor(ImGuiCol idx, ImU32 col);
            // modify a style color. always use this if you modify the style aft
er NewFrame().
    IMGUI_API void          PushStyleColor(ImGuiCol idx, const ImVec4& col);
    IMGUI_API void          PopStyleColor(int count = 1);
    IMGUI_API void          PushStyleVar(ImGuiStyleVar idx, float val);
            // modify a style float variable. always use this if you modify the
style after NewFrame()!
    IMGUI_API void          PushStyleVar(ImGuiStyleVar idx, const ImVec2& val);
            // modify a style ImVec2 variable. "
    IMGUI_API void          PushStyleVarX(ImGuiStyleVar idx, float val_x);
            // modify X component of a style ImVec2 variable. "
    IMGUI_API void          PushStyleVarY(ImGuiStyleVar idx, float val_y);
            // modify Y component of a style ImVec2 variable. "
    IMGUI_API void          PopStyleVar(int count = 1);
    IMGUI_API void          PushItemFlag(ImGuiItemFlags option, bool enabled);
            // modify specified shared item flag, e.g. PushItemFlag(ImGuiItemFla
gs_NoTabStop, true)
    IMGUI_API void          PopItemFlag();

    // Parameters stacks (current window)
    IMGUI_API void          PushItemWidth(float item_width);
            // push width of items for common large "item+label" widgets. >0.0f:
 width in pixels, <0.0f align xx pixels to the right of window (so -FLT_MIN alwa
ys align width to the right side).
    IMGUI_API void          PopItemWidth();
    IMGUI_API void          SetNextItemWidth(float item_width);
            // set width of the _next_ common large "item+label" widget. >0.0f:
width in pixels, <0.0f align xx pixels to the right of window (so -FLT_MIN alway
s align width to the right side)
    IMGUI_API float         CalcItemWidth();
            // width of item given pushed settings and current cursor position.
NOT necessarily the width of last item unlike most 'Item' functions.
    IMGUI_API void          PushTextWrapPos(float wrap_local_pos_x = 0.0f);
            // push word-wrapping position for Text*() commands. < 0.0f: no wrap
ping; 0.0f: wrap to end of window (or column); > 0.0f: wrap at 'wrap_pos_x' posi
tion in window local space
    IMGUI_API void          PopTextWrapPos();

    // Style read access
    // - Use the ShowStyleEditor() function to interactively see/edit the colors
.
    IMGUI_API ImVec2        GetFontTexUvWhitePixel();
            // get UV coordinate for a white pixel, useful to draw custom shapes
 via the ImDrawList API
    IMGUI_API ImU32         GetColorU32(ImGuiCol idx, float alpha_mul = 1.0f);
            // retrieve given style color with style alpha applied and optional
extra alpha multiplier, packed as a 32-bit value suitable for ImDrawList
    IMGUI_API ImU32         GetColorU32(const ImVec4& col);
            // retrieve given color with style alpha applied, packed as a 32-bit
 value suitable for ImDrawList
    IMGUI_API ImU32         GetColorU32(ImU32 col, float alpha_mul = 1.0f);
            // retrieve given color with style alpha applied, packed as a 32-bit
 value suitable for ImDrawList
    IMGUI_API const ImVec4& GetStyleColorVec4(ImGuiCol idx);
            // retrieve style color as stored in ImGuiStyle structure. use to fe
ed back into PushStyleColor(), otherwise use GetColorU32() to get style color wi
th style alpha baked in.

    // Layout cursor positioning
    // - By "cursor" we mean the current output position.
    // - The typical widget behavior is to output themselves at the current curs
or position, then move the cursor one line down.
    // - You can call SameLine() between widgets to undo the last carriage retur
n and output at the right of the preceding widget.
    // - YOU CAN DO 99% OF WHAT YOU NEED WITH ONLY GetCursorScreenPos() and GetC
ontentRegionAvail().
    // - Attention! We currently have inconsistencies between window-local and a
bsolute positions we will aim to fix with future API:
    //    - Absolute coordinate:        GetCursorScreenPos(), SetCursorScreenPos
(), all ImDrawList:: functions. -> this is the preferred way forward.
    //    - Window-local coordinates:   SameLine(offset), GetCursorPos(), SetCur
sorPos(), GetCursorStartPos(), PushTextWrapPos()
    //    - Window-local coordinates:   GetContentRegionMax(), GetWindowContentR
egionMin(), GetWindowContentRegionMax() --> all obsoleted. YOU DON'T NEED THEM.
    // - GetCursorScreenPos() = GetCursorPos() + GetWindowPos(). GetWindowPos()
is almost only ever useful to convert from window-local to absolute coordinates.
 Try not to use it.
    IMGUI_API ImVec2        GetCursorScreenPos();
            // cursor position, absolute coordinates. THIS IS YOUR BEST FRIEND (
prefer using this rather than GetCursorPos(), also more useful to work with ImDr
awList API).
    IMGUI_API void          SetCursorScreenPos(const ImVec2& pos);
            // cursor position, absolute coordinates. THIS IS YOUR BEST FRIEND.
    IMGUI_API ImVec2        GetContentRegionAvail();
            // available space from current position. THIS IS YOUR BEST FRIEND.
    IMGUI_API ImVec2        GetCursorPos();
            // [window-local] cursor position in window-local coordinates. This
is not your best friend.
    IMGUI_API float         GetCursorPosX();
            // [window-local] "
    IMGUI_API float         GetCursorPosY();
            // [window-local] "
    IMGUI_API void          SetCursorPos(const ImVec2& local_pos);
            // [window-local] "
    IMGUI_API void          SetCursorPosX(float local_x);
            // [window-local] "
    IMGUI_API void          SetCursorPosY(float local_y);
            // [window-local] "
    IMGUI_API ImVec2        GetCursorStartPos();
            // [window-local] initial cursor position, in window-local coordinat
es. Call GetCursorScreenPos() after Begin() to get the absolute coordinates vers
ion.

    // Other layout functions
    IMGUI_API void          Separator();
            // separator, generally horizontal. inside a menu bar or in horizont
al layout mode, this becomes a vertical separator.
    IMGUI_API void          SameLine(float offset_from_start_x=0.0f, float spaci
ng=-1.0f);  // call between widgets or groups to layout them horizontally. X pos
ition given in window coordinates.
    IMGUI_API void          NewLine();
            // undo a SameLine() or force a new line when in a horizontal-layout
 context.
    IMGUI_API void          Spacing();
            // add vertical spacing.
    IMGUI_API void          Dummy(const ImVec2& size);
            // add a dummy item of given size. unlike InvisibleButton(), Dummy()
 won't take the mouse click or be navigable into.
    IMGUI_API void          Indent(float indent_w = 0.0f);
            // move content position toward the right, by indent_w, or style.Ind
entSpacing if indent_w <= 0
    IMGUI_API void          Unindent(float indent_w = 0.0f);
            // move content position back to the left, by indent_w, or style.Ind
entSpacing if indent_w <= 0
    IMGUI_API void          BeginGroup();
            // lock horizontal starting position
    IMGUI_API void          EndGroup();
            // unlock horizontal starting position + capture the whole group bou
nding box into one "item" (so you can use IsItemHovered() or layout primitives s
uch as SameLine() on whole group, etc.)
    IMGUI_API void          AlignTextToFramePadding();
            // vertically align upcoming text baseline to FramePadding.y so that
 it will align properly to regularly framed items (call if you have text on a li
ne before a framed item)
    IMGUI_API float         GetTextLineHeight();
            // ~ FontSize
    IMGUI_API float         GetTextLineHeightWithSpacing();
            // ~ FontSize + style.ItemSpacing.y (distance in pixels between 2 co
nsecutive lines of text)
    IMGUI_API float         GetFrameHeight();
            // ~ FontSize + style.FramePadding.y * 2
    IMGUI_API float         GetFrameHeightWithSpacing();
            // ~ FontSize + style.FramePadding.y * 2 + style.ItemSpacing.y (dist
ance in pixels between 2 consecutive lines of framed widgets)

    // ID stack/scopes
    // Read the FAQ (docs/FAQ.md or http://dearimgui.com/faq) for more details a
bout how ID are handled in dear imgui.
    // - Those questions are answered and impacted by understanding of the ID st
ack system:
    //   - "Q: Why is my widget not reacting when I click on it?"
    //   - "Q: How can I have widgets with an empty label?"
    //   - "Q: How can I have multiple widgets with the same label?"
    // - Short version: ID are hashes of the entire ID stack. If you are creatin
g widgets in a loop you most likely
    //   want to push a unique identifier (e.g. object pointer, loop index) to u
niquely differentiate them.
    // - You can also use the "Label##foobar" syntax within widget label to dist
inguish them from each others.
    // - In this header file we use the "label"/"name" terminology to denote a s
tring that will be displayed + used as an ID,
    //   whereas "str_id" denote a string that is only used as an ID and not nor
mally displayed.
    IMGUI_API void          PushID(const char* str_id);
            // push string into the ID stack (will hash string).
    IMGUI_API void          PushID(const char* str_id_begin, const char* str_id_
end);       // push string into the ID stack (will hash string).
    IMGUI_API void          PushID(const void* ptr_id);
            // push pointer into the ID stack (will hash pointer).
    IMGUI_API void          PushID(int int_id);
            // push integer into the ID stack (will hash integer).
    IMGUI_API void          PopID();
            // pop from the ID stack.
    IMGUI_API ImGuiID       GetID(const char* str_id);
            // calculate unique ID (hash of whole ID stack + given parameter). e
.g. if you want to query into ImGuiStorage yourself
    IMGUI_API ImGuiID       GetID(const char* str_id_begin, const char* str_id_e
nd);
    IMGUI_API ImGuiID       GetID(const void* ptr_id);
    IMGUI_API ImGuiID       GetID(int int_id);

    // Widgets: Text
    IMGUI_API void          TextUnformatted(const char* text, const char* text_e
nd = NULL); // raw text without formatting. Roughly equivalent to Text("%s", tex
t) but: A) doesn't require null terminated string if 'text_end' is specified, B)
 it's faster, no memory copy is done, no buffer size limits, recommended for lon
g chunks of text.
    IMGUI_API void          Text(const char* fmt, ...)
            IM_FMTARGS(1); // formatted text
    IMGUI_API void          TextV(const char* fmt, va_list args)
            IM_FMTLIST(1);
    IMGUI_API void          TextColored(const ImVec4& col, const char* fmt, ...)
            IM_FMTARGS(2); // shortcut for PushStyleColor(ImGuiCol_Text, col); T
ext(fmt, ...); PopStyleColor();
    IMGUI_API void          TextColoredV(const ImVec4& col, const char* fmt, va_
list args)  IM_FMTLIST(2);
    IMGUI_API void          TextDisabled(const char* fmt, ...)
            IM_FMTARGS(1); // shortcut for PushStyleColor(ImGuiCol_Text, style.C
olors[ImGuiCol_TextDisabled]); Text(fmt, ...); PopStyleColor();
    IMGUI_API void          TextDisabledV(const char* fmt, va_list args)
            IM_FMTLIST(1);
    IMGUI_API void          TextWrapped(const char* fmt, ...)
            IM_FMTARGS(1); // shortcut for PushTextWrapPos(0.0f); Text(fmt, ...)
; PopTextWrapPos();. Note that this won't work on an auto-resizing window if the
re's no other widgets to extend the window width, yoy may need to set a size usi
ng SetNextWindowSize().
    IMGUI_API void          TextWrappedV(const char* fmt, va_list args)
            IM_FMTLIST(1);
    IMGUI_API void          LabelText(const char* label, const char* fmt, ...)
            IM_FMTARGS(2); // display text+label aligned the same way as value+l
abel widgets
    IMGUI_API void          LabelTextV(const char* label, const char* fmt, va_li
st args)    IM_FMTLIST(2);
    IMGUI_API void          BulletText(const char* fmt, ...)
            IM_FMTARGS(1); // shortcut for Bullet()+Text()
    IMGUI_API void          BulletTextV(const char* fmt, va_list args)
            IM_FMTLIST(1);
    IMGUI_API void          SeparatorText(const char* label);
            // currently: formatted text with a horizontal line

    // Widgets: Main
    // - Most widgets return true when the value has been changed or when presse
d/selected
    // - You may also use one of the many IsItemXXX functions (e.g. IsItemActive
, IsItemHovered, etc.) to query widget state.
    IMGUI_API bool          Button(const char* label, const ImVec2& size = ImVec
2(0, 0));   // button
    IMGUI_API bool          SmallButton(const char* label);
            // button with (FramePadding.y == 0) to easily embed within text
    IMGUI_API bool          InvisibleButton(const char* str_id, const ImVec2& si
ze, ImGuiButtonFlags flags = 0); // flexible button behavior without the visuals
, frequently useful to build custom behaviors using the public api (along with I
sItemActive, IsItemHovered, etc.)
    IMGUI_API bool          ArrowButton(const char* str_id, ImGuiDir dir);
            // square button with an arrow shape
    IMGUI_API bool          Checkbox(const char* label, bool* v);
    IMGUI_API bool          CheckboxFlags(const char* label, int* flags, int fla
gs_value);
    IMGUI_API bool          CheckboxFlags(const char* label, unsigned int* flags
, unsigned int flags_value);
    IMGUI_API bool          RadioButton(const char* label, bool active);
            // use with e.g. if (RadioButton("one", my_value==1)) { my_value = 1
; }
    IMGUI_API bool          RadioButton(const char* label, int* v, int v_button)
;           // shortcut to handle the above pattern when value is an integer
    IMGUI_API void          ProgressBar(float fraction, const ImVec2& size_arg =
 ImVec2(-FLT_MIN, 0), const char* overlay = NULL);
    IMGUI_API void          Bullet();
            // draw a small circle + keep the cursor on the same line. advance c
ursor x position by GetTreeNodeToLabelSpacing(), same distance that TreeNode() u
ses
    IMGUI_API bool          TextLink(const char* label);
            // hyperlink text button, return true when clicked
    IMGUI_API bool          TextLinkOpenURL(const char* label, const char* url =
 NULL);     // hyperlink text button, automatically open file/url when clicked

    // Widgets: Images
    // - Read about ImTextureID/ImTextureRef  here: https://github.com/ocornut/i
mgui/wiki/Image-Loading-and-Displaying-Examples
    // - 'uv0' and 'uv1' are texture coordinates. Read about them from the same
link above.
    // - Image() pads adds style.ImageBorderSize on each side, ImageButton() add
s style.FramePadding on each side.
    // - ImageButton() draws a background based on regular Button() color + opti
onally an inner background if specified.
    // - An obsolete version of Image(), before 1.91.9 (March 2025), had a 'tint
_col' parameter which is now supported by the ImageWithBg() function.
    IMGUI_API void          Image(ImTextureRef tex_ref, const ImVec2& image_size
, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1));
    IMGUI_API void          ImageWithBg(ImTextureRef tex_ref, const ImVec2& imag
e_size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), cons
t ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1,
1));
    IMGUI_API bool          ImageButton(const char* str_id, ImTextureRef tex_ref
, const ImVec2& image_size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1
= ImVec2(1, 1), const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_co
l = ImVec4(1, 1, 1, 1));

    // Widgets: Combo Box (Dropdown)
    // - The BeginCombo()/EndCombo() api allows you to manage your contents and
selection state however you want it, by creating e.g. Selectable() items.
    // - The old Combo() api are helpers over BeginCombo()/EndCombo() which are
kept available for convenience purpose. This is analogous to how ListBox are cre
ated.
    IMGUI_API bool          BeginCombo(const char* label, const char* preview_va
lue, ImGuiComboFlags flags = 0);
    IMGUI_API void          EndCombo(); // only call EndCombo() if BeginCombo()
returns true!
    IMGUI_API bool          Combo(const char* label, int* current_item, const ch
ar* const items[], int items_count, int popup_max_height_in_items = -1);
    IMGUI_API bool          Combo(const char* label, int* current_item, const ch
ar* items_separated_by_zeros, int popup_max_height_in_items = -1);      // Separ
ate items with \0 within a string, end item-list with \0\0. e.g. "One\0Two\0Thre
e\0"
    IMGUI_API bool          Combo(const char* label, int* current_item, const ch
ar* (*getter)(void* user_data, int idx), void* user_data, int items_count, int p
opup_max_height_in_items = -1);

    // Widgets: Drag Sliders
    // - CTRL+Click on any drag box to turn them into an input box. Manually inp
ut values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_
AlwaysClamp to always clamp.
    // - For all the Float2/Float3/Float4/Int2/Int3/Int4 versions of every funct
ion, note that a 'float v[X]' function argument is the same as 'float* v',
    //   the array syntax is just a way to document the number of elements that
are expected to be accessible. You can pass address of your first element out of
 a contiguous set, e.g. &myvector.x
    // - Adjust format string to decorate the value with a prefix, a suffix, or
adapt the editing and display precision e.g. "%.3f" -> 1.234; "%5.2f secs" -> 01
.23 secs; "Biscuit: %.0f" -> Biscuit: 1; etc.
    // - Format string may also be set to NULL or use the default format ("%f" o
r "%d").
    // - Speed are per-pixel of mouse movement (v_speed=0.2f: mouse needs to mov
e by 5 pixels to increase value by 1). For keyboard/gamepad navigation, minimum
speed is Max(v_speed, minimum_step_at_given_precision).
    // - Use v_min < v_max to clamp edits to given limits. Note that CTRL+Click
manual input can override those limits if ImGuiSliderFlags_AlwaysClamp is not us
ed.
    // - Use v_max = FLT_MAX / INT_MAX etc to avoid clamping to a maximum, same
with v_min = -FLT_MAX / INT_MIN to avoid clamping to a minimum.
    // - We use the same sets of flags for DragXXX() and SliderXXX() functions a
s the features are the same and it makes it easier to swap them.
    // - Legacy: Pre-1.78 there are DragXXX() function signatures that take a fi
nal `float power=1.0f' argument instead of the `ImGuiSliderFlags flags=0' argume
nt.
    //   If you get a warning converting a float to ImGuiSliderFlags, read https
://github.com/ocornut/imgui/issues/3361
    IMGUI_API bool          DragFloat(const char* label, float* v, float v_speed
 = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", Im
GuiSliderFlags flags = 0);     // If v_min >= v_max we have no bound
    IMGUI_API bool          DragFloat2(const char* label, float v[2], float v_sp
eed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f",
 ImGuiSliderFlags flags = 0);
    IMGUI_API bool          DragFloat3(const char* label, float v[3], float v_sp
eed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f",
 ImGuiSliderFlags flags = 0);
    IMGUI_API bool          DragFloat4(const char* label, float v[4], float v_sp
eed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f",
 ImGuiSliderFlags flags = 0);
    IMGUI_API bool          DragFloatRange2(const char* label, float* v_current_
min, float* v_current_max, float v_speed = 1.0f, float v_min = 0.0f, float v_max
 = 0.0f, const char* format = "%.3f", const char* format_max = NULL, ImGuiSlider
Flags flags = 0);
    IMGUI_API bool          DragInt(const char* label, int* v, float v_speed = 1
.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlags f
lags = 0);  // If v_min >= v_max we have no bound
    IMGUI_API bool          DragInt2(const char* label, int v[2], float v_speed
= 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlag
s flags = 0);
    IMGUI_API bool          DragInt3(const char* label, int v[3], float v_speed
= 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlag
s flags = 0);
    IMGUI_API bool          DragInt4(const char* label, int v[4], float v_speed
= 1.0f, int v_min = 0, int v_max = 0, const char* format = "%d", ImGuiSliderFlag
s flags = 0);
    IMGUI_API bool          DragIntRange2(const char* label, int* v_current_min,
 int* v_current_max, float v_speed = 1.0f, int v_min = 0, int v_max = 0, const c
har* format = "%d", const char* format_max = NULL, ImGuiSliderFlags flags = 0);
    IMGUI_API bool          DragScalar(const char* label, ImGuiDataType data_typ
e, void* p_data, float v_speed = 1.0f, const void* p_min = NULL, const void* p_m
ax = NULL, const char* format = NULL, ImGuiSliderFlags flags = 0);
    IMGUI_API bool          DragScalarN(const char* label, ImGuiDataType data_ty
pe, void* p_data, int components, float v_speed = 1.0f, const void* p_min = NULL
, const void* p_max = NULL, const char* format = NULL, ImGuiSliderFlags flags =
0);

    // Widgets: Regular Sliders
    // - CTRL+Click on any slider to turn them into an input box. Manually input
 values aren't clamped by default and can go off-bounds. Use ImGuiSliderFlags_Al
waysClamp to always clamp.
    // - Adjust format string to decorate the value with a prefix, a suffix, or
adapt the editing and display precision e.g. "%.3f" -> 1.234; "%5.2f secs" -> 01
.23 secs; "Biscuit: %.0f" -> Biscuit: 1; etc.
    // - Format string may also be set to NULL or use the default format ("%f" o
r "%d").
    // - Legacy: Pre-1.78 there are SliderXXX() function signatures that take a
final `float power=1.0f' argument instead of the `ImGuiSliderFlags flags=0' argu
ment.
    //   If you get a warning converting a float to ImGuiSliderFlags, read https
://github.com/ocornut/imgui/issues/3361
    IMGUI_API bool          SliderFloat(const char* label, float* v, float v_min
, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);     //
adjust format to decorate the value with a prefix or a suffix for in-slider labe
ls or unit display.
    IMGUI_API bool          SliderFloat2(const char* label, float v[2], float v_
min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          SliderFloat3(const char* label, float v[3], float v_
min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          SliderFloat4(const char* label, float v[4], float v_
min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          SliderAngle(const char* label, float* v_rad, float v
_degrees_min = -360.0f, float v_degrees_max = +360.0f, const char* format = "%.0
f deg", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          SliderInt(const char* label, int* v, int v_min, int
v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          SliderInt2(const char* label, int v[2], int v_min, i
nt v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          SliderInt3(const char* label, int v[3], int v_min, i
nt v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          SliderInt4(const char* label, int v[4], int v_min, i
nt v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    IMGUI_API bool          SliderScalar(const char* label, ImGuiDataType data_t
ype, void* p_data, const void* p_min, const void* p_max, const char* format = NU
LL, ImGuiSliderFlags flags = 0);
    IMGUI_API bool          SliderScalarN(const char* label, ImGuiDataType data_
type, void* p_data, int components, const void* p_min, const void* p_max, const
char* format = NULL, ImGuiSliderFlags flags = 0);
    IMGUI_API bool          VSliderFloat(const char* label, const ImVec2& size,
float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlag
s flags = 0);
    IMGUI_API bool          VSliderInt(const char* label, const ImVec2& size, in
t* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags =
0);
    IMGUI_API bool          VSliderScalar(const char* label, const ImVec2& size,
 ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, co
nst char* format = NULL, ImGuiSliderFlags flags = 0);

    // Widgets: Input with Keyboard
    // - If you want to use InputText() with std::string or any custom dynamic s
tring type, see misc/cpp/imgui_stdlib.h and comments in imgui_demo.cpp.
    // - Most of the ImGuiInputTextFlags flags are only useful for InputText() a
nd not for InputFloatX, InputIntX, InputDouble etc.
    IMGUI_API bool          InputText(const char* label, char* buf, size_t buf_s
ize, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void
* user_data = NULL);
    IMGUI_API bool          InputTextMultiline(const char* label, char* buf, siz
e_t buf_size, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0,
ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
    IMGUI_API bool          InputTextWithHint(const char* label, const char* hin
t, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCall
back callback = NULL, void* user_data = NULL);
    IMGUI_API bool          InputFloat(const char* label, float* v, float step =
 0.0f, float step_fast = 0.0f, const char* format = "%.3f", ImGuiInputTextFlags
flags = 0);
    IMGUI_API bool          InputFloat2(const char* label, float v[2], const cha
r* format = "%.3f", ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputFloat3(const char* label, float v[3], const cha
r* format = "%.3f", ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputFloat4(const char* label, float v[4], const cha
r* format = "%.3f", ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputInt(const char* label, int* v, int step = 1, in
t step_fast = 100, ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputInt2(const char* label, int v[2], ImGuiInputTex
tFlags flags = 0);
    IMGUI_API bool          InputInt3(const char* label, int v[3], ImGuiInputTex
tFlags flags = 0);
    IMGUI_API bool          InputInt4(const char* label, int v[4], ImGuiInputTex
tFlags flags = 0);
    IMGUI_API bool          InputDouble(const char* label, double* v, double ste
p = 0.0, double step_fast = 0.0, const char* format = "%.6f", ImGuiInputTextFlag
s flags = 0);
    IMGUI_API bool          InputScalar(const char* label, ImGuiDataType data_ty
pe, void* p_data, const void* p_step = NULL, const void* p_step_fast = NULL, con
st char* format = NULL, ImGuiInputTextFlags flags = 0);
    IMGUI_API bool          InputScalarN(const char* label, ImGuiDataType data_t
ype, void* p_data, int components, const void* p_step = NULL, const void* p_step
_fast = NULL, const char* format = NULL, ImGuiInputTextFlags flags = 0);

    // Widgets: Color Editor/Picker (tip: the ColorEdit* functions have a little
 color square that can be left-clicked to open a picker, and right-clicked to op
en an option menu.)
    // - Note that in C++ a 'float v[X]' function argument is the _same_ as 'flo
at* v', the array syntax is just a way to document the number of elements that a
re expected to be accessible.
    // - You can pass the address of a first float element out of a contiguous s
tructure, e.g. &myvector.x
    IMGUI_API bool          ColorEdit3(const char* label, float col[3], ImGuiCol
orEditFlags flags = 0);
    IMGUI_API bool          ColorEdit4(const char* label, float col[4], ImGuiCol
orEditFlags flags = 0);
    IMGUI_API bool          ColorPicker3(const char* label, float col[3], ImGuiC
olorEditFlags flags = 0);
    IMGUI_API bool          ColorPicker4(const char* label, float col[4], ImGuiC
olorEditFlags flags = 0, const float* ref_col = NULL);
    IMGUI_API bool          ColorButton(const char* desc_id, const ImVec4& col,
ImGuiColorEditFlags flags = 0, const ImVec2& size = ImVec2(0, 0)); // display a
color square/button, hover for details, return true when pressed.
    IMGUI_API void          SetColorEditOptions(ImGuiColorEditFlags flags);
                // initialize current options (generally on application startup)
 if you want to select a default format, picker type, etc. User will be able to
change many settings, unless you pass the _NoOptions flag to your calls.

    // Widgets: Trees
    // - TreeNode functions return true when the node is open, in which case you
 need to also call TreePop() when you are finished displaying the tree node cont
ents.
    IMGUI_API bool          TreeNode(const char* label);
    IMGUI_API bool          TreeNode(const char* str_id, const char* fmt, ...) I
M_FMTARGS(2);   // helper variation to easily decorelate the id from the display
ed string. Read the FAQ about why and how to use ID. to align arbitrary text at
the same level as a TreeNode() you can use Bullet().
    IMGUI_API bool          TreeNode(const void* ptr_id, const char* fmt, ...) I
M_FMTARGS(2);   // "
    IMGUI_API bool          TreeNodeV(const char* str_id, const char* fmt, va_li
st args) IM_FMTLIST(2);
    IMGUI_API bool          TreeNodeV(const void* ptr_id, const char* fmt, va_li
st args) IM_FMTLIST(2);
    IMGUI_API bool          TreeNodeEx(const char* label, ImGuiTreeNodeFlags fla
gs = 0);
    IMGUI_API bool          TreeNodeEx(const char* str_id, ImGuiTreeNodeFlags fl
ags, const char* fmt, ...) IM_FMTARGS(3);
    IMGUI_API bool          TreeNodeEx(const void* ptr_id, ImGuiTreeNodeFlags fl
ags, const char* fmt, ...) IM_FMTARGS(3);
    IMGUI_API bool          TreeNodeExV(const char* str_id, ImGuiTreeNodeFlags f
lags, const char* fmt, va_list args) IM_FMTLIST(3);
    IMGUI_API bool          TreeNodeExV(const void* ptr_id, ImGuiTreeNodeFlags f
lags, const char* fmt, va_list args) IM_FMTLIST(3);
    IMGUI_API void          TreePush(const char* str_id);
                // ~ Indent()+PushID(). Already called by TreeNode() when return
ing true, but you can call TreePush/TreePop yourself if desired.
    IMGUI_API void          TreePush(const void* ptr_id);
                // "
    IMGUI_API void          TreePop();
                // ~ Unindent()+PopID()
    IMGUI_API float         GetTreeNodeToLabelSpacing();
                // horizontal distance preceding label when using TreeNode*() or
 Bullet() == (g.FontSize + style.FramePadding.x*2) for a regular unframed TreeNo
de
    IMGUI_API bool          CollapsingHeader(const char* label, ImGuiTreeNodeFla
gs flags = 0);  // if returning 'true' the header is open. doesn't indent nor pu
sh on ID stack. user doesn't have to call TreePop().
    IMGUI_API bool          CollapsingHeader(const char* label, bool* p_visible,
 ImGuiTreeNodeFlags flags = 0); // when 'p_visible != NULL': if '*p_visible==tru
e' display an additional small close button on upper right of the header which w
ill set the bool to false when clicked, if '*p_visible==false' don't display the
 header.
    IMGUI_API void          SetNextItemOpen(bool is_open, ImGuiCond cond = 0);
                // set next TreeNode/CollapsingHeader open state.
    IMGUI_API void          SetNextItemStorageID(ImGuiID storage_id);
                // set id to use for open/close storage (default to same as item
 id).

    // Widgets: Selectables
    // - A selectable highlights when hovered, and can display another color whe
n selected.
    // - Neighbors selectable extend their highlight bounds in order to leave no
 gap between them. This is so a series of selected Selectable appear contiguous.
    IMGUI_API bool          Selectable(const char* label, bool selected = false,
 ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0)); // "bool se
lected" carry the selection state (read-only). Selectable() is clicked is return
s true so you can modify your selection state. size.x==0.0: use remaining width,
 size.x>0.0: specify width. size.y==0.0: use label height, size.y>0.0: specify h
eight
    IMGUI_API bool          Selectable(const char* label, bool* p_selected, ImGu
iSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));      // "bool* p
_selected" point to the selection state (read-write), as a convenient helper.

    // Multi-selection system for Selectable(), Checkbox(), TreeNode() functions
 [BETA]
    // - This enables standard multi-selection/range-selection idioms (CTRL+Mous
e/Keyboard, SHIFT+Mouse/Keyboard, etc.) in a way that also allow a clipper to be
 used.
    // - ImGuiSelectionUserData is often used to store your item index within th
e current view (but may store something else).
    // - Read comments near ImGuiMultiSelectIO for instructions/details and see
'Demo->Widgets->Selection State & Multi-Select' for demo.
    // - TreeNode() is technically supported but... using this correctly is more
 complicated. You need some sort of linear/random access to your tree,
    //   which is suited to advanced trees setups already implementing filters a
nd clipper. We will work simplifying the current demo.
    // - 'selection_size' and 'items_count' parameters are optional and used by
a few features. If they are costly for you to compute, you may avoid them.
    IMGUI_API ImGuiMultiSelectIO*   BeginMultiSelect(ImGuiMultiSelectFlags flags
, int selection_size = -1, int items_count = -1);
    IMGUI_API ImGuiMultiSelectIO*   EndMultiSelect();
    IMGUI_API void                  SetNextItemSelectionUserData(ImGuiSelectionU
serData selection_user_data);
    IMGUI_API bool                  IsItemToggledSelection();
                // Was the last item selection state toggled? Useful if you need
 the per-item information _before_ reaching EndMultiSelect(). We only returns to
ggle _event_ in order to handle clipping correctly.

    // Widgets: List Boxes
    // - This is essentially a thin wrapper to using BeginChild/EndChild with th
e ImGuiChildFlags_FrameStyle flag for stylistic changes + displaying a label.
    // - If you don't need a label you can probably simply use BeginChild() with
 the ImGuiChildFlags_FrameStyle flag for the same result.
    // - You can submit contents and manage your selection state however you wan
t it, by creating e.g. Selectable() or any other items.
    // - The simplified/old ListBox() api are helpers over BeginListBox()/EndLis
tBox() which are kept available for convenience purpose. This is analogous to ho
w Combos are created.
    // - Choose frame width:   size.x > 0.0f: custom  /  size.x < 0.0f or -FLT_M
IN: right-align   /  size.x = 0.0f (default): use current ItemWidth
    // - Choose frame height:  size.y > 0.0f: custom  /  size.y < 0.0f or -FLT_M
IN: bottom-align  /  size.y = 0.0f (default): arbitrary default height which can
 fit ~7 items
    IMGUI_API bool          BeginListBox(const char* label, const ImVec2& size =
 ImVec2(0, 0)); // open a framed scrolling region
    IMGUI_API void          EndListBox();
                // only call EndListBox() if BeginListBox() returned true!
    IMGUI_API bool          ListBox(const char* label, int* current_item, const
char* const items[], int items_count, int height_in_items = -1);
    IMGUI_API bool          ListBox(const char* label, int* current_item, const
char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int
 height_in_items = -1);

    // Widgets: Data Plotting
    // - Consider using ImPlot (https://github.com/epezent/implot) which is much
 better!
    IMGUI_API void          PlotLines(const char* label, const float* values, in
t values_count, int values_offset = 0, const char* overlay_text = NULL, float sc
ale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0, 0),
int stride = sizeof(float));
    IMGUI_API void          PlotLines(const char* label, float(*values_getter)(v
oid* data, int idx), void* data, int values_count, int values_offset = 0, const
char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT_MAX,
 ImVec2 graph_size = ImVec2(0, 0));
    IMGUI_API void          PlotHistogram(const char* label, const float* values
, int values_count, int values_offset = 0, const char* overlay_text = NULL, floa
t scale_min = FLT_MAX, float scale_max = FLT_MAX, ImVec2 graph_size = ImVec2(0,
0), int stride = sizeof(float));
    IMGUI_API void          PlotHistogram(const char* label, float (*values_gett
er)(void* data, int idx), void* data, int values_count, int values_offset = 0, c
onst char* overlay_text = NULL, float scale_min = FLT_MAX, float scale_max = FLT
_MAX, ImVec2 graph_size = ImVec2(0, 0));

    // Widgets: Value() Helpers.
    // - Those are merely shortcut to calling Text() with a format string. Outpu
t single value in "name: value" format (tip: freely declare more in your code to
 handle your types. you can add functions to the ImGui namespace)
    IMGUI_API void          Value(const char* prefix, bool b);
    IMGUI_API void          Value(const char* prefix, int v);
    IMGUI_API void          Value(const char* prefix, unsigned int v);
    IMGUI_API void          Value(const char* prefix, float v, const char* float
_format = NULL);

    // Widgets: Menus
    // - Use BeginMenuBar() on a window ImGuiWindowFlags_MenuBar to append to it
s menu bar.
    // - Use BeginMainMenuBar() to create a menu bar at the top of the screen an
d append to it.
    // - Use BeginMenu() to create a menu. You can call BeginMenu() multiple tim
e with the same identifier to append more items to it.
    // - Not that MenuItem() keyboardshortcuts are displayed as a convenience bu
t _not processed_ by Dear ImGui at the moment.
    IMGUI_API bool          BeginMenuBar();
                // append to menu-bar of current window (requires ImGuiWindowFla
gs_MenuBar flag set on parent window).
    IMGUI_API void          EndMenuBar();
                // only call EndMenuBar() if BeginMenuBar() returns true!
    IMGUI_API bool          BeginMainMenuBar();
                // create and append to a full screen menu-bar.
    IMGUI_API void          EndMainMenuBar();
                // only call EndMainMenuBar() if BeginMainMenuBar() returns true
!
    IMGUI_API bool          BeginMenu(const char* label, bool enabled = true);
                // create a sub-menu entry. only call EndMenu() if this returns
true!
    IMGUI_API void          EndMenu();
                // only call EndMenu() if BeginMenu() returns true!
    IMGUI_API bool          MenuItem(const char* label, const char* shortcut = N
ULL, bool selected = false, bool enabled = true);  // return true when activated
.
    IMGUI_API bool          MenuItem(const char* label, const char* shortcut, bo
ol* p_selected, bool enabled = true);              // return true when activated
 + toggle (*p_selected) if p_selected != NULL

    // Tooltips
    // - Tooltips are windows following the mouse. They do not take focus away.
    // - A tooltip window can contain items of any types.
    // - SetTooltip() is more or less a shortcut for the 'if (BeginTooltip()) {
Text(...); EndTooltip(); }' idiom (with a subtlety that it discard any previousl
y submitted tooltip)
    IMGUI_API bool          BeginTooltip();
                // begin/append a tooltip window.
    IMGUI_API void          EndTooltip();
                // only call EndTooltip() if BeginTooltip()/BeginItemTooltip() r
eturns true!
    IMGUI_API void          SetTooltip(const char* fmt, ...) IM_FMTARGS(1);
                // set a text-only tooltip. Often used after a ImGui::IsItemHove
red() check. Override any previous call to SetTooltip().
    IMGUI_API void          SetTooltipV(const char* fmt, va_list args) IM_FMTLIS
T(1);

    // Tooltips: helpers for showing a tooltip when hovering an item
    // - BeginItemTooltip() is a shortcut for the 'if (IsItemHovered(ImGuiHovere
dFlags_ForTooltip) && BeginTooltip())' idiom.
    // - SetItemTooltip() is a shortcut for the 'if (IsItemHovered(ImGuiHoveredF
lags_ForTooltip)) { SetTooltip(...); }' idiom.
    // - Where 'ImGuiHoveredFlags_ForTooltip' itself is a shortcut to use 'style
.HoverFlagsForTooltipMouse' or 'style.HoverFlagsForTooltipNav' depending on acti
ve input type. For mouse it defaults to 'ImGuiHoveredFlags_Stationary | ImGuiHov
eredFlags_DelayShort'.
    IMGUI_API bool          BeginItemTooltip();
                // begin/append a tooltip window if preceding item was hovered.
    IMGUI_API void          SetItemTooltip(const char* fmt, ...) IM_FMTARGS(1);
                // set a text-only tooltip if preceding item was hovered. overri
de any previous call to SetTooltip().
    IMGUI_API void          SetItemTooltipV(const char* fmt, va_list args) IM_FM
TLIST(1);

    // Popups, Modals
    //  - They block normal mouse hovering detection (and therefore most mouse i
nteractions) behind them.
    //  - If not modal: they can be closed by clicking anywhere outside them, or
 by pressing ESCAPE.
    //  - Their visibility state (~bool) is held internally instead of being hel
d by the programmer as we are used to with regular Begin*() calls.
    //  - The 3 properties above are related: we need to retain popup visibility
 state in the library because popups may be closed as any time.
    //  - You can bypass the hovering restriction by using ImGuiHoveredFlags_All
owWhenBlockedByPopup when calling IsItemHovered() or IsWindowHovered().
    //  - IMPORTANT: Popup identifiers are relative to the current ID stack, so
OpenPopup and BeginPopup generally needs to be at the same level of the stack.
    //    This is sometimes leading to confusing mistakes. May rework this in th
e future.
    //  - BeginPopup(): query popup state, if open start appending into the wind
ow. Call EndPopup() afterwards if returned true. ImGuiWindowFlags are forwarded
to the window.
    //  - BeginPopupModal(): block every interaction behind the window, cannot b
e closed by user, add a dimming background, has a title bar.
    IMGUI_API bool          BeginPopup(const char* str_id, ImGuiWindowFlags flag
s = 0);                         // return true if the popup is open, and you can
 start outputting to it.
    IMGUI_API bool          BeginPopupModal(const char* name, bool* p_open = NUL
L, ImGuiWindowFlags flags = 0); // return true if the modal is open, and you can
 start outputting to it.
    IMGUI_API void          EndPopup();
                                // only call EndPopup() if BeginPopupXXX() retur
ns true!

    // Popups: open/close functions
    //  - OpenPopup(): set popup state to open. ImGuiPopupFlags are available fo
r opening options.
    //  - If not modal: they can be closed by clicking anywhere outside them, or
 by pressing ESCAPE.
    //  - CloseCurrentPopup(): use inside the BeginPopup()/EndPopup() scope to c
lose manually.
    //  - CloseCurrentPopup() is called by default by Selectable()/MenuItem() wh
en activated (FIXME: need some options).
    //  - Use ImGuiPopupFlags_NoOpenOverExistingPopup to avoid opening a popup i
f there's already one at the same level. This is equivalent to e.g. testing for
!IsAnyPopupOpen() prior to OpenPopup().
    //  - Use IsWindowAppearing() after BeginPopup() to tell if a window just op
ened.
    //  - IMPORTANT: Notice that for OpenPopupOnItemClick() we exceptionally def
ault flags to 1 (== ImGuiPopupFlags_MouseButtonRight) for backward compatibility
 with older API taking 'int mouse_button = 1' parameter
    IMGUI_API void          OpenPopup(const char* str_id, ImGuiPopupFlags popup_
flags = 0);                     // call to mark popup as open (don't call every
frame!).
    IMGUI_API void          OpenPopup(ImGuiID id, ImGuiPopupFlags popup_flags =
0);                             // id overload to facilitate calling from nested
 stacks
    IMGUI_API void          OpenPopupOnItemClick(const char* str_id = NULL, ImGu
iPopupFlags popup_flags = 1);   // helper to open popup when clicked on last ite
m. Default to ImGuiPopupFlags_MouseButtonRight == 1. (note: actually triggers on
 the mouse _released_ event to be consistent with popup behaviors)
    IMGUI_API void          CloseCurrentPopup();
                                // manually close the popup we have begin-ed int
o.

    // Popups: open+begin combined functions helpers
    //  - Helpers to do OpenPopup+BeginPopup where the Open action is triggered
by e.g. hovering an item and right-clicking.
    //  - They are convenient to easily create context menus, hence the name.
    //  - IMPORTANT: Notice that BeginPopupContextXXX takes ImGuiPopupFlags just
 like OpenPopup() and unlike BeginPopup(). For full consistency, we may add ImGu
iWindowFlags to the BeginPopupContextXXX functions in the future.
    //  - IMPORTANT: Notice that we exceptionally default their flags to 1 (== I
mGuiPopupFlags_MouseButtonRight) for backward compatibility with older API takin
g 'int mouse_button = 1' parameter, so if you add other flags remember to re-add
 the ImGuiPopupFlags_MouseButtonRight.
    IMGUI_API bool          BeginPopupContextItem(const char* str_id = NULL, ImG
uiPopupFlags popup_flags = 1);  // open+begin popup when clicked on last item. U
se str_id==NULL to associate the popup to previous item. If you want to use that
 on a non-interactive item such as Text() you need to pass in an explicit ID her
e. read comments in .cpp!
    IMGUI_API bool          BeginPopupContextWindow(const char* str_id = NULL, I
mGuiPopupFlags popup_flags = 1);// open+begin popup when clicked on current wind
ow.
    IMGUI_API bool          BeginPopupContextVoid(const char* str_id = NULL, ImG
uiPopupFlags popup_flags = 1);  // open+begin popup when clicked in void (where
there are no windows).

    // Popups: query functions
    //  - IsPopupOpen(): return true if the popup is open at the current BeginPo
pup() level of the popup stack.
    //  - IsPopupOpen() with ImGuiPopupFlags_AnyPopupId: return true if any popu
p is open at the current BeginPopup() level of the popup stack.
    //  - IsPopupOpen() with ImGuiPopupFlags_AnyPopupId + ImGuiPopupFlags_AnyPop
upLevel: return true if any popup is open.
    IMGUI_API bool          IsPopupOpen(const char* str_id, ImGuiPopupFlags flag
s = 0);                         // return true if the popup is open.

    // Tables
    // - Full-featured replacement for old Columns API.
    // - See Demo->Tables for demo code. See top of imgui_tables.cpp for general
 commentary.
    // - See ImGuiTableFlags_ and ImGuiTableColumnFlags_ enums for a description
 of available flags.
    // The typical call flow is:
    // - 1. Call BeginTable(), early out if returning false.
    // - 2. Optionally call TableSetupColumn() to submit column name/flags/defau
lts.
    // - 3. Optionally call TableSetupScrollFreeze() to request scroll freezing
of columns/rows.
    // - 4. Optionally call TableHeadersRow() to submit a header row. Names are
pulled from TableSetupColumn() data.
    // - 5. Populate contents:
    //    - In most situations you can use TableNextRow() + TableSetColumnIndex(
N) to start appending into a column.
    //    - If you are using tables as a sort of grid, where every column is hol
ding the same type of contents,
    //      you may prefer using TableNextColumn() instead of TableNextRow() + T
ableSetColumnIndex().
    //      TableNextColumn() will automatically wrap-around into the next row i
f needed.
    //    - IMPORTANT: Comparatively to the old Columns() API, we need to call T
ableNextColumn() for the first column!
    //    - Summary of possible call flow:
    //        - TableNextRow() -> TableSetColumnIndex(0) -> Text("Hello 0") -> T
ableSetColumnIndex(1) -> Text("Hello 1")  // OK
    //        - TableNextRow() -> TableNextColumn()      -> Text("Hello 0") -> T
ableNextColumn()      -> Text("Hello 1")  // OK
    //        -                   TableNextColumn()      -> Text("Hello 0") -> T
ableNextColumn()      -> Text("Hello 1")  // OK: TableNextColumn() automatically
 gets to next row!
    //        - TableNextRow()                           -> Text("Hello 0")
                                          // Not OK! Missing TableSetColumnIndex
() or TableNextColumn()! Text will not appear!
    // - 5. Call EndTable()
    IMGUI_API bool          BeginTable(const char* str_id, int columns, ImGuiTab
leFlags flags = 0, const ImVec2& outer_size = ImVec2(0.0f, 0.0f), float inner_wi
dth = 0.0f);
    IMGUI_API void          EndTable();
// only call EndTable() if BeginTable() returns true!
    IMGUI_API void          TableNextRow(ImGuiTableRowFlags row_flags = 0, float
 min_row_height = 0.0f); // append into the first cell of a new row.
    IMGUI_API bool          TableNextColumn();
// append into the next column (or first column of next row if currently in last
 column). Return true when column is visible.
    IMGUI_API bool          TableSetColumnIndex(int column_n);
// append into the specified column. Return true when column is visible.

    // Tables: Headers & Columns declaration
    // - Use TableSetupColumn() to specify label, resizing policy, default width
/weight, id, various other flags etc.
    // - Use TableHeadersRow() to create a header row and automatically submit a
 TableHeader() for each column.
    //   Headers are required to perform: reordering, sorting, and opening the c
ontext menu.
    //   The context menu can also be made available in columns body using ImGui
TableFlags_ContextMenuInBody.
    // - You may manually submit headers using TableNextRow() + TableHeader() ca
lls, but this is only useful in
    //   some advanced use cases (e.g. adding custom widgets in header row).
    // - Use TableSetupScrollFreeze() to lock columns/rows so they stay visible
when scrolled.
    IMGUI_API void          TableSetupColumn(const char* label, ImGuiTableColumn
Flags flags = 0, float init_width_or_weight = 0.0f, ImGuiID user_id = 0);
    IMGUI_API void          TableSetupScrollFreeze(int cols, int rows);
// lock columns/rows so they stay visible when scrolled.
    IMGUI_API void          TableHeader(const char* label);
// submit one header cell manually (rarely used)
    IMGUI_API void          TableHeadersRow();
// submit a row with headers cells based on data provided to TableSetupColumn()
+ submit context menu
    IMGUI_API void          TableAngledHeadersRow();
// submit a row with angled headers for every column with the ImGuiTableColumnFl
ags_AngledHeader flag. MUST BE FIRST ROW.

    // Tables: Sorting & Miscellaneous functions
    // - Sorting: call TableGetSortSpecs() to retrieve latest sort specs for the
 table. NULL when not sorting.
    //   When 'sort_specs->SpecsDirty == true' you should sort your data. It wil
l be true when sorting specs have
    //   changed since last call, or the first time. Make sure to set 'SpecsDirt
y = false' after sorting,
    //   else you may wastefully sort your data every frame!
    // - Functions args 'int column_n' treat the default value of -1 as the same
 as passing the current column index.
    IMGUI_API ImGuiTableSortSpecs*  TableGetSortSpecs();
// get latest sort specs for the table (NULL if not sorting).  Lifetime: don't h
old on this pointer over multiple frames or past any subsequent call to BeginTab
le().
    IMGUI_API int                   TableGetColumnCount();
// return number of columns (value passed to BeginTable)
    IMGUI_API int                   TableGetColumnIndex();
// return current column index.
    IMGUI_API int                   TableGetRowIndex();
// return current row index (header rows are accounted for)
    IMGUI_API const char*           TableGetColumnName(int column_n = -1);
// return "" if column didn't have a name declared by TableSetupColumn(). Pass -
1 to use current column.
    IMGUI_API ImGuiTableColumnFlags TableGetColumnFlags(int column_n = -1);
// return column flags so you can query their Enabled/Visible/Sorted/Hovered sta
tus flags. Pass -1 to use current column.
    IMGUI_API void                  TableSetColumnEnabled(int column_n, bool v);
// change user accessible enabled/disabled state of a column. Set to false to hi
de the column. User can use the context menu to change this themselves (right-cl
ick in headers, or right-click in columns body with ImGuiTableFlags_ContextMenuI
nBody)
    IMGUI_API int                   TableGetHoveredColumn();
// return hovered column. return -1 when table is not hovered. return columns_co
unt if the unused space at the right of visible columns is hovered. Can also use
 (TableGetColumnFlags() & ImGuiTableColumnFlags_IsHovered) instead.
    IMGUI_API void                  TableSetBgColor(ImGuiTableBgTarget target, I
mU32 color, int column_n = -1);  // change the color of a cell, row, or column.
See ImGuiTableBgTarget_ flags for details.

    // Legacy Columns API (prefer using Tables!)
    // - You can also use SameLine(pos_x) to mimic simplified columns.
    IMGUI_API void          Columns(int count = 1, const char* id = NULL, bool b
orders = true);
    IMGUI_API void          NextColumn();
                // next column, defaults to current row or next row if the curre
nt row is finished
    IMGUI_API int           GetColumnIndex();
                // get current column index
    IMGUI_API float         GetColumnWidth(int column_index = -1);
                // get column width (in pixels). pass -1 to use current column
    IMGUI_API void          SetColumnWidth(int column_index, float width);
                // set column width (in pixels). pass -1 to use current column
    IMGUI_API float         GetColumnOffset(int column_index = -1);
                // get position of column line (in pixels, from the left side of
 the contents region). pass -1 to use current column, otherwise 0..GetColumnsCou
nt() inclusive. column 0 is typically 0.0f
    IMGUI_API void          SetColumnOffset(int column_index, float offset_x);
                // set position of column line (in pixels, from the left side of
 the contents region). pass -1 to use current column
    IMGUI_API int           GetColumnsCount();

    // Tab Bars, Tabs
    // - Note: Tabs are automatically created by the docking system (when in 'do
cking' branch). Use this to create tab bars/tabs yourself.
    IMGUI_API bool          BeginTabBar(const char* str_id, ImGuiTabBarFlags fla
gs = 0);        // create and append into a TabBar
    IMGUI_API void          EndTabBar();
                // only call EndTabBar() if BeginTabBar() returns true!
    IMGUI_API bool          BeginTabItem(const char* label, bool* p_open = NULL,
 ImGuiTabItemFlags flags = 0); // create a Tab. Returns true if the Tab is selec
ted.
    IMGUI_API void          EndTabItem();
                // only call EndTabItem() if BeginTabItem() returns true!
    IMGUI_API bool          TabItemButton(const char* label, ImGuiTabItemFlags f
lags = 0);      // create a Tab behaving like a button. return true when clicked
. cannot be selected in the tab bar.
    IMGUI_API void          SetTabItemClosed(const char* tab_or_docked_window_la
bel);           // notify TabBar or Docking system of a closed tab/window ahead
(useful to reduce visual flicker on reorderable tab bars). For tab-bar: call aft
er BeginTabBar() and before Tab submissions. Otherwise call with a window name.

    // Logging/Capture
    // - All text output from the interface can be captured into tty/file/clipbo
ard. By default, tree nodes are automatically opened during logging.
    IMGUI_API void          LogToTTY(int auto_open_depth = -1);
                // start logging to tty (stdout)
    IMGUI_API void          LogToFile(int auto_open_depth = -1, const char* file
name = NULL);   // start logging to file
    IMGUI_API void          LogToClipboard(int auto_open_depth = -1);
                // start logging to OS clipboard
    IMGUI_API void          LogFinish();
                // stop logging (close file, etc.)
    IMGUI_API void          LogButtons();
                // helper to display buttons for logging to tty/file/clipboard
    IMGUI_API void          LogText(const char* fmt, ...) IM_FMTARGS(1);
                // pass text data straight to log (without being displayed)
    IMGUI_API void          LogTextV(const char* fmt, va_list args) IM_FMTLIST(1
);

    // Drag and Drop
    // - On source items, call BeginDragDropSource(), if it returns true also ca
ll SetDragDropPayload() + EndDragDropSource().
    // - On target candidates, call BeginDragDropTarget(), if it returns true al
so call AcceptDragDropPayload() + EndDragDropTarget().
    // - If you stop calling BeginDragDropSource() the payload is preserved howe
ver it won't have a preview tooltip (we currently display a fallback "..." toolt
ip, see #1725)
    // - An item can be both drag source and drop target.
    IMGUI_API bool          BeginDragDropSource(ImGuiDragDropFlags flags = 0);
                                    // call after submitting an item which may b
e dragged. when this return true, you can call SetDragDropPayload() + EndDragDro
pSource()
    IMGUI_API bool          SetDragDropPayload(const char* type, const void* dat
a, size_t sz, ImGuiCond cond = 0);  // type is a user defined string of maximum
32 characters. Strings starting with '_' are reserved for dear imgui internal ty
pes. Data is copied and held by imgui. Return true when payload has been accepte
d.
    IMGUI_API void          EndDragDropSource();
                                    // only call EndDragDropSource() if BeginDra
gDropSource() returns true!
    IMGUI_API bool                  BeginDragDropTarget();
                                    // call after submitting an item that may re
ceive a payload. If this returns true, you can call AcceptDragDropPayload() + En
dDragDropTarget()
    IMGUI_API const ImGuiPayload*   AcceptDragDropPayload(const char* type, ImGu
iDragDropFlags flags = 0);          // accept contents of a given type. If ImGui
DragDropFlags_AcceptBeforeDelivery is set you can peek into the payload before t
he mouse button is released.
    IMGUI_API void                  EndDragDropTarget();
                                    // only call EndDragDropTarget() if BeginDra
gDropTarget() returns true!
    IMGUI_API const ImGuiPayload*   GetDragDropPayload();
                                    // peek directly into the current payload fr
om anywhere. returns NULL when drag and drop is finished or inactive. use ImGuiP
ayload::IsDataType() to test for the payload type.

    // Disabling [BETA API]
    // - Disable all user interactions and dim items visuals (applying style.Dis
abledAlpha over current colors)
    // - Those can be nested but it cannot be used to enable an already disabled
 section (a single BeginDisabled(true) in the stack is enough to keep everything
 disabled)
    // - Tooltips windows by exception are opted out of disabling.
    // - BeginDisabled(false)/EndDisabled() essentially does nothing but is prov
ided to facilitate use of boolean expressions (as a micro-optimization: if you h
ave tens of thousands of BeginDisabled(false)/EndDisabled() pairs, you might wan
t to reformulate your code to avoid making those calls)
    IMGUI_API void          BeginDisabled(bool disabled = true);
    IMGUI_API void          EndDisabled();

    // Clipping
    // - Mouse hovering is affected by ImGui::PushClipRect() calls, unlike direc
t calls to ImDrawList::PushClipRect() which are render only.
    IMGUI_API void          PushClipRect(const ImVec2& clip_rect_min, const ImVe
c2& clip_rect_max, bool intersect_with_current_clip_rect);
    IMGUI_API void          PopClipRect();

    // Focus, Activation
    IMGUI_API void          SetItemDefaultFocus();
                // make last item the default focused item of a newly appearing
window.
    IMGUI_API void          SetKeyboardFocusHere(int offset = 0);
                // focus keyboard on the next widget. Use positive 'offset' to a
ccess sub components of a multiple component widget. Use -1 to access previous w
idget.

    // Keyboard/Gamepad Navigation
    IMGUI_API void          SetNavCursorVisible(bool visible);
                // alter visibility of keyboard/gamepad cursor. by default: show
 when using an arrow key, hide when clicking with mouse.

    // Overlapping mode
    IMGUI_API void          SetNextItemAllowOverlap();
                // allow next item to be overlapped by a subsequent item. Useful
 with invisible buttons, selectable, treenode covering an area where subsequent
items may need to be added. Note that both Selectable() and TreeNode() have dedi
cated flags doing this.

    // Item/Widgets Utilities and Query Functions
    // - Most of the functions are referring to the previous Item that has been
submitted.
    // - See Demo Window under "Widgets->Querying Status" for an interactive vis
ualization of most of those functions.
    IMGUI_API bool          IsItemHovered(ImGuiHoveredFlags flags = 0);
                // is the last item hovered? (and usable, aka not blocked by a p
opup, etc.). See ImGuiHoveredFlags for more options.
    IMGUI_API bool          IsItemActive();
                // is the last item active? (e.g. button being held, text field
being edited. This will continuously return true while holding mouse button on a
n item. Items that don't interact will always return false)
    IMGUI_API bool          IsItemFocused();
                // is the last item focused for keyboard/gamepad navigation?
    IMGUI_API bool          IsItemClicked(ImGuiMouseButton mouse_button = 0);
                // is the last item hovered and mouse clicked on? (**)  == IsMou
seClicked(mouse_button) && IsItemHovered()Important. (**) this is NOT equivalent
 to the behavior of e.g. Button(). Read comments in function definition.
    IMGUI_API bool          IsItemVisible();
                // is the last item visible? (items may be out of sight because
of clipping/scrolling)
    IMGUI_API bool          IsItemEdited();
                // did the last item modify its underlying value this frame? or
was pressed? This is generally the same as the "bool" return value of many widge
ts.
    IMGUI_API bool          IsItemActivated();
                // was the last item just made active (item was previously inact
ive).
    IMGUI_API bool          IsItemDeactivated();
                // was the last item just made inactive (item was previously act
ive). Useful for Undo/Redo patterns with widgets that require continuous editing
.
    IMGUI_API bool          IsItemDeactivatedAfterEdit();
                // was the last item just made inactive and made a value change
when it was active? (e.g. Slider/Drag moved). Useful for Undo/Redo patterns with
 widgets that require continuous editing. Note that you may get false positives
(some widgets such as Combo()/ListBox()/Selectable() will return true even when
clicking an already selected item).
    IMGUI_API bool          IsItemToggledOpen();
                // was the last item open state toggled? set by TreeNode().
    IMGUI_API bool          IsAnyItemHovered();
                // is any item hovered?
    IMGUI_API bool          IsAnyItemActive();
                // is any item active?
    IMGUI_API bool          IsAnyItemFocused();
                // is any item focused?
    IMGUI_API ImGuiID       GetItemID();
                // get ID of last item (~~ often same ImGui::GetID(label) before
hand)
    IMGUI_API ImVec2        GetItemRectMin();
                // get upper-left bounding rectangle of the last item (screen sp
ace)
    IMGUI_API ImVec2        GetItemRectMax();
                // get lower-right bounding rectangle of the last item (screen s
pace)
    IMGUI_API ImVec2        GetItemRectSize();
                // get size of last item

    // Viewports
    // - Currently represents the Platform Window created by the application whi
ch is hosting our Dear ImGui windows.
    // - In 'docking' branch with multi-viewport enabled, we extend this concept
 to have multiple active viewports.
    // - In the future we will extend this concept further to also represent Pla
tform Monitor and support a "no main platform window" operation mode.
    IMGUI_API ImGuiViewport* GetMainViewport();
                // return primary/default viewport. This can never be NULL.

    // Background/Foreground Draw Lists
    IMGUI_API ImDrawList*   GetBackgroundDrawList();
                // this draw list will be the first rendered one. Useful to quic
kly draw shapes/text behind dear imgui contents.
    IMGUI_API ImDrawList*   GetForegroundDrawList();
                // this draw list will be the last rendered one. Useful to quick
ly draw shapes/text over dear imgui contents.

    // Miscellaneous Utilities
    IMGUI_API bool          IsRectVisible(const ImVec2& size);
                // test if rectangle (of given size, starting from cursor positi
on) is visible / not clipped.
    IMGUI_API bool          IsRectVisible(const ImVec2& rect_min, const ImVec2&
rect_max);      // test if rectangle (in screen space) is visible / not clipped.
 to perform coarse clipping on user's side.
    IMGUI_API double        GetTime();
                // get global imgui time. incremented by io.DeltaTime every fram
e.
    IMGUI_API int           GetFrameCount();
                // get global imgui frame count. incremented by 1 every frame.
    IMGUI_API ImDrawListSharedData* GetDrawListSharedData();
                // you may use this when creating your own ImDrawList instances.
    IMGUI_API const char*   GetStyleColorName(ImGuiCol idx);
                // get a string corresponding to the enum value (for display, sa
ving, etc.).
    IMGUI_API void          SetStateStorage(ImGuiStorage* storage);
                // replace current window storage with our own (if you want to m
anipulate it yourself, typically clear subsection of it)
    IMGUI_API ImGuiStorage* GetStateStorage();

    // Text Utilities
    IMGUI_API ImVec2        CalcTextSize(const char* text, const char* text_end
= NULL, bool hide_text_after_double_hash = false, float wrap_width = -1.0f);

    // Color Utilities
    IMGUI_API ImVec4        ColorConvertU32ToFloat4(ImU32 in);
    IMGUI_API ImU32         ColorConvertFloat4ToU32(const ImVec4& in);
    IMGUI_API void          ColorConvertRGBtoHSV(float r, float g, float b, floa
t& out_h, float& out_s, float& out_v);
    IMGUI_API void          ColorConvertHSVtoRGB(float h, float s, float v, floa
t& out_r, float& out_g, float& out_b);

    // Inputs Utilities: Keyboard/Mouse/Gamepad
    // - the ImGuiKey enum contains all possible keyboard, mouse and gamepad inp
uts (e.g. ImGuiKey_A, ImGuiKey_MouseLeft, ImGuiKey_GamepadDpadUp...).
    // - (legacy: before v1.87, we used ImGuiKey to carry native/user indices as
 defined by each backends. This was obsoleted in 1.87 (2022-02) and completely r
emoved in 1.91.5 (2024-11). See https://github.com/ocornut/imgui/issues/4921)
    // - (legacy: any use of ImGuiKey will assert when key < 512 to detect passi
ng legacy native/user indices)
    IMGUI_API bool          IsKeyDown(ImGuiKey key);
                // is key being held.
    IMGUI_API bool          IsKeyPressed(ImGuiKey key, bool repeat = true);
                // was key pressed (went from !Down to Down)? if repeat=true, us
es io.KeyRepeatDelay / KeyRepeatRate
    IMGUI_API bool          IsKeyReleased(ImGuiKey key);
                // was key released (went from Down to !Down)?
    IMGUI_API bool          IsKeyChordPressed(ImGuiKeyChord key_chord);
                // was key chord (mods + key) pressed, e.g. you can pass 'ImGuiM
od_Ctrl | ImGuiKey_S' as a key-chord. This doesn't do any routing or focus check
, please consider using Shortcut() function instead.
    IMGUI_API int           GetKeyPressedAmount(ImGuiKey key, float repeat_delay
, float rate);  // uses provided repeat rate/delay. return a count, most often 0
 or 1 but might be >1 if RepeatRate is small enough that DeltaTime > RepeatRate
    IMGUI_API const char*   GetKeyName(ImGuiKey key);
                // [DEBUG] returns English name of the key. Those names are prov
ided for debugging purpose and are not meant to be saved persistently nor compar
ed.
    IMGUI_API void          SetNextFrameWantCaptureKeyboard(bool want_capture_ke
yboard);        // Override io.WantCaptureKeyboard flag next frame (said flag is
 left for your application to handle, typically when true it instructs your app
to ignore inputs). e.g. force capture keyboard when your widget is being hovered
. This is equivalent to setting "io.WantCaptureKeyboard = want_capture_keyboard"
; after the next NewFrame() call.

    // Inputs Utilities: Shortcut Testing & Routing [BETA]
    // - ImGuiKeyChord = a ImGuiKey + optional ImGuiMod_Alt/ImGuiMod_Ctrl/ImGuiM
od_Shift/ImGuiMod_Super.
    //       ImGuiKey_C                          // Accepted by functions taking
 ImGuiKey or ImGuiKeyChord arguments
    //       ImGuiMod_Ctrl | ImGuiKey_C          // Accepted by functions taking
 ImGuiKeyChord arguments
    //   only ImGuiMod_XXX values are legal to combine with an ImGuiKey. You CAN
NOT combine two ImGuiKey values.
    // - The general idea is that several callers may register interest in a sho
rtcut, and only one owner gets it.
    //      Parent   -> call Shortcut(Ctrl+S)    // When Parent is focused, Pare
nt gets the shortcut.
    //        Child1 -> call Shortcut(Ctrl+S)    // When Child1 is focused, Chil
d1 gets the shortcut (Child1 overrides Parent shortcuts)
    //        Child2 -> no call                  // When Child2 is focused, Pare
nt gets the shortcut.
    //   The whole system is order independent, so if Child1 makes its calls bef
ore Parent, results will be identical.
    //   This is an important property as it facilitate working with foreign cod
e or larger codebase.
    // - To understand the difference:
    //   - IsKeyChordPressed() compares mods and call IsKeyPressed() -> function
 has no side-effect.
    //   - Shortcut() submits a route, routes are resolved, if it currently can
be routed it calls IsKeyChordPressed() -> function has (desirable) side-effects
as it can prevents another call from getting the route.
    // - Visualize registered routes in 'Metrics/Debugger->Inputs'.
    IMGUI_API bool          Shortcut(ImGuiKeyChord key_chord, ImGuiInputFlags fl
ags = 0);
    IMGUI_API void          SetNextItemShortcut(ImGuiKeyChord key_chord, ImGuiIn
putFlags flags = 0);

    // Inputs Utilities: Key/Input Ownership [BETA]
    // - One common use case would be to allow your items to disable standard in
puts behaviors such
    //   as Tab or Alt key handling, Mouse Wheel scrolling, etc.
    //   e.g. Button(...); SetItemKeyOwner(ImGuiKey_MouseWheelY); to make hoveri
ng/activating a button disable wheel for scrolling.
    // - Reminder ImGuiKey enum include access to mouse buttons and gamepad, so
key ownership can apply to them.
    // - Many related features are still in imgui_internal.h. For instance, most
 IsKeyXXX()/IsMouseXXX() functions have an owner-id-aware version.
    IMGUI_API void          SetItemKeyOwner(ImGuiKey key);
                // Set key owner to last item ID if it is hovered or active. Equ
ivalent to 'if (IsItemHovered() || IsItemActive()) { SetKeyOwner(key, GetItemID(
));'.

    // Inputs Utilities: Mouse
    // - To refer to a mouse button, you may use named enums in your code e.g. I
mGuiMouseButton_Left, ImGuiMouseButton_Right.
    // - You can also use regular integer: it is forever guaranteed that 0=Left,
 1=Right, 2=Middle.
    // - Dragging operations are only reported after mouse has moved a certain d
istance away from the initial clicking position (see 'lock_threshold' and 'io.Mo
useDraggingThreshold')
    IMGUI_API bool          IsMouseDown(ImGuiMouseButton button);
                // is mouse button held?
    IMGUI_API bool          IsMouseClicked(ImGuiMouseButton button, bool repeat
= false);       // did mouse button clicked? (went from !Down to Down). Same as
GetMouseClickedCount() == 1.
    IMGUI_API bool          IsMouseReleased(ImGuiMouseButton button);
                // did mouse button released? (went from Down to !Down)
    IMGUI_API bool          IsMouseDoubleClicked(ImGuiMouseButton button);
                // did mouse button double-clicked? Same as GetMouseClickedCount
() == 2. (note that a double-click will also report IsMouseClicked() == true)
    IMGUI_API bool          IsMouseReleasedWithDelay(ImGuiMouseButton button, fl
oat delay);     // delayed mouse release (use very sparingly!). Generally used w
ith 'delay >= io.MouseDoubleClickTime' + combined with a 'io.MouseClickedLastCou
nt==1' test. This is a very rarely used UI idiom, but some apps use this: e.g. M
S Explorer single click on an icon to rename.
    IMGUI_API int           GetMouseClickedCount(ImGuiMouseButton button);
                // return the number of successive mouse-clicks at the time wher
e a click happen (otherwise 0).
    IMGUI_API bool          IsMouseHoveringRect(const ImVec2& r_min, const ImVec
2& r_max, bool clip = true);// is mouse hovering given bounding rect (in screen
space). clipped by current clipping settings, but disregarding of other consider
ation of focus/window ordering/popup-block.
    IMGUI_API bool          IsMousePosValid(const ImVec2* mouse_pos = NULL);
                // by convention we use (-FLT_MAX,-FLT_MAX) to denote that there
 is no mouse available
    IMGUI_API bool          IsAnyMouseDown();
                // [WILL OBSOLETE] is any mouse button held? This was designed f
or backends, but prefer having backend maintain a mask of held mouse buttons, be
cause upcoming input queue system will make this invalid.
    IMGUI_API ImVec2        GetMousePos();
                // shortcut to ImGui::GetIO().MousePos provided by user, to be c
onsistent with other calls
    IMGUI_API ImVec2        GetMousePosOnOpeningCurrentPopup();
                // retrieve mouse position at the time of opening popup we have
BeginPopup() into (helper to avoid user backing that value themselves)
    IMGUI_API bool          IsMouseDragging(ImGuiMouseButton button, float lock_
threshold = -1.0f);         // is mouse dragging? (uses io.MouseDraggingThreshol
d if lock_threshold < 0.0f)
    IMGUI_API ImVec2        GetMouseDragDelta(ImGuiMouseButton button = 0, float
 lock_threshold = -1.0f);   // return the delta from the initial clicking positi
on while the mouse button is pressed or was just released. This is locked and re
turn 0.0f until the mouse moves past a distance threshold at least once (uses io
.MouseDraggingThreshold if lock_threshold < 0.0f)
    IMGUI_API void          ResetMouseDragDelta(ImGuiMouseButton button = 0);
                //
    IMGUI_API ImGuiMouseCursor GetMouseCursor();
                // get desired mouse cursor shape. Important: reset in ImGui::Ne
wFrame(), this is updated during the frame. valid before Render(). If you use so
ftware rendering by setting io.MouseDrawCursor ImGui will render those for you
    IMGUI_API void          SetMouseCursor(ImGuiMouseCursor cursor_type);
                // set desired mouse cursor shape
    IMGUI_API void          SetNextFrameWantCaptureMouse(bool want_capture_mouse
);              // Override io.WantCaptureMouse flag next frame (said flag is le
ft for your application to handle, typical when true it instructs your app to ig
nore inputs). This is equivalent to setting "io.WantCaptureMouse = want_capture_
mouse;" after the next NewFrame() call.

    // Clipboard Utilities
    // - Also see the LogToClipboard() function to capture GUI into clipboard, o
r easily output text data to the clipboard.
    IMGUI_API const char*   GetClipboardText();
    IMGUI_API void          SetClipboardText(const char* text);

    // Settings/.Ini Utilities
    // - The disk functions are automatically called if io.IniFilename != NULL (
default is "imgui.ini").
    // - Set io.IniFilename to NULL to load/save manually. Read io.WantSaveIniSe
ttings description about handling .ini saving manually.
    // - Important: default value "imgui.ini" is relative to current working dir
! Most apps will want to lock this to an absolute path (e.g. same path as execut
ables).
    IMGUI_API void          LoadIniSettingsFromDisk(const char* ini_filename);
                // call after CreateContext() and before the first call to NewFr
ame(). NewFrame() automatically calls LoadIniSettingsFromDisk(io.IniFilename).
    IMGUI_API void          LoadIniSettingsFromMemory(const char* ini_data, size
_t ini_size=0); // call after CreateContext() and before the first call to NewFr
ame() to provide .ini data from your own data source.
    IMGUI_API void          SaveIniSettingsToDisk(const char* ini_filename);
                // this is automatically called (if io.IniFilename is not empty)
 a few seconds after any modification that should be reflected in the .ini file
(and also by DestroyContext).
    IMGUI_API const char*   SaveIniSettingsToMemory(size_t* out_ini_size = NULL)
;               // return a zero-terminated string with the .ini data which you
can save by your own mean. call when io.WantSaveIniSettings is set, then save da
ta by your own mean and clear io.WantSaveIniSettings.

    // Debug Utilities
    // - Your main debugging friend is the ShowMetricsWindow() function, which i
s also accessible from Demo->Tools->Metrics Debugger
    IMGUI_API void          DebugTextEncoding(const char* text);
    IMGUI_API void          DebugFlashStyleColor(ImGuiCol idx);
    IMGUI_API void          DebugStartItemPicker();
    IMGUI_API bool          DebugCheckVersionAndDataLayout(const char* version_s
tr, size_t sz_io, size_t sz_style, size_t sz_vec2, size_t sz_vec4, size_t sz_dra
wvert, size_t sz_drawidx); // This is called by IMGUI_CHECKVERSION() macro.
#ifndef IMGUI_DISABLE_DEBUG_TOOLS
    IMGUI_API void          DebugLog(const char* fmt, ...)           IM_FMTARGS(
1); // Call via IMGUI_DEBUG_LOG() for maximum stripping in caller code!
    IMGUI_API void          DebugLogV(const char* fmt, va_list args) IM_FMTLIST(
1);
#endif

    // Memory Allocators
    // - Those functions are not reliant on the current context.
    // - DLL users: heaps and globals are not shared across DLL boundaries! You
will need to call SetCurrentContext() + SetAllocatorFunctions()
    //   for each static/DLL boundary you are calling from. Read "Context and Me
mory Allocators" section of imgui.cpp for more details.
    IMGUI_API void          SetAllocatorFunctions(ImGuiMemAllocFunc alloc_func,
ImGuiMemFreeFunc free_func, void* user_data = NULL);
    IMGUI_API void          GetAllocatorFunctions(ImGuiMemAllocFunc* p_alloc_fun
c, ImGuiMemFreeFunc* p_free_func, void** p_user_data);
    IMGUI_API void*         MemAlloc(size_t size);
    IMGUI_API void          MemFree(void* ptr);

} // namespace ImGui

//-----------------------------------------------------------------------------
// [SECTION] Flags & Enumerations
//-----------------------------------------------------------------------------

// Flags for ImGui::Begin()
// (Those are per-window flags. There are shared flags in ImGuiIO: io.ConfigWind
owsResizeFromEdges and io.ConfigWindowsMoveFromTitleBarOnly)
enum ImGuiWindowFlags_
{
    ImGuiWindowFlags_None                   = 0,
    ImGuiWindowFlags_NoTitleBar             = 1 << 0,   // Disable title-bar
    ImGuiWindowFlags_NoResize               = 1 << 1,   // Disable user resizing
 with the lower-right grip
    ImGuiWindowFlags_NoMove                 = 1 << 2,   // Disable user moving t
he window
    ImGuiWindowFlags_NoScrollbar            = 1 << 3,   // Disable scrollbars (w
indow can still scroll with mouse or programmatically)
    ImGuiWindowFlags_NoScrollWithMouse      = 1 << 4,   // Disable user vertical
ly scrolling with mouse wheel. On child window, mouse wheel will be forwarded to
 the parent unless NoScrollbar is also set.
    ImGuiWindowFlags_NoCollapse             = 1 << 5,   // Disable user collapsi
ng window by double-clicking on it. Also referred to as Window Menu Button (e.g.
 within a docking node).
    ImGuiWindowFlags_AlwaysAutoResize       = 1 << 6,   // Resize every window t
o its content every frame
    ImGuiWindowFlags_NoBackground           = 1 << 7,   // Disable drawing backg
round color (WindowBg, etc.) and outside border. Similar as using SetNextWindowB
gAlpha(0.0f).
    ImGuiWindowFlags_NoSavedSettings        = 1 << 8,   // Never load/save setti
ngs in .ini file
    ImGuiWindowFlags_NoMouseInputs          = 1 << 9,   // Disable catching mous
e, hovering test with pass through.
    ImGuiWindowFlags_MenuBar                = 1 << 10,  // Has a menu-bar
    ImGuiWindowFlags_HorizontalScrollbar    = 1 << 11,  // Allow horizontal scro
llbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(wi
dth,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo i
n the "Horizontal Scrolling" section.
    ImGuiWindowFlags_NoFocusOnAppearing     = 1 << 12,  // Disable taking focus
when transitioning from hidden to visible state
    ImGuiWindowFlags_NoBringToFrontOnFocus  = 1 << 13,  // Disable bringing wind
ow to front when taking focus (e.g. clicking on it or programmatically giving it
 focus)
    ImGuiWindowFlags_AlwaysVerticalScrollbar= 1 << 14,  // Always show vertical
scrollbar (even if ContentSize.y < Size.y)
    ImGuiWindowFlags_AlwaysHorizontalScrollbar=1<< 15,  // Always show horizonta
l scrollbar (even if ContentSize.x < Size.x)
    ImGuiWindowFlags_NoNavInputs            = 1 << 16,  // No keyboard/gamepad n
avigation within the window
    ImGuiWindowFlags_NoNavFocus             = 1 << 17,  // No focusing toward th
is window with keyboard/gamepad navigation (e.g. skipped by CTRL+TAB)
    ImGuiWindowFlags_UnsavedDocument        = 1 << 18,  // Display a dot next to
 the title. When used in a tab/docking context, tab is selected when clicking th
e X + closure is not assumed (will wait for user to stop submitting the tab). Ot
herwise closure is assumed when pressing the X, so if you keep submitting the ta
b may reappear at end of tab bar.
    ImGuiWindowFlags_NoNav                  = ImGuiWindowFlags_NoNavInputs | ImG
uiWindowFlags_NoNavFocus,
    ImGuiWindowFlags_NoDecoration           = ImGuiWindowFlags_NoTitleBar | ImGu
iWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollap
se,
    ImGuiWindowFlags_NoInputs               = ImGuiWindowFlags_NoMouseInputs | I
mGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus,

    // [Internal]
    ImGuiWindowFlags_ChildWindow            = 1 << 24,  // Don't use! For intern
al use by BeginChild()
    ImGuiWindowFlags_Tooltip                = 1 << 25,  // Don't use! For intern
al use by BeginTooltip()
    ImGuiWindowFlags_Popup                  = 1 << 26,  // Don't use! For intern
al use by BeginPopup()
    ImGuiWindowFlags_Modal                  = 1 << 27,  // Don't use! For intern
al use by BeginPopupModal()
    ImGuiWindowFlags_ChildMenu              = 1 << 28,  // Don't use! For intern
al use by BeginMenu()
};

// Flags for ImGui::InputText()
enum ImGuiInputTextFlags_
{
    ImGuiInputTextFlags_None                = 0,
    ImGuiInputTextFlags_CharsDecimal        = 1 << 0,   // Allow 0123456789.+-*/
    ImGuiInputTextFlags_CharsHexadecimal    = 1 << 1,   // Allow 0123456789ABCDE
Fabcdef
    ImGuiInputTextFlags_CharsUppercase      = 1 << 2,   // Turn a..z into A..Z
    ImGuiInputTextFlags_CharsNoBlank        = 1 << 3,   // Filter out spaces, ta
bs
    ImGuiInputTextFlags_AutoSelectAll       = 1 << 4,   // Select entire text wh
en first taking mouse focus
    ImGuiInputTextFlags_EnterReturnsTrue    = 1 << 5,   // Return 'true' when En
ter is pressed (as opposed to every time the value was modified). Consider using
 IsItemDeactivatedAfterEdit() instead.
    ImGuiInputTextFlags_CallbackCompletion  = 1 << 6,   // Callback on pressing
 TAB (for completion handling)
    ImGuiInputTextFlags_CallbackHistory     = 1 << 7,   // Callback on pressing
 Up/Down arrows (for history handling)
    ImGuiInputTextFlags_CallbackAlways      = 1 << 8,   // Callback on each iter
ation
    ImGuiInputTextFlags_CallbackCharFilter  = 1 << 9,   // Callback on character
 inputs to replace or discard them. Modify 'EventChar' to replace or discard, o
r return 1 in callback to discard.
    ImGuiInputTextFlags_AllowTabInput       = 1 << 10,  // Pressing TAB input a
'\t' character into the text field
    ImGuiInputTextFlags_CtrlEnterForNewLine = 1 << 11,  // In multi-line mode, v
alidate with Enter, add new line with Ctrl+Enter (default is opposite: validate
with Ctrl+Enter, add line with Enter).
    ImGuiInputTextFlags_NoHorizontalScroll  = 1 << 12,  // Disable following the
 cursor horizontally
    ImGuiInputTextFlags_AlwaysInsertMode    = 1 << 13,  // Insert mode
    ImGuiInputTextFlags_ReadOnly            = 1 << 14,  // Read-only mode
    ImGuiInputTextFlags_Password            = 1 << 15,  // Password mode, displa
y all characters as '*'
    ImGuiInputTextFlags_NoUndoRedo          = 1 << 16,  // Disable undo/redo. No
te that input text owns the text data while active, if you want to provide your
own undo/redo stack you need e.g. to call ClearActiveID().
    ImGuiInputTextFlags_CharsScientific     = 1 << 17,  // Allow 0123456789.+-*/
eE (Scientific notation input)
    ImGuiInputTextFlags_CallbackResize      = 1 << 18,  // Callback on buffer re
size (imgui_stdlib.h implementation needs this)
    ImGuiInputTextFlags_CallbackEdit        = 1 << 19,  // Callback on any edit
(note that InputText() already returns true on edit, the callback is useful main
ly to manipulate the underlying buffer while focus is active)
};

// Flags for ImGui::TreeNodeEx(), ImGui::CollapsingHeader()
enum ImGuiTreeNodeFlags_
{
    ImGuiTreeNodeFlags_None                 = 0,
    ImGuiTreeNodeFlags_Selected             = 1 << 0,   // Draw as selected
    ImGuiTreeNodeFlags_Framed               = 1 << 1,   // Draw frame with backg
round (e.g. for CollapsingHeader)
    ImGuiTreeNodeFlags_AllowItemOverlap     = 1 << 2,   // Hit testing to allow
subsequent widgets to overlap this one
    ImGuiTreeNodeFlags_NoTreePushOnOpen     = 1 << 3,   // Don't do a TreePush()
 when open (e.g. for CollapsingHeader) = no extra indent nor pushing on ID stack
    ImGuiTreeNodeFlags_NoAutoOpenOnLog      = 1 << 4,   // Don't automatically a
nd temporarily open node when Logging is active (by default logging will automat
ically open tree nodes)
    ImGuiTreeNodeFlags_DefaultOpen          = 1 << 5,   // Default node to be op
en
    ImGuiTreeNodeFlags_OpenOnDoubleClick    = 1 << 6,   // Need double-click to
open node
    ImGuiTreeNodeFlags_OpenOnArrow          = 1 << 7,   // Only open when clicki
ng on the arrow part. If ImGuiTreeNodeFlags_OpenOnDoubleClick is also set, single
-click arrow or double-click all box to open.
    ImGuiTreeNodeFlags_Leaf                 = 1 << 8,   // No collapsing, no arr
ow (use as a convenience for leaf nodes).
    ImGuiTreeNodeFlags_Bullet               = 1 << 9,   // Display a bullet inst
ead of arrow
    ImGuiTreeNodeFlags_FramePadding         = 1 << 10,  // Use FramePadding (eve
n for an unframed text node) to vertically align text baseline to regular widget
 height. Equivalent to calling AlignTextToFramePadding() before the node.
    ImGuiTreeNodeFlags_SpanAvailWidth       = 1 << 11,  // Extend hit box to the
 right-most edge, even if not framed. This is not the default in order to allow
adding other items on the same line. In the future we may remove the need for th
is flag as default behavior.
    ImGuiTreeNodeFlags_SpanFullWidth        = 1 << 12,  // Extend hit box to the
 left-most and right-most edges (cover the indent area).
    ImGuiTreeNodeFlags_NavLeftJumpsBackHere = 1 << 13,  // (WIP) Nav: left arrow
 moves to the parent node instead of staying in the same level. This is not ful
ly supported in all navigation options.
    ImGuiTreeNodeFlags_CollapsingHeader     = ImGuiTreeNodeFlags_Framed | ImGuiT
reeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_NoAutoOpenOnLog
};

// Flags for ImGui::Selectable()
enum ImGuiSelectableFlags_
{
    ImGuiSelectableFlags_None               = 0,
    ImGuiSelectableFlags_DontClosePopups    = 1 << 0,   // Clicking this doesn't
 close parent popup window
    ImGuiSelectableFlags_SpanAllColumns     = 1 << 1,   // Selectable frame can
span all columns (text will still fit in current column)
    ImGuiSelectableFlags_AllowDoubleClick   = 1 << 2,   // Generate press events
 on double clicks too
    ImGuiSelectableFlags_Disabled           = 1 << 3,   // Cannot be selected, d
isplay grayed out text
    ImGuiSelectableFlags_AllowItemOverlap   = 1 << 4    // (WIP) Hit testing to
allow subsequent widgets to overlap this one
};

// Flags for ImGui::BeginCombo()
enum ImGuiComboFlags_
{
    ImGuiComboFlags_None                    = 0,
    ImGuiComboFlags_PopupAlignLeft          = 1 << 0,   // Align the popup towar
d the left by default
    ImGuiComboFlags_HeightSmall             = 1 << 1,   // Max ~4 items visible.
 Tip: If you want your combo popup to be a specific size you can use SetNextWind
owSizeConstraints() prior to calling BeginCombo()
    ImGuiComboFlags_HeightRegular           = 1 << 2,   // Max ~8 items visible
(default)
    ImGuiComboFlags_HeightLarge             = 1 << 3,   // Max ~20 items visible
    ImGuiComboFlags_HeightLargest           = 1 << 4,   // As many fitting items
 as possible
    ImGuiComboFlags_NoArrowButton           = 1 << 5,   // Display on the previe
w box without the square arrow button
    ImGuiComboFlags_NoPreview               = 1 << 6,   // Display only a square
 arrow button
    ImGuiComboFlags_HeightMask_             = ImGuiComboFlags_HeightSmall | ImGu
iComboFlags_HeightRegular | ImGuiComboFlags_HeightLarge | ImGuiComboFlags_Height
Largest
};

// Flags for ImGui::BeginTabBar()
enum ImGuiTabBarFlags_
{
    ImGuiTabBarFlags_None                           = 0,
    ImGuiTabBarFlags_Reorderable                    = 1 << 0,   // Allow manuall
y dragging tabs to re-order them
    ImGuiTabBarFlags_AutoSelectNewTabs              = 1 << 1,   // Automatically
 select new tabs when they appear
    ImGuiTabBarFlags_TabListPopupButton             = 1 << 2,   // Disable butto
ns to open the tab list popup
    ImGuiTabBarFlags_NoCloseWithMiddleMouseButton   = 1 << 3,   // Disable behav
ior of closing tabs (that are submitted with p_open != NULL) with middle mouse b
utton. You can still handle it manually.
    ImGuiTabBarFlags_NoTabListScrollingButtons      = 1 << 4,   // Disable scrol
ling buttons (apply when fitting policy is ImGuiTabBarFlags_FittingPolicyScroll)
    ImGuiTabBarFlags_NoTooltip                      = 1 << 5,   // Disable toolt
ips when hovering a tab
    ImGuiTabBarFlags_FittingPolicyResizeDown        = 1 << 6,   // Resize tabs w
hen they don't fit
    ImGuiTabBarFlags_FittingPolicyScroll            = 1 << 7,   // Add scroll bu
ttons when tabs don't fit
    ImGuiTabBarFlags_FittingPolicyMask_             = ImGuiTabBarFlags_FittingPo
licyResizeDown | ImGuiTabBarFlags_FittingPolicyScroll,
    ImGuiTabBarFlags_FittingPolicyDefault_          = ImGuiTabBarFlags_FittingPo
licyResizeDown
};

// Flags for ImGui::BeginTabItem()
enum ImGuiTabItemFlags_
{
    ImGuiTabItemFlags_None                          = 0,
    ImGuiTabItemFlags_UnsavedDocument               = 1 << 0,   // Display a dot
 next to the title.
    ImGuiTabItemFlags_SetSelected                   = 1 << 1,   // Trigger flag
to programmatically make the tab selected when calling BeginTabItem()
    ImGuiTabItemFlags_NoCloseWithMiddleMouseButton  = 1 << 2,   // Disable behav
ior of closing tabs (that are submitted with p_open != NULL) with middle mouse b
utton. You can still handle it manually.
    ImGuiTabItemFlags_NoPushId                      = 1 << 3,   // Don't call Pu
shID()/PopID() on BeginTabItem()/EndTabItem()
    ImGuiTabItemFlags_NoTooltip                     = 1 << 4,   // Disable toolt
ip for the given tab
    ImGuiTabItemFlags_NoReorder                     = 1 << 5,   // Disable reord
ering this tab or having another tab cross over this tab
    ImGuiTabItemFlags_Leading                       = 1 << 6,   // Enforce the t
ab position to the left of the tab bar (after the tab list popup button)
    ImGuiTabItemFlags_Trailing                      = 1 << 7    // Enforce the t
ab position to the right of the tab bar (before the scrolling buttons)
};

// Flags for ImGui::Focused()
enum ImGuiFocusedFlags_
{
    ImGuiFocusedFlags_None                          = 0,
    ImGuiFocusedFlags_ChildWindows                  = 1 << 0,   // IsWindowFocus
ed(): Return true if any children of the window is focused
    ImGuiFocusedFlags_RootWindow                    = 1 << 1,   // IsWindowFocus
ed(): Test from root window (top most parent of the current hierarchy)
    ImGuiFocusedFlags_AnyWindow                     = 1 << 2,   // IsWindowFocus
ed(): Return true if any window is focused. Important: If you are trying to tell
 how to dispatch your low-level inputs, do NOT use this. Use 'io.WantCaptureMous
e' instead.
    ImGuiFocusedFlags_NoPopupHierarchy              = 1 << 3,   // IsWindowFocus
ed(): Do not consider popup hierarchy (do not treat popup emitter as parent of p
opup)
    ImGuiFocusedFlags_RootAndChildWindows           = ImGuiFocusedFlags_RootWind
ow | ImGuiFocusedFlags_ChildWindows
};

// Flags for ImGui::Hovered()
enum ImGuiHoveredFlags_
{
    ImGuiHoveredFlags_None                          = 0,        // Return true i
f directly over the item/window, not obstructed by another window, not obstructed
 by an active popup or modal blocking inputs under them.
    ImGuiHoveredFlags_ChildWindows                  = 1 << 0,   // IsWindowHover
ed() only: Return true if any children of the window is hovered
    ImGuiHoveredFlags_RootWindow                    = 1 << 1,   // IsWindowHover
ed() only: Test from root window (top most parent of the current hierarchy)
    ImGuiHoveredFlags_AnyWindow                     = 1 << 2,   // IsWindowHover
ed() only: Return true if any window is hovered
    ImGuiHoveredFlags_NoPopupHierarchy              = 1 << 3,   // IsWindowHover
ed() only: Do not consider popup hierarchy (do not treat popup emitter as parent
 of popup)
    ImGuiHoveredFlags_AllowWhenBlockedByPopup       = 1 << 5,   // Return true e
ven if a popup window is blocking access to this item/window
    //ImGuiHoveredFlags_AllowWhenBlockedByModal     = 1 << 6,   // Return true e
ven if a modal popup window is blocking access to this item/window. Unused in 1.
80.
    ImGuiHoveredFlags_AllowWhenBlockedByActiveItem  = 1 << 7,   // Return true e
ven if an active item is blocking access to this item/window. Useful for Drag an
d Drop patterns.
    ImGuiHoveredFlags_AllowWhenOverlapped           = 1 << 8,   // Return true e
ven if the position is obstructed or overlapped by another window
    ImGuiHoveredFlags_AllowWhenDisabled             = 1 << 9,   // Return true e
ven if the item is disabled
    ImGuiHoveredFlags_RectOnly                      = ImGuiHoveredFlags_AllowWhe
nBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredF
lags_AllowWhenOverlapped,
    ImGuiHoveredFlags_RootAndChildWindows           = ImGuiHoveredFlags_RootWind
ow | ImGuiHoveredFlags_ChildWindows
};

// Flags for ImGui::BeginDragDropSource(), ImGui::AcceptDragDropPayload()
enum ImGuiDragDropFlags_
{
    ImGuiDragDropFlags_None                         = 0,
    // BeginDragDropSource() flags
    ImGuiDragDropFlags_SourceNoPreviewTooltip       = 1 << 0,   // By default, a
successful call to BeginDragDropSource opens a tooltip so you can display a prev
iew or description of the source contents. This flag disable this behavior.
    ImGuiDragDropFlags_SourceNoDisableHover         = 1 << 1,   // By default, w
hen dragging we clear data so that IsItemHovered() will return false, to avoid s
ubsequent user code submitting tooltips. This flag disables this behavior so you
 can still call IsItemHovered() on the source item.
    ImGuiDragDropFlags_SourceNoHoldToOpenOthers     = 1 << 2,   // Disable the b
ehavior that allows to open tree nodes and collapsing header by holding over the
m while dragging a source item.
    ImGuiDragDropFlags_SourceAllowNullID            = 1 << 3,   // Allow items s
uch as Text(), Image() that have no unique identifier to be used as drag source,
 by manufacturing a temporary identifier based on their window-relative position
. This is perhaps supported on most items but not all.
    ImGuiDragDropFlags_SourceExtern                 = 1 << 4,   // External sour
ce (from outside of dear imgui), won't attempt to read current item/window info.
 Will always return true. Only one Extern source can be active simultaneously.
    ImGuiDragDropFlags_SourceAutoExpirePayload      = 1 << 5,   // Automatically
 expire the payload if the source cease to be submitted (otherwise payloads are
persisting while being dragged)
    // AcceptDragDropPayload() flags
    ImGuiDragDropFlags_AcceptBeforeDelivery         = 1 << 10,  // AcceptDragDro
pPayload() will returns true even before the mouse button is released. You can t
hen call IsDelivery() to test if the payload needs to be delivered.
    ImGuiDragDropFlags_AcceptNoDrawDefaultRect      = 1 << 11,  // Do not draw t
he default highlight rectangle when hovering over target.
    ImGuiDragDropFlags_AcceptNoPreviewTooltip       = 1 << 12,  // Request hidin
g the BeginDragDropSource tooltip from the BeginDragDropTarget site.
    ImGuiDragDropFlags_AcceptPeekOnly               = ImGuiDragDropFlags_AcceptB
eforeDelivery | ImGuiDragDropFlags_AcceptNoDrawDefaultRect // For peeking ahead
and inspecting the payload before delivery.
};

// Standard Drag and Drop payload types. You can define you own payload types us
ing short strings. Types starting with '_' are defined by Dear ImGui.
#define IMGUI_PAYLOAD_TYPE_COLOR_3F     "_COL3F"    // float[3]: Standard type f
or colors, without alpha. User code may use this type.
#define IMGUI_PAYLOAD_TYPE_COLOR_4F     "_COL4F"    // float[4]: Standard type f
or colors. User code may use this type.

// A primary data type
enum ImGuiDataType_
{
    ImGuiDataType_S8,       // signed char / char (with sensible compilers)
    ImGuiDataType_U8,       // unsigned char
    ImGuiDataType_S16,      // short
    ImGuiDataType_U16,      // unsigned short
    ImGuiDataType_S32,      // int
    ImGuiDataType_U32,      // unsigned int
    ImGuiDataType_S64,      // long long / __int64
    ImGuiDataType_U64,      // unsigned long long / unsigned __int64
    ImGuiDataType_Float,    // float
    ImGuiDataType_Double,   // double
    ImGuiDataType_COUNT
};

// A cardinal direction
enum ImGuiDir_
{
    ImGuiDir_None    = -1,
    ImGuiDir_Left    = 0,
    ImGuiDir_Right   = 1,
    ImGuiDir_Up      = 2,
    ImGuiDir_Down    = 3,
    ImGuiDir_COUNT
};

// A sorting direction
enum ImGuiSortDirection_
{
    ImGuiSortDirection_None         = 0,
    ImGuiSortDirection_Ascending    = 1,    // Ascending = 0->9, A->Z etc.
    ImGuiSortDirection_Descending   = 2     // Descending = 9->0, Z->A etc.
};
...
and so on... I will truncate the file here as it's very long. The rest of the file will be included.
