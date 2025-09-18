#include "ESP.hpp"
#include "../Engine_classes.hpp"
#include "../imgui/imgui.h"
#include <Windows.h>

using namespace SDK;

void ESP::Render() {
    auto world = UWorld::GetWorld();
    if (!world) return;

    auto game_state = world->GameState;
    if (!game_state) return;

    auto local_player = world->OwningGameInstance->LocalPlayers[0];
    if (!local_player) return;

    auto player_controller = local_player->PlayerController;
    if (!player_controller) return;

    for (int i = 0; i < game_state->PlayerArray.Num(); ++i) {
        auto player_state = game_state->PlayerArray[i];
        if (!player_state) continue;

        auto pawn = player_state->PawnPrivate;
        if (!pawn || pawn == player_controller->AcknowledgedPawn) continue;

        FVector origin, extent;
        pawn->GetActorBounds(true, &origin, &extent);

        FVector2D screen_location;
        if (player_controller->ProjectWorldLocationToScreen(origin, &screen_location, false)) {
            FString player_name_fstr = player_state->GetPlayerName();

            char player_name[256];
            WideCharToMultiByte(CP_UTF8, 0, player_name_fstr.c_str(), -1, player_name, sizeof(player_name), NULL, NULL);

            ImGui::GetBackgroundDrawList()->AddText(
                ImVec2(screen_location.X, screen_location.Y),
                IM_COL32(255, 0, 0, 255), // Red
                player_name
            );
        }
    }
}
