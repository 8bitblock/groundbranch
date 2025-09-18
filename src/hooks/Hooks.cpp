#include "Hooks.hpp"
#include <Windows.h>
#include <d3d11.h>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include "../Engine_classes.hpp"
#include "../Global_DEFINES.h"
#include "../esp/ESP.hpp"

// Forward declaration of the hooked WndProc
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Hooks
{
    // Define global variables
    Present_t oPresent = nullptr;
    ResizeBuffers_t oResizeBuffers = nullptr;
    PostRender_t oPostRender = nullptr;
    ID3D11Device* pDevice = nullptr;
    ID3D11DeviceContext* pContext = nullptr;
    ID3D11RenderTargetView* pRenderTargetView = nullptr;
    HWND hWindow = nullptr;
    WNDPROC oWndProc = nullptr;
    VMT* pGameViewportVMT = nullptr;
    bool bImGuiInitialized = false;
    bool bShowMenu = true;
    bool bEnableESP = true;
    void** pSwapChainVTable = nullptr;


    // Hook for window messages
    LRESULT __stdcall hkWndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if (uMsg == WM_KEYDOWN && wParam == VK_INSERT) {
            bShowMenu = !bShowMenu;
        }

        if (bImGuiInitialized && bShowMenu && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
            return true;
        }

        return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
    }

    // Hook for Present
    HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
        if (!bImGuiInitialized) {
            if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice))) {
                pDevice->GetImmediateContext(&pContext);
                DXGI_SWAP_CHAIN_DESC desc;
                pSwapChain->GetDesc(&desc);
                hWindow = desc.OutputWindow;

                oWndProc = (WNDPROC)SetWindowLongPtr(hWindow, GWLP_WNDPROC, (LONG_PTR)hkWndProc);

                ImGui::CreateContext();
                ImGuiIO& io = ImGui::GetIO(); (void)io;
                ImGui_ImplWin32_Init(hWindow);
                ImGui_ImplDX11_Init(pDevice, pContext);

                ID3D11Texture2D* pBackBuffer;
                pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
                if (pBackBuffer) {
                    pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
                    pBackBuffer->Release();
                }

                bImGuiInitialized = true;
            }
        }

        if (bImGuiInitialized) {
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            if (bShowMenu)
            {
                ImGui::Begin("ESP Menu");
                ImGui::Checkbox("Enable ESP", &bEnableESP);
                ImGui::End();
            }

            ImGui::Render();
            pContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        }

        return oPresent(pSwapChain, SyncInterval, Flags);
    }

    // Hook for ResizeBuffers
    HRESULT __stdcall hkResizeBuffers(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) {
        if (pRenderTargetView) {
            pRenderTargetView->Release();
            pRenderTargetView = nullptr;
        }

        HRESULT hr = oResizeBuffers(pSwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

        ID3D11Texture2D* pBackBuffer;
        pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
        if (pBackBuffer) {
            pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
            pBackBuffer->Release();
        }

        return hr;
    }

    // Hook for PostRender, now only used for things that need the game's canvas
    void __fastcall hkPostRender(void* thisptr, void* edx, void* canvas) {
        if (oPostRender) {
             ((void(__thiscall*)(void*, void*))oPostRender)(thisptr, canvas);
        }

        // ESP rendering should happen here if it's drawing in world space.
        if (bEnableESP)
        {
            ESP::Render();
        }
    }

    bool HookD3D11()
    {
        WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, DefWindowProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "DX11 Dummy", NULL };
        RegisterClassEx(&wc);
        HWND hWnd = CreateWindow(wc.lpszClassName, NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, NULL, NULL, wc.hInstance, NULL);

        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };

        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 1;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hWnd;
        sd.SampleDesc.Count = 1;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        IDXGISwapChain* pDummySwapChain = nullptr;
        ID3D11Device* pDummyDevice = nullptr;
        ID3D11DeviceContext* pDummyContext = nullptr;

        HRESULT res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 2, D3D11_SDK_VERSION, &sd, &pDummySwapChain, &pDummyDevice, &featureLevel, &pDummyContext);

        if (FAILED(res))
        {
            DestroyWindow(hWnd);
            UnregisterClass(wc.lpszClassName, wc.hInstance);
            return false;
        }

        pSwapChainVTable = *(void***)pDummySwapChain;

        pDummySwapChain->Release();
        pDummyDevice->Release();
        pDummyContext->Release();
        DestroyWindow(hWnd);
        UnregisterClass(wc.lpszClassName, wc.hInstance);

        DWORD oldProtect;
        VirtualProtect(pSwapChainVTable, sizeof(void*) * 20, PAGE_EXECUTE_READWRITE, &oldProtect);

        oPresent = (Present_t)pSwapChainVTable[8];
        oResizeBuffers = (ResizeBuffers_t)pSwapChainVTable[13];

        pSwapChainVTable[8] = hkPresent;
        pSwapChainVTable[13] = hkResizeBuffers;

        VirtualProtect(pSwapChainVTable, sizeof(void*) * 20, oldProtect, &oldProtect);

        return true;
    }

    void UnhookD3D11()
    {
        if (pSwapChainVTable)
        {
            DWORD oldProtect;
            VirtualProtect(pSwapChainVTable, sizeof(void*) * 20, PAGE_EXECUTE_READWRITE, &oldProtect);
            pSwapChainVTable[8] = oPresent;
            pSwapChainVTable[13] = oResizeBuffers;
            VirtualProtect(pSwapChainVTable, sizeof(void*) * 20, oldProtect, &oldProtect);
        }
    }


    void Initialize() {
        HookD3D11();

        if (SDK::GEngine && SDK::GEngine->GameViewport) {
            pGameViewportVMT = new VMT(SDK::GEngine->GameViewport);
            oPostRender = (PostRender_t)pGameViewportVMT->Hook(POST_RENDER_INDEX, hkPostRender);
        }
    }

    void Shutdown() {
        UnhookD3D11();

        if (pGameViewportVMT) {
            delete pGameViewportVMT;
            pGameViewportVMT = nullptr;
        }

        if (oWndProc && hWindow) {
            SetWindowLongPtr(hWindow, GWLP_WNDPROC, (LONG_PTR)oWndProc);
        }

        if (bImGuiInitialized) {
            ImGui_ImplDX11_Shutdown();
            ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();
        }
    }
}
