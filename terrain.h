#pragma once
#include <box2d/box2d.h>
#include <raylib.h>
#include <vector>
#include <deque>
#include <math.h>
#include "carb.h"
#include "effects.h"
#include "audios.h"

// --- CLASE ITEM REPARACION ---
class RepairItem
{
public:
    b2BodyId bodyId;
    bool collected;
    float x, y;

    RepairItem(b2WorldId worldId, float posX, float posY, float scale)
    {
        x = posX;
        y = posY;
        collected = false;

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.position = (b2Vec2){x / scale, y / scale};
        bodyDef.type = b2_staticBody;
        bodyId = b2CreateBody(worldId, &bodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.isSensor = true;
        b2Polygon box = b2MakeBox(12.0f / scale, 12.0f / scale);
        b2CreatePolygonShape(bodyId, &shapeDef, &box);
    }

    void Destroy()
    {
        if (b2Body_IsValid(bodyId))
        {
            b2DestroyBody(bodyId);
        }
    }

    void Draw(Texture2D tex)
    {
        if (collected)
            return;

        if (tex.id > 0)
        {
            float size = 60.0f;
            Rectangle source = {0.0f, 0.0f, (float)tex.width, (float)tex.height};
            Rectangle dest = {x, y - 25.0f, size, size};
            Vector2 origin = {size / 2.0f, size / 2.0f};
            DrawTexturePro(tex, source, dest, origin, 0.0f, WHITE);
        }
        else
        {
            DrawRectangle(x - 3, y - 15, 6, 30, GREEN);
            DrawRectangle(x - 15, y - 3, 30, 6, GREEN);
            DrawCircle(x, y, 15, Fade(GREEN, 0.3f));
        }
    }
};

// --- CLASE BIDÃ“N ---
class FuelCan
{
public:
    b2BodyId bodyId;
    bool collected;
    float x, y;

    FuelCan(b2WorldId worldId, float posX, float posY, float scale)
    {
        x = posX;
        y = posY;
        collected = false;

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.position = (b2Vec2){x / scale, y / scale};
        bodyDef.type = b2_staticBody;
        bodyId = b2CreateBody(worldId, &bodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.isSensor = true;

        b2Polygon box = b2MakeBox(15.0f / scale, 20.0f / scale);
        b2CreatePolygonShape(bodyId, &shapeDef, &box);
    }

    void Destroy()
    {
        if (b2Body_IsValid(bodyId))
        {
            b2DestroyBody(bodyId);
        }
    }

    void Draw(Texture2D tex)
    {
        if (collected)
            return;
        // Usar textura si existe, si no, cuadro rojo
        if (tex.id > 0)
        {
            // Aumentamos tamaño. Antes era {x, y, 30, 40}
            float w = 45.0f; // Nuevo ancho
            float h = 60.0f; // Nuevo alto
            DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height},
                           {x, y, w, h}, {w / 2.0f, h / 2.0f}, 0.0f, WHITE);
        }
        else
        {
            DrawRectangle(x - 10, y - 15, 20, 30, RED);
        }
    }
};

// --- CLASE MINA ---
class Mine
{
public:
    b2BodyId bodyId;
    bool exploded;
    float x, y;

    Mine(b2WorldId worldId, float posX, float posY, float scale)
    {
        x = posX;
        y = posY;
        exploded = false;

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.position = (b2Vec2){x / scale, y / scale};
        bodyDef.type = b2_staticBody;
        bodyId = b2CreateBody(worldId, &bodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.isSensor = true;

        b2Polygon box = b2MakeBox(15.0f / scale, 5.0f / scale);
        b2CreatePolygonShape(bodyId, &shapeDef, &box);
    }

    void Destroy()
    {
        if (b2Body_IsValid(bodyId))
        {
            b2DestroyBody(bodyId);
        }
    }

    void Draw(Texture2D tex)
    {
        if (exploded)
            return;
        if (tex.id > 0)
        {
            // En Mine::Draw(Texture2D tex)
            float w = 30.0f; // Antes 30
            float h = 15.0f; // Antes 15
            DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height},
                           {x, y, w, h}, {w / 2.0f, h / 2.0f}, 0.0f, WHITE);
        }
        else
        {
            DrawRectangle(x - 15, y - 5, 30, 10, RED);
        }
    }
};

// --- CLASE ITEM MISIL ---
class MissileItem
{
public:
    b2BodyId bodyId;
    bool collected;
    float x, y;

    MissileItem(b2WorldId worldId, float posX, float posY, float scale)
    {
        x = posX;
        y = posY;
        collected = false;

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.position = (b2Vec2){x / scale, y / scale};
        bodyDef.type = b2_staticBody;
        bodyId = b2CreateBody(worldId, &bodyDef);

        b2ShapeDef shapeDef = b2DefaultShapeDef();
        shapeDef.isSensor = true;
        b2Polygon box = b2MakeBox(10.0f / scale, 10.0f / scale); // Caja mas peque
        b2CreatePolygonShape(bodyId, &shapeDef, &box);
    }

    void Destroy()
    {
        if (b2Body_IsValid(bodyId))
        {
            b2DestroyBody(bodyId);
        }
    }

    void Draw(Texture2D tex)
    {
        if (collected)
            return;

        if (tex.id > 0)
        {
            // 1. TAMAÑO CUADRADO
            float size = 50.0f; // 50x50 píxeles

            Rectangle source = {0.0f, 0.0f, (float)tex.width, (float)tex.height};

            // 2. POSICIÓN
            // 'y - 25.0f' sube el centro del objeto para que la base toque el suelo
            // (La mitad de 50 es 25)
            Rectangle dest = {
                x,
                y - 25.0f,
                size,
                size};

            Vector2 origin = {size / 2.0f, size / 2.0f};

            // 3. ROTACIÓN 0.0f (Normal)
            DrawTexturePro(tex, source, dest, origin, 0.0f, WHITE);
        }
        else
        {
            // Cuadrado morado si no hay textura
            DrawRectangle(x - 20, y - 40, 40, 40, PURPLE);
        }
    }
};

// Configuracion del Terreno
const int POINTS_PER_SEGMENT = 50;
const float SEGMENT_LENGTH = 1000.0f;

class Terrain
{
private:
    b2WorldId worldId;
    float scale;
    ExplosionManager *explosionMgr = nullptr;
    Texture2D explosionMineTex;
    GameAudio *audio = nullptr;
    // VARIABLES NUEVAS PARA EL MURO
    b2BodyId spawnWallId;
    bool spawnWallCreated;

    struct GroundSegment
    {
        b2BodyId bodyId;
        float endX;
    };
    std::deque<GroundSegment> segments;
    std::vector<FuelCan> cans;
    std::vector<Mine> mines;               // Lista de minas
    std::vector<MissileItem> missileItems; // <--- NUEVA LISTA
    std::vector<b2BodyId> backWalls;       // Lista de muros
    std::vector<RepairItem> repairItems;   // Nueva lista

    float lastGenX;
    float lastGenY;

public:
    std::vector<Vector2> visualPoints;

    void SetAudio(GameAudio *ptr) { audio = ptr; }

    Terrain(b2WorldId world, float physicsScale)
        : worldId(world), scale(physicsScale),
          lastGenX(-500.0f), lastGenY(500.0f),
          spawnWallCreated(false) // Inicializar
    {
        GenerateSegment(true);

        // CREAR MURO DE SPAWN VISIBLE
        CreateSpawnWall();
    }
    float GetHeightAt(float xCheck)
    {
        if (visualPoints.empty())
            return 10000.0f; // Valor seguro si no hay terreno

        for (size_t i = 0; i < visualPoints.size() - 1; i++)
        {
            if (xCheck >= visualPoints[i].x && xCheck < visualPoints[i + 1].x)
            {
                // InterpolaciÃ³n lineal simple
                float t = (xCheck - visualPoints[i].x) / (visualPoints[i + 1].x - visualPoints[i].x);
                return visualPoints[i].y + t * (visualPoints[i + 1].y - visualPoints[i].y);
            }
        }
        return 10000.0f; // Fuera de rango
    }

    void SetExplosionManager(ExplosionManager *mgr) { explosionMgr = mgr; }
    void SetExplosionTexture(Texture2D mineExp) { explosionMineTex = mineExp; }

    void Update(Vector2 playerPos, Car &car)
    {
        // Generar
        if (playerPos.x + 2000.0f > lastGenX)
        {
            GenerateSegment(false);
        }

        // Limpiar Terreno
        if (!segments.empty() && segments.front().endX < playerPos.x - 9000.0f)
        {
            b2DestroyBody(segments.front().bodyId);
            segments.pop_front();

            // --- CREAR MURO INVISIBLE JUSTO DONDE SE BORRÃ“ EL TERRENO ---
            float wallX = playerPos.x - 9000.0f; // Donde estÃ¡ el lÃ­mite de borrado

            b2BodyDef wallDef = b2DefaultBodyDef();
            wallDef.position = (b2Vec2){wallX / scale, 0.0f};
            wallDef.type = b2_staticBody;
            b2BodyId wallId = b2CreateBody(worldId, &wallDef);

            // Forma: Pared vertical gigante
            b2ShapeDef wallShape = b2DefaultShapeDef();
            b2Polygon wallBox = b2MakeBox(20.0f / scale, 10000.0f / scale); // 20m ancho, muy tall
            b2CreatePolygonShape(wallId, &wallShape, &wallBox);

            backWalls.push_back(wallId);

            if (visualPoints.size() > 200)
            {
                visualPoints.erase(visualPoints.begin(), visualPoints.begin() + POINTS_PER_SEGMENT);
            }
        }

        // --- LIMPIAR MUROS VIEJOS ---
        for (auto it = backWalls.begin(); it != backWalls.end();)
        {
            b2Vec2 wallPos = b2Body_GetPosition(*it);
            if (wallPos.x * scale < playerPos.x - 11000.0f)
            {
                b2DestroyBody(*it);
                it = backWalls.erase(it);
            }
            else
            {
                ++it;
            }
        }

        // Limpiar Gasolina
        for (auto it = cans.begin(); it != cans.end();)
        {
            Vector2 itemPos = {it->x, it->y};
            if (!it->collected && CheckCollisionCircles(playerPos, 60, itemPos, 20))
            {
                it->collected = true;
                car.AddFuel(50.0f);
                it->Destroy();
                it = cans.erase(it);
            }
            else if (it->x < playerPos.x - 9000)
            {
                it->Destroy();
                it = cans.erase(it);
            }
            else
            {
                ++it;
            }
        }

        // Limpiar/Explotar Minas
        for (auto it = mines.begin(); it != mines.end();)
        {
            Vector2 itemPos = {it->x, it->y};
            if (!it->exploded && CheckCollisionCircles(playerPos, 60, itemPos, 20))
            {
                it->exploded = true;
                car.ApplyMineExplosion(); // IMPULSO AL AUTO
                if (explosionMgr)
                    explosionMgr->AddMineExplosion(itemPos, explosionMineTex);

                if (audio)
                    PlaySound(audio->mineExplosion); // <--- AGREGAR
                it->Destroy();
                it = mines.erase(it);
            }
            else if (it->x < playerPos.x - 9000)
            {
                it->Destroy();
                it = mines.erase(it);
            }
            else
            {
                ++it;
            }
        }

        // Limpiar Misiles recogibles
        for (auto it = missileItems.begin(); it != missileItems.end();)
        {
            Vector2 itemPos = {it->x, it->y};
            if (!it->collected && CheckCollisionCircles(playerPos, 60, itemPos, 20))
            {
                it->collected = true;
                car.AddAmmo(3);
                it->Destroy();
                it = missileItems.erase(it);
            }
            else if (it->x < playerPos.x - 9000)
            {
                it->Destroy();
                it = missileItems.erase(it);
            }
            else
            {
                ++it;
            }
        }

        // Limpiar Items de Reparacion
        for (auto it = repairItems.begin(); it != repairItems.end();)
        {
            Vector2 itemPos = {it->x, it->y};
            // Solo recoger si NO hay quiz activo
            if (!it->collected && CheckCollisionCircles(playerPos, 60, itemPos, 20) && !car.IsQuizActive())
            {
                it->collected = true;
                car.ActivateQuiz(playerPos.x / scale);


                it->Destroy();
                it = repairItems.erase(it);
            }
            else if (it->x < playerPos.x - 9000)
            {
                it->Destroy();
                it = repairItems.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    // Dentro de class Terrain (parte public)
    void Draw(Texture2D fuelTex, Texture2D mineTex, Texture2D missileTex, Texture2D repairTex)
    {
        if (visualPoints.size() < 2)
            return;

        Color dirtColor = {60, 40, 20, 255};
        Color grassColor = {30, 180, 30, 255};

        Color asphalt = {60, 60, 60, 255}; // Oscuro
        Color debris = {80, 80, 80, 255};  // Gris oscuro

        // Tierra
        for (size_t i = 0; i < visualPoints.size() - 1; i++)
        {
            Vector2 p1 = visualPoints[i];
            Vector2 p2 = visualPoints[i + 1];
            Vector2 p1_deep = {p1.x, p1.y + 2000.0f};
            Vector2 p2_deep = {p2.x, p2.y + 2000.0f};
            DrawTriangle(p1, p1_deep, p2, debris);
            DrawTriangle(p2, p1_deep, p2_deep, debris);
        }

        // Pasto
        for (size_t i = 0; i < visualPoints.size() - 1; i++)
        {
            DrawLineEx(visualPoints[i], visualPoints[i + 1], 8.0f, asphalt);

            float stakeW = 10.0f; // Ancho visual
            float stakeH = 50.0f; // Alto visual (stakeDepth)
            float manualOff = 10.0f;

            Vector2 pos = visualPoints[i];

            // Dibuja también un puntito amarillo en la unión exacta para referencia
            DrawCircleV(pos, 3.0f, YELLOW);
        }

        // DIBUJAR BASE DE SPAWN
        if (spawnWallCreated && b2Body_IsValid(spawnWallId))
        {
            // Obtener posición y dimensiones físicas
            b2Vec2 pos = b2Body_GetPosition(spawnWallId);
            float screenX = pos.x * scale;
            float screenY = pos.y * scale;

            float baseWidth = 600.0f;
            float baseHeight = 250.0f;

            // Calcular esquina superior izquierda visual
            float leftX = screenX - (baseWidth / 2.0f);
            float topY = screenY - (baseHeight / 2.0f);
            float rightX = screenX + (baseWidth / 2.0f); // Esta es la cara frontal

            // 1. Estructura principal (Cuerpo de la base)
            // Un gris oscuro sólido industrial
            DrawRectangle(leftX, topY, baseWidth, baseHeight, {40, 40, 45, 255});

            // 2. Techo reforzado
            DrawRectangle(leftX, topY, baseWidth, 20.0f, {30, 30, 35, 255});
            DrawRectangleLines(leftX, topY, baseWidth, 20.0f, {60, 60, 70, 255});

            // 3. Detalles laterales (Paneles a lo largo de la base)
            for (float bx = leftX; bx < rightX - 40; bx += 100.0f)
            {
                // Líneas verticales de separación de paneles
                DrawLineEx({bx, topY + 20}, {bx, topY + baseHeight}, 2.0f, {30, 30, 35, 255});

                // Pequeñas ventanas o ventilas
                DrawRectangle(bx + 20, topY + 50, 40, 20, {20, 20, 20, 255});
                DrawRectangleLines(bx + 20, topY + 50, 40, 20, {60, 60, 70, 255});
            }

            // 4. CARA FRONTAL (La "puerta" o final de la base)
            // Dibujamos un borde más grueso en la derecha para simular la compuerta de salida
            float doorWidth = 40.0f;
            float doorX = rightX - doorWidth;

            // Marco de la puerta
            DrawRectangle(doorX, topY, doorWidth, baseHeight, {60, 60, 65, 255});
            DrawRectangleLines(doorX, topY, doorWidth, baseHeight, {80, 80, 90, 255});

            // Franjas de advertencia solo en la puerta (negro y amarillo)
            for (int i = 0; i < 10; i++)
            {
                float stripeY = topY + (i * 25);
                DrawLineEx({doorX, stripeY}, {rightX, stripeY + 15}, 5.0f, {200, 180, 20, 200});
            }

            // Luz roja pequeña en la parte superior de la puerta (sin texto)
            DrawCircle(doorX + doorWidth / 2, topY + 30, 4, RED);
            DrawCircle(doorX + doorWidth / 2, topY + 30, 2, WHITE); // Brillo
        }

        // Items - AQUÍ ES DONDE PASAMOS LAS TEXTURAS
        for (auto &can : cans)
            can.Draw(fuelTex);

        for (auto &mine : mines)
            mine.Draw(mineTex);

        for (auto &m : missileItems)
            m.Draw(missileTex);

        for (auto &item : repairItems)
            item.Draw(repairTex);
    }

private:
    void CreateSpawnWall()
    {
        if (spawnWallCreated)
            return;

        // Configuración de la Base
        float baseWidth = 600.0f;  // Ahora es 600px de largo (hacia atrás)
        float baseHeight = 250.0f; // Un poco más alta

        // Queremos que el FRENTE de la base esté en x = -80 aprox (detrás del spawn)
        // Así que el centro debe estar desplazado hacia la izquierda
        float frontFaceX = -80.0f;
        float centerX = frontFaceX - (baseWidth / 2.0f);
        float centerY = 500.0f - (baseHeight / 2.0f) + 100.0f; // Ajuste para que pise el suelo (y = 500)

        b2BodyDef wallDef = b2DefaultBodyDef();
        wallDef.position = {centerX / scale, 500.0f / scale - (baseHeight / 2.0f) / scale};
        // Nota: En Box2D el origen suele ser el centro, ajustamos para que la base se asiente en el suelo
        // Simplifiquemos: Ponemos el centro físico donde calculamos
        wallDef.position = {centerX / scale, 500.0f / scale - (baseHeight / 2.0f) / scale};

        // Mejor ajuste manual de altura para que quede alineado al suelo (y=500 es el piso)
        // Si la altura es 250, el centro debe estar en 500 - 125 = 375
        wallDef.position = {centerX / scale, (500.0f - baseHeight / 2.0f) / scale};

        wallDef.type = b2_staticBody;

        spawnWallId = b2CreateBody(worldId, &wallDef);

        b2ShapeDef wallShape = b2DefaultShapeDef();
       

        // Crear la caja gigante
        b2Polygon wallBox = b2MakeBox((baseWidth / 2.0f) / scale, (baseHeight / 2.0f) / scale);
        b2CreatePolygonShape(spawnWallId, &wallShape, &wallBox);

        spawnWallCreated = true;
    }

    void GenerateSegment(bool isStart)
    {
        std::vector<b2Vec2> chainPoints;
        std::vector<b2Vec2> safetyPoints;
        float step = SEGMENT_LENGTH / POINTS_PER_SEGMENT;

        // Conectar con el anterior
        chainPoints.push_back((b2Vec2){lastGenX / scale, lastGenY / scale});
        safetyPoints.push_back((b2Vec2){lastGenX / scale, (lastGenY + 20.0f) / scale});

        for (int i = 0; i < POINTS_PER_SEGMENT; i++)
        {
            lastGenX += step;

            if (isStart)
            {
                lastGenY = 500.0f;
            }
            else
            {
                float noise1 = sinf(lastGenX * 0.003f) * 150.0f;
                float noise2 = sinf(lastGenX * 0.01f) * 30.0f;
                lastGenY = 500.0f + noise1 + noise2;

                // Generar Items
                if (GetRandomValue(0, 100) < 1)
                { // 2% Bidones
                    cans.push_back(FuelCan(worldId, lastGenX, lastGenY - 50.0f, scale));
                }
                else if (GetRandomValue(0, 100) < 0.5)
                { // 2% Minas
                    mines.push_back(Mine(worldId, lastGenX, lastGenY - 10.0f, scale));
                }
                else if (GetRandomValue(0, 100) < 0.5)
                { // 0.5% Misiles
                    missileItems.push_back(MissileItem(worldId, lastGenX, lastGenY - 40.0f, scale));
                }
                else if (GetRandomValue(0, 100) < 0.01)
                { // 0.3% probabilidad
                    repairItems.push_back(RepairItem(worldId, lastGenX, lastGenY - 50.0f, scale));
                }
            }

            chainPoints.push_back((b2Vec2){lastGenX / scale, lastGenY / scale});
            safetyPoints.push_back((b2Vec2){lastGenX / scale, (lastGenY + 20.0f) / scale});
            visualPoints.push_back({lastGenX, lastGenY});

        } // Fin del for

        b2BodyDef bodyDef = b2DefaultBodyDef();
        bodyDef.position = (b2Vec2){0.0f, 0.0f};
        b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

        // Superficie principal
        b2ChainDef chainDef = b2DefaultChainDef();
        chainDef.points = chainPoints.data();
        chainDef.count = chainPoints.size();
        chainDef.isLoop = false;
        // chainDef.friction = 2.0f;
        b2CreateChain(bodyId, &chainDef);

        // Cadena de seguridad (la que ya tenías)
        b2ChainDef safetyDef = b2DefaultChainDef();
        safetyDef.points = safetyPoints.data();
        safetyDef.count = safetyPoints.size();
        safetyDef.isLoop = false;
        b2CreateChain(bodyId, &safetyDef);

        // --- SOLUCIÓN PARA UNIONES (NUEVO CÓDIGO) ---
        // Si no es el primer segmento del juego, ponemos un círculo en el inicio
        // para sellar la unión con el segmento anterior.
        // --- SOLUCIÓN MEJORADA: LA ESTACA DE UNIÓN ---
        if (!isStart && chainPoints.size() > 0)
        {
            b2ShapeDef sealShape = b2DefaultShapeDef();

            b2Vec2 joinPoint = chainPoints[0];

            float stakeWidth = 10.0f / scale;
            float stakeDepth = 70.0f / scale;

            // --- AJUSTE MANUAL DE ALTURA ---
            // Cuanto mayor sea este número, MÁS ABAJO estará la estaca.
            // 10.0f significa 10 pixeles más abajo del punto de unión.
            float manualOffset = -75.0f / scale;

            b2Vec2 stakeCenter = {
                joinPoint.x,
                // Posición Y = (Punto Unión) - (Mitad de la altura de la caja) - (Tu Desplazamiento)
                // Recuerda que en simulación Y crece hacia arriba, así que RESTAR es BAJAR
                joinPoint.y - (stakeDepth / 2.0f) - manualOffset};

            b2Rot zeroRot = {1.0f, 0.0f};

            b2Polygon stakePoly = b2MakeOffsetBox(
                stakeWidth / 2.0f,
                stakeDepth / 2.0f,
                stakeCenter,
                zeroRot);

            b2CreatePolygonShape(bodyId, &sealShape, &stakePoly);
        }

        // ---------------------------------------------

        segments.push_back({bodyId, lastGenX});
    }

}; // <--- ESTA ES LA LLAVE QUE FALTABA