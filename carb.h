/*
 * Proyecto: MATHMAX - RELOADED
 * Archivo: carb.h
 * Proposito: gestionar vehiculo, recursos, escudo y retos matematicos.
 * Requisitos: RF03, RF04, RF05, RF06, RF07.
 * Base POO: Gael Flores Canedo / Israel Gomez Ahumada.
 * Adaptacion V&V: Gael Flores / Jorge Roel Vasquez; asistencia de Codex.
 * Revision: 2026-11-09
 */
#pragma once
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <functional>
#include <math.h>
#include <vector>

#include <box2d/box2d.h>
#include <raylib.h>

#include "audios.h"
#include "effects.h"
#include "verification.h"

class Car
{
private:
    b2BodyId chassisId;
    b2BodyId wheelBackId;
    b2BodyId wheelFrontId;
    b2JointId springBackId;
    b2JointId springFrontId;
    b2BodyId roofSensorId;
    bool roofTouchingGround;
    GameAudio *audio = nullptr;

    Vector2 lastPos = {0, 0};

    float scale;
    float width, height;
    float wheelOffset;

    // Items del auto
    float fuel;
    bool isDead;
    float maxFuel;

    int lives;
    int maxLives;

    float invulnerabilityTimer;
    bool isInvulnerable;

    int parrysCount = 0;

    // --- ESTRUCTURA MISIL ---
    struct MissileProjectile
    {
        b2BodyId bodyId;
        bool active;
        float timeAlive;
        b2Vec2 lastVel;
    };

    int ammo;
    int maxAmmo;
    std::vector<MissileProjectile> activeMissiles;
    b2WorldId worldRef;

    // --- ESCUDO Y PARRY ---
    bool shieldActive;
    float shieldTimer;    // Cuánto tiempo le queda al escudo (max 3.0s)
    float shieldCooldown; // Tiempo para volver a usarlo (max 2.0s)
    float parryWindow;    // Tiempo restante para hacer Parry (max 0.40s)

    ExplosionManager *explosionMgr = nullptr;
    Texture2D explosionCarTex;
    Texture2D explosionMissileTex;

public:
    // Sistema de Quiz Matemático
    struct MathQuestion
    {
        int num1, num2;
        char operation; // '+', '-', '*', '/'
        int correctAnswer;
        int options[3]{}; // Las 3 opciones
    };

private:
    MathQuestion currentQuestion{};
    bool isQuizActive;
    float quizTimer;
    Vector2 quizPosition;

public:
    void SetExplosionManager(ExplosionManager *mgr)
    {
        explosionMgr = mgr;
    }
    void SetExplosionTextures(Texture2D carExp, Texture2D missileExp)
    {
        explosionCarTex = carExp;
        explosionMissileTex = missileExp;
    }

    Car(b2WorldId worldId, float x, float y)
    {
        scale = 30.0f;
        width = 160.0f;
        height = 50.0f;
        wheelOffset = 55.0f;

        // Inicializar Items
        fuel = 100.0f;
        maxFuel = 100.0f;
        isDead = false;

        // Inicializar Escudo
        shieldActive = false;
        shieldTimer = 0.0f;
        shieldCooldown = 0.0f;
        parryWindow = 0.0f;

        lives = 5;
        maxLives = 5;

        invulnerabilityTimer = 0.0f;
        isInvulnerable = false;

        isQuizActive = false;
        quizTimer = 0.0f;

        worldRef = worldId;
        ammo = 0;
        maxAmmo = 6;

        roofTouchingGround = false;

        // --- 1. CHASIS ---
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_dynamicBody;
        bodyDef.position = (b2Vec2){x / scale, y / scale};
        chassisId = b2CreateBody(worldId, &bodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = 1.0f;

        b2Polygon chassisBox = b2MakeBox((width / 2.0f) / scale, (height / 2.0f) / scale);
        b2CreatePolygonShape(chassisId, &shapeDef, &chassisBox);

        // --- CREAR SENSOR EN EL TECHO (para detectar caída de cabeza) ---
        b2BodyDef roofDef = b2DefaultBodyDef();
        roofDef.position = bodyDef.position; // Misma posición que el chasis
        roofDef.type = b2_dynamicBody;
        roofSensorId = b2CreateBody(worldId, &roofDef);

        // Crear sensor pequeño en la parte superior
        b2ShapeDef sensorDef = b2DefaultShapeDef();
        sensorDef.isSensor = true; // IMPORTANTE: Es un sensor, no colisiona

        b2Polygon roofBox =
            b2MakeBox((width / 2.5f) / scale, 5.0f / scale); // Pequeño rectángulo arriba
        b2CreatePolygonShape(roofSensorId, &sensorDef, &roofBox);

        // Unir el sensor al chasis para que se mueva con él
        b2WeldJointDef weldDef = b2DefaultWeldJointDef();
        weldDef.bodyIdA = chassisId;
        weldDef.bodyIdB = roofSensorId;
        weldDef.localAnchorA = (b2Vec2){0.0f, -(height / 2.0f) / scale};
        weldDef.localAnchorB = (b2Vec2){0.0f, 0.0f};
        b2CreateWeldJoint(worldId, &weldDef);

        // --- 2. RUEDAS ---
        b2ShapeDef wheelShapeDef = b2DefaultShapeDef();
        wheelShapeDef.density = 4.2f;

        b2Circle circle;
        circle.radius = 22.0f / scale;
        circle.center = (b2Vec2){0.0f, 0.0f};

        float suspensionHeight = 50.0f;

        // Rueda Trasera
        bodyDef.position = (b2Vec2){(x - wheelOffset) / scale, (y + suspensionHeight) / scale};
        wheelBackId = b2CreateBody(worldId, &bodyDef);
        b2CreateCircleShape(wheelBackId, &wheelShapeDef, &circle);

        // Rueda Delantera
        bodyDef.position = (b2Vec2){(x + wheelOffset) / scale, (y + suspensionHeight) / scale};
        wheelFrontId = b2CreateBody(worldId, &bodyDef);
        b2CreateCircleShape(wheelFrontId, &wheelShapeDef, &circle);

        // --- 3. SUSPENSIÓN ---
        b2Vec2 axis = {0.0f, 1.0f};
        b2WheelJointDef jointDef = b2DefaultWheelJointDef();
        jointDef.bodyIdA = chassisId;
        jointDef.localAxisA = axis;

        jointDef.hertz = 3.5f;
        jointDef.dampingRatio = 0.35f;
        jointDef.enableLimit = true;
        jointDef.lowerTranslation = -0.3f;
        jointDef.upperTranslation = 0.8f;
        jointDef.enableMotor = true;
        jointDef.maxMotorTorque = 1000.0f;

        float anchorHeight = 1.5f;

        // TRASERA
        jointDef.bodyIdB = wheelBackId;
        jointDef.localAnchorA = (b2Vec2){-wheelOffset / scale, anchorHeight};
        jointDef.localAnchorB = (b2Vec2){0.0f, 0.0f};
        springBackId = b2CreateWheelJoint(worldId, &jointDef);

        // DELANTERA
        jointDef.bodyIdB = wheelFrontId;
        jointDef.localAnchorA = (b2Vec2){wheelOffset / scale, anchorHeight};
        springFrontId = b2CreateWheelJoint(worldId, &jointDef);
    }

    // --- MÉTODOS PÚBLICOS ---

    // AUDIO
    void SetAudio(GameAudio *audioPtr)
    {
        audio = audioPtr;
    }

    void AddFuel(float amount)
    {
        if (isDead || !std::isfinite(amount) || amount <= 0.0f)
        {
            return;
        }
        fuel += std::min(amount, maxFuel - fuel);
        if (fuel > maxFuel)
        {
            fuel = maxFuel;
        }
    }

    float GetFuel()
    {
        return fuel;
    }
    bool IsDead()
    {
        return isDead;
    }

    Vector2 GetPosition()
    {
        if (isDead)
        {
            return lastPos; // Devuelve la última posición segura (sin tocar Box2D)
        }

        if (b2Body_IsValid(chassisId))
        {
            b2Vec2 pos = b2Body_GetPosition(chassisId);
            lastPos = {pos.x * scale, pos.y * scale}; // Guardar caché
            return lastPos;
        }
        return lastPos;
    }

    // --- NUEVO MÉTODO PARA MINAS ---
    void ApplyMineExplosion()
    {
        float explosionPower = 500.0f;
        b2Vec2 impulse = {0.0f, -explosionPower};
        b2Body_ApplyLinearImpulseToCenter(chassisId, impulse, true);

        float currentAngVel = b2Body_GetAngularVelocity(chassisId);
        b2Body_SetAngularVelocity(chassisId, currentAngVel * 0.5f);
    }

    // --- MÉTODOS DE MISILES ---
    void AddAmmo(int amount)
    {
        if (amount > 0)
        {
            ammo += std::min(amount, maxAmmo - ammo);
        }
    }

    int GetAmmo()
    {
        return ammo;
    }

    void Shoot()
    {
        if (ammo <= 0 || ammo > maxAmmo)
        {
            return;
        }
        ammo--;

        if (audio)
        {
            PlaySound(audio->shoot);
        }

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.type = b2_dynamicBody;
        bodyDef.isBullet = true;

        b2Vec2 carPos = b2Body_GetPosition(chassisId);
        b2Rot carRot = b2Body_GetRotation(chassisId);
        float angle = b2Rot_GetAngle(carRot);

        float offX = 0.0f;
        float offY = -2.0f;

        float c = cosf(angle);
        float s = sinf(angle);
        float finalX = carPos.x + (offX * c - offY * s);
        float finalY = carPos.y + (offX * s + offY * c);

        bodyDef.position = (b2Vec2){finalX, finalY};

        b2Vec2 carVel = b2Body_GetLinearVelocity(chassisId);
        float shootSpeed = 60.0f;

        b2Vec2 shootVel = {c * shootSpeed, s * shootSpeed};
        b2Vec2 finalVel = {carVel.x + shootVel.x,
                           carVel.y + shootVel.y}; // Calcular la velocidad final
        bodyDef.linearVelocity = finalVel;

        b2BodyId mBody = b2CreateBody(worldRef, &bodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.density = 2.0f;
        b2Circle circle = {{0, 0}, 0.3f};
        b2CreateCircleShape(mBody, &shapeDef, &circle);

        // GUARDAR LA VELOCIDAD INICIAL
        activeMissiles.push_back({mBody, true, 0.0f, finalVel});
    }

    // --- ACTUALIZAR MISILES CON CALLBACK ---
    // Usamos std::function para evitar incluir "terrain.h" aquí y causar error circular
    void UpdateMissiles(std::function<float(float)> getGroundHeight)
    {
        for (auto it = activeMissiles.begin(); it != activeMissiles.end();)
        {
            if (!it->active)
            {
                it = activeMissiles.erase(it);
                continue;
            }

            it->timeAlive += 1.0f / 60.0f;
            b2Vec2 pos = b2Body_GetPosition(it->bodyId);
            b2Vec2 velocity = b2Body_GetLinearVelocity(it->bodyId);

            // 1. Chequeo visual con margen de seguridad
            float screenX = pos.x * scale;
            float screenY = pos.y * scale;
            float groundY = getGroundHeight(screenX);
            // +10px de margen para asegurar que si visualmente toca, explote
            bool hitVisualGround = (screenY >= groundY - 10.0f);

            // 2. Chequeo Físico (Impacto)
            float dvx = velocity.x - it->lastVel.x;
            float dvy = velocity.y - it->lastVel.y;
            float impactForce = sqrtf(dvx * dvx + dvy * dvy);

            // Si hay impacto físico (>1.0f) O visual O tiempo agotado
            if (hitVisualGround || impactForce > 1.0f || it->timeAlive > 3.0f)
            {
                // ... (Código de explosión Rocket Jump igual) ...
                b2Vec2 carPos = b2Body_GetPosition(chassisId);
                float dist = b2Distance(pos, carPos);
                if (dist < 10.0f)
                {
                    float dx = carPos.x - pos.x;
                    float dy = carPos.y - pos.y;
                    float length = sqrtf(dx * dx + dy * dy);

                    if (length > 0.001f)
                    {
                        dx /= length;
                        dy /= length;
                    }
                    float impulsePower = 800.0f * (1.0f - (dist / 10.0f));
                    b2Body_ApplyLinearImpulseToCenter(
                        chassisId, (b2Vec2){dx * impulsePower, dy * impulsePower}, true);
                }
                Vector2 explosionPos = {pos.x * scale, pos.y * scale};
                if (explosionMgr)
                {
                    explosionMgr->AddMissileExplosion(explosionPos, explosionMissileTex);
                }

                if (audio)
                {
                    PlaySound(audio->missileExplosion);
                }

                b2DestroyBody(it->bodyId);
                it = activeMissiles.erase(it);
            }
            else
            {
                // IMPORTANTE: Actualizar lastVel SOLO si no explotó
                it->lastVel = velocity;
                ++it;
            }
        }
    }

    // Getter para que el EnemyManager vea los misiles y detecte impactos
    std::vector<MissileProjectile> &GetActiveMissiles()
    {
        return activeMissiles;
    }

    int GetMaxAmmo()
    {
        return maxAmmo;
    }

    // Getter para obtener el ID del cuerpo (para que los enemigos te apunten)
    b2BodyId GetBodyId()
    {
        return chassisId;
    }

    // --- GETTERS PARA ESCUDO ---
    bool IsShieldActive()
    {
        return shieldActive;
    }
    bool IsParryActive()
    {
        return shieldActive && (parryWindow > 0.0f);
    }

    // En la sección public de carb.h
    float GetParryWindow()
    {
        return parryWindow;
    }
    float GetShieldTimer()
    {
        return shieldTimer;
    }

    // Método para recargar instantáneamente (al hacer parry exitoso)
    void InstantRechargeShield()
    {
        shieldActive = false;
        shieldTimer = 0.0f;
        shieldCooldown = 0.0f;
        fuel = maxFuel;
        if (audio)
        {
            PlaySound(audio->parrySuccess);
        }
    }

    // Getter para colision
    b2BodyId GetRoofSensorId()
    {
        return roofSensorId;
    }

    bool IsRoofTouchingGround()
    {
        return roofTouchingGround;
    }
    void SetRoofTouching(bool touching)
    {
        roofTouchingGround = touching;
    }

    // getter stats parry
    int GetParrysCount() const
    {
        return parrysCount;
    }
    void AddParry()
    {
        parrysCount++;
    }
    void ResetParryCount()
    {
        parrysCount = 0;
    }

    void StopMovement()
    {
        if (!b2Body_IsValid(chassisId))
        {
            return;
        }

        // Detener todo el movimiento físico
        b2Body_SetLinearVelocity(chassisId, {0, 0});
        b2Body_SetAngularVelocity(chassisId, 0);

        // Apagar los motores de las ruedas
        b2WheelJoint_EnableMotor(springBackId, false);
        b2WheelJoint_EnableMotor(springFrontId, false);
    }

    void UpdateInput()
    {
        if (isDead)
        {
            return;
        }
        float fuelConsumption = 0.055f;
        fuel -= fuelConsumption;

        if (fuel < 0.0f)
        {
            fuel = 0.0f;
        }

        float motorSpeed = 0.0f;
        float maxSpeed = 60.0f;
        float torque = 300.0f;
        bool accelerating = false;

        // Actualizar quiz
        UpdateQuiz(GetFrameTime());

        if (GameKeyDown(KEY_D))
        {
            if (fuel > 0)
            {
                motorSpeed = maxSpeed;
                accelerating = true;
            }
        }
        else if (GameKeyDown(KEY_A))
        {
            if (fuel > 0)
            {
                motorSpeed = -maxSpeed;
                accelerating = true;
            }
        }

        b2Vec2 vel = b2Body_GetLinearVelocity(chassisId);
        float speed = b2Length(vel);

        if (fuel <= 0.0f && speed < 0.5f && !isQuizActive)
        {

            isDead = true;
        }

        if (accelerating)
        {
            b2WheelJoint_SetMotorSpeed(springBackId, motorSpeed);
            b2WheelJoint_SetMaxMotorTorque(springBackId, torque);
            b2WheelJoint_EnableMotor(springBackId, true);

            b2WheelJoint_SetMotorSpeed(springFrontId, motorSpeed);
            b2WheelJoint_SetMaxMotorTorque(springFrontId, torque);
            b2WheelJoint_EnableMotor(springFrontId, true);
        }
        else
        {
            b2WheelJoint_EnableMotor(springBackId, false);
            b2WheelJoint_EnableMotor(springFrontId, false);
        }

        if (audio && !isDead)
        {
            if (!IsSoundPlaying(audio->engine))
            {
                PlaySound(audio->engine);
            }
            // Cambiar pitch según velocidad
            b2Vec2 vel = b2Body_GetLinearVelocity(chassisId);
            float speed = b2Length(vel);

            float pitch = 0.5f + (speed / 60.0f); // 0.5 base, sube con velocidad
            if (pitch > 2.0f)
            {
                pitch = 2.0f;
            }
            SetSoundPitch(audio->engine, pitch);
        }
        else if (audio && isDead)
        {
            StopSound(audio->engine);
        }

        if (GameKeyPressed(KEY_SPACE))
        {
            Shoot();
        }

        // --- LÓGICA DE ESCUDO (SHIFT) ---
        float dt = GetFrameTime(); // Necesitas dt (delta time)

        // 1. Manejo de tiempos
        if (shieldActive)
        {
            shieldTimer -= dt;
            parryWindow -= dt; // Ventana de parry se reduce

            if (shieldTimer <= 0.0f)
            {
                // Se acabó el escudo, entra en cooldown
                shieldActive = false;
                shieldCooldown = 2.0f;
            }
        }
        else
        {
            // Si no está activo, bajamos el cooldown
            if (shieldCooldown > 0.0f)
            {
                shieldCooldown -= dt;
            }
        }

        // 2. Activación
        // Si presionas SHIFT, no está activo y no hay cooldown -> ACTIVAR
        if (GameKeyPressed(KEY_LEFT_SHIFT) && !shieldActive && shieldCooldown <= 0.0f)
        {
            shieldActive = true;
            shieldTimer = 3.0f;  // Dura 3 segundos
            parryWindow = 0.40f; // Parry disponible los primeros 0.40s

            if (audio)
            {
                PlaySound(audio->shieldOn);
            }
        }

        GetPosition();
    }

    // Getters de vidas
    int GetLives()
    {
        return lives;
    }
    int GetMaxLives()
    {
        return maxLives;
    }

    // Getters de invulnerabilidad
    bool IsInvulnerable()
    {
        return isInvulnerable;
    }

    // Activar invulnerabilidad
    void SetInvulnerable(float duration)
    {
        invulnerabilityTimer = duration;
        isInvulnerable = true;
    }

    // Actualizar invulnerabilidad
    void UpdateInvulnerability(float dt)
    {
        if (isInvulnerable)
        {
            invulnerabilityTimer -= dt;
            if (invulnerabilityTimer <= 0.0f)
            {
                isInvulnerable = false;
                invulnerabilityTimer = 0.0f;
            }
        }
    }

    // Activar quiz (se llama cuando recoge item de reparación)
    void ActivateQuiz(float playerMeters)
    {
        isQuizActive = true;
        quizTimer = 10.0f;
        quizPosition = GetPosition();

        // Generar pregunta según distancia
        GenerateMathQuestion(playerMeters);
    }

    // Generar pregunta según zona
    void GenerateMathQuestion(float playerMeters)
    {
        currentQuestion.num1 = GetRandomValue(1, 20);
        currentQuestion.num2 = GetRandomValue(1, 20);

        if (playerMeters < 2000.0f)
        {
            currentQuestion.operation = '+';
            currentQuestion.correctAnswer = currentQuestion.num1 + currentQuestion.num2;
        }
        else if (playerMeters < 4000.0f)
        {
            currentQuestion.operation = '-';
            currentQuestion.correctAnswer = currentQuestion.num1 - currentQuestion.num2;
        }
        else if (playerMeters < 6000.0f)
        {
            currentQuestion.operation = '*';
            currentQuestion.correctAnswer = currentQuestion.num1 * currentQuestion.num2;
        }
        else
        {
            int opIndex = GetRandomValue(0, 3);
            if (opIndex == 0)
            {
                currentQuestion.operation = '+';
                currentQuestion.correctAnswer = currentQuestion.num1 + currentQuestion.num2;
            }
            else if (opIndex == 1)
            {
                currentQuestion.operation = '-';
                currentQuestion.correctAnswer = currentQuestion.num1 - currentQuestion.num2;
            }
            else if (opIndex == 2)
            {
                currentQuestion.operation = '*';
                currentQuestion.correctAnswer = currentQuestion.num1 * currentQuestion.num2;
            }
            else
            {
                currentQuestion.operation = '/';
                // Asegurar división exacta
                currentQuestion.num2 = GetRandomValue(1, 5);
                currentQuestion.num1 = currentQuestion.num2 * GetRandomValue(1, 10);
                currentQuestion.correctAnswer = currentQuestion.num1 / currentQuestion.num2;
            }
        }

        // RF07: cero es una respuesta valida, no una marca de casilla vacia.
        const int correctIndex = GetRandomValue(0, 2);
        for (int i = 0; i < 3; ++i)
        {
            if (i == correctIndex)
            {
                currentQuestion.options[i] = currentQuestion.correctAnswer;
                continue;
            }
            int candidate = 0;
            bool repeated = false;
            do
            {
                candidate = currentQuestion.correctAnswer + GetRandomValue(-10, 10);
                repeated = candidate == currentQuestion.correctAnswer;
                for (int j = 0; j < i; ++j)
                {
                    repeated = repeated || currentQuestion.options[j] == candidate;
                }
            } while (repeated);
            currentQuestion.options[i] = candidate;
        }
    }

    // Responder pregunta (índice 0, 1 o 2)
    void AnswerQuestion(int optionIndex)
    {
        if (!isQuizActive || optionIndex < 0 || optionIndex >= 3)
        {
            return;
        }

        if (currentQuestion.options[optionIndex] == currentQuestion.correctAnswer)
        {

            lives = (lives < maxLives) ? lives + 1 : maxLives;

            if (audio)
            {
                PlaySound(audio->recover);
            }
        }
        else
        {

            if (audio)
            {
                PlaySound(audio->wrong);
            }
        }

        isQuizActive = false;
        quizTimer = 0.0f;
    }

    // Perder una vida
    void LoseLife()
    {
        if (isDead || isInvulnerable)
        {
            return;
        }

        lives--;
        SetInvulnerable(2.0f);

        if (audio)
        {
            if (lives <= 0)
            {
                PlaySound(audio->carExplosion); // Muerte
            }
            else
            {
                PlaySound(audio->damage); // (Golpe normal)
            }
        }

        if (lives <= 0)
        {
            isDead = true;
            StopMovement();
        }
    }

    // Getters para quiz
    bool IsQuizActive()
    {
        return isQuizActive;
    }

    float GetQuizTimer()
    {
        return quizTimer;
    }
    MathQuestion GetCurrentQuestion()
    {
        return currentQuestion;
    }

    // Actualizar quiz
    void UpdateQuiz(float dt)
    {
        if (!isQuizActive)
        {
            return;
        }

        quizTimer -= dt;
        if (quizTimer <= 0.0f)
        {
            isQuizActive = false;
        }
    }

    void CheckHeadCrash(float groundHeight)
    {
        if (isDead)
        {
            return;
        }

        // Obtenemos la posición Y del sensor del techo
        b2Vec2 roofPos = b2Body_GetPosition(roofSensorId);
        float roofScreenY = roofPos.y * scale;

        // Comparamos con la altura del suelo que recibimos como parámetro
        if (roofScreenY >= groundHeight - 40.0f)
        {
            if (!IsRoofTouchingGround())
            {
                if (!IsInvulnerable())
                {
                    LoseLife();
                }
                SetRoofTouching(true);
            }
        }
        else
        {
            SetRoofTouching(false);
        }
    }

    // Firma corregida: recibe 3 texturas
    void Draw(Texture2D chassisTex, Texture2D wheelTex, Texture2D missileTex)
    {
        if (isDead)
        {
            return;
        }

        Vector2 screenPos = GetPosition();

        b2Rot rot = b2Body_GetRotation(chassisId);
        float angleDeg = b2Rot_GetAngle(rot) * RAD2DEG;

        // Dibujar chasis
        Rectangle sourceRec = {0.0f, 0.0f, (float)chassisTex.width, (float)chassisTex.height};
        Rectangle destRec = {screenPos.x, screenPos.y, width, height};
        Vector2 origin = {width / 2.0f, height / 2.0f};

        DrawTexturePro(chassisTex, sourceRec, destRec, origin, angleDeg, WHITE);

        // Suspensión y ruedas
        DrawShock(chassisId, wheelBackId, -wheelOffset);
        DrawShock(chassisId, wheelFrontId, wheelOffset);
        DrawWheel(wheelBackId, wheelTex);
        DrawWheel(wheelFrontId, wheelTex);

        // --- DIBUJAR MISILES CON TEXTURA ---
        for (const auto &m : activeMissiles)
        {
            b2Vec2 mpos = b2Body_GetPosition(m.bodyId);
            b2Vec2 vel = b2Body_GetLinearVelocity(m.bodyId);

            float angle = atan2f(vel.y, vel.x) * RAD2DEG;

            Rectangle source = {0, 0, (float)missileTex.width, (float)missileTex.height};
            Rectangle dest = {mpos.x * scale, mpos.y * scale, 45.0f, 15.0f};

            // Ajusta el origen
            Vector2 origin = {22.5f, 7.5f};
            DrawTexturePro(missileTex, source, dest, origin, angle, WHITE);
        }

        // --- ESCUDO MEJORADO (Estilo Burbuja de Energía) ---
        if (shieldActive)
        {
            Vector2 spos = GetPosition();

            // 1. Animación de "respiración" (Pulse)
            // Usa GetTime() para una onda seno suave. Si está en parry, pulsa más rápido.
            float time = (float)GetTime();
            float pulseSpeed = (parryWindow > 0.0f) ? 20.0f : 4.0f; // Rápido en parry, lento normal
            float pulseSize = sinf(time * pulseSpeed) * 2.5f;       // Oscila +/- 2.5 pixeles
            float baseRadius = 85.0f;
            float radius = baseRadius + pulseSize;

            // 2. Definir colores según estado
            Color mainColor;
            Color rimColor;

            if (parryWindow > 0.0f)
            {
                // MODO PARRY: Energía Dorada Alta
                mainColor = {255, 255, 0, 50};   // Amarillo muy transparente
                rimColor = {255, 255, 200, 255}; // Blanco/Amarillo brillante borde
            }
            else
            {
                // MODO ESCUDO: Azul Tecnológico
                mainColor = {0, 200, 255, 30}; // Cian muy transparente
                rimColor = {0, 255, 255, 200}; // Cian neón borde
            }

            // 3. DIBUJO POR CAPAS (Efecto Burbuja)

            // Capa A: Relleno suave (el interior de la burbuja)
            DrawCircleV(spos, radius, mainColor);

            // Capa B: Gradiente interno (brillo central)
            DrawCircleGradient(spos.x, spos.y, radius * 0.8f, Fade(rimColor, 0.0f),
                               Fade(mainColor, 0.2f));

            // Capa C: Borde exterior grueso (halo de energía)
            DrawRing(spos, radius, radius + 4.0f, 0, 360, 0, Fade(rimColor, 0.4f));

            // Capa D: Borde fino y nítido (el límite físico)
            DrawRing(spos, radius, radius + 1.5f, 0, 360, 0, rimColor);

            // Capa E: Anillo interior decorativo (tecnológico)
            DrawRing(spos, radius - 15.0f, radius - 13.0f, 0 + (time * 50), 120 + (time * 50), 0,
                     Fade(rimColor, 0.5f));
            DrawRing(spos, radius - 15.0f, radius - 13.0f, 180 + (time * 50), 300 + (time * 50), 0,
                     Fade(rimColor, 0.5f));

            // (El texto de PARRY ha sido eliminado)
        }

        // Barra de Cooldown (Mantenemos esto visualmente limpio debajo del coche)
        if (!shieldActive && shieldCooldown > 0.0f)
        {
            Vector2 spos = GetPosition();
            float barWidth = 60.0f;
            float fill = (shieldCooldown / 2.0f) * barWidth;

            // Fondo gris oscuro
            DrawRectangle(spos.x - 30, spos.y + 55, barWidth, 4, {50, 50, 50, 200});
            // Barra de carga roja
            DrawRectangle(spos.x - 30, spos.y + 55, fill, 4, {255, 50, 50, 200});
        }

        // Invulnerabilidad
        // --- INVULNERABILIDAD MEJORADA (Efecto Matriz / Glitch) ---
        if (isInvulnerable)
        {
            Vector2 spos = GetPosition();
            float time = (float)GetTime();

            // Calculamos cuánto tiempo de invulnerabilidad queda (0.0 a 1.0)
            // Asumimos que invulnerabilityTimer baja desde 2.0 (o el valor que uses)
            float maxTime = 2.0f; // Ajusta si tu tiempo máximo es diferente
            float progress = invulnerabilityTimer / maxTime;
            if (progress > 1.0f)
            {
                progress = 1.0f;
            }

            // 1. Color base: Verde Matrix con intensidad variable
            // Parpadea más rápido cuando se está acabando
            float flashSpeed = (progress < 0.3f) ? 30.0f : 10.0f;
            float alphaPulse = (sinf(time * flashSpeed) + 1.0f) * 0.5f; // 0.0 a 1.0

            Color matrixGreen = {0, 255, 100, (unsigned char)(100 + (alphaPulse * 100))};
            Color brightWhite = {200, 255, 200, (unsigned char)(200 * alphaPulse)};

            // 2. Ondas expansivas concéntricas (Efecto Radar)
            // Creamos 3 anillos que se expanden desde el centro
            float waveSize = 120.0f;
            for (int i = 0; i < 3; i++)
            {
                // Desfase para cada anillo basado en el tiempo
                float waveProgress = fmodf(time * 2.0f + (i * 0.33f), 1.0f);
                float currentRadius = waveProgress * waveSize;
                float waveAlpha = 1.0f - waveProgress; // Se desvanece al crecer

                if (currentRadius > 40.0f) // Solo dibujar si es más grande que el coche
                {
                    DrawRing(spos, currentRadius, currentRadius + 2.0f, 0, 360, 0,
                             Fade(matrixGreen, waveAlpha * 0.6f));
                }
            }

            // 3. Barrera de "Código" Hexagonal (Decoración)
            // Dibujamos un hexágono giratorio discontinuo alrededor
            float hexRadius = 90.0f;
            float rotSpeed = time * 90.0f; // Gira 90 grados por segundo

            // Dibujamos 3 segmentos de arco (como brackets rotando)
            for (int i = 0; i < 3; i++)
            {
                float startAngle = rotSpeed + (i * 120);
                float endAngle = startAngle + 60;
                DrawRing(spos, hexRadius, hexRadius + 3.0f, startAngle, endAngle, 6,
                         Fade(matrixGreen, 0.8f));
            }

            // 4. Brillo intenso sobre el coche (Flash cuando te golpean)
            // Dibuja un círculo relleno muy transparente justo encima del sprite del coche
            DrawCircleV(spos, 50.0f, Fade(brightWhite, 0.2f));
        }
    }

private:
    // ESTAS FUNCIONES AHORA ESTÁN DENTRO DE LA CLASE CORRECTAMENTE
    void DrawShock(b2BodyId chassis, b2BodyId wheel, float offsetX)
    {
        b2Vec2 cPos = b2Body_GetPosition(chassis);
        b2Rot cRot = b2Body_GetRotation(chassis);
        b2Vec2 wPos = b2Body_GetPosition(wheel);

        float c = cRot.c;
        float s = cRot.s;
        float offX_m = offsetX / scale;
        float offY_m = (height / 2.0f) / scale;

        float anchorX = cPos.x + (offX_m * c - offY_m * s);
        float anchorY = cPos.y + (offX_m * s + offY_m * c);

        DrawLineEx((Vector2){anchorX * scale, anchorY * scale},
                   (Vector2){wPos.x * scale, wPos.y * scale}, 4.0f, DARKGRAY);
        DrawLineEx((Vector2){anchorX * scale, anchorY * scale},
                   (Vector2){wPos.x * scale, wPos.y * scale}, 2.0f, YELLOW);
    }

    void DrawWheel(b2BodyId wheelId, Texture2D wheelTex)
    {
        b2Vec2 pos = b2Body_GetPosition(wheelId);
        b2Rot rot = b2Body_GetRotation(wheelId);
        float angleDeg = b2Rot_GetAngle(rot) * RAD2DEG;
        float radius = 22.0f;

        Rectangle source = {0.0f, 0.0f, (float)wheelTex.width, (float)wheelTex.height};
        Rectangle dest = {pos.x * scale, pos.y * scale, radius * 2, radius * 2};
        Vector2 origin = {radius, radius};

        DrawTexturePro(wheelTex, source, dest, origin, angleDeg, WHITE);
    }
};
