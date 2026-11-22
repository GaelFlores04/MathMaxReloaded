/*
 * Proyecto: MATHMAX - RELOADED
 * Archivo: fisicaM.h
 * Proposito: poseer el mundo fisico y ejecutar su paso fijo en segundos.
 * Requisitos: RIE01, RDC02.
 * Base POO: Gael Flores Canedo / Israel Gomez Ahumada.
 * Adaptacion V&V: Gael Flores / Jorge Roel Vasquez; asistencia de Codex.
 * Revision: 2026-11-09
 */
#pragma once
#include <box2d/box2d.h>
#include <raylib.h>
#include <raymath.h> // Necesario para Vector2Scale si queremos mezclar

const float SCALE = 30.0f; // 30 pixeles = 1 metro
constexpr float PHYSICS_STEP_SECONDS = 1.0f / 60.0f;
constexpr int PHYSICS_SUBSTEPS = 20;

class PhysicsWorld
{
private:
    b2WorldId worldId;

public:
    PhysicsWorld()
    {
        // En v3 todo usa estructuras de definición (Def)
        b2WorldDef worldDef = b2DefaultWorldDef();
        worldDef.gravity = (b2Vec2){0.0f, 9.8f}; // Inicialización estilo C

        worldId = b2CreateWorld(&worldDef);
    }

    ~PhysicsWorld()
    {
        b2DestroyWorld(worldId);
    }

    PhysicsWorld(const PhysicsWorld &) = delete;
    PhysicsWorld &operator=(const PhysicsWorld &) = delete;

    void Update()
    {
        // v3 Step: worldId, timeStep, subSteps
        b2World_Step(worldId, PHYSICS_STEP_SECONDS, PHYSICS_SUBSTEPS);
    }

    b2WorldId GetWorldId()
    {
        return worldId;
    }

    // Fábrica de suelos
    void CreateGround(float x, float y, float w, float h)
    {
        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.position = (b2Vec2){x / SCALE, y / SCALE};

        b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        // b2MakeBox toma "extents" (radio), es decir mitad de ancho/alto
        b2Polygon box = b2MakeBox((w / 2.0f) / SCALE, (h / 2.0f) / SCALE);

        b2CreatePolygonShape(bodyId, &shapeDef, &box);
    }
};
