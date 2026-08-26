#pragma once
#include <raylib.h>
#include <vector>
#include <algorithm>

struct AnimatedExplosion
{
    Vector2 position;
    Texture2D texture;
    int totalFrames;
    int framesPerRow;
    int currentFrame;
    float frameDuration;
    float frameTimer;
    float scale;
    int frameWidth;
    int frameHeight;

    AnimatedExplosion(Vector2 pos, Texture2D tex, int frames, int fpr, int fw, int fh, float duration, float s = 1.0f)
        : position(pos), texture(tex), totalFrames(frames), framesPerRow(fpr), 
          currentFrame(0), frameDuration(duration), frameTimer(0.0f), scale(s),
          frameWidth(fw), frameHeight(fh) {}

    void Update(float dt)
    {
        frameTimer += dt;
        if (frameTimer >= frameDuration)
        {
            frameTimer = 0.0f;
            currentFrame++;
        }
    }

    bool IsAlive() const
    {
        return currentFrame < totalFrames;
    }

    void Draw()
    {
        if (!IsAlive() || texture.id == 0)
            return;

        // --- CÁLCULO DE FRAME EXACTO ---
        int frameRow = currentFrame / framesPerRow;
        int frameCol = currentFrame % framesPerRow;

        Rectangle sourceRec = {
            (float)(frameCol * frameWidth),
            (float)(frameRow * frameHeight),
            (float)frameWidth,
            (float)frameHeight
        };

        // --- TAMAÑO EN PANTALLA CON ESCALA RELATIVA ---
        float visualWidth = frameWidth * scale;
        float visualHeight = frameHeight * scale;

        Rectangle destRec = {
            position.x,
            position.y,
            visualWidth,
            visualHeight
        };

        Vector2 origin = {visualWidth / 2.0f, visualHeight / 2.0f};

        DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
    }
};

class ExplosionManager
{
private:
    std::vector<AnimatedExplosion> explosions;

public:
    // --- EXPLOSIÓN DEL AUTO (explosionC.png) ---
    // Imagen: 3024x504 (6 frames en 1 fila)
    // Cada frame: 3024/6 = 504px ancho, 504px alto
    void AddCarExplosion(Vector2 pos, Texture2D tex)
    {
        explosions.push_back(AnimatedExplosion(pos, tex, 6, 6, 504, 504, 0.1f, 1.8f));
    }
    //  explosions.push_back(AnimatedExplosion(pos, tex,(frames), (frames horizontales), (ancho), (alto), (duracion de frame), (tamano)));

    // --- EXPLOSIÓN DEL HELICÓPTERO (ExplosionH.png) ---
    // Imagen: 1645x1920 (20 frames en 5 columnas x 4 filas)
    // Cada frame: 1645/5 = 329px ancho, 1920/4 = 480px alto
    void AddHelicopterExplosion(Vector2 pos, Texture2D tex)
    {
        explosions.push_back(AnimatedExplosion(pos, tex, 20, 5, 329, 480, 0.06f, 1.3f));
    }

    // --- EXPLOSIÓN DEL MISIL (explosionM.png) ---
    // Imagen: 1000x720 (10 frames en 5 columnas x 2 filas)
    // Cada frame: 1000/5 = 200px ancho, 720/2 = 360px alto
    void AddMissileExplosion(Vector2 pos, Texture2D tex)
    {
        explosions.push_back(AnimatedExplosion(pos, tex, 5, 5, 200, 180, 0.08f, 0.9f));
    }

    // --- EXPLOSIÓN DE MINA (explosionMi.jpg) ---
    // Imagen: 9600x3840 (10 frames en 5 columnas x 2 filas)
    // Cada frame: 9600/5 = 1920px ancho, 3840/2 = 1920px alto
    void AddMineExplosion(Vector2 pos, Texture2D tex)
    {
        explosions.push_back(AnimatedExplosion(pos, tex, 10, 5, 1920, 1920, 0.1f, 0.1f));
    }

    void Update(float dt)
    {
        for (auto &exp : explosions)
            exp.Update(dt);

        // Eliminar explosiones que terminaron
        explosions.erase(
            std::remove_if(explosions.begin(), explosions.end(),
                [](const AnimatedExplosion &e) { return !e.IsAlive(); }),
            explosions.end());
    }

    void Draw()
    {
        for (auto &exp : explosions)
            exp.Draw();
    }
};
