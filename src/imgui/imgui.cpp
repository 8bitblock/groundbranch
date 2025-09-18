// dear imgui, v1.92.3 WIP
// (main code and documentation)

// Help:
// - See links below.
// - Call and read ImGui::ShowDemoWindow() in imgui_demo.cpp. All applications i
n examples/ are doing that.
// - Read top of imgui.cpp for more details, links and comments.

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

// For first-time users having issues compiling/linking/running:
// please post in https://github.com/ocornut/imgui/discussions if you cannot fin
d a solution in resources above.
// Everything else should be asked in 'Issues'! We are building a database of cr
oss-linked knowledge there.
// Since 1.92, we encourage font loading questions to also be posted in 'Issues'
.

// Copyright (c) 2014-2025 Omar Cornut
// Developed by Omar Cornut and every direct or indirect contributors to the Git
Hub.
// See LICENSE.txt for copyright and licensing details (standard MIT License).
// This library is free but needs your support to sustain development and mainte
nance.
// Businesses: you can support continued development via B2B invoiced technical
support, maintenance and sponsoring contracts.
// PLEASE reach out at omar AT dearimgui DOT com. See https://github.com/ocornut
/imgui/wiki/Funding
// Businesses: you can also purchase licenses for the Dear ImGui Automation/Test
 Engine.

// It is recommended that you don't modify imgui.cpp! It will become difficult f
or you to update the library.
// Note that 'ImGui::' being a namespace, you can add functions into the namespa
ce from your own source files, without
// modifying imgui.h or imgui.cpp. You may include imgui_internal.h to access in
ternal data structures, but it doesn't
// come with any guarantee of forward compatibility. Discussing your changes on
the GitHub Issue Tracker may lead you
// to a better solution or official support for them.

/*

Index of this file:

DOCUMENTATION

- MISSION STATEMENT
- CONTROLS GUIDE
- PROGRAMMER GUIDE
  - READ FIRST
  - HOW TO UPDATE TO A NEWER VERSION OF DEAR IMGUI
  - GETTING STARTED WITH INTEGRATING DEAR IMGUI IN YOUR CODE/ENGINE
  - HOW A SIMPLE APPLICATION MAY LOOK LIKE
  - USING CUSTOM BACKEND / CUSTOM ENGINE
- API BREAKING CHANGES (read me when you update!)
- FREQUENTLY ASKED QUESTIONS (FAQ)
  - Read all answers online: https://www.dearimgui.com/faq, or in docs/FAQ.md (w
ith a Markdown viewer)

CODE
(search for "[SECTION]" in the code to find them)

// [SECTION] INCLUDES
// [SECTION] FORWARD DECLARATIONS
// [SECTION] CONTEXT AND MEMORY ALLOCATORS
// [SECTION] USER FACING STRUCTURES (ImGuiStyle, ImGuiIO, ImGuiPlatformIO)
// [SECTION] MISC HELPERS/UTILITIES (Geometry functions)
// [SECTION] MISC HELPERS/UTILITIES (String, Format, Hash functions)
// [SECTION] MISC HELPERS/UTILITIES (File functions)
// [SECTION] MISC HELPERS/UTILITIES (ImText* functions)
// [SECTION] MISC HELPERS/UTILITIES (Color functions)
// [SECTION] ImGuiStorage
// [SECTION] ImGuiTextFilter
// [SECTION] ImGuiTextBuffer, ImGuiTextIndex
// [SECTION] ImGuiListClipper
// [SECTION] STYLING
// [SECTION] RENDER HELPERS
// [SECTION] INITIALIZATION, SHUTDOWN
// [SECTION] MAIN CODE (most of the code! lots of stuff, needs tidying up!)
// [SECTION] FONTS, TEXTURES
// [SECTION] ID STACK
// [SECTION] INPUTS
// [SECTION] ERROR CHECKING, STATE RECOVERY
// [SECTION] ITEM SUBMISSION
// [SECTION] LAYOUT
// [SECTION] SCROLLING
// [SECTION] TOOLTIPS
// [SECTION] POPUPS
// [SECTION] WINDOW FOCUS
// [SECTION] KEYBOARD/GAMEPAD NAVIGATION
// [SECTION] DRAG AND DROP
// [SECTION] LOGGING/CAPTURING
// [SECTION] SETTINGS
// [SECTION] LOCALIZATION
// [SECTION] VIEWPORTS, PLATFORM WINDOWS
// [SECTION] PLATFORM DEPENDENT HELPERS
// [SECTION] METRICS/DEBUGGER WINDOW
// [SECTION] DEBUG LOG WINDOW
// [SECTION] OTHER DEBUG TOOLS (ITEM PICKER, ID STACK TOOL)

*/

//-----------------------------------------------------------------------------
// DOCUMENTATION
//-----------------------------------------------------------------------------

/*

 MISSION STATEMENT
 =================

 - Easy to use to create code-driven and data-driven tools.
 - Easy to use to create ad hoc short-lived tools and long-lived, more elaborate
 tools.
 - Easy to hack and improve.
 - Minimize setup and maintenance.
 - Minimize state storage on user side.
 - Minimize state synchronization.
 - Portable, minimize dependencies, run on target (consoles, phones, etc.).
 - Efficient runtime and memory consumption.

 Designed primarily for developers and content-creators, not the typical end-use
r!
 Some of the current weaknesses (which we aim to address in the future) includes
:

 - Doesn't look fancy.
 - Limited layout features, intricate layouts are typically crafted in code.


 CONTROLS GUIDE
 ==============

 - MOUSE CONTROLS
   - Mouse wheel:                   Scroll vertically.
   - SHIFT+Mouse wheel:             Scroll horizontally.
   - Click [X]:                     Close a window, available when 'bool* p_open
' is passed to ImGui::Begin().
   - Click ^, Double-Click title:   Collapse window.
   - Drag on corner/border:         Resize window (double-click to auto fit wind
ow to its contents).
   - Drag on any empty space:       Move window (unless io.ConfigWindowsMoveFrom
TitleBarOnly = true).
   - Left-click outside popup:      Close popup stack (right-click over underlyi
ng popup: Partially close popup stack).

 - TEXT EDITOR
   - Hold SHIFT or Drag Mouse:      Select text.
   - CTRL+Left/Right:               Word jump.
   - CTRL+Shift+Left/Right:         Select words.
   - CTRL+A or Double-Click:        Select All.
   - CTRL+X, CTRL+C, CTRL+V:        Use OS clipboard.
   - CTRL+Z                         Undo.
   - CTRL+Y or CTRL+Shift+Z:        Redo.
   - ESCAPE:                        Revert text to its original value.
   - On OSX, controls are automatically adjusted to match standard OSX text edit
ing 2ts and behaviors.

 - KEYBOARD CONTROLS
   - Basic:
     - Tab, SHIFT+Tab               Cycle through text editable fields.
     - CTRL+Tab, CTRL+Shift+Tab     Cycle through windows.
     - CTRL+Click                   Input text into a Slider or Drag widget.
   - Extended features with `io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboar
d`:
     - Tab, SHIFT+Tab:              Cycle through every items.
     - Arrow keys                   Move through items using directional navigat
ion. Tweak value.
     - Arrow keys + Alt, Shift      Tweak slower, tweak faster (when using arrow
 keys).
     - Enter                        Activate item (prefer text input when possib
le).
     - Space                        Activate item (prefer tweaking with arrows w
hen possible).
     - Escape                       Deactivate item, leave child window, close p
opup.
     - Page Up, Page Down           Previous page, next page.
     - Home, End                    Scroll to top, scroll to bottom.
     - Alt                          Toggle between scrolling layer and menu laye
r.
     - CTRL+Tab then Ctrl+Arrows    Move window. Hold SHIFT to resize instead of
 moving.
   - Output when ImGuiConfigFlags_NavEnableKeyboard set,
     - io.WantCaptureKeyboard flag is set when keyboard is claimed.
     - io.NavActive: true when a window is focused and it doesn't have the ImGui
WindowFlags_NoNavInputs flag set.
     - io.NavVisible: true when the navigation cursor is visible (usually goes t
o back false when mouse is used).

 - GAMEPAD CONTROLS
   - Enable with 'io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad'.
   - Particularly useful to use Dear ImGui on a console system (e.g. PlayStation
, Switch, Xbox) without a mouse!
   - Download controller mapping PNG/PSD at http://dearimgui.com/controls_sheets
   - Backend support: backend needs to:
      - Set 'io.BackendFlags |= ImGuiBackendFlags_HasGamepad' + call io.AddKeyEv
ent/AddKeyAnalogEvent() with ImGuiKey_Gamepad_XXX keys.
      - For analog values (0.0f to 1.0f), backend is responsible to handling a d
ead-zone and rescaling inputs accordingly.
        Backend code will probably need to transform your raw inputs (such as e.
g. remapping your 0.2..0.9 raw input range to 0.0..1.0 imgui range, etc.).
   - If you need to share inputs between your game and the Dear ImGui interface,
 the easiest approach is to go all-or-nothing,
     with a buttons combo to toggle the target. Please reach out if you think th
e game vs navigation input sharing could be improved.

 - REMOTE INPUTS SHARING & MOUSE EMULATION
   - PS4/PS5 users: Consider emulating a mouse cursor with DualShock touch pad o
r a spare analog stick as a mouse-emulation fallback.
   - Consoles/Tablet/Phone users: Consider using a Synergy 1.x server (on your P
C) + run examples/libs/synergy/uSynergy.c (on your console/tablet/phone app)
     in order to share your PC mouse/keyboard.
   - See https://github.com/ocornut/imgui/wiki/Useful-Extensions#remoting for ot
her remoting solutions.
   - On a TV/console system where readability may be lower or mouse inputs may b
e awkward, you may want to set the io.ConfigNavMoveSetMousePos flag.
     Enabling io.ConfigNavMoveSetMousePos + ImGuiBackendFlags_HasSetMousePos ins
tructs Dear ImGui to move your mouse cursor along with navigation movements.
     When enabled, the NewFrame() function may alter 'io.MousePos' and set 'io.W
antSetMousePos' to notify you that it wants the mouse cursor to be moved.
     When that happens your backend NEEDS to move the OS or underlying mouse cur
sor on the next frame. Some of the backends in examples/ do that.
     (If you set the NavEnableSetMousePos flag but don't honor 'io.WantSetMouseP
os' properly, Dear ImGui will misbehave as it will see your mouse moving back &
forth!)
     (In a setup when you may not have easy control over the mouse cursor, e.g.
uSynergy.c doesn't expose moving remote mouse cursor, you may want
     to set a boolean to ignore your other external mouse positions until the ex
ternal source is moved again.)


 PROGRAMMER GUIDE
 ================

 READ FIRST
 ----------
 - Remember to check the wonderful Wiki (https://github.com/ocornut/imgui/wiki)
 - Your code creates the UI every frame of your application loop, if your code d
oesn't run the UI is gone!
   The UI can be highly dynamic, there are no construction or destruction steps,
 less superfluous
   data retention on your side, less state duplication, less state synchronizati
on, fewer bugs.
 - Call and read ImGui::ShowDemoWindow() for demo code demonstrating most featur
es.
   Or browse https://pthom.github.io/imgui_manual_online/manual/imgui_manual.htm
l for interactive web version.
 - The library is designed to be built from sources. Avoid pre-compiled binaries
 and packaged versions. See imconfig.h to configure your build.
 - Dear ImGui is an implementation of the IMGUI paradigm (immediate-mode graphic
al user interface, a term coined by Casey Muratori).
   You can learn about IMGUI principles at http://www.johno.se/book/imgui.html,
http://mollyrocket.com/861 & more links in Wiki.
 - Dear ImGui is a "single pass" rasterizing implementation of the IMGUI paradig
m, aimed at ease of use and high-performances.
   For every application frame, your UI code will be called only once. This is i
n contrast to e.g. Unity's implementation of an IMGUI,
   where the UI code is called multiple times ("multiple passes") from a single
entry point. There are pros and cons to both approaches.
 - Our origin is on the top-left. In axis aligned bounding boxes, Min = top-left
, Max = bottom-right.
 - Please make sure you have asserts enabled (IM_ASSERT redirects to assert() by
 default, but can be redirected).
   If you get an assert, read the messages and comments around the assert.
 - This codebase aims to be highly optimized:
   - A typical idle frame should never call malloc/free.
   - We rely on a maximum of constant-time or O(N) algorithms. Limiting searches
/scans as much as possible.
   - We put particular energy in making sure performances are decent with typica
l "Debug" build settings as well.
     Which mean we tend to avoid over-relying on "zero-cost abstraction" as they
 aren't zero-cost at all.
 - This codebase aims to be both highly opinionated and highly flexible:
   - This code works because of the things it choose to solve or not solve.
   - C++: this is a pragmatic C-ish codebase: we don't use fancy C++ features, w
e don't include C++ headers,
     and ImGui:: is a namespace. We rarely use member functions (and when we did
, I am mostly regretting it now).
     This is to increase compatibility, increase maintainability and facilitate
use from other languages.
   - C++: ImVec2/ImVec4 do not expose math operators by default, because it is e
xpected that you use your own math types.
     See FAQ "How can I use my own math types instead of ImVec2/ImVec4?" for det
ails about setting up imconfig.h for that.
     We can can optionally export math operators for ImVec2/ImVec4 using IMGUI_D
EFINE_MATH_OPERATORS, which we use internally.
   - C++: pay attention that ImVector<> manipulates plain-old-data and does not
honor construction/destruction
     (so don't use ImVector in your code or at our own risk!).
   - Building: We don't use nor mandate a build system for the main library.
     This is in an effort to ensure that it works in the real world aka with any
 esoteric build setup.
     This is also because providing a build system for the main library would be
 of little-value.
     The build problems are almost never coming from the main library but from s
pecific backends.


 HOW TO UPDATE TO A NEWER VERSION OF DEAR IMGUI
 ----------------------------------------------
 - Update submodule or copy/overwrite every file.
 - About imconfig.h:
   - You may modify your copy of imconfig.h, in this case don't overwrite it.
   - or you may locally branch to modify imconfig.h and merge/rebase latest.
   - or you may '#define IMGUI_USER_CONFIG "my_config_file.h"' globally from you
r build system to
     specify a custom path for your imconfig.h file and instead not have to modi
fy the default one.

 - Overwrite all the sources files except for imconfig.h (if you have modified y
our copy of imconfig.h)
 - Or maintain your own branch where you have imconfig.h modified as a top-most
commit which you can regularly rebase over "master".
 - You can also use '#define IMGUI_USER_CONFIG "my_config_file.h" to redirect co
nfiguration to your own file.
 - Read the "API BREAKING CHANGES" section (below). This is where we list occasi
onal API breaking changes.
   If a function/type has been renamed / or marked obsolete, try to fix the name
 in your code before it is permanently removed
   from the public API. If you have a problem with a missing function/symbols, s
earch for its name in the code, there will
   likely be a comment about it. Please report any issue to the GitHub page!
 - To find out usage of old API, you can add '#define IMGUI_DISABLE_OBSOLETE_FUN
CTIONS' in your configuration file.
 - Try to keep your copy of Dear ImGui reasonably up to date!


 GETTING STARTED WITH INTEGRATING DEAR IMGUI IN YOUR CODE/ENGINE
 ---------------------------------------------------------------
 - See https://github.com/ocornut/imgui/wiki/Getting-Started.
 - Run and study the examples and demo in imgui_demo.cpp to get acquainted with
the library.
 - In the majority of cases you should be able to use unmodified backends files
available in the backends/ folder.
 - Add the Dear ImGui source files + selected backend source files to your proje
cts or using your preferred build system.
   It is recommended you build and statically link the .cpp files as part of you
r project and NOT as a shared library (DLL).
 - You can later customize the imconfig.h file to tweak some compile-time behavi
or, such as integrating Dear ImGui types with your own maths types.
 - When using Dear ImGui, your programming IDE is your friend: follow the declar
ation of variables, functions and types to find comments about them.
 - Dear ImGui never touches or knows about your GPU state. The only function tha
t knows about GPU is the draw function that you provide.
   Effectively it means you can create widgets at any time in your code, regardl
ess of considerations of being in "update" vs "render"
   phases of your own application. All rendering information is stored into comm
and-lists that you will retrieve after calling ImGui::Render().
 - Refer to the backends and demo applications in the examples/ folder for instr
uction on how to setup your code.
 - If you are running over a standard OS with a common graphics API, you should
be able to use unmodified imgui_impl_*** files from the examples/ folder.


 HOW A SIMPLE APPLICATION MAY LOOK LIKE
 --------------------------------------

 USING THE EXAMPLE BACKENDS (= imgui_impl_XXX.cpp files from the backends/ folde
r).
 The sub-folders in examples/ contain examples applications following this struc
ture.

     // Application init: create a dear imgui context, setup some options, load
fonts
     ImGui::CreateContext();
     ImGuiIO& io = ImGui::GetIO();
     // TODO: Set optional io.ConfigFlags values, e.g. 'io.ConfigFlags |= ImGuiC
onfigFlags_NavEnableKeyboard' to enable keyboard controls.
     // TODO: Fill optional fields of the io structure later.
     // TODO: Load TTF/OTF fonts if you don't want to use the default font.

     // Initialize helper Platform and Renderer backends (here we are using imgu
i_impl_win32.cpp and imgui_impl_dx11.cpp)
     ImGui_ImplWin32_Init(hwnd);
     ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

     // Application main loop
     while (true)
     {
         // Feed inputs to dear imgui, start new frame
         ImGui_ImplDX11_NewFrame();
         ImGui_ImplWin32_NewFrame();
         ImGui::NewFrame();

         // Any application code here
         ImGui::Text("Hello, world!");

         // Render dear imgui into framebuffer
         ImGui::Render();
         ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
         g_pSwapChain->Present(1, 0);
     }

     // Shutdown
     ImGui_ImplDX11_Shutdown();
     ImGui_ImplWin32_Shutdown();
     ImGui::DestroyContext();

 To decide whether to dispatch mouse/keyboard inputs to Dear ImGui to the rest o
f your application,
 you should read the 'io.WantCaptureMouse', 'io.WantCaptureKeyboard' and 'io.Wan
tTextInput' flags!
 Please read the FAQ entry "How can I tell whether to dispatch mouse/keyboard to
 Dear ImGui or my application?" about this.


USING CUSTOM BACKEND / CUSTOM ENGINE
------------------------------------

IMPLEMENTING YOUR PLATFORM BACKEND:
 -> see https://github.com/ocornut/imgui/blob/master/docs/BACKENDS.md for basic
instructions.
 -> the Platform backends in impl_impl_XXX.cpp files contain many implementation
s.

IMPLEMENTING YOUR RenderDrawData() function:
 -> see https://github.com/ocornut/imgui/blob/master/docs/BACKENDS.md
 -> the Renderer Backends in impl_impl_XXX.cpp files contain many implementation
s of a ImGui_ImplXXXX_RenderDrawData() function.

IMPLEMENTING SUPPORT for ImGuiBackendFlags_RendererHasTextures:
 -> see https://github.com/ocornut/imgui/blob/master/docs/BACKENDS.md
 -> the Renderer Backends in impl_impl_XXX.cpp files contain many implementation
s of a ImGui_ImplXXXX_UpdateTexture() function.

 Basic application/backend skeleton:

     // Application init: create a Dear ImGui context, setup some options, load
fonts
     ImGui::CreateContext();
     ImGuiIO& io = ImGui::GetIO();
     // TODO: set io.ConfigXXX values, e.g.
     io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable keyboard c
ontrols

     // TODO: Load TTF/OTF fonts if you don't want to use the default font.
     io.Fonts->AddFontFromFileTTF("NotoSans.ttf");

     // Application main loop
     while (true)
     {
        // Setup low-level inputs, e.g. on Win32: calling GetKeyboardState(), or
 write to those fields from your Windows message handlers, etc.
        // (In the examples/ app this is usually done within the ImGui_ImplXXX_N
ewFrame() function from one of the demo Platform Backends)
        io.DeltaTime = 1.0f/60.0f;              // set the time elapsed since th
e previous frame (in seconds)
        io.DisplaySize.x = 1920.0f;             // set the current display width
        io.DisplaySize.y = 1280.0f;             // set the current display heigh
t here
        io.AddMousePosEvent(mouse_x, mouse_y);  // update mouse position
        io.AddMouseButtonEvent(0, mouse_b[0]);  // update mouse button states
        io.AddMouseButtonEvent(1, mouse_b[1]);  // update mouse button states

        // Call NewFrame(), after this point you can use ImGui::* functions anyt
ime
        // (So you want to try calling NewFrame() as early as you can in your ma
in loop to be able to use Dear ImGui everywhere)
        ImGui::NewFrame();

        // Most of your application code here
        ImGui::Text("Hello, world!");
        MyGameUpdate(); // may use any Dear ImGui functions, e.g. ImGui::Begin("
My window"); ImGui::Text("Hello, world!"); ImGui::End();
        MyGameRender(); // may use any Dear ImGui functions as well!

        // End the dear imgui frame
        // (You want to try calling EndFrame/Render as late as you can, to be ab
le to use Dear ImGui in your own game rendering code)
        ImGui::EndFrame(); // this is automatically called by Render(), but avai
lable
        ImGui::Render();

        // Update textures
        ImDrawData* draw_data = ImGui::GetDrawData();
        for (ImTextureData* tex : *draw_data->Textures)
            if (tex->Status != ImTextureStatus_OK)
                MyImGuiBackend_UpdateTexture(tex);

        // Render dear imgui contents, swap buffers
        MyImGuiBackend_RenderDrawData(draw_data);
        SwapBuffers();
     }

     // Shutdown
     ImGui::DestroyContext();



 API BREAKING CHANGES
 ====================

 Occasionally introducing changes that are breaking the API. We try to make the
breakage minor and easy to fix.
 Below is a change-log of API breaking changes only. If you are using one of the
 functions listed, expect to have to fix some code.
 When you are not sure about an old symbol or function name, try using the Searc
h/Find function of your IDE to look for comments or references in all imgui file
s.
 You can read releases logs https://github.com/ocornut/imgui/releases for more d
etails.

 - 2025/08/08 (1.92.2) - Backends: SDL_GPU3: Changed ImTextureID type from SDL_G
PUTextureSamplerBinding* to SDL_GPUTexture*, which is more natural and easier fo
r user to manage. If you need to change the current sampler, you can access the
ImGui_ImplSDLGPU3_RenderState struct. (#8866, #8163, #7998, #7988)
 - 2025/07/31 (1.92.2) - Tabs: Renamed ImGuiTabBarFlags_FittingPolicyResizeDown
to ImGuiTabBarFlags_FittingPolicyShrink. Kept inline redirection enum (will obso
lete).
 - 2025/06/25 (1.92.0) - Layout: commented out legacy ErrorCheckUsingSetCursorPo
sToExtendParentBoundaries() fallback obsoleted in 1.89 (August 2022) which allow
ed a SetCursorPos()/SetCursorScreenPos() call WITHOUT AN ITEM
                         to extend parent window/cell boundaries. Replaced with
assert/tooltip that would already happens if previously using IMGUI_DISABLE_OBSO
LETE_FUNCTIONS. (#5548, #4510, #3355, #1760, #1490, #4152, #150)
                         - Incorrect way to make a window content size 200x200:
                              Begin(...) + SetCursorScreenPos(GetCursorScreenPos
() + ImVec2(200,200)) + End();
                         - Correct ways to make a window content size 200x200:
                              Begin(...) + SetCursorScreenPos(GetCursorScreenPos
() + ImVec2(200,200)) + Dummy(ImVec2(0,0)) + End();
                              Begin(...) + Dummy(ImVec2(200,200)) + End();
                         - TL;DR; if the assert triggers, you can add a Dummy({0
,0}) call to validate extending parent boundaries.
 - 2025/06/11 (1.92.0) - THIS VERSION CONTAINS THE LARGEST AMOUNT OF BREAKING CH
ANGES SINCE 2015! I TRIED REALLY HARD TO KEEP THEM TO A MINIMUM, REDUCE THE AMOU
NT OF INTERFERENCES, BUT INEVITABLY SOME USERS WILL BE AFFECTED.
                         IN ORDER TO HELP US IMPROVE THE TRANSITION PROCESS, INC
L. DOCUMENTATION AND COMMENTS, PLEASE REPORT **ANY** DOUBT, CONFUSION, QUESTIONS
, FEEDBACK TO: https://github.com/ocornut/imgui/issues/
                         As part of the plan to reduce impact of API breaking ch
anges, several unfinished changes/features/refactors related to font and text sy
stems and scaling will be part of subsequent releases (1.92.1+).
                         If you are updating from an old version, and expecting
a massive or difficult update, consider first updating to 1.91.9 to reduce the a
mount of changes.
                       - Hard to read? Refer to 'docs/Changelog.txt' for a less
compact and more complete version of this!
                       - Fonts: **IMPORTANT**: if your app was solving the OSX/i
OS Retina screen specific logical vs display scale problem by setting io.Display
FramebufferScale (e.g. to 2.0f) + setting io.FontGlobalScale (e.g. to 1.0f/2.0f)
 + loading fonts at scaled sizes (e.g. size X * 2.0f):
                         This WILL NOT map correctly to the new system! Because
font will rasterize as requested size.
                         - With a legacy backend (< 1.92): Instead of setting io
.FontGlobalScale = 1.0f/N -> set ImFontCfg::RasterizerDensity = N. This already
worked before, but is now pretty much required.
                         - With a new backend (1.92+): This should be all automa
tic. FramebufferScale is automatically used to set current font RasterizerDensit
y. FramebufferScale is a per-viewport property provided by backend through the P
latform_GetWindowFramebufferScale() handler in 'docking' branch.
                       - Fonts: **IMPORTANT** on Font Sizing: Before 1.92, fonts
 were of a single size. They can now be dynamically sized.
                         - PushFont() API now has a REQUIRED size parameter.
                         - Before 1.92: PushFont() always used font "default" si
ze specified in AddFont() call. It is equivalent to calling PushFont(font, font-
>LegacySize).
                         - Since  1.92: PushFont(font, 0.0f) preserve the curren
t font size which is a shared value.
                         - To use old behavior: use 'ImGui::PushFont(font, font-
>LegacySize)' at call site.
                         - Kept inline single parameter function. Will obsolete.
                       - Fonts: **IMPORTANT** on Font Merging:
                         - When searching for a glyph in multiple merged fonts:
we search for the FIRST font source which contains the desired glyph.
                           Because the user doesn't need to provide glyph ranges
 any more, it is possible that a glyph that you expected to fetch from a seconda
ry/merged icon font may be erroneously fetched from the primary font.
                         - When searching for a glyph in multiple merged fonts:
we now search for the FIRST font source which contains the desired glyph. This i
s technically a different behavior than before!
                         - e.g. If you are merging fonts you may have glyphs tha
t you expected to load from Font Source 2 which exists in Font Source 1.
                           After the update and when using a new backend, those
glyphs may now loaded from Font Source 1!
                         - We added `ImFontConfig::GlyphExcludeRanges[]` to spec
ify ranges to exclude from a given font source:
                             // Add Font Source 1 but ignore ICON_MIN_FA..ICON_M
AX_FA range
                             static ImWchar exclude_ranges[] = { ICON_MIN_FA, IC
ON_MAX_FA, 0 };
                             ImFontConfig cfg1;
                             cfg1.GlyphExcludeRanges = exclude_ranges;
                             io.Fonts->AddFontFromFileTTF("segoeui.ttf", 0.0f, &
cfg1);
                             // Add Font Source 2, which expects to use the rang
e above
                             ImFontConfig cfg2;
                             cfg2.MergeMode = true;
                             io.Fonts->AddFontFromFileTTF("FontAwesome4.ttf", 0.
0f, &cfg2);
                         - You can use `Metrics/Debugger->Fonts->Font->Input Gly
phs Overlap Detection Tool` to see list of glyphs available in multiple font sou
rces. This can facilitate unde
                       - Fonts: **IMPORTANT** on Thread Safety:
                          - A few functions such as font->CalcTextSizeA() were,
by sheer luck (== accidentally) thread-safe even thou we had never provided that
 guarantee. They are definitively not thread-safe anymore as new glyphs may be l
oaded.
                       - Fonts: ImFont::FontSize was removed and does not make s
ense anymore. ImFont::LegacySize is the size passed to AddFont().
                       - Fonts: Removed support for PushFont(NULL) which was a s
hortcut for "default font".
                       - Fonts: Renamed/moved 'io.FontGlobalScale' to 'style.Fon
tScaleMain'.
                       - Textures: all API functions taking a 'ImTextureID' para
meter are now taking a 'ImTextureRef'. Affected functions are: ImGui::Image(), I
mGui::ImageWithBg(), ImGui::ImageButton(), ImDrawList::AddImage(), ImDrawList::A
ddImageQuad(), ImDrawList::AddImageRounded().
                       - Fonts: obsoleted ImFontAtlas::GetTexDataAsRGBA32(), Get
TexDataAsAlpha8(), Build(), SetTexID(), IsBuilt() functions. The new protocol fo
r backends to handle textures doesn't need them. Kept redirection functions (wil
l obsolete).
                       - Fonts: ImFontConfig::OversampleH/OversampleV default to
 automatic (== 0) since v1.91.8. It is quite important you keep it automatic unt
il we decide if we want to provide a way to express finer policy, otherwise you
will likely waste texture space when using large glyphs. Note that the imgui_fre
etype backend doesn't use and does not need oversampling.
                       - Fonts: specifying glyph ranges is now unnecessary. The
value of ImFontConfig::GlyphRanges[] is only useful for legacy backends. All Get
GlyphRangesXXXX() functions are now marked obsolete: GetGlyphRangesDefault(), Ge
tGlyphRangesGreek(), GetGlyphRangesKorean(), GetGlyphRangesJapanese(), GetGlyphR
angesChineseSimplifiedCommon(), GetGlyphRangesChineseFull(), GetGlyphRangesCyril
lic(), GetGlyphRangesThai(), GetGlyphRangesVietnamese().
                       - Fonts: removed ImFontAtlas::TexDesiredWidth to enforce
a texture width. (#327)
                       - Fonts: if you create and manage ImFontAtlas instances y
ourself (instead of relying on ImGuiContext to create one), you'll need to call
ImFontAtlasUpdateNewFrame() yourself. An assert will trigger if you don't.
                       - Fonts: obsolete ImGui::SetWindowFontScale() which is no
t useful anymore. Prefer using 'PushFont(NULL, style.FontSizeBase * factor)' or
to manipulate other scaling factors.
                       - Fonts: obsoleted ImFont::Scale which is not useful anym
ore.
                       - Fonts: generally reworked Internals of ImFontAtlas and
ImFont. While in theory a vast majority of users shouldn't be affected, some use
 cases or extensions might be. Among other things:
                          - ImDrawCmd::TextureId has been changed to ImDrawCmd::
TexRef.
                          - ImFontAtlas::TexID has been changed to ImFontAtlas::
TexRef.
                          - ImFontAtlas::ConfigData[] has been renamed to ImFont
Atlas::Sources[]
                          - ImFont::ConfigData[], ConfigDataCount has been renam
ed to Sources[], SourceCount.
                          - Each ImFont has a number of ImFontBaked instances co
rresponding to actively used sizes. ImFont::GetFontBaked(size) retrieves the one
 for a given size.
                          - Fields moved from ImFont to ImFontBaked: IndexAdvanc
eX[], Glyphs[], Ascent, Descent, FindGlyph(), FindGlyphNoFallback(), GetCharAdva
nce().
                          - Fields moved from ImFontAtlas to ImFontAtlas->Tex: I
mFontAtlas::TexWidth => TexData->Width, ImFontAtlas::TexHeight => TexData->Heigh
t, ImFontAtlas::TexPixelsAlpha8/TexPixelsRGBA32 => TexData->GetPixels().
                          - Widget code may use ImGui::GetFontBaked() instead of
 ImGui::GetFont() to access font data for current font at current font size (and
 you may use font->GetFontBaked(size) to access it for any other size.)
                       - Fonts: (users of imgui_freetype): renamed ImFontAtlas::
FontBuilderFlags to ImFontAtlas::FontLoaderFlags. Renamed ImFontConfig::FontBuil
derFlags to ImFontConfig::FontLoaderFlags. Renamed ImGuiFreeTypeBuilderFlags to
ImGuiFreeTypeLoaderFlags.
                         If you used runtime imgui_freetype selection rather tha
n the default IMGUI_ENABLE_FREETYPE compile-time option: Renamed/reworked ImFont
BuilderIO into ImFontLoader. Renamed ImGuiFreeType::GetBuilderForFreeType() to I
mGuiFreeType::GetFontLoader().
                           - old:  io.Fonts->FontBuilderIO = ImGuiFreeType::GetB
uilderForFreeType()
                           - new:  io.Fonts->FontLoader = ImGuiFreeType::GetFont
Loader()
                           - new:  io.Fonts->SetFontLoader(ImGuiFreeType::GetFon
tLoader()) to change dynamically at runtime [from 1.92.1]
                       - Fonts: (users of custom rectangles, see #8466): Renamed
 AddCustomRectRegular() to AddCustomRect(). Added GetCustomRect() as a replaceme
nt for GetCustomRectByIndex() + CalcCustomRectUV().
                           - The output type of GetCustomRect() is now ImFontAtl
asRect, which include UV coordinates. X->x, Y->y, Width->w, Height->h.
                           - old:
                                const ImFontAtlasCustomRect* r = atlas->GetCusto
mRectByIndex(custom_rect_id);
                                ImVec2 uv0, uv1;
                                atlas->GetCustomRectUV(r, &uv0, &uv1);
                                ImGui::Image(atlas->TexRef, ImVec2(r->w, r->h),
uv0, uv1);
                           - new;
                                ImFontAtlasRect r;
                                atlas->GetCustomRect(custom_rect_id, &r);
                                ImGui::Image(atlas->TexRef, ImVec2(r.w, r.h), r.
uv0, r.uv1);
                           - We added a redirecting typedef but haven't attempte
d to magically redirect the field names, as this API is rarely used and the fix
is simple.
                           - Obsoleted AddCustomRectFontGlyph() as the API does
not make sense for scalable fonts. Kept existing function which uses the font "d
efault size" (Sources[0]->LegacySize). Added a helper AddCustomRectFontGlyphForS
ize() which is immediately marked obsolete, but can facilitate transitioning old
 code.
                           - Prefer adding a font source (ImFontConfig) using a
custom/procedural loader.
                       - DrawList: Renamed ImDrawList::PushTextureID()/PopTextur
eID() to PushTexture()/PopTexture().
                       - Backends: removed ImGui_ImplXXXX_CreateFontsTexture()/I
mGui_ImplXXXX_DestroyFontsTexture() for all backends that had them. They should
not be necessary any more.
 - 2025/05/23 (1.92.0) - Fonts: changed ImFont::CalcWordWrapPositionA() to ImFon
t::CalcWordWrapPosition()
                            - old:  const char* ImFont::CalcWordWrapPositionA(fl
oat scale, const char* text, ....);
                            - new:  const char* ImFont::CalcWordWrapPosition (fl
oat size,  const char* text, ....);
                         The leading 'float scale' parameters was changed to 'fl
oat size'. This was necessary as 'scale' is assuming standard font size which is
 a concept we aim to eliminate in an upcoming update. Kept inline redirection fu
nction.
 - 2025/05/15 (1.92.0) - TreeNode: renamed ImGuiTreeNodeFlags_NavLeftJumpsBackHe
re to ImGuiTreeNodeFlags_NavLeftJumpsToParent for clarity. Kept inline redirecti
on enum (will obsolete).
 - 2025/05/15 (1.92.0) - Commented out PushAllowKeyboardFocus()/PopAllowKeyboard
Focus() which was obsoleted in 1.89.4. Use PushItemFlag(ImGuiItemFlags_NoTabStop
, !tab_stop)/PopItemFlag() instead. (#3092)
 - 2025/05/15 (1.92.0) - Commented out ImGuiListClipper::ForceDisplayRangeByIndi
ces() which was obsoleted in 1.89.6. Use ImGuiListClipper::IncludeItemsByIndex()
 instead.
 - 2025/03/05 (1.91.9) - BeginMenu(): Internals: reworked mangling of menu windo
ws to use "###Menu_00" etc. instead of "##Menu_00", allowing them to also store
the menu name before it. This shouldn't affect code unless directly accessing me
nu window from their mangled name.
 - 2025/04/16 (1.91.9) - Internals: RenderTextEllipsis() function removed the 'f
loat clip_max_x' parameter directly preceding 'float ellipsis_max_x'. Values wer
e identical for a vast majority of users. (#8387)
 - 2025/02/27 (1.91.9) - Image(): removed 'tint_col' and 'border_col' parameter
from Image() function. Added ImageWithBg() replacement. (#8131, #8238)
                            - old: void Image      (ImTextureID tex_id, ImVec2 i
mage_size, ImVec2 uv0 = (0,0), ImVec2 uv1 = (1,1), ImVec4 tint_col = (1,1,1,1),
ImVec4 border_col = (0,0,0,0));
                            - new: void Image      (ImTextureID tex_id, ImVec2 i
mage_size, ImVec2 uv0 = (0,0), ImVec2 uv1 = (1,1));
                            - new: void ImageWithBg(ImTextureID tex_id, ImVec2 i
mage_size, ImVec2 uv0 = (0,0), ImVec2 uv1 = (1,1), ImVec4 bg_col = (0,0,0,0), Im
Vec4 tint_col = (1,1,1,1));
                            - TL;DR: 'border_col' had misleading side-effect on
layout, 'bg_col' was missing, parameter order couldn't be consistent with ImageB
utton().
                            - new behavior always use ImGuiCol_Border color + st
yle.ImageBorderSize / ImGuiStyleVar_ImageBorderSize.
                            - old behavior altered border size (and therefore la
yout) based on border color's alpha, which caused variety of problems + old beha
vior a fixed 1.0f for border size which was not tweakable.
                            - kept legacy signature (will obsolete), which mimic
s the old behavior,  but uses Max(1.0f, style.ImageBorderSize) when border_col i
s specified.
                            - added ImageWithBg() function which has both 'bg_co
l' (which was missing) and 'tint_col'. It was impossible to add 'bg_col' to Imag
e() with a parameter order consistent with other functions, so we decided to rem
ove 'tint_col' and introduce ImageWithBg().
 - 2025/02/25 (1.91.9) - internals: fonts: ImFontAtlas::ConfigData[] has been re
named to ImFontAtlas::Sources[]. ImFont::ConfigData[], ConfigDataCount has been
renamed to Sources[], SourcesCount.
 - 2025/02/06 (1.91.9) - renamed ImFontConfig::GlyphExtraSpacing.x to ImFontConf
ig::GlyphExtraAdvanceX.
 - 2025/01/22 (1.91.8) - removed ImGuiColorEditFlags_AlphaPreview (made value 0)
: it is now the default behavior.
                         prior to 1.91.8: alpha was made opaque in the preview b
y default _unless_ using ImGuiColorEditFlags_AlphaPreview. We now display the pr
eview as transparent by default. You can use ImGuiColorEditFlags_AlphaOpaque to
use old behavior.
                         the new flags (ImGuiColorEditFlags_AlphaOpaque, ImGuiCo
lorEditFlags_AlphaNoBg + existing ImGuiColorEditFlags_AlphaPreviewHalf) may be c
ombined better and allow finer controls:
 - 2025/01/14 (1.91.7) - renamed ImGuiTreeNodeFlags_SpanTextWidth to ImGuiTreeNo
deFlags_SpanLabelWidth for consistency with other names. Kept redirection enum (
will obsolete). (#6937)
 - 2024/11/27 (1.91.6) - changed CRC32 table from CRC32-adler to CRC32c polynomi
al in order to be compatible with the result of SSE 4.2 instructions.
                         As a result, old .ini data may be partially lost (docki
ng and tables information particularly).
                         Because some users have crafted and storing .ini data a
s a way to workaround limitations of the docking API, we are providing a '#defin
e IMGUI_USE_LEGACY_CRC32_ADLER' compile-time option to keep using old CRC32 tabl
es if you cannot afford invalidating old .ini data.
 - 2024/11/06 (1.91.5) - commented/obsoleted out pre-1.87 IO system (equivalent
to using IMGUI_DISABLE_OBSOLETE_KEYIO or IMGUI_DISABLE_OBSOLETE_FUNCTIONS before
)
                            - io.KeyMap[] and io.KeysDown[] are removed (obsolet
ed February 2022).
                            - io.NavInputs[] and ImGuiNavInput are removed (obso
leted July 2022).
                            - pre-1.87 backends are not supported:
                               - backends need to call io.AddKeyEvent(), io.AddM
ouseEvent() instead of writing to io.KeysDown[], io.MouseDown[] fields.
                               - backends need to call io.AddKeyAnalogEvent() fo
r gamepad values instead of writing to io.NavInputs[] fields.
                            - for more reference:
                              - read 1.87 and 1.88 part of this section or read
Changelog for 1.87 and 1.88.
                              - read https://github.com/ocornut/imgui/issues/492
1
                            - if you have trouble updating a very old codebase u
sing legacy backend-specific key codes: consider updating to 1.91.4 first, then
#define IMGUI_DISABLE_OBSOLETE_KEYIO, then update to latest.
                       - obsoleted ImGuiKey_COUNT (it is unusually error-prone/m
isleading since valid keys don't start at 0). probably use ImGuiKey_NamedKey_BEG
IN/ImGuiKey_NamedKey_END?
                       - fonts: removed const qualifiers from most font function
s in prevision for upcoming font improvements.
 - 2024/10/18 (1.91.4) - renamed ImGuiCol_NavHighlight to ImGuiCol_NavCursor (fo
r consistency with newly exposed and reworked features). Kept inline redirection
 enum (will obsolete).
 - 2024/10/14 (1.91.4) - moved ImGuiConfigFlags_NavEnableSetMousePos to standalo
ne io.ConfigNavMoveSetMousePos bool.
                         moved ImGuiConfigFlags_NavNoCaptureKeyboard to standalo
ne io.ConfigNavCaptureKeyboard bool (note the inverted value!).
                         kept legacy names (will obsolete) + code that copies se
ttings once the first time. Dynamically changing the old value won't work. Switc
h to using the new value!
 - 2024/10/10 (1.91.4) - the typedef for ImTextureID now defaults to ImU64 inste
ad of void*. (#1641)
                         this removes the requirement to redefine it for backend
s which are e.g. storing descriptor sets or other 64-bits structures when buildi
ng on 32-bits archs. It therefore simplify various building scripts/helpers.
                         you may have compile-time issues if you were casting to
 'void*' instead of 'ImTextureID' when passing your types to functions taking Im
TextureID values, e.g. ImGui::Image().
                         in doubt it is almost always better to do an intermedia
te intptr_t cast, since it allows casting any pointer/integer type without warni
ng:
                            - May warn:    ImGui::Image((void*)MyTextureData, ..
.);
                            - May warn:    ImGui::Image((void*)(intptr_t)MyTextu
reData, ...);
                            - Won't warn:  ImGui::Image((ImTextureID)(intptr_t)M
yTextureData, ...);
  -                      note that you can always define ImTextureID to be your
own high-level structures (with dedicated constructors) if you like.
 - 2024/10/03 (1.91.3) - drags: treat v_min==v_max as a valid clamping range whe
n != 0.0f. Zero is a still special value due to legacy reasons, unless using ImG
uiSliderFlags_ClampZeroRange. (#7968, #3361, #76)
                       - drags: extended behavior of ImGuiSliderFlags_AlwaysClam
p to include _ClampZeroRange. It considers v_min==v_max==0.0f as a valid clampin
g range (aka edits not allowed).
                         although unlikely, it you wish to only clamp on text in
put but want v_min==v_max==0.0f to mean unclamped drags, you can use _ClampOnInp
ut instead of _AlwaysClamp. (#7968, #3361, #76)
 - 2024/09/10 (1.91.2) - internals: using multiple overlaid ButtonBehavior() wit
h same ID will now have io.ConfigDebugHighlightIdConflicts=true feature emit a w
arning. (#8030)
                         it was one of the rare case where using same ID is lega
l. workarounds: (1) use single ButtonBehavior() call with multiple _MouseButton
flags, or (2) surround the calls with PushItemFlag(ImGuiItemFlags_AllowDuplicate
Id, true); ... PopItemFlag()
 - 2024/08/23 (1.91.1) - renamed ImGuiChildFlags_Border to ImGuiChildFlags_Borde
rs for consistency. kept inline redirection flag.
 - 2024/08/22 (1.91.1) - moved some functions from ImGuiIO to ImGuiPlatformIO st
ructure:
                            - io.GetClipboardTextFn         -> platform_io.Platf
orm_GetClipboardTextFn + changed 'void* user_data' to 'ImGuiContext* ctx'. Pull
your user data from platform_io.ClipboardUserData.
                            - io.SetClipboardTextFn         -> platform_io.Platf
orm_SetClipboardTextFn + same as above line.
                            - io.PlatformOpenInShellFn      -> platform_io.Platf
orm_OpenInShellFn (#7660)
                            - io.PlatformSetImeDataFn       -> platform_io.Platf
orm_SetImeDataFn
                            - io.PlatformLocaleDecimalPoint -> platform_io.Platf
orm_LocaleDecimalPoint (#7389, #6719, #2278)
                            - access those via GetPlatformIO() instead of GetIO(
).
                         some were introduced very recently and often automatica
lly setup by core library and backends, so for those we are exceptionally not ma
intaining a legacy redirection symbol.
                       - commented the old ImageButton() signature obsoleted in
1.89 (~August 2022). As a reminder:
                            - old ImageButton() before 1.89 used ImTextureId as
item id (created issue with e.g. multiple buttons in same scope, transient textu
re id values, opaque computation of ID)
                            - new ImageButton() since 1.89 requires an explicit
'const char* str_id'
                            - old ImageButton() before 1.89 had frame_padding' o
verride argument.
                            - new ImageButton() since 1.89 always use style.Fram
ePadding, which you can freely override with PushStyleVar()/PopStyleVar().
 - 2024/07/25 (1.91.0) - obsoleted GetContentRegionMax(), GetWindowContentRegion
Min() and GetWindowContentRegionMax(). (see #7838 on GitHub for more info)
                         you should never need those functions. you can do every
thing with GetCursorScreenPos() and GetContentRegionAvail() in a more simple way
.
                            - instead of:  GetWindowContentRegionMax().x - GetCu
rsorPos().x
                            - you can use: GetContentRegionAvail().x
                            - instead of:  GetWindowContentRegionMax().x + GetWi
ndowPos().x
                            - you can use: GetCursorScreenPos().x + GetContentRe
gionAvail().x // when called from left edge of window
                            - instead of:  GetContentRegionMax()
                            - you can use: GetContentRegionAvail() + GetCursorSc
reenPos() - GetWindowPos() // right edge in local coordinates
                            - instead of:  GetWindowContentRegionMax().x - GetWi
ndowContentRegionMin().x
                            - you can use: GetContentRegionAvail() // when calle
d from left edge of window
 - 2024/07/15 (1.91.0) - renamed ImGuiSelectableFlags_DontClosePopups to ImGuiSe
lectableFlags_NoAutoClosePopups. (#1379, #1468, #2200, #4936, #5216, #7302, #757
3)
                         (internals: also renamed ImGuiItemFlags_SelectableDontC
losePopup into ImGuiItemFlags_AutoClosePopups with inverted behaviors)
 - 2024/07/15 (1.91.0) - obsoleted PushButtonRepeat()/PopButtonRepeat() in favor
 of using new PushItemFlag(ImGuiItemFlags_ButtonRepeat, ...)/PopItemFlag().
 - 2024/07/02 (1.91.0) - commented out obsolete ImGuiModFlags (renamed to ImGuiK
eyChord in 1.89). (#4921, #456)
                       - commented out obsolete ImGuiModFlags_XXX values (rename
d to ImGuiMod_XXX in 1.89). (#4921, #456)
                            - ImGuiModFlags_Ctrl -> ImGuiMod_Ctrl, ImGuiModFlags
_Shift -> ImGuiMod_Shift etc.
 - 2024/07/02 (1.91.0) - IO, IME: renamed platform IME hook and added explicit c
ontext for consistency and future-proofness.
                            - old: io.SetPlatformImeDataFn(ImGuiViewport* viewpo
rt, ImGuiPlatformImeData* data);
                            - new: io.PlatformSetImeDataFn(ImGuiContext* ctx, Im
GuiViewport* viewport, ImGuiPlatformImeData* data);
 - 2024/06/21 (1.90.9) - BeginChild: added ImGuiChildFlags_NavFlattened as a rep
lacement for the window flag ImGuiWindowFlags_NavFlattened: the feature only eve
r made sense for BeginChild() anyhow.
                            - old: BeginChild("Name", size, 0, ImGuiWindowFlags_
NavFlattened);
                            - new: BeginChild("Name", size, ImGuiChildFlags_NavF
lattened, 0);
 - 2024/06/21 (1.90.9) - io: ClearInputKeys() (first exposed in 1.89.8) doesn't
clear mouse data, newly added ClearInputMouse() does.
 - 2024/06/20 (1.90.9) - renamed ImGuiDragDropFlags_SourceAutoExpirePayload to I
mGuiDragDropFlags_PayloadAutoExpire.
 - 2024/06/18 (1.90.9) - style: renamed ImGuiCol_TabActive -> ImGuiCol_TabSelect
ed, ImGuiCol_TabUnfocused -> ImGuiCol_TabDimmed, ImGuiCol_TabUnfocusedActive ->
ImGuiCol_TabDimmedSelected.
 - 2024/06/10 (1.90.9) - removed old nested structure: renaming ImGuiStorage::Im
GuiStoragePair type to ImGuiStoragePair (simpler for many languages).
 - 2024/06/06 (1.90.8) - reordered ImGuiInputTextFlags values. This should not b
e breaking unless you are using generated headers that have values not matching
the main library.
 - 2024/06/06 (1.90.8) - removed 'ImGuiButtonFlags_MouseButtonDefault_ = ImGuiBu
ttonFlags_MouseButtonLeft', was mostly unused and misleading.
 - 2024/05/27 (1.90.7) - commented out obsolete symbols marked obsolete in 1.88
(May 2022):
                            - old: CaptureKeyboardFromApp(bool)
                            - new: SetNextFrameWantCaptureKeyboard(bool)
                            - old: CaptureMouseFromApp(bool)
                            - new: SetNextFrameWantCaptureMouse(bool)
 - 2024/05/22 (1.90.7) - inputs (internals): renamed ImGuiKeyOwner_None to ImGui
KeyOwner_NoOwner, to make use more explicit and reduce confusion with the defaul
t it is a non-zero value and cannot be the default value (never made public, but
 disclosing as I expect a few users caught on owner-aware inputs).
                       - inputs (internals): renamed ImGuiInputFlags_RouteGlobal
Low -> ImGuiInputFlags_RouteGlobal, ImGuiInputFlags_RouteGlobal -> ImGuiInputFla
gs_RouteGlobalOverFocused, ImGuiInputFlags_RouteGlobalHigh -> ImGuiInputFlags_Ro
uteGlobalHighest.
                       - inputs (internals): Shortcut(), SetShortcutRouting(): s
wapped last two parameters order in function signatures:
                            - old: Shortcut(ImGuiKeyChord key_chord, ImGuiID own
er_id = 0, ImGuiInputFlags flags = 0);
                            - new: Shortcut(ImGuiKeyChord key_chord, ImGuiInputF
lags flags = 0, ImGuiID owner_id = 0);
                       - inputs (internals): owner-aware versions of IsKeyPresse
d(), IsKeyChordPressed(), IsMouseClicked(): swapped last two parameters order in
 function signatures.
                            - old: IsKeyPressed(ImGuiKey key, ImGuiID owner_id,
ImGuiInputFlags flags = 0);
                            - new: IsKeyPressed(ImGuiKey key, ImGuiInputFlags fl
ags, ImGuiID owner_id = 0);
                            - old: IsMouseClicked(ImGuiMouseButton button, ImGui
ID owner_id, ImGuiInputFlags flags = 0);
                            - new: IsMouseClicked(ImGuiMouseButton button, ImGui
InputFlags flags, ImGuiID owner_id = 0);
                         for various reasons those changes makes sense. They are
 being made because making some of those API public.
                         only past users of imgui_internal.h with the extra para
meters will be affected. Added asserts for valid flags in various functions to d
etect _some_ misuses, BUT NOT ALL.
 - 2024/05/16 (1.90.7) - inputs: on macOS X, Cmd and Ctrl keys are now automatic
ally swapped by io.AddKeyEvent() as this naturally align with how macOS X uses t
hose keys.
                           - it shouldn't really affect you unless you had custo
m shortcut swapping in place for macOS X apps.
                           - removed ImGuiMod_Shortcut which was previously dyna
mically remapping to Ctrl or Cmd/Super. It is now unnecessary to specific cross-
platform idiomatic shortcuts. (#2343, #4084, #5923, #456)
 - 2024/05/14 (1.90.7) - backends: SDL_Renderer2 and SDL_Renderer3 backend now t
ake a SDL_Renderer* in their RenderDrawData() functions.
 - 2024/04/18 (1.90.6) - TreeNode: Fixed a layout inconsistency when using an em
pty/hidden label followed by a SameLine() call. (#7505, #282)
                           - old: TreeNode("##Hidden"); SameLine(); Text("Hello"
);     // <-- This was actually incorrect! BUT appeared to look ok with the defa
ult style where ItemSpacing.x == FramePadding.x * 2 (it didn't look aligned othe
rwise).
                           - new: TreeNode("##Hidden"); SameLine(0, 0); Text("He
llo"); // <-- This is correct for all styles values.
                         with the fix, IF you were successfully using TreeNode("
")+SameLine(); you will now have extra spacing between your TreeNode and the fol
lowing item.
                         You'll need to change the SameLine() call to SameLine(0
,0) to remove this extraneous spacing. This seemed like the more sensible fix th
at's not making things less consistent.
                         (Note: when using this idiom you are likely to also use
 ImGuiTreeNodeFlags_SpanAvailWidth).
 - 2024/03/18 (1.90.5) - merged the radius_x/radius_y parameters in ImDrawList::
AddEllipse(), AddEllipseFilled() and PathEllipticalArcTo() into a single ImVec2
parameter. Exceptionally, because those functions were added in 1.90, we are not
 adding inline redirection functions. The transition is easy and should affect f
ew users. (#2743, #7417)
 - 2024/03/08 (1.90.5) - inputs: more formally obsoleted GetKeyIndex() when IMGU
I_DISABLE_OBSOLETE_FUNCTIONS is set. It has been unnecessary and a no-op since 1
.87 (it returns the same value as passed when used with a 1.87+ backend using io
.AddKeyEvent() function). (#4921)
                           - IsKeyPressed(GetKeyIndex(ImGuiKey_XXX)) -> use IsKe
yPressed(ImGuiKey_XXX)
 - 2024/01/15 (1.90.2) - commented out obsolete ImGuiIO::ImeWindowHandle marked
obsolete in 1.87, favor of writing to 'void* ImGuiViewport::PlatformHandleRaw'.
 - 2023/12/19 (1.90.1) - commented out obsolete ImGuiKey_KeyPadEnter redirection
 to ImGuiKey_KeypadEnter.
 - 2023/11/06 (1.90.1) - removed CalcListClipping() marked obsolete in 1.86. Pre
fer using ImGuiListClipper which can return non-contiguous ranges.
 - 2023/11/05 (1.90.1) - imgui_freetype: commented out ImGuiFreeType::BuildFontA
tlas() obsoleted in 1.81. prefer using #define IMGUI_ENABLE_FREETYPE or see comm
ented code for manual calls.
 - 2023/11/05 (1.90.1) - internals,columns: commented out legacy ImGuiColumnsFla
gs_XXX symbols redirecting to ImGuiOldColumnsFlags_XXX, obsoleted from imgui_int
ernal.h in 1.80.
 - 2023/11/09 (1.90.0) - removed IM_OFFSETOF() macro in favor of using offsetof(
) available in C++11. Kept redirection define (will obsolete).
 - 2023/11/07 (1.90.0) - removed BeginChildFrame()/EndChildFrame() in favor of u
sing BeginChild() with the ImGuiChildFlags_FrameStyle flag. kept inline redirect
ion function (will obsolete).
                         those functions were merely PushStyle/PopStyle helpers,
 the removal isn't so much motivated by needing to add the feature in BeginChild
(), but by the necessity to avoid BeginChildFrame() signature mismatching BeginC
hild() signature and features.
 - 2023/11/02 (1.90.0) - BeginChild: upgraded 'bool border = true' parameter to
'ImGuiChildFlags flags' type, added ImGuiChildFlags_Border equivalent. As with o
ur prior "bool-to-flags" API updates, the ImGuiChildFlags_Border value is guaran
teed to be == true forever to ensure a smoother transition, meaning all existing
 calls will still work.
                           - old: BeginChild("Name", size, true)
                           - new: BeginChild("Name", size, ImGuiChildFlags_Borde
r)
                           - old: BeginChild("Name", size, false)
                           - new: BeginChild("Name", size) or BeginChild("Name",
 0) or BeginChild("Name", size, ImGuiChildFlags_None)
                         **AMEND FROM THE FUTURE: from 1.91.1, 'ImGuiChildFlags_
Border' is called 'ImGuiChildFlags_Borders'**
 - 2023/11/02 (1.90.0) - BeginChild: added child-flag ImGuiChildFlags_AlwaysUseW
indowPadding as a replacement for the window-flag ImGuiWindowFlags_AlwaysUseWind
owPadding: the feature only ever made sense for BeginChild() anyhow.
                           - old: BeginChild("Name", size, 0, ImGuiWindowFlags_A
lwaysUseWindowPadding);
                           - new: BeginChild("Name", size, ImGuiChildFlags_Alway
sUseWindowPadding, 0);
 - 2023/09/27 (1.90.0) - io: removed io.MetricsActiveAllocations introduced in 1
.63. Same as 'g.DebugMemAllocCount - g.DebugMemFreeCount' (still displayed in Me
trics, unlikely to be accessed by end-user).
 - 2023/09/26 (1.90.0) - debug tools: Renamed ShowStackToolWindow() ("Stack Tool
") to ShowIDStackToolWindow() ("ID Stack Tool"), as earlier name was misleading.
 Kept inline redirection function. (#4631)
 - 2023/09/15 (1.90.0) - ListBox, Combo: changed signature of "name getter" call
back in old one-liner ListBox()/Combo() apis. kept inline redirection function (
will obsolete).
                           - old: bool Combo(const char* label, int* current_ite
m, bool (*getter)(void* user_data, int idx, const char** out_text), ...)
                           - new: bool Combo(const char* label, int* current_ite
m, const char* (*getter)(void* user_data, int idx), ...);
                           - old: bool ListBox(const char* label, int* current_i
tem, bool (*getting)(void* user_data, int idx, const char** out_text), ...);
                           - new: bool ListBox(const char* label, int* current_i
tem, const char* (*getter)(void* user_data, int idx), ...);
 - 2023/09/08 (1.90.0) - commented out obsolete redirecting functions:
                           - GetWindowContentRegionWidth()  -> use GetWindowCont
entRegionMax().x - GetWindowContentRegionMin().x. Consider that generally 'GetCo
ntentRegionAvail().x' is more useful.
                           - ImDrawCornerFlags_XXX          -> use ImDrawFlags_R
oundCornersXXX flags. Read 1.82 Changelog for details + grep commented names in
sources.
                       - commented out runtime support for hardcoded ~0 or 0x01.
.0x0F rounding flags values for AddRect()/AddRectFilled()/PathRect()/AddImageRou
nded() -> use ImDrawFlags_RoundCornersXXX flags. Read 1.82 Changelog for details
 - 2023/08/25 (1.89.9) - clipper: Renamed IncludeRangeByIndices() (also called F
orceDisplayRangeByIndices() before 1.89.6) to IncludeItemsByIndex(). Kept inline
 redirection function. Sorry!
 - 2023/07/12 (1.89.8) - ImDrawData: CmdLists now owned, changed from ImDrawList
** to ImVector<ImDrawList*>. Majority of users shouldn't be affected, but you ca
nnot compare to NULL nor reassign manually anymore. Instead use AddDrawList(). (
#6406, #4879, #1878)
 - 2023/06/28 (1.89.7) - overlapping items: obsoleted 'SetItemAllowOverlap()' (c
alled after item) in favor of calling 'SetNextItemAllowOverlap()' (called before
 item). 'SetItemAllowOverlap()' didn't and couldn't work reliably since 1.89 (20
22-11-15).
 - 2023/06/28 (1.89.7) - overlapping items: renamed 'ImGuiTreeNodeFlags_AllowIte
mOverlap' to 'ImGuiTreeNodeFlags_AllowOverlap', 'ImGuiSelectableFlags_AllowItemO
verlap' to 'ImGuiSelectableFlags_AllowOverlap'. Kept redirecting enums (will obs
olete).
 - 2023/06/28 (1.89.7) - overlapping items: IsItemHovered() now by default retur
n false when querying an item using AllowOverlap mode which is being overlapped.
 Use ImGuiHoveredFlags_AllowWhenOverlappedByItem to revert to old behavior.
 - 2023/06/28 (1.89.7) - overlapping items: Selectable and TreeNode don't allow
overlap when active so overlapping widgets won't appear as hovered. While this f
ixes a common small visual issue, it also means that calling IsItemHovered() aft
er a non-reactive elements - e.g. Text() - overlapping an active one may fail if
 you don't use IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem). (#
6610)
 - 2023/06/20 (1.89.7) - moved io.HoverDelayShort/io.HoverDelayNormal to style.H
overDelayShort/style.HoverDelayNormal. As the fields were added in 1.89 and expe
cted to be left unchanged by most users, or only tweaked once during app initial
ization, we are exceptionally accepting the breakage.
 - 2023/05/30 (1.89.6) - backends: renamed "imgui_impl_sdlrenderer.cpp" to "imgu
i_impl_sdlrenderer2.cpp" and "imgui_impl_sdlrenderer.h" to "imgui_impl_sdlrender
er2.h". This is in prevision for the future release of SDL3.
 - 2023/05/22 (1.89.6) - listbox: commented out obsolete/redirecting functions t
hat were marked obsolete more than two years ago:
                           - ListBoxHeader()  -> use BeginListBox() (note how tw
o variants of ListBoxHeader() existed. Check commented versions in imgui.h for r
eference)
                           - ListBoxFooter()  -> use EndListBox()
 - 2023/05/15 (1.89.6) - clipper: commented out obsolete redirection constructor
 'ImGuiListClipper(int items_count, float items_height = -1.0f)' that was marked
 obsolete in 1.79. Use default constructor + clipper.Begin().
 - 2023/05/15 (1.89.6) - clipper: renamed ImGuiListClipper::ForceDisplayRangeByI
ndices() to ImGuiListClipper::IncludeRangeByIndices().
 - 2023/03/14 (1.89.4) - commented out redirecting enums/functions names that we
re marked obsolete two years ago:
                           - ImGuiSliderFlags_ClampOnInput        -> use ImGuiSl
iderFlags_AlwaysClamp
                           - ImGuiInputTextFlags_AlwaysInsertMode -> use ImGuiIn
putTextFlags_AlwaysOverwrite
                           - ImDrawList::AddBezierCurve()         -> use ImDrawL
ist::AddBezierCubic()
                           - ImDrawList::PathBezierCurveTo()      -> use ImDrawL
ist::PathBezierCubicCurveTo()
 - 2023/03/09 (1.89.4) - renamed PushAllowKeyboardFocus()/PopAllowKeyboardFocus(
) to PushTabStop()/PopTabStop(). Kept inline redirection functions (will obsolet
e).
 - 2023/03/09 (1.89.4) - tooltips: Added 'bool' return value to BeginTooltip() f
or API consistency. Please only submit contents and call EndTooltip() if BeginTo
oltip() returns true. In reality the function will _currently_ always return tru
e, but further changes down the line may change this, best to clarify API sooner
.
 - 2023/02/15 (1.89.4) - moved the optional "courtesy maths operators" implement
ation from imgui_internal.h in imgui.h.
                         Even though we encourage using your own maths types and
 operators by setting up IM_VEC2_CLASS_EXTRA,
                         it has been frequently requested by people to use our o
wn. We had an opt-in define which was
                         previously fulfilled in imgui_internal.h. It is now ful
filled in imgui.h. (#6164)
                           - OK:     #define IMGUI_DEFINE_MATH_OPERATORS / #incl
ude "imgui.h" / #include "imgui_internal.h"
                           - Error:  #include "imgui.h" / #define IMGUI_DEFINE_M
ATH_OPERATORS / #include "imgui_internal.h"
 - 2023/02/07 (1.89.3) - backends: renamed "imgui_impl_sdl.cpp" to "imgui_impl_s
dl2.cpp" and "imgui_impl_sdl.h" to "imgui_impl_sdl2.h". (#6146) This is in previ
sion for the future release of SDL3.
 - 2022/10/26 (1.89)   - commented out redirecting OpenPopupContextItem() which
was briefly the name of OpenPopupOnItemClick() from 1.77 to 1.79.
 - 2022/10/12 (1.89)   - removed runtime patching of invalid "%f"/"%0.f" format
strings for DragInt()/SliderInt(). This was obsoleted in 1.61 (May 2018). See 1.
61 changelog for details.
 - 2022/09/26 (1.89)   - renamed and merged keyboard modifiers key enums and fla
gs into a same set. Kept inline redirection enums (will obsolete).
                           - ImGuiKey_ModCtrl  and ImGuiModFlags_Ctrl  -> ImGuiM
od_Ctrl
                           - ImGuiKey_ModShift and ImGuiModFlags_Shift -> ImGuiM
od_Shift
                           - ImGuiKey_ModAlt   and ImGuiModFlags_Alt   -> ImGuiM
od_Alt
                           - ImGuiKey_ModSuper and ImGuiModFlags_Super -> ImGuiM
od_Super
                         the ImGuiKey_ModXXX were introduced in 1.87 and mostly
used by backends.
                         the ImGuiModFlags_XXX have been exposed in imgui.h but
not really used by any public api only by third-party extensions.
                         exceptionally commenting out the older ImGuiKeyModFlags
_XXX names ahead of obsolescence schedule to reduce confusion and because they w
ere not meant to be used anyway.
 - 2022/09/20 (1.89)   - ImGuiKey is now a typed enum, allowing ImGuiKey_XXX sym
bols to be named in debuggers.
                         this will require uses of legacy backend-dependent indi
ces to be casted, e.g.
                            - with imgui_impl_glfw:  IsKeyPressed(GLFW_KEY_A) ->
 IsKeyPressed((ImGuiKey)GLFW_KEY_A);
                            - with imgui_impl_win32: IsKeyPressed('A')        ->
 IsKeyPressed((ImGuiKey)'A')
                            - etc. However if you are upgrading code you might w
ell use the better, backend-agnostic IsKeyPressed(ImGuiKey_A) now!
 - 2022/09/12 (1.89) - removed the bizarre legacy default argument for 'TreePush
(const void* ptr = NULL)', always pass a pointer value explicitly. NULL/nullptr
is ok but require cast, e.g. TreePush((void*)nullptr);
 - 2022/09/05 (1.89) - commented out redirecting functions/enums names that were
 marked obsolete in 1.77 and 1.78 (June 2020):
                         - DragScalar(), DragScalarN(), DragFloat(), DragFloat2(
), DragFloat3(), DragFloat4(): For old signatures ending with (..., const char*
format, float power = 1.0f) -> use (..., format ImGuiSliderFlags_Logarithmic) if
 power != 1.0f.
                         - SliderScalar(), SliderScalarN(), SliderFloat(), Slide
rFloat2(), SliderFloat3(), SliderFloat4(): For old signatures ending with (...,
const char* format, float power = 1.0f) -> use (..., format ImGuiSliderFlags_Log
arithmic) if power != 1.0f.
                         - BeginPopupContextWindow(const char*, ImGuiMouseButton
, bool) -> use BeginPopupContextWindow(const char*, ImGuiPopupFlags)
 - 2022/09/02 (1.89) - obsoleted using SetCursorPos()/SetCursorScreenPos() to ex
tend parent window/cell boundaries.
                       this relates to when moving the cursor position beyond cu
rrent boundaries WITHOUT submitting an item.
                         - previously this would make the window content size ~2
00x200:
                              Begin(...) + SetCursorScreenPos(GetCursorScreenPos
() + ImVec2(200,200)) + End();
                         - instead, please submit an item:
                              Begin(...) + SetCursorScreenPos(GetCursorScreenPos
() + ImVec2(200,200)) + Dummy(ImVec2(0,0)) + End();
                         - alternative:
                              Begin(...) + Dummy(ImVec2(200,200)) + End();
                         - content size is now only extended when submitting an
item!
                         - with '#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS' this
will now be detected and assert.
                         - without '#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS' th
is will silently be fixed until we obsolete it.
 - 2022/08/03 (1.89) - changed signature of ImageButton() function. Kept redirec
tion function (will obsolete).
                        - added 'const char* str_id' parameter + removed 'int fr
ame_padding = -1' parameter.
                        - old signature: bool ImageButton(ImTextureID tex_id, Im
Vec2 size, ImVec2 uv0 = ImVec2(0,0), ImVec2 uv1 = ImVec2(1,1), int frame_padding
 = -1, ImVec4 bg_col = ImVec4(0,0,0,0), ImVec4 tint_col = ImVec4(1,1,1,1));
                          - used the ImTextureID value to create an ID. This was
 inconsistent with other functions, led to ID conflicts, and caused problems wit
h engines using transient ImTextureID values.
                          - had a FramePadding override which was inconsistent w
ith other functions and made the already-long signature even longer.
                        - new signature: bool ImageButton(const char* str_id, Im
TextureID tex_id, ImVec2 size, ImVec2 uv0 = ImVec2(0,0), ImVec2 uv1 = ImVec2(1,1
), ImVec4 bg_col = ImVec4(0,0,0,0), ImVec4 tint_col = ImVec4(1,1,1,1));
                          - requires an explicit identifier. You may still use e
.g. PushID() calls and then pass an empty identifier.
                          - always uses style.FramePadding for padding, to be co
nsistent with other buttons. You may use PushStyleVar() to alter this.
 - 2022/07/08 (1.89) - inputs: removed io.NavInputs[] and ImGuiNavInput enum (fo
llowing 1.87 changes).
                        - Official backends from 1.87+                  -> no is
sue.
                        - Official backends from 1.60 to 1.86           -> will
build and convert gamepad inputs, unless IMGUI_DISABLE_OBSOLETE_KEYIO is defined
. Need updating!
                        - Custom backends not writing to io.NavInputs[] -> no is
sue.
                        - Custom backends writing to io.NavInputs[]     -> will
build and convert gamepad inputs, unless IMGUI_DISABLE_OBSOLETE_KEYIO is defined
. Need fixing!
                        - TL;DR: Backends should call io.AddKeyEvent()/io.AddKey
AnalogEvent() with ImGuiKey_GamepadXXX values instead of filling io.NavInput[].
 - 2022/06/15 (1.88) - renamed IMGUI_DISABLE_METRICS_WINDOW to IMGUI_DISABLE_DEB
UG_TOOLS for correctness. kept support for old define (will obsolete).
 - 2022/05/03 (1.88) - backends: osx: removed ImGui_ImplOSX_HandleEvent() from b
ackend API in favor of backend automatically handling event capture. All ImGui_I
mplOSX_HandleEvent() calls should be removed as they are now unnecessary.
 - 2022/04/05 (1.88) - inputs: renamed ImGuiKeyModFlags to ImGuiModFlags. Kept i
nline redirection enums (will obsolete). This was never used in public API funct
ions but technically present in imgui.h and ImGuiIO.
 - 2022/01/20 (1.87) - inputs: reworded gamepad IO.
                        - Backend writing to io.NavInputs[]            -> backen
d should call io.AddKeyEvent()/io.AddKeyAnalogEvent() with ImGuiKey_GamepadXXX v
alues.
 - 2022/01/19 (1.87) - sliders, drags: removed support for legacy arithmetic ope
rators (+,+-,*,/) when inputting text. This doesn't break any api/code but a fea
ture that used to be accessible by end-users (which seemingly no one used).
 - 2022/01/17 (1.87) - inputs: reworked mouse IO.
                        - Backend writing to io.MousePos               -> backen
d should call io.AddMousePosEvent()
                        - Backend writing to io.MouseDown[]            -> backen
d should call io.AddMouseButtonEvent()
                        - Backend writing to io.MouseWheel             -> backen
d should call io.AddMouseWheelEvent()
                        - Backend writing to io.MouseHoveredViewport   -> backen
d should call io.AddMouseViewportEvent() [Docking branch w/ multi-viewports only
]
                       note: for all calls to IO new functions, the Dear ImGui c
ontext should be bound/current.
                       read https://github.com/ocornut/imgui/issues/4921 for det
ails.
 - 2022/01/10 (1.87) - inputs: reworked keyboard IO. Removed io.KeyMap[], io.Key
sDown[] in favor of calling io.AddKeyEvent(), ImGui::IsKeyDown(). Removed GetKey
Index(), now unnecessary. All IsKeyXXX() functions now take ImGuiKey values. All
 features are still functional until IMGUI_DISABLE_OBSOLETE_KEYIO is defined. Re
ad Changelog and Release Notes for details.
                        - IsKeyPressed(MY_NATIVE_KEY_XXX)              -> use Is
KeyPressed(ImGuiKey_XXX)
                        - IsKeyPressed(GetKeyIndex(ImGuiKey_XXX))      -> use Is
KeyPressed(ImGuiKey_XXX)
                        - Backend writing to io.KeyMap[],io.KeysDown[] -> backen
d should call io.AddKeyEvent() (+ call io.SetKeyEventNativeData() if you want le
gacy user code to still function with legacy key codes).
                        - Backend writing to io.KeyCtrl, io.KeyShift.. -> backen
d should call io.AddKeyEvent() with ImGuiMod_XXX values. *IF YOU PULLED CODE BET
WEEN 2021/01/10 and 2021/01/27: We used to have a io.AddKeyModsEvent() function
which was now replaced by io.AddKeyEvent() with ImGuiMod_XXX values.*
                     - one case won't work with backward compatibility: if your
custom backend used ImGuiKey as mock native indices (e.g. "io.KeyMap[ImGuiKey_A]
 = ImGuiKey_A") because those values are now larger than the legacy KeyDown[] ar
ray. Will assert.
                     - inputs: added ImGuiKey_ModCtrl/ImGuiKey_ModShift/ImGuiKey
_ModAlt/ImGuiKey_ModSuper values to submit keyboard modifiers using io.AddKeyEve
nt(), instead of writing directly to io.KeyCtrl, io.KeyShift, io.KeyAlt, io.KeyS
uper.
 - 2022/01/05 (1.87) - inputs: renamed ImGuiKey_KeyPadEnter to ImGuiKey_KeypadEn
ter to align with new symbols. Kept redirection enum.
 - 2022/01/05 (1.87) - removed io.ImeSetInputScreenPosFn() in favor of more flex
ible io.SetPlatformImeDataFn(). Removed 'void* io.ImeWindowHandle' in favor of w
riting to 'void* ImGuiViewport::PlatformHandleRaw'.
 - 2022/01/01 (1.87) - commented out redirecting functions/enums names that were
 marked obsolete in 1.69, 1.70, 1.71, 1.72 (March-July 2019)
                        - ImGui::SetNextTreeNodeOpen()        -> use ImGui::SetN
extItemOpen()
                        - ImGui::GetContentRegionAvailWidth() -> use ImGui::GetC
ontentRegionAvail().x
                        - ImGui::TreeAdvanceToLabelPos()      -> use ImGui::SetC
ursorPosX(ImGui::GetCursorPosX() + ImGui::GetTreeNodeToLabelSpacing());
                        - ImFontAtlas::CustomRect             -> use ImFontAtlas
CustomRect
                        - ImGuiColorEditFlags_RGB/HSV/HEX     -> use ImGuiColorE
ditFlags_DisplayRGB/HSV/Hex
 - 2021/12/20 (1.86) - backends: removed obsolete Marmalade backend (imgui_impl_
marmalade.cpp) + example. Find last supported version at https://github.com/ocor
nut/imgui/wiki/Bindings
 - 2021/11/04 (1.86) - removed CalcListClipping() function. Prefer using ImGuiLi
stClipper which can return non-contiguous ranges. Please open an issue if you th
ink you really need this function.
 - 2021/08/23 (1.85) - removed GetWindowContentRegionWidth() function. keep inli
ne redirection helper. can use 'GetWindowContentRegionMax().x - GetWindowContent
RegionMin().x' instead for generally 'GetContentRegionAvail().x' is more useful.
 - 2021/07/26 (1.84) - commented out redirecting functions/enums names that were
 marked obsolete in 1.67 and 1.69 (March 2019):
                        - ImGui::GetOverlayDrawList() -> use ImGui::GetForegroun
dDrawList()
                        - ImFont::GlyphRangesBuilder  -> use ImFontGlyphRangesBu
ilder
 - 2021/05/19 (1.83) - backends: obsoleted direct access to ImDrawCmd::TextureId
 in favor of calling ImDrawCmd::GetTexID().
                        - if you are using official backends from the source tre
e: you have nothing to do.
                        - if you have copied old backend code or using your own:
 change access to draw_cmd->TextureId to draw_cmd->GetTexID().
 - 2021/03/12 (1.82) - upgraded ImDrawList::AddRect(), AddRectFilled(), PathRect
() to use ImDrawFlags instead of ImDrawCornersFlags.
                        - ImDrawCornerFlags_TopLeft  -> use ImDrawFlags_RoundCor
nersTopLeft
                        - ImDrawCornerFlags_BotRight -> use ImDrawFlags_RoundCor
nersBottomRight
                        - ImDrawCornerFlags_None     -> use ImDrawFlags_RoundCor
nersNone etc.
                       flags now sanely defaults to 0 instead of 0x0F, consisten
t with all other flags in the API.
                       breaking: the default with rounding > 0.0f is now "round
all corners" vs old implicit "round no corners":
                        - rounding == 0.0f + flags == 0 --> meant no rounding  -
-> unchanged (common use)
                        - rounding  > 0.0f + flags != 0 --> meant rounding     -
-> unchanged (common use)
                        - rounding == 0.0f + flags != 0 --> meant no rounding  -
-> unchanged (unlikely use)
                        - rounding  > 0.0f + flags == 0 --> meant no rounding  -
-> BREAKING (unlikely use): will now round all corners --> use ImDrawFlags_Round
CornersNone or rounding == 0.0f.
                       this ONLY matters for hard coded use of 0 + rounding > 0.
0f. Use of named ImDrawFlags_RoundCornersNone (new) or ImDrawCornerFlags_None (o
ld) are ok.
                       the old ImDrawCornersFlags used awkward default values of
 ~0 or 0xF (4 lower bits set) to signify "round all corners" and we sometimes en
couraged using them as shortcuts.
                       legacy path still support use of hard coded ~0 or any val
ue from 0x1 or 0xF. They will behave the same with legacy paths enabled (will as
sert otherwise).
 - 2021/03/11 (1.82) - removed redirecting functions/enums names that were marke
d obsolete in 1.66 (September 2018):
                        - ImGui::SetScrollHere()              -> use ImGui::SetS
crollHereY()
 - 2021/03/11 (1.82) - clarified that ImDrawList::PathArcTo(), ImDrawList::PathA
rcToFast() won't render with radius < 0.0f. Previously it sorts of accidentally
worked but would generally lead to counter-clockwise paths and have an effect on
 anti-aliasing.
 - 2021/03/10 (1.82) - upgraded ImDrawList::AddPolyline() and PathStroke() "bool
 closed" parameter to "ImDrawFlags flags". The matching ImDrawFlags_Closed value
 is guaranteed to always stay == 1 in the future.
 - 2021/02/22 (1.82) - (*undone in 1.84*) win32+mingw: Re-enabled IME functions
by default even under MinGW. In July 2016, issue #738 had me incorrectly disable
 those default functions for MinGW. MinGW users should: either link with -limm32
, either set their imconfig file  with '#define IMGUI_DISABLE_WIN32_DEFAULT_IME_
FUNCTIONS'.
 - 2021/02/17 (1.82) - renamed rarely used style.CircleSegmentMaxError (old defa
ult = 1.60f) to style.CircleTessellationMaxError (new default = 0.30f) as the me
aning of the value changed.
 - 2021/02/03 (1.81) - renamed ListBoxHeader(const char* label, ImVec2 size) to
BeginListBox(). Kept inline redirection function (will obsolete).
                     - removed ListBoxHeader(const char* label, int items_count,
 int height_in_items = -1) in favor of specifying size. Kept inline redirection
function (will obsolete).
                     - renamed ListBoxFooter() to EndListBox(). Kept inline redi
rection function (will obsolete).
 - 2021/01/26 (1.81) - removed ImGuiFreeType::BuildFontAtlas(). Kept inline redi
rection function. Prefer using '#define IMGUI_ENABLE_FREETYPE', but there's a ru
ntime selection path available too. The shared extra flags parameters (very rare
ly used) are now stored in ImFontAtlas::FontBuilderFlags.
                     - renamed ImFontConfig::RasterizerFlags (used by FreeType)
to ImFontConfig::FontBuilderFlags.
                     - renamed ImGuiFreeType::XXX flags to ImGuiFreeTypeBuilderF
lags_XXX for consistency with other API.
 - 2020/10/12 (1.80) - removed redirecting functions/enums that were marked obso
lete in 1.63 (August 2018):
                        - ImGui::IsItemDeactivatedAfterChange() -> use ImGui::Is
ItemDeactivatedAfterEdit().
                        - ImGuiCol_ModalWindowDarkening       -> use ImGuiCol_Mo
dalWindowDimBg
                        - ImGuiInputTextCallback              -> use ImGuiTextEd
itCallback
                        - ImGuiInputTextCallbackData          -> use ImGuiTextEd
itCallbackData
 - 2020/12/21 (1.80) - renamed ImDrawList::AddBezierCurve() to AddBezierCubic(),
 and PathBezierCurveTo() to PathBezierCubicCurveTo(). Kept inline redirection fu
nction (will obsolete).
 - 2020/12/04 (1.80) - added imgui_tables.cpp file! Manually constructed project
 files will need the new file added!
 - 2020/11/18 (1.80) - renamed undocumented/internals ImGuiColumnsFlags_* to ImG
uiOldColumnFlags_* in prevision of incoming Tables API.
 - 2020/11/03 (1.80) - renamed io.ConfigWindowsMemoryCompactTimer to io.ConfigMe
moryCompactTimer as the feature will apply to other data structures
 - 2020/10/14 (1.80) - backends: moved all backends files (imgui_impl_XXXX.cpp,
imgui_impl_XXXX.h) from examples/ to backends/.
 - 2020/10/12 (1.80) - removed redirecting functions/enums that were marked obso
lete in 1.60 (April 2018):
                        - io.RenderDrawListsFn pointer        -> use ImGui::GetD
rawData() value and call the render function of your backend
                        - ImGui::IsAnyWindowFocused()         -> use ImGui::IsWi
ndowFocused(ImGuiFocusedFlags_AnyWindow)
                        - ImGui::IsAnyWindowHovered()         -> use ImGui::IsWi
ndowHovered(ImGuiHoveredFlags_AnyWindow)
                        - ImGuiStyleVar_Count_                -> use ImGuiStyleV
ar_COUNT
                        - ImGuiMouseCursor_Count_             -> use ImGuiMouseC
ursor_COUNT
                      - removed redirecting functions names that were marked obs
olete in 1.61 (May 2018):
                        - InputFloat (... int decimal_precision ...) -> use Inpu
tFloat (... const char* format ...) with format = "%.Xf" where X is your value f
or decimal_precision.
                        - same for InputFloat2()/InputFloat3()/InputFloat4() var
iants taking a `int decimal_precision` parameter.
 - 2020/10/05 (1.79) - removed ImGuiListClipper: Renamed constructor parameters
which created an ambiguous alternative to using the ImGuiListClipper::Begin() fu
nction, with misleading edge cases (note: imgui_memory_editor <0.40 from imgui_c
lub/ used this old clipper API. Update your copy if needed).
 - 2020/09/25 (1.79) - renamed ImGuiSliderFlags_ClampOnInput to ImGuiSliderFlags
_AlwaysClamp. Kept redirection enum (will obsolete sooner because previous name
was added recently).
 - 2020/09/25 (1.79) - renamed style.TabMinWidthForUnselectedCloseButton to styl
e.TabMinWidthForCloseButton.
 - 2020/09/21 (1.79) - renamed OpenPopupContextItem() back to OpenPopupOnItemCli
ck(), reverting the change from 1.77. For varieties of reason this is more self-
explanatory.
 - 2020/09/21 (1.79) - removed return value from OpenPopupOnItemClick() - return
ed true on mouse release on an item - because it is inconsistent with other popu
p APIs and makes others misleading. It's also and unnecessary: you can use IsWin
dowAppearing() after BeginPopup() for a similar result.
 - 2020/09/17 (1.79) - removed ImFont::DisplayOffset in favor of ImFontConfig::G
lyphOffset. DisplayOffset was applied after scaling and not very meaningful/usef
ul outside of being needed by the default ProggyClean font. If you scaled this v
alue after calling AddFontDefault(), this is now done automatically. It was also
 getting in the way of better font scaling, so let's get rid of it now!
 - 2020/08/17 (1.78) - obsoleted use of the trailing 'float power=1.0f' paramete
r for DragFloat(), DragFloat2(), DragFloat3(), DragFloat4(), DragFloatRange2(),
DragScalar(), DragScalarN(), SliderFloat(), SliderFloat2(), SliderFloat3(), Slid
erFloat4(), SliderScalar(), SliderScalarN(), VSliderFloat() and VSliderScalar().
                       replaced the 'float power=1.0f' argument with integer-bas
ed flags defaulting to 0 (as with all our flags).
                       worked out a backward-compatibility scheme so hopefully m
ost C++ codebase should not be affected. in short, when calling those functions:
                       - if you omitted the 'power' parameter (likely!), you are
 not affected.
                       - if you set the 'power' parameter to 1.0f (same as previ
ous default value): 1/ your compiler may warn on float>int conversion, 2/ everyt
hing else will work. 3/ you can replace the 1.0f value with 0 to fix the warning
, and be technically correct.
                       - if you set the 'power' parameter to >1.0f (to enable no
n-linear editing): 1/ your compiler may warn on float>int conversion, 2/ code wi
ll assert at runtime, 3/ in case asserts are disabled, the code will not crash a
nd enable the _Logarithmic flag. 4/ you can replace the >1.0f value with ImGuiSl
iderFlags_Logarithmic to fix the warning/assert and get a _similar_ effect as pr
evious uses of power >1.0f.
                       see https://github.com/ocornut/imgui/issues/3361 for all
details.
                       kept inline redirection functions (will obsolete) apart f
or: DragFloatRange2(), VSliderFloat(), VSliderScalar(). For those three the 'flo
at power=1.0f' version was removed directly as they were most unlikely ever used
.
                       for shared code, you can version check at compile-time wi
th `#if IMGUI_VERSION_NUM >= 17704`.
                     - obsoleted use of v_min > v_max in DragInt, DragFloat, Dra
gScalar to lock edits (introduced in 1.73, was not demoed nor documented very),
will be replaced by a more generic ReadOnly feature. You may use the ImGuiSlider
Flags_ReadOnly internal flag in the meantime.
 - 2020/06/23 (1.77) - removed BeginPopupContextWindow(const char*, int mouse_bu
tton, bool also_over_items) in favor of BeginPopupContextWindow(const char*, ImG
uiPopupFlags flags) with ImGuiPopupFlags_NoOverItems.
 - 2020/06/15 (1.77) - renamed OpenPopupOnItemClick() to OpenPopupContextItem().
 Kept inline redirection function (will obsolete). [NOTE: THIS WAS REVERTED IN 1
.79]
 - 2020/06/15 (1.77) - removed CalcItemRectClosestPoint() entry point which was
made obsolete and asserting in December 2017.
 - 2020/04/23 (1.77) - removed unnecessary ID (first arg) of ImFontAtlas::AddCus
tomRectRegular().
 - 2020/01/22 (1.75) - ImDrawList::AddCircle()/AddCircleFilled() functions don't
 accept negative radius any more.
 - 2019/12/17 (1.75) - [undid this change in 1.76] made Columns() limited to 64
columns by asserting above that limit. While the current code technically suppor
ts it, future code may not so we're putting the restriction ahead.
 - 2019/12/13 (1.75) - [imgui_internal.h] changed ImRect() default constructor i
nitializes all fields to 0.0f instead of (FLT_MAX,FLT_MAX,-FLT_MAX,-FLT_MAX). If
 you used ImRect::Add() to create bounding boxes by adding multiple points into
it, you may need to fix your initial value.
 - 2019/12/08 (1.75) - removed redirecting functions/enums that were marked obso
lete in 1.53 (December 2017):
                       - ShowTestWindow()                    -> use ShowDemoWind
ow()
                       - IsRootWindowFocused()               -> use IsWindowFocu
sed(ImGuiFocusedFlags_RootWindow)
                       - IsRootWindowOrAnyChildFocused()     -> use IsWindowFocu
sed(ImGuiFocusedFlags_RootAndChildWindows)
                       - SetNextWindowContentWidth(w)        -> use SetNextWindo
wContentSize(ImVec2(w, 0.0f))
                       - GetItemsLineHeightWithSpacing()     -> use GetFrameHeig
htWithSpacing()
                       - ImGuiCol_ChildWindowBg              -> use ImGuiCol_Chi
ldBg
                       - ImGuiStyleVar_ChildWindowRounding   -> use ImGuiStyleVa
r_ChildRounding
                       - ImGuiTreeNodeFlags_AllowOverlapMode -> use ImGuiTreeNod
eFlags_AllowItemOverlap
                       - IMGUI_DISABLE_TEST_WINDOWS          -> use IMGUI_DISABL
E_DEMO_WINDOWS
 - 2019/12/08 (1.75) - obsoleted calling ImDrawList::PrimReserve() with a negati
ve count (which was vaguely documented and rarely if ever used). Instead, we add
ed an explicit PrimUnreserve() API.
 - 2019/12/06 (1.75) - removed implicit default parameter to IsMouseDragging(int
 button = 0) to be consistent with other mouse functions (none of the other func
tions have it).
 - 2019/11/21 (1.74) - ImFontAtlas::AddCustomRectRegular() now requires an ID la
rger than 0x110000 (instead of 0x10000) to conform with supporting Unicode plane
s 1-16 in a future update. ID below 0x110000 will now assert.
 - 2019/11/19 (1.74) - renamed IMGUI_DISABLE_FORMAT_STRING_FUNCTIONS to IMGUI_DI
SABLE_DEFAULT_FORMAT_FUNCTIONS for consistency.
 - 2019/11/19 (1.74) - renamed IMGUI_DISABLE_MATH_FUNCTIONS to IMGUI_DISABLE_DEF
AULT_MATH_FUNCTIONS for consistency.
 - 2019/10/22 (1.74) - removed redirecting functions/enums that were marked obso
lete in 1.52 (October 2017):
                       - Begin() [old 5 args version]        -> use Begin() [3 a
rgs], use SetNextWindowSize() SetNextWindowBgAlpha() if needed
                       - IsRootWindowOrAnyChildHovered()     -> use IsWindowHove
red(ImGuiHoveredFlags_RootAndChildWindows)
                       - AlignFirstTextHeightToWidgets()     -> use AlignTextToF
ramePadding()
                       - SetNextWindowPosCenter()            -> use SetNextWindo
wPos() with a pivot of (0.5f, 0.5f)
                       - ImFont::Glyph                       -> use ImFontGlyph
 - 2019/10/14 (1.74) - inputs: Fixed a miscalculation in the keyboard/mouse "typ
ematic" repeat delay/rate calculation, used by keys and e.g. repeating mouse but
tons as well as the GetKeyPressedAmount() function.
                       if you were using a non-default value for io.KeyRepeatRat
e (previous default was 0.250), you can add +io.KeyRepeatDelay to it to compensa
te for the fix.
                       The function was triggering on: 0.0 and (delay+rate*N) wh
ere (N>=1). Fixed formula responds to (N>=0). Effectively it made io.KeyRepeatRa
te behave like it was set to (io.KeyRepeatRate + io.KeyRepeatDelay).
                       If you never altered io.KeyRepeatRate nor used GetKeyPres
sedAmount() this won't affect you.
 - 2019/07/15 (1.72) - removed TreeAdvanceToLabelPos() which is rarely used and
only does SetCursorPosX(GetCursorPosX() + GetTreeNodeToLabelSpacing()). Kept red
irection function (will obsolete).
 - 2019/07/12 (1.72) - renamed ImFontAtlas::CustomRect to ImFontAtlasCustomRect.
 Kept redirection typedef (will obsolete).
 - 2019/06/14 (1.72) - removed redirecting functions/enums names that were marke
d obsolete in 1.51 (June 2017): ImGuiCol_Column*, ImGuiSetCond_*, IsItemHoveredR
ect(), IsPosHoveringAnyWindow(), IsMouseHoveringAnyWindow(), IsMouseHoveringWind
ow(), IMGUI_ONCE_UPON_A_FRAME. Grep this log for details and new names, or see h
ow they were implemented until 1.71.
 - 2019/06/07 (1.71) - rendering of child window outer decorations (bg color, bo
rder, scrollbars) is now performed as part of the parent window. If you have
                       overlapping child windows in a same parent, and relied on
 their relative z-order to be mapped to their submission order, this will affect
 your rendering.
                       This optimization is disabled if the parent window has no
 visual output, because it appears to be the most common situation leading to th
e creation of overlapping child windows.
                       Please reach out if you are affected.
 - 2019/05/13 (1.71) - renamed SetNextTreeNodeOpen() to SetNextItemOpen(). Kept
inline redirection function (will obsolete).
 - 2019/05/11 (1.71) - changed io.AddInputCharacter(unsigned short c) signature
to io.AddInputCharacter(unsigned int c).
 - 2019/04/29 (1.70) - improved ImDrawList thick strokes (>1.0f) preserving corr
ect thickness up to 90 degrees angles (e.g. rectangles). If you have custom rend
ering using thick lines, they will appear thicker now.
 - 2019/04/29 (1.70) - removed GetContentRegionAvailWidth(), use GetContentRegio
nAvail().x instead. Kept inline redirection function (will obsolete).
 - 2019/03/04 (1.69) - renamed GetOverlayDrawList() to GetForegroundDrawList().
Kept redirection function (will obsolete).
 - 2019/02/26 (1.69) - renamed ImGuiColorEditFlags_RGB/ImGuiColorEditFlags_HSV/I
mGuiColorEditFlags_HEX to ImGuiColorEditFlags_DisplayRGB/ImGuiColorEditFlags_Dis
playHSV/ImGuiColorEditFlags_DisplayHex. Kept redirection enums (will obsolete).
 - 2019/02/14 (1.68) - made it illegal/assert when io.DisplayTime == 0.0f (with
an exception for the first frame). If for some reason your time step calculation
 gives you a zero value, replace it with an arbitrarily small value!
 - 2019/02/01 (1.68) - removed io.DisplayVisibleMin/DisplayVisibleMax (which wer
e marked obsolete and removed from viewport/docking branch already).
 - 2019/01/06 (1.67) - renamed io.InputCharacters[], marked internal as was alwa
ys intended. Please don't access directly, and use AddInputCharacter() instead!
 - 2019/01/06 (1.67) - renamed ImFontAtlas::GlyphRangesBuilder to ImFontGlyphRan
gesBuilder. Kept redirection typedef (will obsolete).
 - 2018/12/20 (1.67) - made it illegal to call Begin("") with an empty string. T
his somehow half-worked before but had various undesirable side-effects.
 - 2018/12/10 (1.67) - renamed io.ConfigResizeWindowsFromEdges to io.ConfigWindo
wsResizeFromEdges as we are doing a large pass on configuration flags.
 - 2018/10/12 (1.66) - renamed misc/stl/imgui_stl.* to misc/cpp/imgui_stdlib.* i
n prevision for other C++ helper files.
 - 2018/09/28 (1.66) - renamed SetScrollHere() to SetScrollHereY(). Kept redirec
tion function (will obsolete).
 - 2018/09/06 (1.65) - renamed stb_truetype.h to imstb_truetype.h, stb_textedit.
h to imstb_textedit.h, and stb_rect_pack.h to imstb_rectpack.h.
                       If you were conveniently using the imgui copy of those ST
B headers in your project you will have to update your include paths.
 - 2018/09/05 (1.65) - renamed io.OptCursorBlink/io.ConfigCursorBlink to io.Conf
igInputTextCursorBlink. (#1427)
 - 2018/08/31 (1.64) - added imgui_widgets.cpp file, extracted and moved widgets
 code out of imgui.cpp into imgui_widgets.cpp. Re-ordered some of the code remai
ning in imgui.cpp.
                       NONE OF THE FUNCTIONS HAVE CHANGED. THE CODE IS SEMANTICA
LLY 100% IDENTICAL, BUT _EVERY_ FUNCTION HAS BEEN MOVED.
                       Because of this, any local modifications to imgui.cpp wil
l likely conflict when you update. Read docs/CHANGELOG.txt for suggestions.
 - 2018/08/22 (1.63) - renamed IsItemDeactivatedAfterChange() to IsItemDeactivat
edAfterEdit() for consistency with new IsItemEdited() API. Kept redirection func
tion (will obsolete soonish as IsItemDeactivatedAfterChange() is very recent).
 - 2018/08/21 (1.63) - renamed ImGuiTextEditCallback to ImGuiInputTextCallback,
ImGuiTextEditCallbackData to ImGuiInputTextCallbackData for consistency. Kept re
direction types (will obsolete).
 - 2018/08/21 (1.63) - removed ImGuiInputTextCallbackData::ReadOnly since it is
a duplication of (ImGuiInputTextCallbackData::Flags & ImGuiInputTextFlags_ReadOn
ly).
 - 2018/08/01 (1.63) - removed per-window ImGuiWindowFlags_ResizeFromAnySide bet
a flag in favor of a global io.ConfigResizeWindowsFromEdges [update 1.67 renamed
 to ConfigWindowsResizeFromEdges] to enable the feature.
 - 2018/08/01 (1.63) - renamed io.OptCursorBlink to io.ConfigCursorBlink [-> io.
ConfigInputTextCursorBlink in 1.65], io.OptMacOSXBehaviors to ConfigMacOSXBehavi
ors for consistency.
 - 2018/07/22 (1.63) - changed ImGui::GetTime() return value from float to doubl
e to avoid accumulating floating point imprecisions over time.
 - 2018/07/08 (1.63) - style: renamed ImGuiCol_ModalWindowDarkening to ImGuiCol_
ModalWindowDimBg for consistency with other features. Kept redirection enum (wil
l obsolete).
 - 2018/06/08 (1.62) - examples: the imgui_impl_XXX files have been split to sep
arate platform (Win32, GLFW, SDL2, etc.) from renderer (DX11, OpenGL, Vulkan,  e
tc.).
                       old backends will still work as is, however prefer using
the separated backends as they will be updated to support multi-viewports.
                       when adopting new backends follow the main.cpp code of yo
ur preferred examples/ folder to know which functions to call.
                       in particular, note that old backends called ImGui::NewFr
ame() at the end of their ImGui_ImplXXXX_NewFrame() function.
 - 2018/06/06 (1.62) - renamed GetGlyphRangesChinese() to GetGlyphRangesChineseF
ull() to distinguish other variants and discourage using the full set.
 - 2018/06/06 (1.62) - TreeNodeEx()/TreeNodeBehavior(): the ImGuiTreeNodeFlags_C
ollapsingHeader helper now include the ImGuiTreeNodeFlags_NoTreePushOnOpen flag.
 See Changelog for details.
 - 2018/05/03 (1.61) - DragInt(): the default compile-time format string has bee
n changed from "%.0f" to "%d", as we are not using integers internally any more.
                       If you used DragInt() with custom format strings, make su
re you change them to use %d or an integer-compatible format.
                       To honor backward-compatibility, the DragInt() code will
currently parse and modify format strings to replace %*f with %d, giving time to
 users to upgrade their code.
                       If you have IMGUI_DISABLE_OBSOLETE_FUNCTIONS enabled, the
 code will instead assert! You may run a reg-exp search on your codebase for e.g
. "DragInt.*%f" to help you find them.
 - 2018/04/28 (1.61) - obsoleted InputFloat() functions taking an optional "int
decimal_precision" in favor of an equivalent and more flexible "const char* form
at",
                       consistent with other functions. Kept redirection functio
ns (will obsolete).
 - 2018/04/09 (1.61) - IM_DELETE() helper function added in 1.60 doesn't clear t
he input _pointer_ reference, more consistent with expectation and allows passin
g r-value.
 - 2018/03/20 (1.60) - renamed io.WantMoveMouse to io.WantSetMousePos for consis
tency and ease of understanding (was added in 1.52, _not_ used by core and only
honored by some backend ahead of merging the Nav branch).
 - 2018/03/12 (1.60) - removed ImGuiCol_CloseButton, ImGuiCol_CloseButtonActive,
 ImGuiCol_CloseButtonHovered as the closing cross uses regular button colors now
.
 - 2018/03/08 (1.60) - changed ImFont::DisplayOffset.y to default to 0 instead o
f +1. Fixed rounding of Ascent/Descent to match TrueType renderer. If you were a
dding or subtracting to ImFont::DisplayOffset check if your fonts are correctly
aligned vertically.
 - 2018/03/03 (1.60) - renamed ImGuiStyleVar_Count_ to ImGuiStyleVar_COUNT and I
mGuiMouseCursor_Count_ to ImGuiMouseCursor_COUNT for consistency with other publ
ic enums.
 - 2018/02/18 (1.60) - BeginDragDropSource(): temporarily removed the optional m
ouse_button=0 parameter because it is not really usable in many situations at th
e moment.
 - 2018/02/16 (1.60) - obsoleted the io.RenderDrawListsFn callback, you can call
 your graphics engine render function after ImGui::Render(). Use ImGui::GetDrawD
ata() to retrieve the ImDrawData* to display.
 - 2018/02/07 (1.60) - reorganized context handling to be more explicit,
                       - YOU NOW NEED TO CALL ImGui::CreateContext() AT THE BEGI
NNING OF YOUR APP, AND CALL ImGui::DestroyContext() AT THE END.
                       - removed Shutdown() function, as DestroyContext() serve
this purpose.
                       - you may pass a ImFontAtlas* pointer to CreateContext()
to share a font atlas between contexts. Otherwise CreateContext() will create it
s own font atlas instance.
                       - removed allocator parameters from CreateContext(), they
 are now setup with SetAllocatorFunctions(), and shared by all contexts.
                       - removed the default global context and font atlas insta
nce, which were confusing for users of DLL reloading and users of multiple conte
xts.
 - 2018/01/31 (1.60) - moved sample TTF files from extra_fonts/ to misc/fonts/.
If you loaded files directly from the imgui repo you may need to update your pat
hs.
 - 2018/01/11 (1.60) - obsoleted IsAnyWindowHovered() in favor of IsWindowHovere
d(ImGuiHoveredFlags_AnyWindow). Kept redirection function (will obsolete).
 - 2018/01/11 (1.60) - obsoleted IsAnyWindowFocused() in favor of IsWindowFocuse
d(ImGuiFocusedFlags_AnyWindow). Kept redirection function (will obsolete).
 - 2018/01/03 (1.60) - renamed ImGuiSizeConstraintCallback to ImGuiSizeCallback,
 ImGuiSizeConstraintCallbackData to ImGuiSizeCallbackData.
 - 2017/12/29 (1.60) - removed CalcItemRectClosestPoint() which was weird and no
t really used by anyone except demo code. If you need it it's easy to replicate
on your side.
 - 2017/12/24 (1.53) - renamed the emblematic ShowTestWindow() function to ShowD
emoWindow(). Kept redirection function (will obsolete).
 - 2017/12/21 (1.53) - ImDrawList: renamed style.AntiAliasedShapes to style.Anti
AliasedFill for consistency and as a way to explicitly break code that manipulat
e those flag at runtime. You can now manipulate ImDrawList::Flags
 - 2017/12/21 (1.53) - ImDrawList: removed 'bool anti_aliased = true' final para
meter of ImDrawList::AddPolyline() and ImDrawList::AddConvexPolyFilled(). Prefer
 manipulating ImDrawList::Flags if you need to toggle them during the frame.
 - 2017/12/14 (1.53) - using the ImGuiWindowFlags_NoScrollWithMouse flag on a ch
ild window forwards the mouse wheel event to the parent window, unless either Im
GuiWindowFlags_NoInputs or ImGuiWindowFlags_NoScrollbar are also set.
 - 2017/12/13 (1.53) - renamed GetItemsLineHeightWithSpacing() to GetFrameHeight
WithSpacing(). Kept redirection function (will obsolete).
 - 2017/12/13 (1.53) - obsoleted IsRootWindowFocused() in favor of using IsWindo
wFocused(ImGuiFocusedFlags_RootWindow). Kept redirection function (will obsolete
).
                     - obsoleted IsRootWindowOrAnyChildFocused() in favor of usi
ng IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows). Kept redirection func
tion (will obsolete).
 - 2017/12/12 (1.53) - renamed ImGuiTreeNodeFlags_AllowOverlapMode to ImGuiTreeN
odeFlags_AllowItemOverlap. Kept redirection enum (will obsolete).
 - 2017/12/10 (1.53) - removed SetNextWindowContentWidth(), prefer using SetNext
WindowContentSize(). Kept redirection function (will obsolete).
 - 2017/11/27 (1.53) - renamed ImGuiTextBuffer::append() helper to appendf(), ap
pendv() to appendfv(). If you copied the 'Log' demo in your code, it uses append
v() so that needs to be renamed.
 - 2017/11/18 (1.53) - Style, Begin: removed ImGuiWindowFlags_ShowBorders window
 flag. Borders are now fully set up in the ImGuiStyle structure (see e.g. style.
FrameBorderSize, style.WindowBorderSize). Use ImGui::ShowStyleEditor() to look t
hem up.
                       Please note that the style system will keep evolving (hop
efully stabilizing in Q1 2018), and so custom styles will probably subtly break
over time. It is recommended you use the StyleColorsClassic(), StyleColorsDark()
, StyleColorsLight() functions.
 - 2017/11/18 (1.53) - Style: removed ImGuiCol_ComboBg in favor of combo boxes u
sing ImGuiCol_PopupBg for consistency.
 - 2017/11/18 (1.53) - Style: renamed ImGuiCol_ChildWindowBg to ImGuiCol_ChildBg
.
 - 2017/11/18 (1.53) - Style: renamed style.ChildWindowRounding to style.ChildRo
unding, ImGuiStyleVar_ChildWindowRounding to ImGuiStyleVar_ChildRounding.
 - 2017/11/02 (1.53) - obsoleted IsRootWindowOrAnyChildHovered() in favor of usi
ng IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);
 - 2017/10/24 (1.52) - renamed IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCS/IMGUI
_DISABLE_WIN32_DEFAULT_IME_FUNCS to IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTI
ONS/IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS for consistency.
 - 2017/10/20 (1.52) - changed IsWindowHovered() default parameters behavior to
return false if an item is active in another window (e.g. click-dragging item fr
om another window to this window). You can use the newly introduced IsWindowHove
red() flags to requests this specific behavior if you need it.
 - 2017/10/20 (1.52) - marked IsItemHoveredRect()/IsMouseHoveringWindow() as obs
olete, in favor of using the newly introduced flags for IsItemHovered() and IsWi
ndowHovered(). See https://github.com/ocornut/imgui/issues/1382 for details.
                       removed the IsItemRectHovered()/IsWindowRectHovered() nam
es introduced in 1.51 since they were merely more consistent names for the two f
unctions we are now obsoleting.
                         IsItemHoveredRect()        --> IsItemHovered(ImGuiHover
edFlags_RectOnly)
                         IsMouseHoveringAnyWindow() --> IsWindowHovered(ImGuiHov
eredFlags_AnyWindow)
                         IsMouseHoveringWindow()    --> IsWindowHovered(ImGuiHov
eredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveIt
em) [weird, old behavior]
 - 2017/10/17 (1.52) - marked the old 5-parameters version of Begin() as obsolet
e (still available). Use SetNextWindowSize()+Begin() instead!
 - 2017/10/11 (1.52) - renamed AlignFirstTextHeightToWidgets() to AlignTextToFra
mePadding(). Kept inline redirection function (will obsolete).
 - 2017/09/26 (1.52) - renamed ImFont::Glyph to ImFontGlyph. Kept redirection ty
pedef (will obsolete).
 - 2017/09/25 (1.52) - removed SetNextWindowPosCenter() because SetNextWindowPos
() now has the optional pivot information to do the same and more. Kept redirect
ion function (will obsolete).
 - 2017/08/25 (1.52) - io.MousePos needs to be set to ImVec2(-FLT_MAX,-FLT_MAX)
when mouse is unavailable/missing. Previously ImVec2(-1,-1) was enough but we no
w accept negative mouse coordinates. In your backend if you need to support unav
ailable mouse, make sure to replace "io.MousePos = ImVec2(-1,-1)" with "io.Mouse
Pos = ImVec2(-FLT_MAX,-FLT_MAX)".
 - 2017/08/22 (1.51) - renamed IsItemHoveredRect() to IsItemRectHovered(). Kept
inline redirection function (will obsolete). -> (1.52) use IsItemHovered(ImGuiHo
veredFlags_RectOnly)!
                     - renamed IsMouseHoveringAnyWindow() to IsAnyWindowHovered(
) for consistency. Kept inline redirection function (will obsolete).
                     - renamed IsMouseHoveringWindow() to IsWindowRectHovered()
for consistency. Kept inline redirection function (will obsolete).
 - 2017/08/20 (1.51) - renamed GetStyleColName() to GetStyleColorName() for cons
istency.
 - 2017/08/20 (1.51) - added PushStyleColor(ImGuiCol idx, ImU32 col) overload, w
hich _might_ cause an "ambiguous call" compilation error if you are using ImColo
r() with implicit cast. Cast to ImU32 or ImVec4 explicitly to fix.
 - 2017/08/15 (1.51) - marked the weird IMGUI_ONCE_UPON_A_FRAME helper macro as
obsolete. prefer using the more explicit ImGuiOnceUponAFrame type.
 - 2017/08/15 (1.51) - changed parameter order for BeginPopupContextWindow() fro
m (const char*,int buttons,bool also_over_items) to (const char*,int buttons,boo
l also_over_items). Note that most calls relied on default parameters completely
.
 - 2017/08/13 (1.51) - renamed ImGuiCol_Column to ImGuiCol_Separator, ImGuiCol_C
olumnHovered to ImGuiCol_SeparatorHovered, ImGuiCol_ColumnActive to ImGuiCol_Sep
aratorActive. Kept redirection enums (will obsolete).
 - 2017/08/11 (1.51) - renamed ImGuiSetCond_Always to ImGuiCond_Always, ImGuiSet
Cond_Once to ImGuiCond_Once, ImGuiSetCond_FirstUseEver to ImGuiCond_FirstUseEver
, ImGuiSetCond_Appearing to ImGuiCond_Appearing. Kept redirection enums (will ob
solete).
 - 2017/08/09 (1.51) - removed ValueColor() helpers, they are equivalent to call
ing Text(label) + SameLine() + ColorButton().
 - 2017/08/08 (1.51) - removed ColorEditMode() and ImGuiColorEditMode in favor o
f ImGuiColorEditFlags and parameters to the various Color*() functions. The SetC
olorEditOptions() allows to initialize default but the user can still change the
m with right-click context menu.
                     - changed prototype of 'ColorEdit4(const char* label, float
 col[4], bool show_alpha = true)' to 'ColorEdit4(const char* label, float col[4]
, ImGuiColorEditFlags flags = 0)', where passing flags = 0x01 is a safe no-op (h
ello dodgy backward compatibility!). - check and run the demo window, under "Col
or/Picker Widgets", to understand the various new options.
                     - changed prototype of rarely used 'ColorButton(ImVec4 col,
 bool small_height = false, bool outline_border = true)' to 'ColorButton(const c
har* desc_id, ImVec4 col, ImGuiColorEditFlags flags = 0, ImVec2 size = ImVec2(0,
 0))'
 - 2017/07/20 (1.51) - removed IsPosHoveringAnyWindow(ImVec2), which was partly
broken and misleading. ASSERT + redirect user to io.WantCaptureMouse
 - 2017/05/26 (1.50) - removed ImFontConfig::MergeGlyphCenterV in favor of a mor
e multipurpose ImFontConfig::GlyphOffset.
 - 2017/05/01 (1.50) - renamed ImDrawList::PathFill() (rarely used directly) to
ImDrawList::PathFillConvex() for clarity.
 - 2016/11/06 (1.50) - BeginChild(const char*) now applies the stack id to the p
rovided label, consistently with other functions as it should always have been.
It shouldn't affect you unless (extremely unlikely) you were appending multiple
times to a same child from different locations of the stack id. If that's the ca
se, generate an id with GetID() and use it instead of passing string to BeginChi
ld().
 - 2016/10/15 (1.50) - avoid 'void* user_data' parameter to io.SetClipboardTextF
n/io.GetClipboardTextFn pointers. We pass io.ClipboardUserData to it.
 - 2016/09/25 (1.50) - style.WindowTitleAlign is now a ImVec2 (ImGuiAlign enum w
as removed). set to (0.5f,0.5f) for horizontal+vertical centering, (0.0f,0.0f) f
or upper-left, etc.
 - 2016/07/30 (1.50) - SameLine(x) with x>0.0f is now relative to left of column
/group if any, and not always to left of window. This was sort of always the int
ent and hopefully, breakage should be minimal.
 - 2016/05/12 (1.49) - title bar (using ImGuiCol_TitleBg/ImGuiCol_TitleBgActive
colors) isn't rendered over a window background (ImGuiCol_WindowBg color) anymor
e.
                       If your TitleBg/TitleBgActive alpha was 1.0f or you are u
sing the default theme it will not affect you, otherwise if <1.0f you need to tw
eak your custom theme to readjust for the fact that we don't draw a WindowBg bac
kground behind the title bar.
                       This helper function will convert an old TitleBg/TitleBgA
ctive color into a new one with the same visual output, given the OLD color and
the OLD WindowBg color:
                       ImVec4 ConvertTitleBgCol(const ImVec4& win_bg_col, const
ImVec4& title_bg_col) { float new_a = 1.0f - ((1.0f - win_bg_col.w) * (1.0f - ti
tle_bg_col.w)), k = title_bg_col.w / new_a; return ImVec4((win_bg_col.x * win_bg
_col.w + title_bg_col.x) * k, (win_bg_col.y * win_bg_col.w + title_bg_col.y) * k
, (win_bg_col.z * win_bg_col.w + title_bg_col.z) * k, new_a); }
                       If this is confusing, pick the RGB value from title bar f
rom an old screenshot and apply this as TitleBg/TitleBgActive. Or you may just c
reate TitleBgActive from a tweaked TitleBg color.
 - 2016/05/07 (1.49) - removed confusing set of GetInternalState(), GetInternalS
tateSize(), SetInternalState() functions. Now using CreateContext(), DestroyCont
ext(), GetCurrentContext(), SetCurrentContext().
 - 2016/05/02 (1.49) - renamed SetNextTreeNodeOpened() to SetNextTreeNodeOpen(),
 no redirection.
 - 2016/05/01 (1.49) - obsoleted old signature of CollapsingHeader(const char* l
abel, const char* str_id = NULL, bool display_frame = true, bool default_open =
false) as extra parameters were badly designed and rarely used. You can replace
the "default_open = true" flag in new API with CollapsingHeader(label, ImGuiTree
NodeFlags_DefaultOpen).
 - 2016/04/26 (1.49) - changed ImDrawList::PushClipRect(ImVec4 rect) to ImDrawLi
st::PushClipRect(Imvec2 min,ImVec2 max,bool intersect_with_current_clip_rect=fal
se). Note that higher-level ImGui::PushClipRect() is preferable because it will
clip at logic/widget level, whereas ImDrawList::PushClipRect() only affect your
renderer.
 - 2016/04/03 (1.48) - removed style.WindowFillAlphaDefault setting which was re
dundant. Bake default BG alpha inside style.Colors[ImGuiCol_WindowBg] and all ot
her Bg color values. (ref GitHub issue #337).
 - 2016/04/03 (1.48) - renamed ImGuiCol_TooltipBg to ImGuiCol_PopupBg, used by p
opups/menus and tooltips. popups/menus were previously using ImGuiCol_WindowBg.
(ref github issue #337)
 - 2016/03/21 (1.48) - renamed GetWindowFont() to GetFont(), GetWindowFontSize()
 to GetFontSize(). Kept inline redirection function (will obsolete).
 - 2016/03/02 (1.48) - InputText() completion/history/always callbacks: if you m
odify the text buffer manually (without using DeleteChars()/InsertChars() helper
) you need to maintain the BufTextLen field. added an assert.
 - 2016/01/23 (1.48) - fixed not honoring exact width passed to PushItemWidth(),
 previously it would add extra FramePadding.x*2 over that width. if you had manu
al pixel-perfect alignment in place it might affect you.
 - 2015/12/27 (1.48) - fixed ImDrawList::AddRect() which used to render a rectan
gle 1 px too large on each axis.
 - 2015/12/04 (1.47) - renamed Color() helpers to ValueColor() - dangerously nam
ed, rarely used and probably to be made obsolete.
 - 2015/08/29 (1.45) - with the addition of horizontal scrollbar we made various
 fixes to inconsistencies with dealing with cursor position.
                       GetCursorPos()/SetCursorPos() functions now include the s
crolled amount. It shouldn't affect the majority of users, but take note that Se
tCursorPosX(100.0f) puts you at +100 from the starting x position which may incl
ude scrolling, not at +100 from the window left side.
                       GetContentRegionMax()/GetWindowContentRegionMin()/GetWind
owContentRegionMax() functions allow include the scrolled amount. Typically thos
e were used in cases where no scrolling would happen so it may not be a problem,
 but watch out!
 - 2015/08/29 (1.45) - renamed style.ScrollbarWidth to style.ScrollbarSize
 - 2015/08/05 (1.44) - split imgui.cpp into extra files: imgui_demo.cpp imgui_dr
aw.cpp imgui_internal.h that you need to add to your project.
 - 2015/07/18 (1.44) - fixed angles in ImDrawList::PathArcTo(), PathArcToFast()
(introduced in 1.43) being off by an extra PI for no justifiable reason
 - 2015/07/14 (1.43) - add new ImFontAtlas::AddFont() API. For the old AddFont**
*, moved the 'font_no' parameter of ImFontAtlas::AddFont** functions to the ImFo
ntConfig structure.
                       you need to render your textured triangles with bilinear
filtering to benefit from sub-pixel positioning of text.
 - 2015/07/08 (1.43) - switched rendering data to use indexed rendering. this is
 saving a fair amount of CPU/GPU and enables us to get anti-aliasing for a margi
nal cost.
                       this necessary change will break your rendering function!
 the fix should be very easy. sorry for that :(
                     - if you are using a vanilla copy of one of the imgui_impl_
XXX.cpp provided in the example, you just need to update your copy and you can i
gnore the rest.
                     - the signature of the io.RenderDrawListsFn handler has cha
nged!
                       old: ImGui_XXXX_RenderDrawLists(ImDrawList** const cmd_li
sts, int cmd_lists_count)
                       new: ImGui_XXXX_RenderDrawLists(ImDrawData* draw_data).
                         parameters: 'cmd_lists' becomes 'draw_data->CmdLists',
'cmd_lists_count' becomes 'draw_data->CmdListsCount'
                         ImDrawList: 'commands' becomes 'CmdBuffer', 'vtx_buffer
' becomes 'VtxBuffer', 'IdxBuffer' is new.
                         ImDrawCmd:  'vtx_count' becomes 'ElemCount', 'clip_rect
' becomes 'ClipRect', 'user_callback' becomes 'UserCallback', 'texture_id' becom
es 'TextureId'.
                     - each ImDrawList now contains both a vertex buffer and an
index buffer. For each command, render ElemCount/3 triangles using indices from
the index buffer.
                     - if you REALLY cannot render indexed primitives, you can c
all the draw_data->DeIndexAllBuffers() method to de-index the buffers. This is s
low and a waste of CPU/GPU. Prefer using indexed rendering!
                     - refer to code in the examples/ folder or ask on the GitHu
b if you are unsure of how to upgrade. please upgrade!
 - 2015/07/10 (1.43) - changed SameLine() parameters from int to float.
 - 2015/07/02 (1.42) - renamed SetScrollPosHere() to SetScrollFromCursorPos(). K
ept inline redirection function (will obsolete).
 - 2015/07/02 (1.42) - renamed GetScrollPosY() to GetScrollY(). Necessary to red
uce confusion along with other scrolling functions, because positions (e.g. curs
or position) are not equivalent to scrolling amount.
 - 2015/06/14 (1.41) - changed ImageButton() default bg_col parameter from (0,0,
0,1) (black) to (0,0,0,0) (transparent) - makes a difference when texture have t
ransparence
 - 2015/06/14 (1.41) - changed Selectable() API from (label, selected, size) to
(label, selected, flags, size). Size override should have been rarely used. Sorr
y!
 - 2015/05/31 (1.40) - renamed GetWindowCollapsed() to IsWindowCollapsed() for c
onsistency. Kept inline redirection function (will obsolete).
 - 2015/05/31 (1.40) - renamed IsRectClipped() to IsRectVisible() for consistenc
y. Note that return value is opposite! Kept inline redirection function (will ob
solete).
 - 2015/05/27 (1.40) - removed the third 'repeat_if_held' parameter from Button(
) - sorry! it was rarely used and inconsistent. Use PushButtonRepeat(true) / Pop
ButtonRepeat() to enable repeat on desired buttons.
 - 2015/05/11 (1.40) - changed BeginPopup() API, takes a string identifier inste
ad of a bool. ImGui needs to manage the open/closed state of popups. Call OpenPo
pup() to actually set the "open" state of a popup. BeginPopup() returns true if
the popup is opened.
 - 2015/05/03 (1.40) - removed style.AutoFitPadding, using style.WindowPadding m
akes more sense (the default values were already the same).
 - 2015/04/13 (1.38) - renamed IsClipped() to IsRectClipped(). Kept inline redir
ection function until 1.50.
 - 2015/04/09 (1.38) - renamed ImDrawList::AddArc() to ImDrawList::AddArcFast()
for compatibility with future API
 - 2015/04/03 (1.38) - removed ImGuiCol_CheckHovered, ImGuiCol_CheckActive, repl
aced with the more general ImGuiCol_FrameBgHovered, ImGuiCol_FrameBgActive.
 - 2014/04/03 (1.38) - removed support for passing -FLT_MAX..+FLT_MAX as the ran
ge for a SliderFloat(). Use DragFloat() or Inputfloat() instead.
 - 2015/03/17 (1.36) - renamed GetItemBoxMin()/GetItemBoxMax()/IsMouseHoveringBo
x() to GetItemRectMin()/GetItemRectMax()/IsMouseHoveringRect(). Kept inline redi
rection function until 1.50.
 - 2015/03/15 (1.36) - renamed style.TreeNodeSpacing to style.IndentSpacing, ImG
uiStyleVar_TreeNodeSpacing to ImGuiStyleVar_IndentSpacing
 - 2015/03/13 (1.36) - renamed GetWindowIsFocused() to IsWindowFocused(). Kept i
nline redirection function until 1.50.
 - 2015/03/08 (1.35) - renamed style.ScrollBarWidth to style.ScrollbarWidth (cas
ing)
 - 2015/02/27 (1.34) - renamed OpenNextNode(bool) to SetNextTreeNodeOpened(bool,
 ImGuiSetCond). Kept inline redirection function until 1.50.
 - 2015/02/27 (1.34) - renamed ImGuiSetCondition_*** to ImGuiSetCond_***, and _F
irstUseThisSession becomes _Once.
 - 2015/02/11 (1.32) - changed text input callback ImGuiTextEditCallback return
type from void-->int. reserved for future use, return 0 for now.
 - 2015/02/10 (1.32) - renamed GetItemWidth() to CalcItemWidth() to clarify its
evolving behavior
 - 2015/02/08 (1.31) - renamed GetTextLineSpacing() to GetTextLineHeightWithSpac
ing()
 - 2015/02/01 (1.31) - removed IO.MemReallocFn (unused)
 - 2015/01/19 (1.30) - renamed ImGuiStorage::GetIntPtr()/GetFloatPtr() to GetInt
Ref()/GetIntRef() because Ptr was conflicting with actual pointer storage functi
ons.
 - 2015/01/11 (1.30) - big font/image API change! now loads TTF file. allow for
multiple fonts. no need for a PNG loader.
 - 2015/01/11 (1.30) - removed GetDefaultFontData(). uses io.Fonts->GetTextureDa
ta*() API to retrieve uncompressed pixels.
                       - old:  const void* png_data; unsigned int png_size; ImGu
i::GetDefaultFontData(NULL, NULL, &png_data, &png_size); [..Upload texture to GP
U..];
                       - new:  unsigned char* pixels; int width, height; io.Font
s->GetTexDataAsRGBA32(&pixels, &width, &height); [..Upload texture to GPU..]; io
.Fonts->SetTexID(YourTexIdentifier);
                       you now have more flexibility to load multiple TTF fonts
and manage the texture buffer for internal needs. It is now recommended that you
 sample the font texture with bilinear interpolation.
 - 2015/01/11 (1.30) - added texture identifier in ImDrawCmd passed to your rend
er function (we can now render images). make sure to call io.Fonts->SetTexID()
 - 2015/01/11 (1.30) - removed IO.PixelCenterOffset (unnecessary, can be handled
 in user projection matrix)
 - 2015/01/11 (1.30) - removed ImGui::IsItemFocused() in favor of ImGui::IsItemA
ctive() which handles all widgets
 - 2014/12/10 (1.18) - removed SetNewWindowDefaultPos() in favor of new generic
API SetNextWindowPos(pos, ImGuiSetCondition_FirstUseEver)
 - 2014/11/28 (1.17) - moved IO.Font*** options to inside the IO.Font-> structur
e (FontYOffset, FontTexUvForWhite, FontBaseScale, FontFallbackGlyph)
 - 2014/11/26 (1.17) - reworked syntax of IMGUI_ONCE_UPON_A_FRAME helper macro t
o increase compiler compatibility
 - 2014/11/07 (1.15) - renamed IsHovered() to IsItemHovered()
 - 2014/10/02 (1.14) - renamed IMGUI_INCLUDE_IMGUI_USER_CPP to IMGUI_INCLUDE_IMG
UI_USER_INL and imgui_user.cpp to imgui_user.inl (more IDE friendly)
 - 2014/09/25 (1.13) - removed 'text_end' parameter from IO.SetClipboardTextFn (
the string is now always zero-terminated for simplicity)
 - 2014/09/24 (1.12) - renamed SetFontScale() to SetWindowFontScale()
 - 2014/09/24 (1.12) - moved IM_MALLOC/IM_REALLOC/IM_FREE preprocessor defines t
o IO.MemAllocFn/IO.MemReallocFn/IO.MemFreeFn
 - 2014/08/30 (1.09) - removed IO.FontHeight (now computed automatically)
 - 2014/08/30 (1.09) - moved IMGUI_FONT_TEX_UV_FOR_WHITE preprocessor define to
IO.FontTexUvForWhite
 - 2014/08/28 (1.09) - changed the behavior of IO.PixelCenterOffset following va
rious rendering fixes
...
// The file is too long, it will be truncated. The rest of the file will be included.
