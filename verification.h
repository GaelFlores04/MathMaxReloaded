/*
 * Proyecto: MATHMAX - RELOADED
 * Archivo: verification.h
 * Proposito: prueba de integracion determinista; RF01, RF02, RF03, RF08, RF10.
 * Adaptacion V&V: Gael Flores / Jorge Roel Vasquez; asistencia de Codex.
 * Revision: 2026-11-09
 */
#pragma once

#include <cassert>
#include <cstdio>
#include <cstring>

#include <raylib.h>

// El ejecutable normal delega directamente en Raylib; el de prueba usa la misma logica del juego.
#ifdef MATHMAX_VERIFY
inline int verificationFrame = 0;
inline const char *verificationState = "MENU";
#endif

inline bool GameKeyPressed(int key)
{
#ifdef MATHMAX_VERIFY
    return (key == KEY_ENTER && (verificationFrame == 60 || verificationFrame == 330)) ||
           (key == KEY_ESCAPE && (verificationFrame == 130 || verificationFrame == 190 ||
                                  verificationFrame == 250 || verificationFrame == 370)) ||
           (key == KEY_M && verificationFrame == 280) ||
           (key == KEY_DOWN && verificationFrame == 320);
#else
    return IsKeyPressed(key);
#endif
}

inline bool GameKeyDown(int key)
{
#ifdef MATHMAX_VERIFY
    return key == KEY_D && ((verificationFrame > 60 && verificationFrame < 130) ||
                            (verificationFrame > 190 && verificationFrame < 250));
#else
    return IsKeyDown(key);
#endif
}

inline bool GameShouldClose()
{
#ifdef MATHMAX_VERIFY
    return WindowShouldClose() || verificationFrame >= 400;
#else
    return WindowShouldClose();
#endif
}

inline void ReportGameState(const char *state)
{
#ifdef MATHMAX_VERIFY
    verificationState = state;
#else
    (void)state;
#endif
}

inline void FinishGameDrawing()
{
    EndDrawing();
#ifdef MATHMAX_VERIFY
    const char *expected = nullptr;
    const char *path = nullptr;
    switch (verificationFrame)
    {
    case 30:
        expected = "MENU";
        path = "menu_real.png";
        break;
    case 100:
        expected = "PLAYING";
        path = "partida_real.png";
        break;
    case 160:
        expected = "PAUSED";
        path = "pausa_real.png";
        break;
    case 220:
        expected = "PLAYING";
        path = "reanudacion_real.png";
        break;
    case 350:
        expected = "STATS";
        path = "estadisticas_real.png";
        break;
    default:
        break;
    }
    if (expected)
    {
        assert(std::strcmp(expected, verificationState) == 0);
        TakeScreenshot(path);
        std::printf("PASS UI frame=%d estado=%s captura=%s\n", verificationFrame, expected, path);
        std::fflush(stdout);
    }
    ++verificationFrame;
#endif
}
