/*
 * Proyecto: MATHMAX - RELOADED
 * Archivo: audios.h
 * Proposito: cargar, validar y liberar musica y efectos de sonido.
 * Requisitos: RIE01, RIE02.
 * Base POO: Gael Flores Canedo / Israel Gomez Ahumada.
 * Adaptacion V&V: Gael Flores / Jorge Roel Vasquez; asistencia de Codex.
 * Revision: 2026-11-09
 */
#pragma once
#include "raylib.h"

struct GameAudio
{
    // --- MÚSICA ---
    Music menuMusic{};
    Music gameMusic{};

    // --- EFECTOS DE COCHE ---
    Sound engine{};
    Sound shoot{};
    Sound shieldOn{};
    Sound parrySuccess{};
    Sound carExplosion{};
    Sound outOfFuel{};
    Sound damage{};
    Sound recover{}; // Vida o reparación
    Sound wrong{};   // Sonido de respuesta incorrecta

    // --- EFECTOS DE ENEMIGOS/ENTORNO ---
    Sound heliRotor{};
    Sound heliExplosion{};
    Sound missileExplosion{};
    Sound mineExplosion{};

    // --- ITEMS ---
    Sound itemPickup{}; // Gasolina, Ammo

    // --- UI ---
    Sound menuMove{};
    Sound menuSelect{};

    bool Load()
    {
        // Ajusta los nombres de archivo a los que tú tengas
        menuMusic = LoadMusicStream("assets/audio/menu_theme.ogg"); // suena
        gameMusic = LoadMusicStream("assets/audio/gameplay_theme.ogg");

        engine = LoadSound("assets/audio/car_loop.wav");          // suena
        shoot = LoadSound("assets/audio/missile.wav");            // suena
        shieldOn = LoadSound("assets/audio/shield.wav");          // suena
        parrySuccess = LoadSound("assets/audio/parry.wav");       // suena
        carExplosion = LoadSound("assets/audio/car_explode.wav"); // suena
        outOfFuel = LoadSound("assets/audio/low_fuel.wav");       // suena
        damage = LoadSound("assets/audio/damage.wav");
        recover = LoadSound("assets/audio/repair.wav");
        wrong = LoadSound("assets/audio/wrong.wav");

        heliRotor = LoadSound("assets/audio/heli_loop.wav"); // suena
        heliExplosion = LoadSound("assets/audio/heli_explode.wav");
        missileExplosion = LoadSound("assets/audio/missile_explode.wav");
        mineExplosion = LoadSound("assets/audio/mine.wav");

        itemPickup = LoadSound("assets/audio/pickup.wav"); // suena

        menuMove = LoadSound("assets/audio/ui_move.wav");     // suena
        menuSelect = LoadSound("assets/audio/ui_select.wav"); // suena

        if (!(IsMusicValid(menuMusic) && IsMusicValid(gameMusic) && IsSoundValid(engine) &&
              IsSoundValid(shoot) && IsSoundValid(shieldOn) && IsSoundValid(parrySuccess) &&
              IsSoundValid(carExplosion) && IsSoundValid(outOfFuel) && IsSoundValid(damage) &&
              IsSoundValid(recover) && IsSoundValid(wrong) && IsSoundValid(heliRotor) &&
              IsSoundValid(heliExplosion) && IsSoundValid(missileExplosion) &&
              IsSoundValid(mineExplosion) && IsSoundValid(itemPickup) && IsSoundValid(menuMove) &&
              IsSoundValid(menuSelect)))
        {
            Unload();
            return false;
        }

        // Volumen base
        SetMusicVolume(menuMusic, 0.5f);
        SetMusicVolume(gameMusic, 0.4f);
        SetSoundVolume(engine, 0.3f);
        SetSoundVolume(heliRotor, 0.3f);
        return true;
    }

    void Unload()
    {
        if (IsMusicValid(menuMusic))
        {
            UnloadMusicStream(menuMusic);
            menuMusic = {};
        }
        if (IsMusicValid(gameMusic))
        {
            UnloadMusicStream(gameMusic);
            gameMusic = {};
        }
        if (IsSoundValid(engine))
        {
            UnloadSound(engine);
            engine = {};
        }
        if (IsSoundValid(shoot))
        {
            UnloadSound(shoot);
            shoot = {};
        }
        if (IsSoundValid(shieldOn))
        {
            UnloadSound(shieldOn);
            shieldOn = {};
        }
        if (IsSoundValid(parrySuccess))
        {
            UnloadSound(parrySuccess);
            parrySuccess = {};
        }
        if (IsSoundValid(carExplosion))
        {
            UnloadSound(carExplosion);
            carExplosion = {};
        }
        if (IsSoundValid(outOfFuel))
        {
            UnloadSound(outOfFuel);
            outOfFuel = {};
        }
        if (IsSoundValid(damage))
        {
            UnloadSound(damage);
            damage = {};
        }
        if (IsSoundValid(recover))
        {
            UnloadSound(recover);
            recover = {};
        }
        if (IsSoundValid(heliRotor))
        {
            UnloadSound(heliRotor);
            heliRotor = {};
        }
        if (IsSoundValid(heliExplosion))
        {
            UnloadSound(heliExplosion);
            heliExplosion = {};
        }
        if (IsSoundValid(missileExplosion))
        {
            UnloadSound(missileExplosion);
            missileExplosion = {};
        }
        if (IsSoundValid(mineExplosion))
        {
            UnloadSound(mineExplosion);
            mineExplosion = {};
        }
        if (IsSoundValid(itemPickup))
        {
            UnloadSound(itemPickup);
            itemPickup = {};
        }
        if (IsSoundValid(menuMove))
        {
            UnloadSound(menuMove);
            menuMove = {};
        }
        if (IsSoundValid(menuSelect))
        {
            UnloadSound(menuSelect);
            menuSelect = {};
        }
        if (IsSoundValid(wrong))
        {
            UnloadSound(wrong);
            wrong = {};
        }
    }
};
