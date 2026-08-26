#pragma once
#include "raylib.h"

struct GameAudio
{
    // --- MÚSICA ---
    Music menuMusic;
    Music gameMusic;

    // --- EFECTOS DE COCHE ---
    Sound engine;
    Sound shoot;
    Sound shieldOn;
    Sound parrySuccess;
    Sound carExplosion;
    Sound outOfFuel;
    Sound damage;
    Sound recover; // Vida o reparación
    Sound wrong;   // Sonido de respuesta incorrecta

    // --- EFECTOS DE ENEMIGOS/ENTORNO ---
    Sound heliRotor;
    Sound heliExplosion;
    Sound missileExplosion;
    Sound mineBeep; // Activación
    Sound mineExplosion;

    // --- ITEMS ---
    Sound itemPickup; // Gasolina, Ammo

    // --- UI ---
    Sound menuMove;
    Sound menuSelect;

    void Load()
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

        // Volumen base
        SetMusicVolume(menuMusic, 0.5f);
        SetMusicVolume(gameMusic, 0.4f);
        SetSoundVolume(engine, 0.3f);
        SetSoundVolume(heliRotor, 0.3f);
    }

    void Unload()
    {
        UnloadMusicStream(menuMusic);
        UnloadMusicStream(gameMusic);
        UnloadSound(engine);
        UnloadSound(shoot);
        UnloadSound(shieldOn);
        UnloadSound(parrySuccess);
        UnloadSound(carExplosion);
        UnloadSound(outOfFuel);
        UnloadSound(damage);
        UnloadSound(recover);
        UnloadSound(heliRotor);
        UnloadSound(heliExplosion);
        UnloadSound(missileExplosion);
        UnloadSound(mineBeep);
        UnloadSound(mineExplosion);
        UnloadSound(itemPickup);
        UnloadSound(menuMove);
        UnloadSound(menuSelect);
        UnloadSound(wrong);
    }
};
