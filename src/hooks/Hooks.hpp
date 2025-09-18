#pragma once
#include <Windows.h>
#include <d3d11.h>
#include "../imgui/imgui.h"
#include "VMT.hpp"

// Forward declare the message handler from the ImGui Win32 backend
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Hooks
{
    // Original function pointers
    typedef HRESULT(__stdcall* Present_t)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
    extern Present_t oPresent;

    typedef HRESULT(__stdcall* ResizeBuffers_t)(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
    extern ResizeBuffers_t oResizeBuffers;

    typedef void(__thiscall* PostRender_t)(void* thisptr, void* canvas);
    extern PostRender_t oPostRender;

    // Hook handlers
    HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
    HRESULT __stdcall hkResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
    void __fastcall hkPostRender(void* thisptr, void* edx, void* canvas);
    LRESULT __stdcall hkWndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    // D3D11 and window variables
    extern ID3D11Device* pDevice;
    extern ID3D11DeviceContext* pContext;
    extern ID3D11RenderTargetView* pRenderTargetView;
    extern HWND hWindow;
    extern WNDPROC oWndProc;

    // VMT hooks
    extern VMT* pSwapChainVMT;
    extern VMT* pGameViewportVMT;

    // Initialization flag
    extern bool bImGuiInitialized;
    extern bool bD3D11Hooked;
    extern bool bShowMenu;
    extern bool bEnableESP;

	void Initialize();
	void Shutdown();
}
