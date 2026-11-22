/*
 * Proyecto: MATHMAX - RELOADED
 * Archivo: enemies.h
 * Proposito: gestionar helicopteros, misiles, impactos y bajas confirmadas.
 * Requisitos: RF04, RF05, RF09, RF10.
 * Base POO: Gael Flores Canedo / Israel Gomez Ahumada.
 * Adaptacion V&V: Gael Flores / Jorge Roel Vasquez; asistencia de Codex.
 * Revision: 2026-11-09
 */
#pragma once
#include <math.h>
#include <vector>

#include <box2d/box2d.h>
#include <raylib.h>

#include "audios.h"
#include "carb.h"
#include "effects.h"

// Estructura para el misil del ENEMIGO
struct EnemyMissile
{
    b2BodyId bodyId;
    bool active;
    float timeAlive;
    b2Vec2 lastVel;
};

class Helicopter
{
public:
    b2BodyId bodyId;
    float scale;
    bool active;
    float shootTimer;
    int health;

    // IA movimiento
    float positionChangeTimer;
    Vector2 targetOffset;

    // --- ANIMACIÓN DEL SPRITESHEET ---
    Texture2D rotorSpritesheet;
    float frameTimer = 0.0f;
    int currentFrame = 0;

    static const int FRAME_WIDTH = 640;
    static const int FRAME_HEIGHT = 400;
    static const int FRAMES_PER_ROW = 5;
    static const int TOTAL_FRAMES = 30;

    Helicopter(b2WorldId worldId, Vector2 startPos, float physicsScale, Texture2D heliSprite)
    {
        scale = physicsScale;
        active = true;
        health = 1;
        shootTimer = 0.0f;
        positionChangeTimer = 0.0f;
        targetOffset = {0.0f, 0.0f};

        rotorSpritesheet = heliSprite;
        frameTimer = 0.0f;
        currentFrame = 0;

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_kinematicBody;
        bodyDef.position = (b2Vec2){startPos.x / scale, startPos.y / scale};
        bodyId = b2CreateBody(worldId, &bodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = 1.0f;
        b2Polygon box = b2MakeBox(110.0f / scale, 55.0f / scale);
        b2CreatePolygonShape(bodyId, &shapeDef, &box);
    }

    void Update(Vector2 playerPos, b2Vec2 playerVel, float dt, int helicopterIndex,
                int totalHelicopters)
    {
        if (!active)
        {
            return;
        }

        frameTimer += dt;
        if (frameTimer >= 0.04f)
        {
            frameTimer = 0.0f;
            currentFrame = (currentFrame + 1) % TOTAL_FRAMES;
        }

        b2Vec2 myPos = b2Body_GetPosition(bodyId);
        positionChangeTimer += dt;
        if (positionChangeTimer >= 2.5f)
        {
            positionChangeTimer = 0.0f;
            targetOffset.x = GetRandomValue(-10, 10);
            targetOffset.y = GetRandomValue(-15, -5);
        }

        float offsetX = targetOffset.x;
        float offsetY = targetOffset.y;

        if (totalHelicopters == 1)
        {
            offsetX = 0.0f;
            offsetY = -12.0f;
        }
        else if (totalHelicopters == 2)
        {
            offsetX += (helicopterIndex == 0) ? -8.0f : 8.0f;
            offsetY += -10.0f;
        }
        else
        {
            if (helicopterIndex == 0)
            {
                offsetX += -10.0f;
            }
            else if (helicopterIndex == 1)
            {
                offsetX += 10.0f;
            }
        }

        float lookAhead = playerVel.x * 0.3f;
        float targetX_m = (playerPos.x / scale) + lookAhead + offsetX;
        float targetY_m = (playerPos.y / scale) + offsetY;

        float errorX = targetX_m - myPos.x;
        float errorY = targetY_m - myPos.y;

        float catchUpX = 3.0f;
        float catchUpY = 1.0f;

        float finalVx = playerVel.x + (errorX * catchUpX);
        float finalVy = playerVel.y + (errorY * catchUpY);

        float maxSpeed = 100.0f;
        float currentSpeed = sqrtf(finalVx * finalVx + finalVy * finalVy);
        if (currentSpeed > maxSpeed)
        {
            float ratio = maxSpeed / currentSpeed;
            finalVx *= ratio;
            finalVy *= ratio;
        }

        b2Body_SetLinearVelocity(bodyId, {finalVx, finalVy});
    }

    void Destroy()
    {
        if (b2Body_IsValid(bodyId))
        {
            b2DestroyBody(bodyId);
        }
    }

    void Draw()
    {
        if (!active)
        {
            return;
        }

        b2Vec2 pos = b2Body_GetPosition(bodyId);
        float x = pos.x * scale;
        float y = pos.y * scale;

        int frameRow = currentFrame / FRAMES_PER_ROW;
        int frameCol = currentFrame % FRAMES_PER_ROW;

        // --- RECORTE EXACTO ---
        Rectangle sourceRec = {(float)(frameCol * FRAME_WIDTH),  // Salta 640px exactos a la derecha
                               (float)(frameRow * FRAME_HEIGHT), // Salta 400px exactos hacia abajo
                               (float)FRAME_WIDTH, (float)FRAME_HEIGHT};

        // --- TAMAÑO EN PANTALLA ---
        // Reducimos el cuadro gigante (640x400) a un tamaño de juego (ej. 110x70)
        // Mantenemos la proporción aproximada (1.6) para que no se estire feo
        float visualWidth = 240.0f;
        float visualHeight = 130.0f;

        Rectangle destRec = {x, y, visualWidth, visualHeight};

        Vector2 origin = {visualWidth / 2.0f, visualHeight / 2.0f};

        DrawTexturePro(rotorSpritesheet, sourceRec, destRec, origin, 0.0f, WHITE);
    }
};

class EnemyManager
{
private:
    b2WorldId worldId;
    float scale;
    std::vector<Helicopter> helis;
    Texture2D helicopterTexture;
    Texture2D missileTexture;
    ExplosionManager *explosionMgr = nullptr;
    Texture2D explosionHeliTex;
    Texture2D explosionMissileTex;
    float respawnTimer;
    bool waitingToRespawn;

    int helicoptersDestroyedCount = 0;

    GameAudio *audio = nullptr;

public:
    std::vector<EnemyMissile> missiles;

    void SetAudio(GameAudio *ptr)
    {
        audio = ptr;
    }

    int GetHelicoptersDestroyedCount() const
    {
        return helicoptersDestroyedCount;
    }
    void AddHelicopterDestroyed()
    {
        helicoptersDestroyedCount++;
    }
    void ResetDestroyedCount()
    {
        helicoptersDestroyedCount = 0;
    }

    EnemyManager(b2WorldId world, float s, Texture2D heliTex, Texture2D missileTex)
    {
        worldId = world;
        scale = s;
        helicopterTexture = heliTex;
        missileTexture = missileTex;
        respawnTimer = 0.0f;
        waitingToRespawn = false;
    }

    // RF02: la partida debe contener un enemigo antes del primer cuadro.
    // Puede llamarse de nuevo sin duplicar el enemigo inicial.
    void InitializeForRun(Vector2 playerPos)
    {
        if (helis.empty())
        {
            const Vector2 spawnPos = {playerPos.x + 600.0f, playerPos.y - 300.0f};
            helis.emplace_back(worldId, spawnPos, scale, helicopterTexture);
            respawnTimer = 0.0f;
        }
    }

    int GetActiveHelicopterCount() const
    {
        return (int)helis.size();
    }

    void SetExplosionManager(ExplosionManager *mgr)
    {
        explosionMgr = mgr;
    }
    void SetExplosionTexture(Texture2D heliExp)
    {
        explosionHeliTex = heliExp;
    }
    void SetMissileExplosionTexture(Texture2D missileExp)
    {
        explosionMissileTex = missileExp;
    }

    void Update(Car &player, float dt)
    {
        if (player.IsDead() || !b2Body_IsValid(player.GetBodyId()))
        {
            return;
        }

        Vector2 playerPos = player.GetPosition();
        float playerMeters = playerPos.x / scale;
        b2Vec2 playerVel = b2Body_GetLinearVelocity(player.GetBodyId());

        if (playerMeters > 100.0f)
        {
            int maxHelis = 1;
            if (playerMeters > 1000.0f)
            {
                maxHelis = 2;
            }
            if (playerMeters > 3500.0f)
            {
                maxHelis = 3;
            }
            if (playerMeters > 6000.0f)
            {
                maxHelis = 4;
            }
            if (playerMeters > 8000.0f)
            {
                maxHelis = 8;
            }

            int currentHelis = helis.size();
            if (currentHelis < maxHelis)
            {
                float respawnTime = (playerMeters > 4000.0f) ? 7.0f : 15.0f;
                respawnTimer += dt;
                if (respawnTimer >= respawnTime)
                {
                    respawnTimer = 0.0f;
                    if (GetRandomValue(0, 100) < 75)
                    {
                        Vector2 spawnPos = {playerPos.x + 600.0f, playerPos.y - 300.0f};
                        helis.push_back(Helicopter(worldId, spawnPos, scale, helicopterTexture));
                    }
                }
            }
        }

        for (auto it = helis.begin(); it != helis.end();)
        {
            int index = std::distance(helis.begin(), it);
            int total = helis.size();
            it->Update(playerPos, playerVel, dt, index, total);

            it->shootTimer += dt;
            if (it->shootTimer > 3.0f)
            {
                it->shootTimer = 0.0f;
                ShootMissile(*it, playerPos);
            }

            // --- COLISIÓN FIABLE: JUGADOR -> HELICÓPTERO ---
            bool destroyed = false;
            auto &playerMissiles = player.GetActiveMissiles();
            for (auto pMit = playerMissiles.begin(); pMit != playerMissiles.end();)
            {
                b2Vec2 mPos = b2Body_GetPosition(pMit->bodyId);
                b2Vec2 hPos = b2Body_GetPosition(it->bodyId);
                float killDist = 130.0f / scale;

                if (b2Distance(mPos, hPos) < killDist)
                {
                    destroyed = true;
                    b2DestroyBody(pMit->bodyId);
                    pMit = playerMissiles.erase(pMit);
                    break;
                }
                else
                {
                    ++pMit;
                }
            }

            if (destroyed)
            {
                b2Vec2 heliPos = b2Body_GetPosition(it->bodyId);
                Vector2 explosionPos = {heliPos.x * scale, heliPos.y * scale};

                if (explosionMgr)
                {
                    explosionMgr->AddHelicopterExplosion(explosionPos, explosionHeliTex);
                }

                if (audio)
                {
                    PlaySound(audio->heliExplosion);
                }

                AddHelicopterDestroyed();

                it->Destroy();
                it = helis.erase(it);
                respawnTimer = 0.0f;
            }
            else
            {
                ++it;
            }
        }
    }

    void ShootMissile(Helicopter &h, Vector2 targetPos)
    {
        b2Vec2 startPos = b2Body_GetPosition(h.bodyId);
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_dynamicBody;
        bodyDef.isBullet = true;
        bodyDef.position = (b2Vec2){startPos.x, startPos.y + 1.0f};

        float targetX_m = targetPos.x / scale;
        float targetY_m = targetPos.y / scale;
        float dx = targetX_m - startPos.x;
        float dy = targetY_m - startPos.y;
        float len = sqrtf(dx * dx + dy * dy);

        float speed = 36.0f;
        b2Vec2 vel = {(dx / len) * speed, (dy / len) * speed};
        bodyDef.linearVelocity = vel;

        b2BodyId mId = b2CreateBody(worldId, &bodyDef);
        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = 1.0f;
        b2Circle circ = {{0, 0}, 0.3f};
        b2CreateCircleShape(mId, &shapeDef, &circ);

        if (audio)
        {
            PlaySound(audio->shoot);
        }

        missiles.push_back({mId, true, 0.0f, vel});
    }

    bool UpdateEnemyMissiles(Car &player, Vector2 &outParryTarget)
    {
        if (player.IsDead() || !b2Body_IsValid(player.GetBodyId()))
        {
            for (auto &m : missiles)
            {
                if (b2Body_IsValid(m.bodyId))
                {
                    b2DestroyBody(m.bodyId);
                }
            }
            missiles.clear();
            return false;
        }
        bool parryHappened = false;
        for (auto it = missiles.begin(); it != missiles.end();)
        {
            it->timeAlive += 1.0f / 60.0f;
            b2BodyId mBody = it->bodyId;
            b2Vec2 mPos = b2Body_GetPosition(mBody);
            b2Vec2 mVel = b2Body_GetLinearVelocity(mBody);

            // --- 1. COLISIÓN CON JUGADOR ---
            b2Vec2 playerPosBox = b2Body_GetPosition(player.GetBodyId());
            float distToPlayer = b2Distance(mPos, playerPosBox);
            float hitRadius = (player.IsShieldActive()) ? (120.0f / scale) : (100.0f / scale);

            if (distToPlayer < hitRadius)
            {
                if (player.IsParryActive())
                {
                    parryHappened = true;
                    player.InstantRechargeShield();
                    player.SetInvulnerable(0.5f);
                    if (!helis.empty())
                    {
                        b2Vec2 hPos = b2Body_GetPosition(helis[0].bodyId);
                        Vector2 explosionPos = {hPos.x * scale, hPos.y * scale};

                        // AGREGAR EXPLOSIÓN AQUÍ
                        if (explosionMgr)
                        {
                            explosionMgr->AddHelicopterExplosion(explosionPos, explosionHeliTex);
                        }

                        if (audio)
                        {
                            PlaySound(audio->missileExplosion);
                        }
                        outParryTarget = explosionPos; // Usar la posición de la explosión
                        helis[0].Destroy();
                        helis.erase(helis.begin());
                        AddHelicopterDestroyed();
                    }
                    else
                    {
                        outParryTarget = {mPos.x * scale + 1000, mPos.y * scale - 1000};
                    }
                    b2DestroyBody(mBody);
                    it = missiles.erase(it);
                    continue;
                }
                else if (player.IsShieldActive())
                {
                    b2DestroyBody(mBody);
                    it = missiles.erase(it);
                    continue;
                }
                else
                {
                    if (!player.IsInvulnerable())
                    {
                        player.LoseLife();
                    }
                    if (!player.IsDead())
                    {
                        b2Vec2 impulse = {mVel.x * 50.0f, mVel.y * 50.0f};
                        if (b2Body_IsValid(player.GetBodyId())) // Extra seguridad
                        {
                            b2Body_ApplyLinearImpulseToCenter(player.GetBodyId(), impulse, true);
                        }
                    }
                    b2DestroyBody(mBody);
                    it = missiles.erase(it);
                    continue;
                }
            }

            // --- 2. COLISIÓN CON SUELO ---
            float dvx = mVel.x - it->lastVel.x;
            float dvy = mVel.y - it->lastVel.y;
            float impactForce = sqrtf(dvx * dvx + dvy * dvy);

            if (impactForce > 1.0f || it->timeAlive > 5.0f)
            {
                if (distToPlayer < (200.0f / scale) && !player.IsShieldActive() &&
                    !player.IsInvulnerable())
                {
                    float dx = playerPosBox.x - mPos.x;
                    float dy = playerPosBox.y - mPos.y;
                    float len = sqrtf(dx * dx + dy * dy);
                    if (len > 0.0001f)
                    {
                        dx /= len;
                        dy /= len;
                    }
                    float force = 1000.0f;
                    if (!player.IsDead() && b2Body_IsValid(player.GetBodyId()))
                    {
                        b2Body_ApplyLinearImpulseToCenter(player.GetBodyId(),
                                                          (b2Vec2){dx * force, dy * force}, true);
                    }
                }

                if (explosionMgr)
                {
                    Vector2 explosionPos = {mPos.x * scale, mPos.y * scale};
                    explosionMgr->AddMissileExplosion(explosionPos, explosionMissileTex);
                }

                if (audio)
                {
                    PlaySound(audio->missileExplosion);
                }

                b2DestroyBody(mBody);
                it = missiles.erase(it);
            }
            else
            {
                it->lastVel = mVel;
                ++it;
            }
        }
        return parryHappened;
    }

    void Draw()
    {
        for (auto &h : helis)
        {
            h.Draw();
        }

        for (auto &m : missiles)
        {
            b2Vec2 pos = b2Body_GetPosition(m.bodyId);
            b2Vec2 vel = b2Body_GetLinearVelocity(m.bodyId);
            float angle = atan2f(vel.y, vel.x) * RAD2DEG;

            Rectangle source = {0, 0, (float)missileTexture.width, (float)missileTexture.height};
            Rectangle dest = {pos.x * scale, pos.y * scale, 45.0f, 15.0f};
            Vector2 origin = {22.5f, 7.5f};

            DrawTexturePro(missileTexture, source, dest, origin, angle, WHITE);
        }
    }
};
